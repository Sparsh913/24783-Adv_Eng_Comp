#include <iostream>
#include <vector>
#include "fssimplewindow.h"


int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	std::vector <float> vtx,col;

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
			vtx.push_back(mx);
			vtx.push_back(my);
			col.push_back(0);
			col.push_back(0);
			col.push_back(0);
			col.push_back(1);
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		{
			const GLfloat vtx[]={0,0,  800.0f,0,  800.0f,600.0f,  0,600.0f};
			const GLfloat col[]={1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT,0,col);
			glVertexPointer(2,GL_FLOAT,0,vtx);
			glDrawArrays(GL_QUADS,0,4);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,0,col.data());
		glVertexPointer(2,GL_FLOAT,0,vtx.data());
		glDrawArrays(GL_LINES,0,vtx.size()/2);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		FsSwapBuffers();
	}
}
