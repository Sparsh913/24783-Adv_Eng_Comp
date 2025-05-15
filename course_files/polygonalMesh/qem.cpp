#include "qem.h"
#include "collapse.h"
double ComputeQEM(const PolygonalMesh &mesh,
    PolygonalMesh::VertexHandle from,
    PolygonalMesh::VertexHandle to)
{
	double sum=0;
	auto toPos=mesh.GetVertexPosition(to);
	for(auto plHd : mesh.FindPolygonsFromVertex(from))
	{
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		if(3==plVtHd.size())
		{
			const YsVec3 vtPos[3]=
			{
				mesh.GetVertexPosition(plVtHd[0]),
				mesh.GetVertexPosition(plVtHd[1]),
				mesh.GetVertexPosition(plVtHd[2]),
			};
			auto nom=(vtPos[1]-vtPos[0])^(vtPos[2]-vtPos[0]);
			nom.Normalize();
			auto dist=(toPos-vtPos[0])*nom;
			sum+=dist*dist;
		}
	}
	return sum;
}


void ApplyQEM_NSquare(PolygonalMesh &mesh)
{
	PolygonalMesh::VertexHandle bestEdVtHd[2]=
	{
		mesh.NullVertex(),
		mesh.NullVertex(),
	};
	double bestQEM=0.0;

	for(auto plHd=mesh.FirstPolygon();
	    mesh.NullPolygon()!=plHd;
	    mesh.MoveToNext(plHd))
	{
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		for(int i=0; i<plVtHd.size(); ++i)
		{
			PolygonalMesh::VertexHandle edVtHd[2]=
			{
				plVtHd[i],
				plVtHd[(i+1)%plVtHd.size()]
			};
			for(int j=0; j<2; ++j)
			{
				auto qem=ComputeQEM(mesh,edVtHd[0],edVtHd[1]);
				if(qem<bestQEM || mesh.NullVertex()==bestEdVtHd[0])
				{
					bestQEM=qem;
					bestEdVtHd[0]=edVtHd[0];
					bestEdVtHd[1]=edVtHd[1];
				}
				std::swap(edVtHd[0],edVtHd[1]);
			}
		}
	}

	CollapseEdge(mesh,bestEdVtHd[0],bestEdVtHd[1]);
printf("%d %d\n",mesh.GetSearchKey(bestEdVtHd[0]),mesh.GetSearchKey(bestEdVtHd[1]));
}


void QEM::Prepare(const PolygonalMesh &mesh)
{
	for(auto plHd=mesh.FirstPolygon();
	    mesh.NullPolygon()!=plHd;
	    mesh.MoveToNext(plHd))
	{
		auto plVtHd=mesh.GetPolygonVertex(plHd);
		for(int i=0; i<plVtHd.size(); ++i)
		{
			PolygonalMesh::VertexHandle edVtHd[2]=
			{
				plVtHd[i],
				plVtHd[(i+1)%plVtHd.size()]
			};
			for(int j=0; j<2; ++j)
			{
				auto qem=ComputeQEM(mesh,edVtHd[0],edVtHd[1]);

				CollapseCandidate can;
				can.from=mesh.GetSearchKey(edVtHd[0]);
				can.to=mesh.GetSearchKey(edVtHd[1]);

				if(edgeToTreeNode.find(can)==edgeToTreeNode.end())
				{
					auto hd=tree.Insert(qem,can);
					edgeToTreeNode[can]=hd;
				}

				std::swap(edVtHd[0],edVtHd[1]);
			}
		}
	}
}
void QEM::ApplyOne(PolygonalMesh &mesh)
{
	auto ndHd=tree.First();
	if(ndHd!=nullptr)
	{
		auto value=tree.GetValue(ndHd);

		PolygonalMesh::VertexHandle fromTo[2]=
		{
			mesh.FindVertex(value.from),
			mesh.FindVertex(value.to),
		};

		for(auto vtHd0 : fromTo)
		{
			for(auto vtHd1 : mesh.GetConnectedVertex(vtHd0))
			{
				PolygonalMesh::VertexHandle edVtHd[2]={vtHd0,vtHd1};
				for(int j=0; j<2; ++j)
				{
					CollapseCandidate can;
					can.from=mesh.GetSearchKey(edVtHd[0]);
					can.to=mesh.GetSearchKey(edVtHd[1]);
					auto found=edgeToTreeNode.find(can);
					if(found!=edgeToTreeNode.end())
					{
						auto hd=found->second;
						tree.Delete(hd);
						edgeToTreeNode.erase(found);
					}
					std::swap(edVtHd[0],edVtHd[1]);
				}
			}
		}

		CollapseEdge(mesh,fromTo[0],fromTo[1]);

		for(auto vtHd0 : fromTo)
		{
			for(auto vtHd1 : mesh.GetConnectedVertex(vtHd0))
			{
				PolygonalMesh::VertexHandle edVtHd[2]={vtHd0,vtHd1};
				for(int j=0; j<2; ++j)
				{
					CollapseCandidate can;
					can.from=mesh.GetSearchKey(edVtHd[0]);
					can.to=mesh.GetSearchKey(edVtHd[1]);

					auto qem=ComputeQEM(mesh,edVtHd[0],edVtHd[1]);

					auto hd=tree.Insert(qem,can);
					edgeToTreeNode[can]=hd;

					std::swap(edVtHd[0],edVtHd[1]);
				}
			}
		}
	}
}
