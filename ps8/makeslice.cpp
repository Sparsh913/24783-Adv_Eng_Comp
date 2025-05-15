#include <stack>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "mesh.h"
#include "makeslice.h"

// DFS-based function to collect and order intersection points
std::vector<YsVec3> MakeSlice(
    const PolygonalMesh &mesh,
    PolygonalMesh::PolygonHandle startPlHd,
    const YsVec3 &planeCenter,
    const YsVec3 &planeNormal)
{
    // Use DFS (stack) to traverse all connected polygons whose faces are intersected by the slicing plane.
    std::stack<PolygonalMesh::PolygonHandle> dfsStack;
    std::unordered_set<unsigned int> visitedPolygonKeys; // track visited polygons (using unsigned keys)
    
    // Container for intersection points (unordered initially)
    std::vector<YsVec3> intersections;
    
    // Start at the picked polygon.
    dfsStack.push(startPlHd);
    visitedPolygonKeys.insert((mesh.GetSearchKey(startPlHd)));

    while(!dfsStack.empty())
    {
        PolygonalMesh::PolygonHandle curPlHd = dfsStack.top();
        dfsStack.pop();
        unsigned int curKey = (mesh.GetSearchKey(curPlHd));
        // Get all vertices for current polygon.
        std::vector<PolygonalMesh::VertexHandle> vtHds = mesh.GetPolygonVertex(curPlHd);
        std::vector<YsVec3> pos;
        for(PolygonalMesh::VertexHandle h : vtHds)
        {
            pos.push_back(mesh.GetVertexPosition(h));
        }
        // For each edge in this polygon, check if the edge crosses the slicing plane.
        for (int i = 0; i < (pos.size()); i++)
        {
            const YsVec3 &p1 = pos[i];
            const YsVec3 &p2 = pos[(i + 1) % pos.size()];
            
            double d1 = (p1 - planeCenter) * planeNormal;
            double d2 = (p2 - planeCenter) * planeNormal;
            
            // If one vertex is very close to the plane, skip this edge. Because it may be a numerical error.
            if (fabs(d1) < 1e-6 || fabs(d2) < 1e-6)
            {
                // Uncomment for debug:
                std::cout << "Vertex near plane on edge " << i << " of polygon " << curKey << ", skipping edge.\n";
                continue;
            }
            
            // Check if the edge is intersected by the plane.
            if ((d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0))
            {
                // Compute intersection point using linear interpolation along the edge.
                double t = d1 / (d1 - d2);
                YsVec3 inter = p1 + (p2 - p1) * t;
                intersections.push_back(inter);
                
                // Also push the neighbor polygon for further exploration.
                PolygonalMesh::PolygonHandle neighbor = mesh.GetNeighborPolygon(curPlHd, i);
                if(neighbor != mesh.NullPolygon())
                {
                    unsigned int neighborKey = (mesh.GetSearchKey(neighbor));
                    if (visitedPolygonKeys.find(neighborKey) == visitedPolygonKeys.end())
                    {
                        visitedPolygonKeys.insert(neighborKey);
                        dfsStack.push(neighbor);
                    }
                }
            }
        }
    }
    
    // If no intersections were found, return empty vector.
    // if(intersections.empty())
    //     return intersections;
        
    // // Compute the centroid of all intersection points.
    // YsVec3 centroid(0.0,0.0,0.0);
    // for(auto &pt : intersections)
    // {
    //     centroid += pt;
    // }
    // centroid /= intersections.size();
    
    // // Next, sort the intersections by angle around the centroid.
    // // Choose a reference vector in the plane:
    // YsVec3 ref(1, 0, 0);
    // if (fabs(ref * planeNormal) > 0.9)
    //     ref.Set(0, 1, 0);
    // // Project the reference onto the slicing plane.
    // ref = ref - planeNormal * (ref * planeNormal);
    // ref.Normalize();
    
    // // Lambda to compute the angle of a point (relative to the centroid) w.r.t the reference.
    // auto angleSort = [centroid, ref, planeNormal](const YsVec3 &a, const YsVec3 &b) -> bool
    // {
    //     YsVec3 da = a - centroid, db = b - centroid;
    //     da.Normalize();
    //     db.Normalize();
    //     double angleA = acos(da * ref);
    //     double angleB = acos(db * ref);
    //     // Adjust angle based on orientation (using cross product with the planeNormal).
    //     if ((ref ^ da) * planeNormal < 0)
    //         angleA = 2 * YsPi - angleA;
    //     if ((ref ^ db) * planeNormal < 0)
    //         angleB = 2 * YsPi - angleB;
    //     return angleA < angleB;
    // };
    
    // std::sort(intersections.begin(), intersections.end(), angleSort);
    
    // Return the ordered intersection points forming the slice loop.
    return intersections;
}
