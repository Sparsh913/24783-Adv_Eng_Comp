#include <vector>
#include "fssimplewindow.h"
#include "solnp.hpp"
#include <iostream>


const double eps=1e-6;


class Point
{
public:
	double x,y;

	Point(){}
	Point(double X,double Y)
	{
		x=X;
		y=Y;
	}
	double SquareLength(void) const
	{
		return x*x+y*y;
	}
	bool operator!=(Point incoming)
	{
		return fabs(x-incoming.x)>eps || fabs(y-incoming.y)>eps;
	}
};
Point operator*(Point a,double b)
{
	a.x*=b;
	a.y*=b;
	return a;
}
Point operator*(double b,Point a)
{
	a.x*=b;
	a.y*=b;
	return a;
}
Point operator+(Point a,Point b)
{
	a.x+=b.x;
	a.y+=b.y;
	return a;
}
Point operator-(Point a,Point b)
{
	a.x-=b.x;
	a.y-=b.y;
	return a;
}

Point CubicBezier(const Point ctp[4],double t)
{
	Point a[3]=
	{
		ctp[0]*(1-t)+ctp[1]*t,
		ctp[1]*(1-t)+ctp[2]*t,
		ctp[2]*(1-t)+ctp[3]*t,
	};

	Point b[2]=
	{
		a[0]*(1-t)+a[1]*t,
		a[1]*(1-t)+a[2]*t,
	};

	return b[0]*(1-t)+b[1]*t;
}





Point ctp[4]={Point(0,0),Point(0,0),Point(0,0),Point(0,0)};
std::vector <Point> stroke;

void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
	for(auto p : ctp)
	{
		glVertex2d(p.x,p.y);
	}
	glEnd();

	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<=20; ++i)
	{
		double t=(double)i/20.0;
		auto p=CubicBezier(ctp,t);
		glVertex2d(p.x,p.y);
	}
	glEnd();

	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	for(auto p : stroke)
	{
		glVertex2d(p.x,p.y);
	}
	glEnd();

	FsSwapBuffers();
}


// Return value is a 1x1 matrix.
// Input parameter is Nx1 matrix, where N is the number of parameters.
dlib::matrix<double, 1, 1> objective_function(const dlib::matrix<double, 4, 1> &m)
{
	ctp[0]=stroke[0];
	ctp[1]=Point(m(0),m(1));
	ctp[2]=Point(m(2),m(3));
	ctp[3]=stroke.back();

	double errSum=0;
	for(int i=0; i<=20; ++i)
	{
		double t=(double)i/20.0;
		auto p=CubicBezier(ctp,t);

		double minDist=10000000.0;
		for(auto q : stroke)
		{
			auto dist=(p-q).SquareLength();
			minDist=std::min(dist,minDist);
		}
		errSum+=minDist;
	}

    dlib::matrix<double,1,1> res;
    res(0)=errSum;

	Draw();
	FsSleep(10);

    return res;
}

int main(void)
{
	int counter=0;
	FsOpenWindow(0,0,800,600,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		if(FSKEY_SPACE==key)
		{
		}


		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		// if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		// {
		// 	ctp[counter].x=mx;
		// 	ctp[counter].y=my;
		// 	counter=(counter+1)%4;
		// }

		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
			stroke.clear();
		}
		if(0!=lb && (0==stroke.size() || stroke.back()!=Point(mx,my)))
		{
			Point p(mx,my);
			stroke.push_back(p);
		}
		if(FSMOUSEEVENT_LBUTTONUP==evt && 0<stroke.size())
		{
			Point ctp[4]=
			{
				stroke[0],
				stroke[stroke.size()/3],
				stroke[stroke.size()*2/3],
				stroke.back()
			};

		    dlib::matrix<double, 4, 3> parameter_data;
		    // Weird syntax that destroys the beauty of C++, but that's what they like.
		    parameter_data=
		        ctp[1].x, -800.0,1600.0,
		        ctp[1].y, -600.0,1200.0,
		        ctp[2].x, -800.0,1600.0,
		        ctp[2].y, -600.0,1200.0;
		
		    cppsolnp::SolveResult calculate = cppsolnp::solnp(objective_function, parameter_data);

	        ::ctp[0]=stroke[0];
	        ::ctp[1].x=calculate.optimum(0);
	        ::ctp[1].y=calculate.optimum(1);
	        ::ctp[2].x=calculate.optimum(2);
	        ::ctp[2].y=calculate.optimum(3);
	        ::ctp[3]=stroke.back();

		    // double minimum=calculate.solve_value;
		}

		Draw();
	}

//    // 2 parameters
//    // 3 values for each parameter, initial value, minimum, maximum
//    dlib::matrix<double, 2, 3> parameter_data;
//    // Weird syntax that destroys the beauty of C++, but that's what they like.
//    parameter_data=
//        0, -10.0,10.0,
//        0, -10.0,10.0;
//
//    cppsolnp::SolveResult calculate = cppsolnp::solnp(objective_function, parameter_data);
//
//    double x=calculate.optimum(0);
//    double y=calculate.optimum(1);
//    double minimum=calculate.solve_value;
//
//    std::cout << x << " " << y << " " << minimum << "\n";
//
    return 0;
}
