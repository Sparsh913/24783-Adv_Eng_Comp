// target_link_libraries(your_executable_target ysclass fssimplewindow ...)

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "fssimplewindow.h"
#include "ysclass.h"
#include "mesh.h"
#include "glutil.h"

const double epsilon=1e-6;

void WindowCoordToLine(YsVec3 pos[2],int wx,int wy,const YsMatrix4x4 &modelView,const YsMatrix4x4 &projection)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	pos[0]=WindowToViewPort(wid,hei,wx,wy);
	pos[1]=pos[0];

	pos[0].SetZ(-1.0);
	pos[1].SetZ( 1.0);

	projection.MulInverse(pos[0],pos[0],1.0);
	projection.MulInverse(pos[1],pos[1],1.0);

	modelView.MulInverse(pos[0],pos[0],1.0);
	modelView.MulInverse(pos[1],pos[1],1.0);
}

PolygonalMesh::PolygonHandle PickedPlHd(
	const PolygonalMesh &mesh,
	int mx,int my,
	const YsMatrix4x4 &modelView,const YsMatrix4x4 &projection)
{
	YsVec3 pos[2];
	WindowCoordToLine(pos,mx,my,modelView,projection);

	double nearDist=0.0;
	PolygonalMesh::PolygonHandle nearPlHd=mesh.NullPolygon();

	for(auto plHd=mesh.FirstPolygon(); mesh.NullPolygon()!=plHd; mesh.MoveToNext(plHd))
	{
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		if(3<=plVtHd.size())
		{
			YsVec3 trVtPos[3]=
			{
				mesh.GetVertexPosition(plVtHd[0]),
				mesh.GetVertexPosition(plVtHd[1]),
				mesh.GetVertexPosition(plVtHd[2]),
			};

			YsVec3 nom=(trVtPos[1]-trVtPos[0])^(trVtPos[2]-trVtPos[0]);
			auto Lnom=nom.GetLength();
			if(epsilon<=Lnom)
			{
				nom/=Lnom;

				YsPlane pln(trVtPos[0],nom);
				YsVec3 itsc;
				if(YSOK==pln.GetIntersection(itsc,pos[0],pos[1]-pos[0]))
				{
					if(0.0<=(itsc-pos[0])*(pos[1]-pos[0]))
					{
						auto side=YsCheckInsidePolygon3(itsc,3,trVtPos);
						if(YSINSIDE==side)
						{
							double dist=(pos[0]-itsc).GetSquareLength();
							if(mesh.NullPolygon()==nearPlHd ||
							   dist<nearDist)
							{
								nearPlHd=plHd;
								nearDist=dist;
							}
						}
					}
				}
			}
		}
	}
	return nearPlHd;
}

PolygonalMesh::VertexHandle PickedVtHd(
	const PolygonalMesh &mesh,
	int mx,int my,
	const YsMatrix4x4 &modelView,const YsMatrix4x4 &projection)
{
	double nearDist=0;
	PolygonalMesh::VertexHandle nearVtHd=mesh.NullVertex();

	int wid,hei;
	FsGetWindowSize(wid,hei);

	for(auto vtHd=mesh.FirstVertex();
	    mesh.NullVertex()!=vtHd;
	    mesh.MoveToNext(vtHd))
	{
		auto pos=mesh.GetVertexPosition(vtHd);
		auto posv=projection*modelView*pos;
		auto posw=ViewPortToWindow(wid,hei,posv);

		auto dist=posv.z();
		if(-1.0<=dist)
		{
			auto dx=posw.x()-mx;
			auto dy=posw.y()-my;
			if(-5<=dx && dx<=5 && -5<=dy && dy<=5)
			{
				if(mesh.NullVertex()==nearVtHd ||
				   dist<nearDist)
				{
					nearVtHd=vtHd;
					nearDist=dist;
				}
			}
		}
	}

	return nearVtHd;
}

