#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>

// Need to define this before including GL headers to prevent redefinition issues
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include "ysgl.h"
#include "fssimplewindow.h"
#include "ysclass.h"

const double YsPI=3.1415927;

// Vertex shader code
const char *const vtxShader[1]=
{
"attribute float angle;\n"
"varying vec4 colorOut;\n"
"void main()\n"
"{\n"
"	float r=abs(sin(angle*2.0));\n"
"	float c=cos(angle);\n"
"	float s=sin(angle);\n"
"\n"
"	gl_Position=vec4(c*sqrt(r),s*sqrt(r),0.0,1.0);\n"
"	colorOut=vec4(abs(c),abs(s),1.0,1.0);\n"
"}\n"
};

// Fragment shader code
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
    GLint compilationStatus;
    glCompileShader(shaderIdent);
    glGetShaderiv(shaderIdent, GL_COMPILE_STATUS, &compilationStatus);
    
    if(GL_TRUE!=compilationStatus)
    {
        GLint infoLogLength;
        glGetShaderiv(shaderIdent, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(0<infoLogLength)
        {
            char *infoLog=new char[infoLogLength];
            GLsizei actualLength;
            glGetShaderInfoLog(shaderIdent, infoLogLength, &actualLength, infoLog);
            std::cout << "Shader compilation error:" << std::endl;
            std::cout << infoLog << std::endl;
            delete [] infoLog;
        }
        return false;
    }
    return true;
}

bool LinkShader(GLuint programIdent, GLuint vertexShaderIdent, GLuint fragmentShaderIdent)
{
    glAttachShader(programIdent, vertexShaderIdent);
    glAttachShader(programIdent, fragmentShaderIdent);
    glLinkProgram(programIdent);
    
    GLint linkStatus;
    glGetProgramiv(programIdent, GL_LINK_STATUS, &linkStatus);
    if(GL_TRUE!=linkStatus)
    {
        GLint infoLogLength;
        glGetProgramiv(programIdent, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(0<infoLogLength)
        {
            char *infoLog=new char[infoLogLength];
            GLsizei actualLength;
            glGetProgramInfoLog(programIdent, infoLogLength, &actualLength, infoLog);
            std::cout << "Shader link error:" << std::endl;
            std::cout << infoLog << std::endl;
            delete [] infoLog;
        }
        return false;
    }
    return true;
}

std::vector<GLfloat> MakeVertexArray(void)
{
    std::vector<GLfloat> vtx;
    // Create an array of angles from 0 to 2π
    for(int i = 0; i < 360; ++i)
    {
        // Fix the double to float conversion warning by using explicit cast
        GLfloat angle = static_cast<GLfloat>(i * YsPI / 180.0);
        vtx.push_back(angle);
    }
    return vtx;
}

int main(int argc, char *argv[])
{
    FsOpenWindow(0, 0, 800, 600, 1);

    // (1) Reserve identifiers for vertex and fragment shaders, and a shader program
    GLuint vertexShaderIdent = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderIdent = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint programIdent = glCreateProgram();

    // (2) Assign shader source code for vertex and fragment shaders
    glShaderSource(vertexShaderIdent, 1, vtxShader, NULL);
    glShaderSource(fragmentShaderIdent, 1, fragShader, NULL);

    // (3) Compile shaders
    if(!CompileShader(vertexShaderIdent) || !CompileShader(fragmentShaderIdent))
    {
        std::cout << "Failed to compile shaders." << std::endl;
        return 1;
    }

    // (4) Link shaders to make a shader program
    if(!LinkShader(programIdent, vertexShaderIdent, fragmentShaderIdent))
    {
        std::cout << "Failed to link shader program." << std::endl;
        return 1;
    }

    // (5) Cache an identifier for the vertex attribute called "angle"
    GLint angleAttribLocation = glGetAttribLocation(programIdent, "angle");
    if(angleAttribLocation < 0)
    {
        std::cout << "Cannot find attribute angle." << std::endl;
        return 1;
    }

    // Create vertex array
    std::cout << "Making Vertex Array." << std::endl;
    auto vtx = MakeVertexArray();

    for(;;)
    {
        FsPollDevice();
        auto key = FsInkey();
        if(FSKEY_ESC == key)
        {
            break;
        }

        int wid, hei;
        FsGetWindowSize(wid, hei);
        double aspect = (double)wid / (double)hei;

        int lb, mb, rb, mx, my;
        auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, wid, hei);

        // Render using the GLSL program
        // (1) Enable the GLSL program
        glUseProgram(programIdent);
        
        // (2) Enable vertex-attribute array
        glEnableVertexAttribArray(angleAttribLocation);
        
        // (3) Set vertex-attribute pointer
        glVertexAttribPointer(angleAttribLocation, 1, GL_FLOAT, GL_FALSE, 0, vtx.data());
        
        // (4) Draw Arrays
        glDrawArrays(GL_LINE_LOOP, 0, vtx.size());
        
        // (5) Disable vertex-attribute array
        glDisableVertexAttribArray(angleAttribLocation);
        
        // Disable the program
        glUseProgram(0);

        FsSwapBuffers();

        FsSleep(10);
    }

    // Clean up
    glDeleteShader(vertexShaderIdent);
    glDeleteShader(fragmentShaderIdent);
    glDeleteProgram(programIdent);

    printf("*\n");

    return 0;
}