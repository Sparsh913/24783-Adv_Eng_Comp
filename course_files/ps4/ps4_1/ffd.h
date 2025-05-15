#ifndef FFD_H_IS_INCLUDED
#define FFD_H_IS_INCLUDED

/*
Free-Form Deformation deforms a vertex first parameterize within a rectangle, and then deform the rectangle to an
arbitrary quadrilateral, and de-parametersize the vertex in the quadrilateral.
*/
class FreeFormDeformation
{
public:
	/*
        Y                rect[1]
        |      +---------+
        |      |         |
        |      |         |
        |      +---------+
        |     rect[0]
        |
        |
        +----------------X
	*/
	Point rect[2];  // Rectangle minimum and maximum coordinates for parameterization.

	/*
        quad[3]      quad[2]
        +------------+
        |            |
        |            |
        |            |
        |            |
        +------------+
        quad[0]      quad[1]
	*/
	Point quad[4];  // Quadrilateral 

	/* Set up the rectangle and the quadrilateral.
	*/
	void InitRect(Point minXY,Point maxXY)
	{
		rect[0]=minXY;
		rect[1]=maxXY;

		quad[0]=minXY;
		quad[1].x=maxXY.x;
		quad[1].y=minXY.y;
		quad[2]=maxXY;
		quad[3].x=minXY.x;
		quad[3].y=maxXY.y;
	}
	Point Parameterize(Point src) const
	{
		Point d=rect[1]-rect[0];
		Point t=src-rect[0];
		t.x/=d.x;	// Size of the rectangle must not be zero.
		t.y/=d.y;
		return t;
	}
	Point Deparameterize(Point uv) const
	{
		Point a[2]=
		{
			quad[0]*(1.0-uv.x)+quad[1]*uv.x,
			quad[3]*(1.0-uv.x)+quad[2]*uv.x,
		};
		return a[0]*(1.0-uv.y)+a[1]*uv.y;
	}

	static std::vector <Point> FFDArrayToControlPoints(int n,const FreeFormDeformation box[])
	{
		std::vector <Point> ctp;
		if(0<n)
		{
			for(int i=0; i<n; ++i)
			{
				ctp.push_back(box[i].quad[0]);
				ctp.push_back(box[i].quad[1]);
			}
			ctp.push_back(box[n-1].quad[3]);
			ctp.push_back(box[n-1].quad[2]);
		}
		return ctp;
	}
	/*! Always use FFDArrayToControlPoints to make control points to return to this function.
	*/
	static void ControlPointsToFFDArray(int n,FreeFormDeformation box[],const std::vector <Point> &ctp)
	{
		for(int i=0; i<n; ++i)
		{
			box[i].quad[0]=ctp[i*2];
			box[i].quad[1]=ctp[i*2+1];
		}
		box[n-1].quad[3]=ctp[n*2];
		box[n-1].quad[2]=ctp[n*2+1];
		for(int i=0; i+1<n; ++i)
		{
			box[i].quad[3]=box[i+1].quad[0];
			box[i].quad[2]=box[i+1].quad[1];
		}
	}

	static void Apply(Point dst[],int n,const FreeFormDeformation ffd[],int np,const Point src[])
	{
		for(int i=0; i<np; ++i)
		{
			dst[i]=src[i]; // Tentative.
			for(int j=0; j<n; ++j)
			{
				auto uv=ffd[j].Parameterize(dst[i]);
				if(0<uv.x && uv.x<=1.0 && 0<uv.y && uv.y<=1.0)
				{
					dst[i]=ffd[j].Deparameterize(uv);
					break;
				}
			}
		}
	}
};

#endif