void MakeDualVertexArrays(
    std::vector <float> &vtx,
    std::vector <float> &col,
    const PolygonalMesh &mesh)
{
	vtx.clear();
	col.clear();
	for(auto plHd=mesh.FirstPolygon();
	    mesh.NullPolygon()!=plHd;
	    mesh.MoveToNext(plHd))
	{
		auto cen=mesh.GetCenter(plHd);
		for(int i=0; i<mesh.GetPolygonNumVertices(plHd); ++i)
		{
			auto neiPlHd=mesh.GetNeighborPolygon(plHd,i);
			if(mesh.NullPolygon()!=neiPlHd)
			{
				auto cen2=mesh.GetCenter(neiPlHd);
				vtx.push_back(cen.x());
				vtx.push_back(cen.y());
				vtx.push_back(cen.z());
				vtx.push_back(cen2.x());
				vtx.push_back(cen2.y());
				vtx.push_back(cen2.z());
				col.push_back(0);
				col.push_back(0);
				col.push_back(0);
				col.push_back(1);
				col.push_back(0);
				col.push_back(0);
				col.push_back(0);
				col.push_back(1);
			}
		}
	}
}

void MakeLowDihedralAngle(
   std::vector <float> &lowDhaVtx,std::vector <float> &lowDhaCol,const PolygonalMesh &mesh)
{
	lowDhaVtx.clear();
	lowDhaCol.clear();
	for(auto plHd=mesh.FirstPolygon();
	    mesh.NullPolygon()!=plHd;
	    mesh.MoveToNext(plHd))
	{
		auto nom0=mesh.GetNormal(plHd);
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		for(int i=0; i<plVtHd.size(); ++i)
		{
			auto neiPlHd=mesh.GetNeighborPolygon(plHd,i);
			if(mesh.NullPolygon()!=neiPlHd)
			{
				if(mesh.GetSearchKey(plHd)<mesh.GetSearchKey(neiPlHd))
				{
					auto nom1=mesh.GetNormal(neiPlHd);
					if(nom0*nom1<0.5)
					{
						auto vtPos1=mesh.GetVertexPosition(plVtHd[i]);
						auto vtPos2=mesh.GetVertexPosition(plVtHd[(i+1)%plVtHd.size()]);
						lowDhaVtx.push_back(vtPos1.x());
						lowDhaVtx.push_back(vtPos1.y());
						lowDhaVtx.push_back(vtPos1.z());
						lowDhaVtx.push_back(vtPos2.x());
						lowDhaVtx.push_back(vtPos2.y());
						lowDhaVtx.push_back(vtPos2.z());
						lowDhaCol.push_back(0);
						lowDhaCol.push_back(0);
						lowDhaCol.push_back(0);
						lowDhaCol.push_back(1);
						lowDhaCol.push_back(0);
						lowDhaCol.push_back(0);
						lowDhaCol.push_back(0);
						lowDhaCol.push_back(1);
					}
				}
			}
		}
	}
}

std::vector <PolygonalMesh::PolygonHandle> GetNNeighbor(
    const PolygonalMesh &mesh,
    PolygonalMesh::PolygonHandle plHd0,
    int N)
{
	std::vector <PolygonalMesh::PolygonHandle> plHds;
	std::unordered_map<unsigned int,unsigned int> visited;
	plHds.push_back(plHd0);
	visited[mesh.GetSearchKey(plHd0)]=0;
	for(size_t i=0; i<plHds.size(); ++i)
	{
		auto plHd=plHds[i];
		if(visited[mesh.GetSearchKey(plHd)]<N)
		{
			for(int j=0; j<mesh.GetPolygonNumVertices(plHd); ++j)
			{
				auto neiPlHd=mesh.GetNeighborPolygon(plHd,j);
				if(mesh.NullPolygon()!=neiPlHd)
				{
					auto found=visited.find(mesh.GetSearchKey(neiPlHd));
					if(found==visited.end())
					{
						auto dist=visited[mesh.GetSearchKey(plHd)];
						visited[mesh.GetSearchKey(neiPlHd)]=dist+1;
						plHds.push_back(neiPlHd);
					}
				}
			}
		}
	}
	return plHds;
}

