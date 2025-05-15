#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "fssimplewindow.h"
#include "ysclass.h"
#include "ysgl.h"

const double YsPI=3.1415927;

const char *const vtxShader[1]=
{
"attribute float angle;\n"
"varying vec4 colorOut;\n"
"void main()\n"
"{\n"
"	float r=abs(sin(angle*2));\n"
"	float c=cos(angle);\n"
"	float s=sin(angle);\n"
"\n"
"	gl_Position=vec4(c*sqrt(r),s*sqrt(r),0,1);\n"
"	colorOut=vec4(abs(c),abs(s),1.0,1.0);\n"
"}\n"
};

const char *const fragShader[1]=
{
"varying vec4 colorOut;\n"
"void main()\n"
"{\n"
"    gl_FragColor=colorOut;\n"
"}\n"
};

bool CompileShader(int shaderIdent)
{
	// Fill this function.
    return true;
}

bool LinkShader(GLuint programIdent,GLuint vertexShaderIdent,GLuint fragmentShaderIdent)
{
	// Fill this function.
    return true;
}

std::vector <GLfloat> MakeVertexArray(void)
{
	std::vector <float> vtx;
	// Fill this function.
	return vtx;
}

int main(int argc,char *argv[])
{
	FsOpenWindow(0,0,800,600,1);


	// Compiling and Linking Shaders
	// What needs to be done:
	// (1) Reserve identifiers for vertex and fragment shaders, and a shader program.
	// (2) Assign shader source code for vertex and fragment shaders.
	// (3) Compile shaders.  If there are errors, stop and return 1.  Complete CompileShader function above and use it.
	// (4) Link shaders together to make a shader program.  Complete LinkShader function above and use it.
	// (5) Cache an identifier for the vertex attribute called "angle".



	// Fill MakeVertexArray function so that it makes a vertex array.

	std::cout << "Making Vertex Array." << std::endl;
	auto vtx=MakeVertexArray();


	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}


		int wid,hei;
		FsGetWindowSize(wid,hei);
		double aspect=(double)wid/(double)hei;

		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glViewport(0,0,wid,hei);

		// Render using the GLSL program.
		// What needs to be done:
		// (1) Enable the GLSL program.
		// (2) Enable a vertex-attribute array.
		// (3) Set vertex-attribute pointer.
		// (4) Draw Arrays.
		// (5) Disable vertex-attribute array.


		FsSwapBuffers();

		FsSleep(10);
	}

	printf("*\n");

	return 0;
}

