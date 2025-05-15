#include <iostream>
#include <fstream>
#include <string>
#include "ysgl.h"
#include "fssimplewindow.h"
#include "renderer.h"



int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	FsChangeToProgramDir();

	PassThroughRenderer passThrough;
	if(true!=passThrough.CompileFile("passThroughVtx.glsl","passThroughFrag.glsl"))
	{
		return 1;
	}

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	    glUseProgram(passThrough.programIdent);
	    const GLfloat vtx[12]=
	    {
	        -1,-1,0,
	         1,-1,0,
	         1, 1,0,
	        -1, 1,0
	    };
	    glEnableVertexAttribArray(passThrough.attribVertexPos);
	    glVertexAttribPointer(passThrough.attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
	    glDrawArrays(GL_TRIANGLE_FAN,0,4);
	    glDisableVertexAttribArray(passThrough.attribVertexPos);

		FsSwapBuffers();
	}

	return 0;
}
