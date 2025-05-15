#include "fssimplewindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <vector>	
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>


class WorkerThread
{
private:
    enum
    {
        TASK_NONE,
        TASK_QUIT,
        TASK_RUN
    };

    int taskType=TASK_NONE;
    std::function <void()> task;
    std::thread thr;
    std::mutex mtx;
    std::condition_variable cond;
    void ThreadFunc();
public:
    WorkerThread();
    ~WorkerThread();
    void Run(std::function <void()>);
    void Wait(void);
};

WorkerThread::WorkerThread()
{
    std::thread t(&WorkerThread::ThreadFunc,this);
    thr.swap(t);
}
WorkerThread::~WorkerThread()
{
    mtx.lock();
    taskType=TASK_QUIT;
    mtx.unlock();
    cond.notify_one();
    thr.join();
}
void WorkerThread::Run(std::function <void()> newTask)
{
    mtx.lock();
    taskType=TASK_RUN;
    task=newTask;
    mtx.unlock();
    cond.notify_one();
}
void WorkerThread::Wait(void)
{
  std::unique_lock <std::mutex> lock(mtx); 
  cond.wait(lock,[&]{return taskType==TASK_NONE;});
}

void WorkerThread::ThreadFunc()
{
    for(;;)
    {
        std::unique_lock <std::mutex> lock(mtx);
        cond.wait(lock,[&]{return taskType!=TASK_NONE;});
        if(TASK_QUIT==taskType)
        {
            break;
        }
        else if(TASK_RUN==taskType)
        {
            task();
            taskType=TASK_NONE;
            cond.notify_one();
        }
    }
}





const double Pi=3.1415927;
const double G=9.8;
const double tol=1.0e-06;

void DrawCircle(double x,double y,double r)
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0; i<64; ++i)
	{
		double a=(double)i*Pi/32.0;
		double c=cos(a);
		double s=sin(a);
		glVertex2d(x+c*r,y+s*r);
	}
	glEnd();
}

void Trans(double &wx,double &wy,double x,double y)
{
	wx=x*10.0+400.0;
	wy=600.0-y*10.0;
}

class Ball
{
public:
	double x,y,vx,vy;
	int groupId;
};

bool CheckProximity(const Ball &ball1,const Ball &ball2)
{
	auto x1=ball1.x;
	auto y1=ball1.y;
	auto x2=ball2.x;
	auto y2=ball2.y;
	double dx=x2-x1;
	double dy=y2-y1;
	double dist=sqrt(dx*dx+dy*dy);
	return dist<2.0;
}

bool CheckVelocityCollision(Ball &ball1,Ball &ball2)
{
	auto x1=ball1.x;
	auto y1=ball1.y;
	auto x2=ball2.x;
	auto y2=ball2.y;
	auto &vx1=ball1.vx;
	auto &vy1=ball1.vy;
	auto &vx2=ball2.vx;
	auto &vy2=ball2.vy;

	double dx=x2-x1;
	double dy=y2-y1;

	double rvx=vx2-vx1;
	double rvy=vy2-vy1;
	if(dx*rvx+dy*rvy<0.0)
	{
		return true;
	}

	return false;
}

void Bounce(Ball &ball1,Ball &ball2)
{
	auto x1=ball1.x;
	auto y1=ball1.y;
	auto x2=ball2.x;
	auto y2=ball2.y;
	auto &vx1=ball1.vx;
	auto &vy1=ball1.vy;
	auto &vx2=ball2.vx;
	auto &vy2=ball2.vy;

	double nx,ny;
	nx=x2-x1;
	ny=y2-y1;

	double n=sqrt(nx*nx+ny*ny);
	if(n<tol)
	{
		return;
	}
	nx/=n;
	ny/=n;

	double k1=vx1*nx+vy1*ny;
	double k2=vx2*nx+vy2*ny;

	vx1=vx1+nx*(k2-k1);
	vy1=vy1+ny*(k2-k1);

	vx2=vx2+nx*(k1-k2);
	vy2=vy2+ny*(k1-k2);
}

void ComputeBounceParallel(Ball ball[],int n,int groupId,std::vector <int> &ballPairs)
{
	for(int i=0; i<n; ++i)
	{
		if(ball[i].groupId==groupId)
		{
			for(int j=0; j<n; ++j)
			{
				if(i!=j)
				{
					if(true==CheckProximity(ball[i],ball[j]))
					{
						if(ball[j].groupId==groupId)
						{
							if(CheckVelocityCollision(ball[i],ball[j]))
							{
								Bounce(ball[i],ball[j]);
							}
						}
						else
						{
							ballPairs.push_back(i);
							ballPairs.push_back(j);
						}
					}
				}
			}
		}
	}
}

void ComputeBounceSerial(Ball ball[],int n,const std::vector <int> &ballPairs)
{
	for(size_t I=0; I<ballPairs.size(); I+=2)
	{
		auto i=ballPairs[I];
		auto j=ballPairs[I+1];

		if(true==CheckProximity(ball[i],ball[j]))
		{
			if(true==CheckVelocityCollision(ball[i],ball[j]))
			{
				Bounce(ball[i],ball[j]);
			}
		}
	}
}

int main(void)
{
	const int nThreads=8;
	WorkerThread thr[nThreads];

	FsOpenWindow(0,0,800,600,1);


	srand(time(nullptr));

	const int n=200;
	Ball ball[n];

	for(int i=0; i<n; ++i)
	{
		ball[i].x=(double)(rand()%78-39);
		ball[i].y=30.0+(double)(rand()%20);
		ball[i].vx=(double)(rand()%20-10);
		ball[i].vy=0.0;
	}



	FsPassedTime();
	for(;;)
	{
		FsPollDevice();

		auto  key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		auto millisec=FsPassedTime();
		double dt=(double)millisec/1000.0;

		for(int i=0; i<n; ++i)
		{
			ball[i].x=ball[i].x+ball[i].vx*dt;
			ball[i].y=ball[i].y+ball[i].vy*dt;
			ball[i].vy-=G*dt;
			if(ball[i].y<1.0 && ball[i].vy<0.0)
			{
				ball[i].vy=-ball[i].vy;
			}
			if((ball[i].x<-39.0 && ball[i].vx<0.0) || (39.0<ball[i].x && 0.0<ball[i].vx))
			{
				ball[i].vx=-ball[i].vx;
			}
		}


		for(auto &b : ball)
		{
			double x=40.0+b.x;
			int g=int((x*(double)nThreads)/80.0);
			if(g<0)
			{
				g=0;
			}
			if(nThreads<=g)
			{
				g=nThreads-1;
			}
			b.groupId=g;
		}

		std::vector <int> ballPairs[nThreads];
		for(int i=0; i<8; ++i)
		{
			thr[i].Run([&,i]{ComputeBounceParallel(ball,n,i,ballPairs[i]);});
		}
		for(auto &t : thr)
		{
			t.Wait();
		}

		for(auto &pair : ballPairs)
		{
			ComputeBounceSerial(ball,n,pair);
		}



		int wid,hei;
		FsGetWindowSize(wid,hei);
		glViewport(0,0,wid,hei);



		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glBegin(GL_LINES);
		for(int x=0; x<800; x+=100)
		{
			glVertex2i(x,0);
			glVertex2i(x,600);
		}
		glEnd();

		for(int i=0; i<n; ++i)
		{
			double wx,wy;
			Trans(wx,wy,ball[i].x,ball[i].y);
			DrawCircle(wx,wy,10.0);
		}

		FsSwapBuffers();

		// FsSleep(20);  // 
		// std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	return 0;
}
