#ifndef POLYGONALMESH_IS_INCLUDED
#define POLYGONALMESH_IS_INCLUDED

#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <ysclass.h>

class HasEdge
{
public:
	class Edge
	{
	public:
		unsigned int edVtKey[2];
		bool operator==(Edge incoming) const
		{
			return (edVtKey[0]==incoming.edVtKey[0] && edVtKey[1]==incoming.edVtKey[1]) ||
			       (edVtKey[0]==incoming.edVtKey[1] && edVtKey[1]==incoming.edVtKey[0]);
		}
		bool operator!=(Edge incoming) const
		{
			return (edVtKey[0]!=incoming.edVtKey[0] || edVtKey[1]!=incoming.edVtKey[1]) &&
			       (edVtKey[0]!=incoming.edVtKey[1] || edVtKey[1]!=incoming.edVtKey[0]);
		}
	};
};

template <>
struct std::hash<HasEdge::Edge>
{
	std::size_t operator()(HasEdge::Edge edge) const noexcept
	{
		auto H=std::max(edge.edVtKey[0],edge.edVtKey[1]);
		auto L=std::min(edge.edVtKey[0],edge.edVtKey[1]);
		return H*11+L*7;
	}
};

class PolygonalMesh : public HasEdge
{
private:
	unsigned int searchKeySeed=1;
	class HasSearchKey
	{
	public:
		unsigned int searchKey;
	};

protected:
    class Vertex : public HasSearchKey
    {
    public:
        YsVec3 pos;
    };
private:
    mutable std::list <Vertex> vtxList;
public:
    class VertexHandle
    {
    friend class PolygonalMesh;
    private:
        std::list <Vertex>::iterator vtxPtr;
    public:
        inline bool operator==(const VertexHandle &vtHd) const
        {
			return vtxPtr==vtHd.vtxPtr;
		}
        inline bool operator!=(const VertexHandle &vtHd) const
        {
			return vtxPtr!=vtHd.vtxPtr;
		}
    };
    VertexHandle AddVertex(const YsVec3 &pos)
    {
		Vertex newVtx;
		newVtx.searchKey=searchKeySeed++;
		newVtx.pos=pos;
		vtxList.push_back(newVtx);

		auto last=vtxList.end();
		--last;

		VertexHandle vtHd;
		vtHd.vtxPtr=last;

		return vtHd;
	}
    inline VertexHandle NullVertex(void) const
    {
		VertexHandle vtHd;
		vtHd.vtxPtr=vtxList.end();
		return vtHd;
	}
    YsVec3 GetVertexPosition(VertexHandle vtHd) const
    {
		return vtHd.vtxPtr->pos;
	}
	void SetVertexPosition(VertexHandle vtHd,const YsVec3 &pos)
	{
		vtHd.vtxPtr->pos=pos;
	}
	unsigned int GetSearchKey(VertexHandle vtHd) const
	{
		return vtHd.vtxPtr->searchKey;
	}
	size_t GetNumVertices(void) const
	{
		return vtxList.size();
	}
	VertexHandle FirstVertex(void) const
	{
		VertexHandle vtHd;
		vtHd.vtxPtr=vtxList.begin();
		return vtHd;
	}
	bool MoveToNext(VertexHandle &vtHd) const
	{
		if(vtHd.vtxPtr!=vtxList.end())
		{
			++vtHd.vtxPtr;
		}
		else
		{
			vtHd.vtxPtr=vtxList.begin();
		}
		return vtHd.vtxPtr!=vtxList.end();
	}



protected:
    class Polygon : public HasSearchKey
    {
    public:
		YsVec3 nom;
        std::vector <VertexHandle> vtHd;
		YsColor col;
    };
private:
    mutable std::list <Polygon> plgList;
public:
    class PolygonHandle
    {
    friend class PolygonalMesh;
    private:
        std::list <Polygon>::iterator plgPtr;
    public:
        PolygonHandle(){};  // C++11 PolygonHandle()=default;
        inline bool operator==(const PolygonHandle &plHd) const
        {
			return plHd.plgPtr==plgPtr;
		}
        inline bool operator!=(const PolygonHandle &plHd) const
        {
			return plHd.plgPtr!=plgPtr;
		}
    };
    PolygonHandle AddPolygon(int nPlVt,const VertexHandle plVtHd[])
    {
		Polygon newPlg;
		newPlg.searchKey=searchKeySeed++;
		for(auto i=0; i<nPlVt; ++i)
		{
			newPlg.vtHd.push_back(plVtHd[i]);
		}

		plgList.push_back(newPlg);
		auto last=plgList.end();
		--last;

		PolygonHandle plHd;
		plHd.plgPtr=last;

		RegisterPolygon(plHd);

		return plHd;
	}
    PolygonHandle AddPolygon(const std::vector <VertexHandle> &plVtHd)
    {
		Polygon newPlg;
		newPlg.searchKey=searchKeySeed++;
		newPlg.vtHd=plVtHd;

		plgList.push_back(newPlg);
		auto last=plgList.end();
		--last;

		PolygonHandle plHd;
		plHd.plgPtr=last;

		RegisterPolygon(plHd);

		return plHd;
	}
	void SetNormal(PolygonHandle plHd,const YsVec3 &nom)
	{
		plHd.plgPtr->nom=nom;
	}
	YsVec3 GetNormal(PolygonHandle plHd) const
	{
		return plHd.plgPtr->nom;
	}
	void SetColor(PolygonHandle plHd,YsColor col)
	{
		plHd.plgPtr->col=col;
	}
	YsColor GetColor(PolygonHandle plHd) const
	{
		return plHd.plgPtr->col;
	}
    inline PolygonHandle NullPolygon(void) const
    {
		PolygonHandle plHd;
		plHd.plgPtr=plgList.end();
		return plHd;
	}
	size_t GetPolygonNumVertices(PolygonHandle plHd) const
	{
		return plHd.plgPtr->vtHd.size();
	}
    const std::vector <VertexHandle> GetPolygonVertex(PolygonHandle plHd) const
    {
		return plHd.plgPtr->vtHd;
	}
	void SetPolygonVertex(PolygonHandle plHd,const std::vector <VertexHandle> newPlVtHd)
	{
		UnregisterPolygon(plHd);
		plHd.plgPtr->vtHd=newPlVtHd;
		RegisterPolygon(plHd);
	}
	int GetPolygonVertex(const VertexHandle *&plVtHd,PolygonHandle plHd) const
	{
		plVtHd=plHd.plgPtr->vtHd.data();
		return plHd.plgPtr->vtHd.size();
	}
	unsigned int GetSearchKey(PolygonHandle plHd) const
	{
		return plHd.plgPtr->searchKey;
	}
	size_t GetNumPolygons(void) const
	{
		return plgList.size();
	}
	PolygonHandle FirstPolygon(void) const
	{
		PolygonHandle plHd;
		plHd.plgPtr=plgList.begin();
		return plHd;
	}
	bool MoveToNext(PolygonHandle &plHd) const
	{
		if(plHd.plgPtr!=plgList.end())
		{
			++plHd.plgPtr;
		}
		else
		{
			plHd.plgPtr=plgList.begin();
		}
		return plHd.plgPtr!=plgList.end();
	}



private:
	std::unordered_map <unsigned int,std::vector <PolygonHandle> > vtxToPlg;
	std::unordered_map <Edge,std::vector <PolygonHandle> > edgeToPlg;

