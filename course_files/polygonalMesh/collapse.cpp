#include "collapse.h"

void CollapseEdge(
    PolygonalMesh &mesh,
    PolygonalMesh::VertexHandle from,
    PolygonalMesh::VertexHandle to)
{
	for(auto plHd : mesh.FindPolygonsFromEdgePiece(from,to))
	{
		mesh.DeletePolygon(plHd);
	}

	for(auto plHd : mesh.FindPolygonsFromVertex(from))
	{
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		for(auto &vtHd : plVtHd)
		{
			if(vtHd==from)
			{
				vtHd=to;
			}
		}
		mesh.SetPolygonVertex(plHd,plVtHd);
	}
}

