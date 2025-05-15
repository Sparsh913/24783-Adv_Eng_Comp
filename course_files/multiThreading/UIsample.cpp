#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <stdio.h>
#include "fssimplewindow.h"

int main(void)
{
	float color[3]={0,0,0};
	std::mutex mtx;
	bool close=false;

	FsOpenWindow(0,0,800,600,1);

	std::thread t([&]()
	{
		for(;;)
		{
			std::string cmd;
			std::cout << ">";
			std::getline(std::cin,cmd);

			if("R"==cmd)
			{
				mtx.lock();
				color[0]=1;
				color[1]=0;
				color[2]=0;
				mtx.unlock();
			}
			else if("G"==cmd)
			{
				mtx.lock();
				color[0]=0;
				color[1]=1;
				color[2]=0;
				mtx.unlock();
			}
			else if("Q"==cmd)
			{
				mtx.lock();
				close=true;
				mtx.unlock();
			}

			mtx.lock();
			if(true==close)
			{
				break;
			}
			mtx.unlock();
		}
	});

	int x=400,y=300,vx=10,vy=10;
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		mtx.lock();
		const auto copy_of_close=close;
		mtx.unlock();
		if(true==copy_of_close)
		{
			break;
		}

		x+=vx;
		y+=vy;
		if((800<=x && 0<vx) || (x<=0 && vx<0))
		{
			vx=-vx;
		}
		if((600<=y && 0<vy) || (y<=0 && vy<0))
		{
			vy=-vy;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		mtx.lock();
		glColor3fv(color);
		mtx.unlock();
		glBegin(GL_QUADS);
		glVertex2i(x-10,y);
		glVertex2i(x,y-10);
		glVertex2i(x+10,y);
		glVertex2i(x,y+10);
		glEnd();
		FsSwapBuffers();

		FsSleep(1);
	}

	mtx.lock();
	close=true;
	mtx.unlock();
	t.join();
	std::cout << "End of main.\n";
}
