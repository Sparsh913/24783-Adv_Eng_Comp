#include <iostream>
#include "fssimplewindow.h"


int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glShadeModel(GL_SMOOTH);

		const GLfloat vtx[]={0,0,  800.0f,0,  800.0f,600.0f,  0,600.0f};
		const GLfloat col[]={1,0,0,1,  0,1,0,1,  0,0,1,1,  1,1,0,1};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,0,col);
		glVertexPointer(2,GL_FLOAT,0,vtx);
		glDrawArrays(GL_QUADS,0,4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		FsSwapBuffers();
	}
}
