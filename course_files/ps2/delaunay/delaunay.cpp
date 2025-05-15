#include "fssimplewindow.h"
#include "ysclass.h"
#include "ysdelaunaytri.h"
#include "ysglfontdata.h"

int main(void)
{
	YsUnconstrainedDelaunayTriangulation del;
	FsOpenWindow(0,0,800,600,1);

	const int iniNode[4]=
	{
		del.AddInitialNode(YsVec2(0.0,0.0)),
		del.AddInitialNode(YsVec2(800,0)),
		del.AddInitialNode(YsVec2(800,600)),
		del.AddInitialNode(YsVec2(0,600)),
	};
	const int iniTri[2][3]=
	{
		{iniNode[0],iniNode[1],iniNode[2]},
		{iniNode[2],iniNode[3],iniNode[0]}
	};
	del.AddInitialTri(iniTri[0]);
	del.AddInitialTri(iniTri[1]);

	for(;;)
	{
		FsPollDevice();

		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
			del.InsertNode(YsVec2(mx,my));
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3f(0,0,0);
		glBegin(GL_LINES);
		for(int i=0; i<del.GetNumTri(); ++i)
		{
			YsVec2 tri[3];
			del.GetTriangleNodePos(tri,i);
			glVertex2dv(tri[0]);
			glVertex2dv(tri[1]);

			glVertex2dv(tri[1]);
			glVertex2dv(tri[2]);

			glVertex2dv(tri[2]);
			glVertex2dv(tri[0]);
		}
		glEnd();

		glColor3f(0,0,0);
		glRasterPos2i(0,32);
		YsGlDrawFontBitmap20x28("Delaunay Triangulation");
		glRasterPos2i(0,60);
		YsGlDrawFontBitmap20x28("ESC.....Close");
		glRasterPos2i(0,88);
		YsGlDrawFontBitmap20x28("Left Button...Insert Node");

		FsSwapBuffers();
	}
}