std::vector <PolygonalMesh::VertexHandle> GetVertexesWithinRadius(
    const PolygonalMesh &mesh,
    PolygonalMesh::VertexHandle vtHd0,
    double radius)
{
	std::unordered_set <unsigned int> visited;
	std::vector <PolygonalMesh::VertexHandle> todo;

	todo.push_back(vtHd0);
	visited.insert(mesh.GetSearchKey(vtHd0));
	auto pos0=mesh.GetVertexPosition(vtHd0);

	while(0<todo.size())
	{
		auto vtHd=todo.back();
		todo.pop_back();

		for(auto neiVtHd : mesh.GetConnectedVertex(vtHd))
		{
			auto neiPos=mesh.GetVertexPosition(neiVtHd);
			if(visited.find(mesh.GetSearchKey(neiVtHd))==visited.end() &&
			   (neiPos-pos0).GetSquareLength()<radius*radius)
			{
				todo.push_back(neiVtHd);
				visited.insert(mesh.GetSearchKey(neiVtHd));
			}
		}
	}

	std::vector <PolygonalMesh::VertexHandle> vtHds;
	for(auto key : visited)
	{
		vtHds.push_back(mesh.FindVertex(key));
	}

	return vtHds;
}


void Paint16(PolygonalMesh &mesh)
{
	YsColor col[16];
	for(int i=0; i<16; ++i)
	{
		int r=i%2;
		int g=(i/2)%2;
		int b=(i/4)%2;

		double R=double(r);
		double G=double(g);
		double B=double(b);

		if(8<=i)
		{
			R/=2.0;
			G/=2.0;
			B/=2.0;
		}

		col[i].SetDoubleRGBA(R,G,B,1.0);

		printf("%lf %lf %lf\n",R,G,B);
	}


	for(auto plHd=mesh.FirstPolygon();
	    mesh.NullPolygon()!=plHd;
	    mesh.MoveToNext(plHd))
	{
		mesh.SetColor(plHd,col[0]);
	}

	for(auto plHd=mesh.FirstPolygon();
	    mesh.NullPolygon()!=plHd;
	    mesh.MoveToNext(plHd))
	{
		bool used[16];
		for(auto &u : used)
		{
			u=false;
		}

		for(int i=0; i<mesh.GetPolygonNumVertices(plHd); ++i)
		{
			auto neiPlHd=mesh.GetNeighborPolygon(plHd,i);
			if(mesh.NullPolygon()!=neiPlHd)
			{
				auto c=mesh.GetColor(neiPlHd);
				for(int j=0; j<16; ++j)
				{
					if(col[j]==c)
					{
						used[j]=true;
						break;
					}
				}
			}
		}

		for(int i=0; i<16; ++i)
		{
			if(used[i]!=true)
			{
				mesh.SetColor(plHd,col[i]);
				break;
			}
		}
	}
};


