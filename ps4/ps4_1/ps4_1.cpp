#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>
#include "solnp.hpp"
#include "fssimplewindow.h"

#include "point.h"  // Defines class Point

#include "data.h"   // Defines NPoints, tibia_template, and tibia_patient
/*
tibia_template and tibia_patient are arrays of Point class, both lengths are NPoints.
See also data.cpp.
*/

#include "ffd.h" // Defines FreeFormDeformation class.


FreeFormDeformation ffd[4]; // ffd is an array of 4 FreeFormDeformation objects
Point tibia_deformed[NPoints]; // tibia_deformed is an array of NPoints Point objects


double compare(const int np1, const Point p1[], const int np2, const Point p2[]) {
    double totalDistance = 0.0;
    
    // For each point in the first polygon
    for(int i = 0; i < np1; ++i) {
        double minDist = 1e10;  // Using a large constant instead of numeric_limits
        
        // Find the closest point in the second polygon
        for(int j = 0; j < np2; ++j) {
            double dx = p1[i].x - p2[j].x;
            double dy = p1[i].y - p2[j].y;
            double dist = dx*dx + dy*dy;  // Using squared distance to avoid sqrt
            if (dist < minDist) minDist = dist;
        }
        
        totalDistance += minDist;
    }
    
    // Do the same in reverse to handle cases where polygons have different point distributions
    for(int i = 0; i < np2; ++i) {
        double minDist = 1e10;  // Using a large constant instead of numeric_limits
        
        for(int j = 0; j < np1; ++j) {
            double dx = p2[i].x - p1[j].x;
            double dy = p2[i].y - p1[j].y;
            double dist = dx*dx + dy*dy;
            if (dist < minDist) minDist = dist;
        }
        
        totalDistance += minDist;
    }
    
    return totalDistance / (np1 + np2); // Average distance to normalize
}

// Return value is a 1x1 matrix.
// Input parameter is Nx1 matrix, where N is the number of parameters.
dlib::matrix<double, 1, 1> objective_function(const dlib::matrix<double, 20, 1> &m)
{
	auto ctp=FreeFormDeformation::FFDArrayToControlPoints(4,ffd);
	for(int i=0; i<10; ++i)
	{
		ctp[i].x=m(i*2);
		ctp[i].y=m(i*2+1);
	}

	FreeFormDeformation::ControlPointsToFFDArray(4,ffd,ctp);
	FreeFormDeformation::Apply(tibia_deformed,4,ffd,NPoints,tibia_template);

    dlib::matrix<double,1,1> res;
    res(0)=compare(NPoints,tibia_patient,NPoints,tibia_deformed);

    return res;
}

