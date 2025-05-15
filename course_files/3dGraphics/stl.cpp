// target_link_libraries(your_executable_target ysclass fssimplewindow ...)

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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



bool ReadBinarySTL(
	std::vector <float> &vtx,
	std::vector <float> &nom,
	std::vector <float> &col,
	const char fileName[])
{
#if 1
	// With C++ standard library
	std::ifstream ifp(fileName,std::ios::binary);
	if(ifp.is_open())
	{
		vtx.clear();
		nom.clear();
		col.clear();

		unsigned char buf[80];
		ifp.read((char *)buf,80);

		int nTri;
		ifp.read((char *)&nTri,4);
		std::cout << nTri << " triangles.\n";

		for(int i=0; i<nTri; i++)
		{
			ifp.read((char *)buf,50);

			const float *t=(const float *)buf;
			for(int j=0; j<3; ++j)
			{
				AddXYZ(nom,t[0],t[1],t[2]);
				AddXYZ(vtx,t[3+j*3],t[4+j*3],t[5+j*3]);
				AddRGBA<float>(col,0.8,0.8,0.8,1.0);
			}
		}

		return true;
	}
#else
	// With C standard library
	FILE *fp=fopen(fileName,"rb");
	if(nullptr!=fp)
	{
		vtx.clear();
		nom.clear();
		col.clear();

		unsigned char buf[80];
		fread(buf,1,80,fp);

		int nTri;
		fread(&nTri,1,4,fp);
		std::cout << nTri << " triangles.\n";

		for(int i=0; i<nTri; i++)
		{
			fread(buf,1,50,fp);

			const float *t=(const float *)buf;
			for(int j=0; j<3; ++j)
			{
				AddXYZ(nom,t[0],t[1],t[2]);
				AddXYZ(vtx,t[3+j*3],t[4+j*3],t[5+j*3]);
				AddRGBA<float>(col,0.8,0.8,0.8,1.0);
			}
		}

		fclose(fp);
		return true;
	}
#endif
	return false;
}

void GetBoundingBox(YsVec3 bbx[2],const std::vector <float> &vtx)
{
	if(3<=vtx.size())
	{
		bbx[0].Set(vtx[0],vtx[1],vtx[2]);
		bbx[1].Set(vtx[0],vtx[1],vtx[2]);

		for(size_t i=0; i+2<vtx.size(); i+=3)
		{
			auto x=vtx[i];
			auto y=vtx[i+1];
			auto z=vtx[i+2];

			bbx[0].SetX(std::min<double>(bbx[0].x(),x));
			bbx[0].SetY(std::min<double>(bbx[0].y(),y));
			bbx[0].SetZ(std::min<double>(bbx[0].z(),z));

			bbx[1].SetX(std::max<double>(bbx[1].x(),x));
			bbx[1].SetY(std::max<double>(bbx[1].y(),y));
			bbx[1].SetZ(std::max<double>(bbx[1].z(),z));
		}
	}
	else
	{
		bbx[0]=YsVec3::Origin();
		bbx[1]=YsVec3::Origin();
	}
}

int main(int ac,char *av[])
{
	std::vector <float> vtx,nom,col;
	if(true!=ReadBinarySTL(vtx,nom,col,av[1]))
	{
		std::cout << "STL read error\n";
		return 0;
	}

	YsVec3 bbx[2];
	GetBoundingBox(bbx,vtx);
	auto l=(bbx[1]-bbx[0]).GetLength();

	std::cout << bbx[0].Txt() << "\n";
	std::cout << bbx[1].Txt() << "\n";

	FsOpenWindow(0,0,800,600,1);

	YsVec3 poi=(bbx[0]+bbx[1])/2.0;;
	double cameraDist=l/(2.0*tan(YsPi/8));
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
		gluPerspective(45.0,aspect,cameraDist*0.01,cameraDist+l);

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
		glDrawArrays(GL_TRIANGLES,0,vtx.size()/3);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	    glDisableClientState(GL_NORMAL_ARRAY);

		FsSwapBuffers();
		
		FsSleep(1);
	}
	return 0;
}
