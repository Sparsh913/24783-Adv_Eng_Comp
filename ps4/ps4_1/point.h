#ifndef POINT_H_IS_INCLUDED
#define POINT_H_IS_INCLUDED

class Point
{
public:
	double x,y;

	inline Point(){}
	inline Point(double X,double Y)
	{
		x=X;
		y=Y;
	}
	inline double SquareLength(void) const
	{
		return x*x+y*y;
	}
};
inline Point operator*(Point a,double b) // operator overloading for multiplication of a point by a double
{
	a.x*=b;
	a.y*=b;
	return a;
}
inline Point operator*(double b,Point a)
{
	a.x*=b;
	a.y*=b;
	return a;
}
inline Point operator+(Point a,Point b)
{
	a.x+=b.x;
	a.y+=b.y;
	return a;
}
inline Point operator-(Point a,Point b)
{
	a.x-=b.x;
	a.y-=b.y;
	return a;
}

#endif
