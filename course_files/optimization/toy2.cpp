#include <vector>
#include "fssimplewindow.h"
#include "solnp.hpp"
#include <iostream>


std::vector <int> coords;

void Draw(double a,double b)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPointSize(4);
	glBegin(GL_POINTS);
	for(int i=0; i+1<coords.size(); i+=2)
	{
		glVertex2i(coords[i],coords[i+1]);
	}
	glEnd();

	{
		double x0=0;
		double x1=800.0;
		double y0=a*x0+b;
		double y1=a*x1+b;
		glBegin(GL_LINES);
		glVertex2d(x0,y0);
		glVertex2d(x1,y1);
		glEnd();
	}

	FsSwapBuffers();
}


// Return value is a 1x1 matrix.
// Input parameter is Nx1 matrix, where N is the number of parameters.
dlib::matrix<double, 1, 1> objective_function(const dlib::matrix<double, 2, 1> &m)
{
    double a=m(0);
    double b=m(1);

	double errSum=0;
	for(int i=0; i+1<coords.size(); i+=2)
	{
		double x=coords[i];
		double y=coords[i+1];
		double e=(a*x+b)-y;
		errSum+=e*e;
	}

    dlib::matrix<double,1,1> res;
    res(0)=errSum;

	Draw(a,b);
	FsSleep(10);

    return res;
}

int main(void)
{
	double a=0,b=0;

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
		    dlib::matrix<double, 2, 3> parameter_data;
		    // Weird syntax that destroys the beauty of C++, but that's what they like.
		    parameter_data=
		        0, -100.0,100.0,
		        0, -1000.0,1000.0;
		
		    cppsolnp::SolveResult calculate = cppsolnp::solnp(objective_function, parameter_data);

			a=calculate.optimum(0);
			b=calculate.optimum(1);
		}


		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
			coords.push_back(mx);
			coords.push_back(my);
		}

		Draw(a,b);
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