int main(int ac,char *av[])
{
	int mode=0; // 0:Normal  1:Dual  2:Low Dihedral Angle

	PolygonalMesh mesh;

	if(true!=mesh.LoadBinStl(av[1]))
	{
		std::cout << "STL read error\n";
		return 0;
	}

	mesh.Stitch();

	std::cout << "Number of Vertices " << mesh.GetNumVertices() << "\n";
	std::cout << "Number of Polygons " << mesh.GetNumPolygons() << "\n";

	auto vtxArrays=mesh.MakeVertexArrays();
	std::vector <float> dualVtx,dualCol;
	std::vector <float> lowDhaVtx,lowDhaCol;

	YsVec3 bbx[2];
	mesh.GetBoundingBox(bbx);
	auto l=(bbx[1]-bbx[0]).GetLength();

	std::cout << bbx[0].Txt() << "\n";
	std::cout << bbx[1].Txt() << "\n";

	FsOpenWindow(0,0,800,600,1);

	YsVec3 poi=(bbx[0]+bbx[1])/2.0;;
	double cameraDist=l/(2.0*tan(YsPi/8));
	YsMatrix3x3 cameraOrientation;

	YsVec3 pickedLine[2]={YsVec3::Origin(),YsVec3::Origin()};
	YsVec3 highlightPos=YsVec3::Origin();
	auto selectedVtHd=mesh.NullVertex();
	std::vector <PolygonalMesh::VertexHandle> connVtHd;

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}
		switch(key)
		{
		case FSKEY_SPACE:
			Paint16(mesh);
			vtxArrays=mesh.MakeVertexArrays();
			break;
		case FSKEY_M:
			mode=(mode+1)%3;
			if(1==mode)
			{
				MakeDualVertexArrays(dualVtx,dualCol,mesh);
			}
			else if(2==mode)
			{
				MakeLowDihedralAngle(lowDhaVtx,lowDhaCol,mesh);
			}
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

		int wid,hei;
		FsGetWindowSize(wid,hei);
		double aspect=double(wid)/double(hei);

		YsMatrix4x4 modelView;
		auto cameraInv=cameraOrientation;
		cameraInv.Invert();
		modelView.Translate(0,0,-cameraDist);
		modelView*=cameraInv;
		modelView.Translate(-poi);

		YsMatrix4x4 projection=MakePerspective(45.0,aspect,cameraDist*0.01,cameraDist+l);

		int lb,mb,rb,mx,my;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
			WindowCoordToLine(pickedLine,mx,my,modelView,projection);
			auto pickedPlHd=PickedPlHd(mesh,mx,my,modelView,projection);
			if(mesh.NullPolygon()!=pickedPlHd)
			{
				for(auto plHd : GetNNeighbor(mesh,pickedPlHd,10))
				{
					YsColor col;
					col.SetDoubleRGBA(1,0,0,1);
					mesh.SetColor(plHd,col);
				}

				vtxArrays=mesh.MakeVertexArrays();
			}

			auto pickedVtHd=PickedVtHd(mesh,mx,my,modelView,projection);
			if(mesh.NullVertex()!=pickedVtHd)
			{
				highlightPos=mesh.GetVertexPosition(pickedVtHd);
				selectedVtHd=pickedVtHd;
				connVtHd=GetVertexesWithinRadius(mesh,pickedVtHd,2.0);
			}
		}



		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		double glProjection[16];
		projection.GetOpenGlCompatibleMatrix(glProjection);
		glMultMatrixd(glProjection); // gluPerspective(45.0,aspect,cameraDist*0.01,cameraDist+l);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	    GLfloat lightDir[]={0,1/sqrt(2.0f),1/sqrt(2.0f),0};
	    glLightfv(GL_LIGHT0,GL_POSITION,lightDir);

		double glModelView[16];
		modelView.GetOpenGlCompatibleMatrix(glModelView);
		glMultMatrixd(glModelView);

		if(0==mode)
		{
		    glEnable(GL_COLOR_MATERIAL);
		    glEnable(GL_LIGHTING);
		    glEnable(GL_LIGHT0);

			glEnableClientState(GL_VERTEX_ARRAY);
		    glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT,0,vtxArrays.col.data());
		    glNormalPointer(GL_FLOAT,0,vtxArrays.nom.data());
			glVertexPointer(3,GL_FLOAT,0,vtxArrays.vtx.data());
			glDrawArrays(GL_TRIANGLES,0,vtxArrays.vtx.size()/3);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		    glDisableClientState(GL_NORMAL_ARRAY);
		}
		else if(1==mode)
		{
		    glDisable(GL_COLOR_MATERIAL);
		    glDisable(GL_LIGHTING);
		    glDisable(GL_LIGHT0);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT,0,dualCol.data());
			glVertexPointer(3,GL_FLOAT,0,dualVtx.data());
			glDrawArrays(GL_LINES,0,dualVtx.size()/3);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		else if(2==mode)
		{
		    glDisable(GL_COLOR_MATERIAL);
		    glDisable(GL_LIGHTING);
		    glDisable(GL_LIGHT0);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4,GL_FLOAT,0,lowDhaCol.data());
			glVertexPointer(3,GL_FLOAT,0,lowDhaVtx.data());
			glDrawArrays(GL_LINES,0,lowDhaVtx.size()/3);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		glColor3f(1,0,0);
		glBegin(GL_LINES);
		glVertex3dv(pickedLine[0]);
		glVertex3dv(pickedLine[1]);
		glEnd();

		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3dv(highlightPos);
		for(auto vtHd : connVtHd)
		{
			glVertex3dv(mesh.GetVertexPosition(vtHd));
		}
		glEnd();


		FsSwapBuffers();
		
		FsSleep(1);
	}
	return 0;
}