	void RegisterPolygon(PolygonHandle plHd)
	{
		for(auto vtHd : plHd.plgPtr->vtHd)
		{
			auto vtKey=GetSearchKey(vtHd);
			vtxToPlg[vtKey].push_back(plHd);
		}
		for(size_t i=0; i<plHd.plgPtr->vtHd.size(); ++i)
		{
			VertexHandle edVtHd[2]=
			{
				plHd.plgPtr->vtHd[i],
				plHd.plgPtr->vtHd[(i+1)%plHd.plgPtr->vtHd.size()]
			};
			Edge edge;
			edge.edVtKey[0]=GetSearchKey(edVtHd[0]);
			edge.edVtKey[1]=GetSearchKey(edVtHd[1]);
			edgeToPlg[edge].push_back(plHd);
		}
	}
	void UnregisterPolygon(PolygonHandle plHd)
	{
		for(auto vtHd : plHd.plgPtr->vtHd)
		{
			auto vtKey=GetSearchKey(vtHd);
			auto found=vtxToPlg.find(vtKey);
			if(vtxToPlg.end()!=found)
			{
				for(int i=found->second.size()-1; 0<=i; --i)
				{
					if(found->second[i]==plHd)
					{
						found->second[i]=found->second.back();
						found->second.pop_back();
					}
				}
			}
			else
			{
				std::cout << "Integrity!\n";
			}

			for(size_t i=0; i<plHd.plgPtr->vtHd.size(); ++i)
			{
				VertexHandle edVtHd[2]=
				{
					plHd.plgPtr->vtHd[i],
					plHd.plgPtr->vtHd[(i+1)%plHd.plgPtr->vtHd.size()]
				};
				Edge edge;
				edge.edVtKey[0]=GetSearchKey(edVtHd[0]);
				edge.edVtKey[1]=GetSearchKey(edVtHd[1]);
				auto found=edgeToPlg.find(edge);
				if(edgeToPlg.end()!=found)
				{
					for(int i=found->second.size()-1; 0<=i; --i)
					{
						if(found->second[i]==plHd)
						{
							found->second[i]=found->second.back();
							found->second.pop_back();
						}
					}
				}
				else
				{
					std::cout << "Integrity!\n";
				}
			}
		}
	}

public:
	std::vector <PolygonHandle> FindPolygonsFromVertex(VertexHandle vtHd) const
	{
		auto vtKey=GetSearchKey(vtHd);
		auto found=vtxToPlg.find(vtKey);
		if(found!=vtxToPlg.end())
		{
			return found->second;
		}
		else
		{
			return std::vector <PolygonHandle>();
		}
	}

