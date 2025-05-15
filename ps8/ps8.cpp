#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "fssimplewindow.h"
#include "ysclass.h"
#include "mesh.h"
#include "glutil.h"
#include "makeslice.h"

const double epsilon = 1e-6;
std::vector<YsVec3> sliceVtx;
std::vector<YsColor> sliceCol;

void WindowCoordToLine(YsVec3 pos[2], int wx, int wy, const YsMatrix4x4 &modelView, const YsMatrix4x4 &projection)
{
	int wid, hei;
	FsGetWindowSize(wid, hei);

	pos[0] = WindowToViewPort(wid, hei, wx, wy);
	pos[1] = pos[0];

	pos[0].SetZ(-1.0);
	pos[1].SetZ(1.0);

	projection.MulInverse(pos[0], pos[0], 1.0);
	projection.MulInverse(pos[1], pos[1], 1.0);

	modelView.MulInverse(pos[0], pos[0], 1.0);
	modelView.MulInverse(pos[1], pos[1], 1.0);
}

PolygonalMesh::PolygonHandle PickedPlHd(
	const PolygonalMesh &mesh,
	int mx, int my,
	const YsMatrix4x4 &modelView, const YsMatrix4x4 &projection)
{
	YsVec3 pos[2];
	WindowCoordToLine(pos, mx, my, modelView, projection);

	double nearDist = 0.0;
	PolygonalMesh::PolygonHandle nearPlHd = mesh.NullPolygon();

	for (auto plHd = mesh.FirstPolygon(); mesh.NullPolygon() != plHd; mesh.MoveToNext(plHd))
	{

		auto plVtHd = mesh.GetPolygonVertex(plHd);
		if (3 <= plVtHd.size())
		{
			YsVec3 trVtPos[3] =
				{
					mesh.GetVertexPosition(plVtHd[0]),
					mesh.GetVertexPosition(plVtHd[1]),
					mesh.GetVertexPosition(plVtHd[2]),
				};

			YsVec3 nom = (trVtPos[1] - trVtPos[0]) ^ (trVtPos[2] - trVtPos[0]);
			auto Lnom = nom.GetLength();
			if (epsilon <= Lnom)
			{
				nom /= Lnom;

				YsPlane pln(trVtPos[0], nom);
				YsVec3 itsc;
				if (YSOK == pln.GetIntersection(itsc, pos[0], pos[1] - pos[0]))
				{
					if (0.0 <= (itsc - pos[0]) * (pos[1] - pos[0]))
					{
						auto side = YsCheckInsidePolygon3(itsc, 3, trVtPos);
						if (YSINSIDE == side)
						{
							double dist = (pos[0] - itsc).GetSquareLength();
							if (mesh.NullPolygon() == nearPlHd || dist < nearDist)
							{
								nearPlHd = plHd;
								nearDist = dist;
							}
						}
					}
				}
			}
		}
	}
	return nearPlHd;
}

YsVec3 ComputePolygonCenter(const PolygonalMesh &mesh, PolygonalMesh::PolygonHandle plHd)
{
	auto vtHdVec = mesh.GetPolygonVertex(plHd);
	YsVec3 sum(0.0, 0.0, 0.0);

	for (auto vtHd : vtHdVec)
	{
		sum += mesh.GetVertexPosition(vtHd);
	}

	if (!vtHdVec.empty())
	{
		sum /= (double)vtHdVec.size();
	}

	return sum;
}