int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	for(int i=0; i<NPoints; ++i)
	{
		tibia_deformed[i]=tibia_template[i];
	}

	ffd[0].InitRect(Point(300.0,80.0),Point(500.0,190.0));
	ffd[1].InitRect(Point(300.0,190.0),Point(500.0,300.0));
	ffd[2].InitRect(Point(300.0,300.0),Point(500.0,410.0));
	ffd[3].InitRect(Point(300.0,410.0),Point(500.0,520.0));

	int selectedCtp=0;
	std::vector <Point> ctp;

	for(;;)
	{
		// Make 10 control points from an array of FFD boxes.
		ctp=FreeFormDeformation::FFDArrayToControlPoints(4,ffd);

		FsPollDevice();
		auto key=FsInkey();

		if(FSKEY_ESC==key)
		{
			break;
		}
		switch(key)
		{
		case FSKEY_N:
			++selectedCtp;
			selectedCtp%=ctp.size();
			break;
		case FSKEY_P:
			--selectedCtp;
			if(selectedCtp<0)
			{
				selectedCtp+=ctp.size();
			}
			break;
		case FSKEY_LEFT:
			ctp[selectedCtp].x-=10.0;
			FreeFormDeformation::ControlPointsToFFDArray(4,ffd,ctp);
			FreeFormDeformation::Apply(tibia_deformed,4,ffd,NPoints,tibia_template);
			std::cout << compare(NPoints,tibia_patient,NPoints,tibia_deformed) << "\n";
			break;
		case FSKEY_RIGHT:
			ctp[selectedCtp].x+=10.0;
			FreeFormDeformation::ControlPointsToFFDArray(4,ffd,ctp);
			FreeFormDeformation::Apply(tibia_deformed,4,ffd,NPoints,tibia_template);
			std::cout << compare(NPoints,tibia_patient,NPoints,tibia_deformed) << "\n";
			break;
		case FSKEY_UP:
			ctp[selectedCtp].y-=10.0;
			FreeFormDeformation::ControlPointsToFFDArray(4,ffd,ctp);
			FreeFormDeformation::Apply(tibia_deformed,4,ffd,NPoints,tibia_template);
			std::cout << compare(NPoints,tibia_patient,NPoints,tibia_deformed) << "\n";
			break;
		case FSKEY_DOWN:
			ctp[selectedCtp].y+=10.0;
			FreeFormDeformation::ControlPointsToFFDArray(4,ffd,ctp);
			FreeFormDeformation::Apply(tibia_deformed,4,ffd,NPoints,tibia_template);
			std::cout << compare(NPoints,tibia_patient,NPoints,tibia_deformed) << "\n";
			break;

		case FSKEY_SPACE:
			{
			    dlib::matrix<double, 20, 3> parameter_data;
			    // Weird syntax that destroys the beauty of C++, but that's what they like.
			    parameter_data=
		            ctp[0].x,0,800.0,
		            ctp[0].y,0,600.0,
		            ctp[1].x,0,800.0,
		            ctp[1].y,0,600.0,
		            ctp[2].x,0,800.0,
		            ctp[2].y,0,600.0,
		            ctp[3].x,0,800.0,
		            ctp[3].y,0,600.0,
		            ctp[4].x,0,800.0,
		            ctp[4].y,0,600.0,
		            ctp[5].x,0,800.0,
		            ctp[5].y,0,600.0,
		            ctp[6].x,0,800.0,
		            ctp[6].y,0,600.0,
		            ctp[7].x,0,800.0,
		            ctp[7].y,0,600.0,
		            ctp[8].x,0,800.0,
		            ctp[8].y,0,600.0,
		            ctp[9].x,0,800.0,
		            ctp[9].y,0,600.0;
			
			    cppsolnp::SolveResult calculate = cppsolnp::solnp(objective_function, parameter_data);

				for(int i=0; i<10; ++i)
				{
					ctp[i].x=calculate.optimum(i*2);
					ctp[i].y=calculate.optimum(i*2+1);
				}
				FreeFormDeformation::ControlPointsToFFDArray(4,ffd,ctp);
				FreeFormDeformation::Apply(tibia_deformed,4,ffd,NPoints,tibia_template);
			}
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3f(1,0,0);
		glBegin(GL_LINE_LOOP);
		for(int i=0; i<NPoints; ++i)
		{
			glVertex2d(tibia_patient[i].x,tibia_patient[i].y);
		}
		glEnd();

		glColor3f(0,0,1);
		glBegin(GL_LINE_LOOP);
		for(int i=0; i<NPoints; ++i)
		{
			glVertex2d(tibia_deformed[i].x,tibia_deformed[i].y);
		}
		glEnd();

		glColor3f(0,1,0);
		for(auto f : ffd)
		{
			glBegin(GL_LINE_LOOP);
			glVertex2d(f.quad[0].x,f.quad[0].y);
			glVertex2d(f.quad[1].x,f.quad[1].y);
			glVertex2d(f.quad[2].x,f.quad[2].y);
			glVertex2d(f.quad[3].x,f.quad[3].y);
			glEnd();
		}

		glColor3f(0,0,0);
		glPointSize(5);
		glBegin(GL_POINTS);
		if(selectedCtp<ctp.size())
		{
			glVertex2d(ctp[selectedCtp].x,ctp[selectedCtp].y);
		}
		glEnd();

		FsSwapBuffers();
	}
}
