#ifndef MAKESLICE_IS_INCLUDED
#define MAKESLICE_IS_INCLUDED
/* { */

#include <vector>
#include <set>
#include "ysclass.h"
#include "mesh.h"

/*! Return a sequence of points that are intersections between an edge of the given mesh and a plane
    that passes through org and perpendicular to nom.
    Start search from polygon plHd0.  If plHd0 is not intersecting with the plane, return an empty vector.
*/
std::vector <YsVec3> MakeSlice(
    const PolygonalMesh &mesh,
    PolygonalMesh::PolygonHandle plHd0,const YsVec3 &org,const YsVec3 &nom);


/* } */
#endif
