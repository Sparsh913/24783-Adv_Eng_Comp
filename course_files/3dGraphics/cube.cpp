// target_link_libraries(your_executable_target ysclass fssimplewindow ...)

#include <iostream>
#include <vector>
#include "fssimplewindow.h"
#include "ysclass.h"



template<class T>
void AddXYZ(std::vector <T> &vtx,T x,T y,T z)
{
	vtx.push_back(x);
	vtx.push_back(y);
	vtx.push_back(z);
}

template<class T>
void AddRGBA(std::vector <T> &col,T r,T g,T b,T a)
{
	col.push_back(r);
	col.push_back(g);
	col.push_back(b);
	col.push_back(a);
}

template <class T>
void MakeCubeVertexArrays(
	std::vector <T> &vtx,
	std::vector <T> &nom,
	std::vector <T> &col,
	T x1,T y1,T z1,T x2,T y2,T z2)
{
	vtx.clear();
	nom.clear();
	col.clear();

    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddXYZ<T>(nom,0,0,-1);
    AddXYZ<T>(nom,0,0,-1);
    AddXYZ<T>(nom,0,0,-1);
    AddXYZ<T>(nom,0,0,-1);
    AddXYZ(vtx,x1,y1,z1);
    AddXYZ(vtx,x2,y1,z1);
    AddXYZ(vtx,x2,y2,z1);
    AddXYZ(vtx,x1,y2,z1);

    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddXYZ<T>(nom,0,0,1);
    AddXYZ<T>(nom,0,0,1);
    AddXYZ<T>(nom,0,0,1);
    AddXYZ<T>(nom,0,0,1);
    AddXYZ(vtx,x1,y1,z2);
    AddXYZ(vtx,x2,y1,z2);
    AddXYZ(vtx,x2,y2,z2);
    AddXYZ(vtx,x1,y2,z2);

    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddXYZ<T>(nom,0,-1,0);
    AddXYZ<T>(nom,0,-1,0);
    AddXYZ<T>(nom,0,-1,0);
    AddXYZ<T>(nom,0,-1,0);
    AddXYZ(vtx,x1,y1,z1);
    AddXYZ(vtx,x2,y1,z1);
    AddXYZ(vtx,x2,y1,z2);
    AddXYZ(vtx,x1,y1,z2);

    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddXYZ<T>(nom,0,1,0);
    AddXYZ<T>(nom,0,1,0);
    AddXYZ<T>(nom,0,1,0);
    AddXYZ<T>(nom,0,1,0);
    AddXYZ(vtx,x1,y2,z1);
    AddXYZ(vtx,x2,y2,z1);
    AddXYZ(vtx,x2,y2,z2);
    AddXYZ(vtx,x1,y2,z2);

    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddXYZ<T>(nom,-1,0,0);
    AddXYZ<T>(nom,-1,0,0);
    AddXYZ<T>(nom,-1,0,0);
    AddXYZ<T>(nom,-1,0,0);
    AddXYZ(vtx,x1,y1,z1);
    AddXYZ(vtx,x1,y2,z1);
    AddXYZ(vtx,x1,y2,z2);
    AddXYZ(vtx,x1,y1,z2);

    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddRGBA<T>(col,0,0,1,1);
    AddXYZ<T>(nom,1,0,0);
    AddXYZ<T>(nom,1,0,0);
    AddXYZ<T>(nom,1,0,0);
    AddXYZ<T>(nom,1,0,0);
    AddXYZ(vtx,x2,y1,z1);
    AddXYZ(vtx,x2,y2,z1);
    AddXYZ(vtx,x2,y2,z2);
    AddXYZ(vtx,x2,y1,z2);
}

int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	std::vector <float> vtx,nom,col;
	MakeCubeVertexArrays<float>(vtx,nom,col,-10,-10,-10,10,10,10);

	YsVec3 poi(0.0,0.0,0.0);
	double cameraDist=30.0;
	YsMatrix3x3 cameraOrientation;

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}


		if(0!=FsGetKeyState(FSKEY_UP))
		{
			cameraOrientation.RotateYZ(YsPi/60.0);
		}
		if(0!=FsGetKeyState(FSKEY_DOWN))
		{
			cameraOrientation.RotateYZ(-YsPi/60.0);
		}
		if(0!=FsGetKeyState(FSKEY_LEFT))
		{
			cameraOrientation.RotateXZ(YsPi/60.0);
		}
		if(0!=FsGetKeyState(FSKEY_RIGHT))
		{
			cameraOrientation.RotateXZ(-YsPi/60.0);
		}
		if(0!=FsGetKeyState(FSKEY_Z))
		{
			cameraDist/=1.01;
			if(cameraDist<1.0)
			{
				cameraDist=1.0;
			}
		}
		if(0!=FsGetKeyState(FSKEY_M))
		{
			cameraDist*=1.01;
		}


		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
		}


		int wid,hei;
		FsGetWindowSize(wid,hei);
		double aspect=double(wid)/double(hei);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,aspect,0.1,100.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	    GLfloat lightDir[]={0,1/sqrt(2.0f),1/sqrt(2.0f),0};
	    glLightfv(GL_LIGHT0,GL_POSITION,lightDir);
	    glEnable(GL_COLOR_MATERIAL);
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);

		YsMatrix4x4 modelView;
		auto cameraInv=cameraOrientation;
		cameraInv.Invert();
		modelView.Translate(0,0,-cameraDist);
		modelView*=cameraInv;
		modelView.Translate(-poi);

		double glModelView[16];
		modelView.GetOpenGlCompatibleMatrix(glModelView);
		glMultMatrixd(glModelView);

		glEnableClientState(GL_VERTEX_ARRAY);
	    glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,0,col.data());
	    glNormalPointer(GL_FLOAT,0,nom.data());
		glVertexPointer(3,GL_FLOAT,0,vtx.data());
		glDrawArrays(GL_QUADS,0,vtx.size()/3);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	    glDisableClientState(GL_NORMAL_ARRAY);

		FsSwapBuffers();
		
		FsSleep(1);
	}
	return 0;
}