int main(int ac, char *av[])
{
	PolygonalMesh mesh;

	if (true != mesh.LoadBinStl(av[1]))
	{
		std::cout << "STL read error\n";
		return 0;
	}

	mesh.Stitch();

	std::cout << "Number of Vertices " << mesh.GetNumVertices() << "\n";
	std::cout << "Number of Polygons " << mesh.GetNumPolygons() << "\n";

	auto vtxArrays = mesh.MakeVertexArrays();

	YsVec3 bbx[2];
	mesh.GetBoundingBox(bbx);
	auto l = (bbx[1] - bbx[0]).GetLength();

	std::cout << bbx[0].Txt() << "\n";
	std::cout << bbx[1].Txt() << "\n";

	FsOpenWindow(0, 0, 800, 600, 1);

	YsVec3 poi = (bbx[0] + bbx[1]) / 2.0;
	double cameraDist = l / (2.0 * tan(YsPi / 8));
	YsMatrix3x3 cameraOrientation;

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	for (;;)
	{
		FsPollDevice();
		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}

		if (0 != FsGetKeyState(FSKEY_UP))
		{
			cameraOrientation.RotateYZ(YsPi / 60.0);
		}
		if (0 != FsGetKeyState(FSKEY_DOWN))
		{
			cameraOrientation.RotateYZ(-YsPi / 60.0);
		}
		if (0 != FsGetKeyState(FSKEY_LEFT))
		{
			cameraOrientation.RotateXZ(YsPi / 60.0);
		}
		if (0 != FsGetKeyState(FSKEY_RIGHT))
		{
			cameraOrientation.RotateXZ(-YsPi / 60.0);
		}
		if (0 != FsGetKeyState(FSKEY_Z))
		{
			cameraDist /= 1.01;
			if (cameraDist < 1.0)
			{
				cameraDist = 1.0;
			}
		}
		if (0 != FsGetKeyState(FSKEY_M))
		{
			cameraDist *= 1.01;
		}

		int wid, hei;
		FsGetWindowSize(wid, hei);
		double aspect = double(wid) / double(hei);

		YsMatrix4x4 modelView;
		auto cameraInv = cameraOrientation;
		cameraInv.Invert();
		modelView.Translate(0, 0, -cameraDist);
		modelView *= cameraInv;
		modelView.Translate(-poi);

		YsMatrix4x4 projection = MakePerspective(45.0, aspect, cameraDist * 0.01, cameraDist + l);

		int lb, mb, rb, mx, my;
		auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);
		if (FSMOUSEEVENT_LBUTTONDOWN == evt)
		{
			auto pickedPlHd = PickedPlHd(mesh, mx, my, modelView, projection);
			if (mesh.NullPolygon() != pickedPlHd)
			{
				YsVec3 center = ComputePolygonCenter(mesh, pickedPlHd);
				YsVec3 normal(1.0, 1.0, 1.0);
				normal.Normalize();

				auto slice = MakeSlice(mesh, pickedPlHd, center, normal);
				sliceVtx.clear();
				sliceCol.clear();
				for (const auto &v : slice)
				{
					sliceVtx.push_back(v);
					sliceCol.push_back(YsColor(255, 0, 0));  // red slice
				}
			}
		} 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		double glProjection[16];
		projection.GetOpenGlCompatibleMatrix(glProjection);
		glMultMatrixd(glProjection);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLfloat lightDir[] = {0, 1 / sqrt(2.0f), 1 / sqrt(2.0f), 0};
		glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		double glModelView[16];
		modelView.GetOpenGlCompatibleMatrix(glModelView);
		glMultMatrixd(glModelView);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, vtxArrays.col.data());
		glNormalPointer(GL_FLOAT, 0, vtxArrays.nom.data());
		glVertexPointer(3, GL_FLOAT, 0, vtxArrays.vtx.data());
		glDrawArrays(GL_TRIANGLES, 0, vtxArrays.vtx.size() / 3);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		// Manual rendering of slice
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < sliceVtx.size(); ++i)
		{
			glVertex3d(sliceVtx[i].xf(), sliceVtx[i].yf(), sliceVtx[i].zf());
		}
		glEnd();

		// glColor3f(0.0f, 1.0f, 0.0f);
		// glBegin(GL_LINES);
		// for (int i = 0; i < sliceVtx.size(); ++i)
		// {
		// 	auto a = sliceVtx[i];
		// 	auto b = sliceVtx[(i + 1) % sliceVtx.size()];
		// 	glVertex3d(a.xf(), a.yf(), a.zf());
		// 	glVertex3d(b.xf(), b.yf(), b.zf());
		// }
		// glEnd();

		// glColor3f(1.0f, 0.0f, 0.0f);
		// glPointSize(6);
		// glBegin(GL_POINTS);
		// for (const auto &v : sliceVtx)
		// {
		// 	glVertex3d(v.xf(), v.yf(), v.zf());
		// }
		// glEnd();


		FsSwapBuffers();
		FsSleep(1);
	}

	return 0;
}
