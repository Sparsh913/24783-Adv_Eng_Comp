#ifndef QEM_H_IS_INCLUDED
#define QEM_H_IS_INCLUDED

#include <unordered_map>
#include "mesh.h"
#include "ysavltree.h"

double ComputeQEM(const PolygonalMesh &mesh,
    PolygonalMesh::VertexHandle from,
    PolygonalMesh::VertexHandle to);

void ApplyQEM_NSquare(PolygonalMesh &mesh);


class QEMBase
{
public:
	class CollapseCandidate
	{
	public:
		unsigned int from,to;
		bool operator==(CollapseCandidate x) const
		{
			return from==x.from && to==x.to;
		}
		bool operator!=(CollapseCandidate x) const
		{
			return from!=x.from || to!=x.to;
		}
	};
};

template <>
struct std::hash <QEMBase::CollapseCandidate>
{
	size_t operator()(const QEMBase::CollapseCandidate &edge) const
	{
		auto H=edge.from;
		auto L=edge.to;
		return H*11+L*7;
	}
};

class QEM : public QEMBase
{
public:
	YsAVLTree <double,CollapseCandidate> tree;
	std::unordered_map <CollapseCandidate,YsAVLTree <double,CollapseCandidate>::NodeHandle> edgeToTreeNode;

	void Prepare(const PolygonalMesh &mesh);
	void ApplyOne(PolygonalMesh &mesh);
};

#endif
