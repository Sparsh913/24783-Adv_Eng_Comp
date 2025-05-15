#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "fssimplewindow.h"
#include "ysdelaunaytri.h"


const double YsPI=3.1415927;
const int nThreads=8;

class BubbleForcePair
{
public:
	size_t i; // Index to a bubble.
	YsVec2 f; // Force to be added.
};

class BubblePacking
{
public:
	const double h=0.03;
	const double m=1.0;
	const double overlap=1.1;
	const double eps=1e-6;
	const double C=4.0; // Damping coefficient
	const double K=0.5; // Spring coefficient

	class Bubble
	{
	public:
		YsVec2 pos,vel,f;
		double rad;
		bool fixed=false;
		int groupId=0;
	};

private:
	std::vector <Bubble> bubbles;

public:
	void PackOnCorners(void);
	void PackOnBoundary(void);
	void PackInside(void);

	void AssignGroupId(void);

	void ClearForce(void);
	void ComputeInterBubbleForce(void);
	void ComputeDampingForce(void);
	void Move(double dt);

	void MakeDelaunay(YsUnconstrainedDelaunayTriangulation &tri);
	void Render(void) const;
	static void DrawCircle(YsVec2 cen,double rad);
};
void BubblePacking::PackOnCorners(void)
{
	Bubble bub;
	bub.rad=h/2;
	bub.vel=YsVec2::Origin();
	bub.f=YsVec2::Origin();
	bub.fixed=true;

	bub.pos.Set(-1.0,-1.0);
	bubbles.push_back(bub);
	bub.pos.Set( 1.0,-1.0);
	bubbles.push_back(bub);
	bub.pos.Set( 1.0, 1.0);
	bubbles.push_back(bub);
	bub.pos.Set(-1.0, 1.0);
	bubbles.push_back(bub);
}
void BubblePacking::PackOnBoundary(void)
{
	YsVec2 corner[5]=
	{
		YsVec2(-1.0,-1.0),
		YsVec2( 1.0,-1.0),
		YsVec2( 1.0, 1.0),
		YsVec2(-1.0, 1.0),
		YsVec2(-1.0,-1.0),
	};
	for(int i=0; i<4; ++i)
	{
		double d=(corner[i+1]-corner[i]).GetLength();
		int n=int(d*overlap/h);
		for(int j=1; j<n; ++j)
		{
			double t=double(j)/double(n);
			auto pos=corner[i]*(1.0-t)+corner[i+1]*t;

			Bubble bub;
			bub.pos=pos;
			bub.rad=h/2;
			bub.vel=YsVec2::Origin();
			bub.f=YsVec2::Origin();
			bub.fixed=true;

			bubbles.push_back(bub);
		}
	}
}
void BubblePacking::PackInside(void)
{
	int n=int(2.0*overlap/h);
	n*=n;
	for(int i=0; i<n; ++i)
	{
		double x=rand();
		double y=rand();
		x/=double(RAND_MAX);
		y/=double(RAND_MAX);
		x*=2.0;
		y*=2.0;
		x-=1.0;
		y-=1.0;
		x*=(1.0-h);
		y*=(1.0-h);

		Bubble bub;
		bub.pos.Set(x,y);
		bub.rad=h/2;
		bub.vel=YsVec2::Origin();
		bub.f=YsVec2::Origin();
		bub.fixed=false;

		bubbles.push_back(bub);
	}
}
void BubblePacking::AssignGroupId(void)
{
}
void BubblePacking::ClearForce(void)
{
	for(auto &bub : bubbles)
	{
		bub.f=YsVec2::Origin();
	}
}
void BubblePacking::ComputeInterBubbleForce(void)
{
	for(size_t i=0; i<bubbles.size(); ++i)
	{
		auto &bubA=bubbles[i];
		for(size_t j=i+1; j<bubbles.size(); ++j)
		{
			auto &bubB=bubbles[j];
			if(true!=bubB.fixed || true!=bubA.fixed)
			{
				auto dist=(bubA.pos-bubB.pos).GetLength();
				auto refDist=bubA.rad+bubB.rad;
				if(dist<refDist)
				// The original bubble packing applies attracting forces for refDist<dist<refDist*1.5
				// It requires Delaunay triangulation to find neighbors.
				// For simplicity, this example only uses repulsive forces.
				{
					double x=dist/refDist;
					double y=K*(1.25*x*x*x-2.375*x*x+1.125);

					YsVec2 u=bubB.pos-bubA.pos;
					double l=u.GetLength();
					if(l<eps)
					{
						u.SetX(1);
					}
					else
					{
						u/=l;
					}
					bubA.f-=u*y;
					bubB.f+=u*y;
				}
			}
		}
	}
}
void BubblePacking::ComputeDampingForce(void)
{
	for(auto &bub : bubbles)
	{
		bub.f-=C*bub.vel;
	}
}
void BubblePacking::Move(double dt)
{
	for(auto &bub : bubbles)
	{
		if(true!=bub.fixed)
		{
			bub.pos+=bub.vel*dt;
			bub.vel+=(bub.f/m)*dt;
		}
	}
}
void BubblePacking::Render(void) const
{
	for(auto &bub : bubbles)
	{
		DrawCircle(bub.pos,bub.rad);
	}
}