	std::vector <PolygonHandle> FindPolygonFromEdge(VertexHandle edVtHd[2]) const
	{
		Edge edge;
		edge.edVtKey[0]=GetSearchKey(edVtHd[0]);
		edge.edVtKey[1]=GetSearchKey(edVtHd[1]);
		auto found=edgeToPlg.find(edge);
		if(edgeToPlg.end()!=found)
		{
			return found->second;
		}
		return std::vector <PolygonHandle>();
	}
	PolygonHandle GetNeighborPolygon(PolygonHandle plHd,int n) const
	{
		auto plVtHd=GetPolygonVertex(plHd);
		if(n<plVtHd.size())
		{
			Edge edge;
			edge.edVtKey[0]=GetSearchKey(plVtHd[n]);
			edge.edVtKey[1]=GetSearchKey(plVtHd[(n+1)%plVtHd.size()]);
			auto found=edgeToPlg.find(edge);
			if(edgeToPlg.end()!=found && 2==found->second.size())
			{
				if(found->second[0]==plHd)
				{
					return found->second[1];
				}
				else if(found->second[1]==plHd)
				{
					return found->second[0];
				}
				else
				{
					std::cout << "Integrity!\n";
				}
			}
		}
		return NullPolygon();
	}


public:
	void CleanUp(void)
	{
		// Will implement.
	}


public:
    bool LoadBinStl(const char fn[]);


public:
    void GetBoundingBox(YsVec3 &mn,YsVec3 &mx) const;
    void GetBoundingBox(YsVec3 bbx[2]) const;


public:
	class OpenGLArrays
	{
	public:
		std::vector <float> vtx;
		std::vector <float> nom;
		std::vector <float> col;
	};
	OpenGLArrays MakeVertexArrays(void) const; // Gabriel +1



public:
	void StitchVertexN2(void);
	void Stitch(void);
};

#endif

