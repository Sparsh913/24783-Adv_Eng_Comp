#ifndef COLLAPSE_H_IS_INCLUDED
#define COLLAPSE_H_IS_INCLUDED

#include "mesh.h"

void CollapseEdge(
    PolygonalMesh &mesh,
    PolygonalMesh::VertexHandle from,
    PolygonalMesh::VertexHandle to);

#endif