void BubblePacking::DrawCircle(YsVec2 cen,double rad)
{
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<32; ++i)
	{
		double a=YsPI*double(i)/16.0;
		double c=cos(a);
		double s=sin(a);
		glVertex2d(cen.x()+c*rad,cen.y()+s*rad);
	}
	glEnd();
}

void BubblePacking::MakeDelaunay(YsUnconstrainedDelaunayTriangulation &tri)
{
	tri.CleanUp();
	const int iniNode[4]=
	{
		tri.AddInitialNode(YsVec2(-2.0,-2.0)),
		tri.AddInitialNode(YsVec2( 2.0,-2.0)),
		tri.AddInitialNode(YsVec2( 2.0, 2.0)),
		tri.AddInitialNode(YsVec2(-2.0, 2.0)),
	};
	const int iniTri[2][3]=
	{
		{iniNode[0],iniNode[1],iniNode[2]},
		{iniNode[2],iniNode[3],iniNode[0]}
	};
	tri.AddInitialTri(iniTri[0]);
	tri.AddInitialTri(iniTri[1]);
	for(auto &bub : bubbles)
	{
		tri.InsertNode(bub.pos);
	}
}

int main(void)
{
	FsOpenWindow(0,0,720,720,1);

	srand(time(nullptr));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.1,1.1,-1.1,1.1,-1.1,1.1);

	bool pause=false;

	YsUnconstrainedDelaunayTriangulation tri;

	BubblePacking packing;
	packing.PackOnCorners();
	packing.PackOnBoundary();
	packing.PackInside();

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
			packing.MakeDelaunay(tri);
			pause=!pause;
		}

		if(true!=pause)
		{
			packing.AssignGroupId();
			for(int i=0; i<10; ++i)
			{
				packing.ClearForce();
				packing.ComputeInterBubbleForce();
				packing.ComputeDampingForce();
				packing.Move(0.01);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		if(true!=pause)
		{
			packing.Render();
		}
		else
		{
			// If paused, show Delaunay triangulation.
			glBegin(GL_LINES);
			for(int i=0; i<tri.GetNumTri(); ++i)
			{
				YsVec2 p[3];
				tri.GetTriangleNodePos(p,i);
				glVertex2dv(p[0]);
				glVertex2dv(p[1]);

				glVertex2dv(p[1]);
				glVertex2dv(p[2]);

				glVertex2dv(p[2]);
				glVertex2dv(p[0]);
			}
			glEnd();
		}
		FsSwapBuffers();
	}

	return 0;
}
