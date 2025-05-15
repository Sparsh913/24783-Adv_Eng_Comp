#ifndef SIMPLEBITMAP_TEMPLATE_24783_IS_INCLUDED
#define SIMPLEBITMAP_TEMPLATE_24783_IS_INCLUDED

#include <algorithm>

template <class ComponentType,int NumComponentPerPixel>
class SimpleBitmapTemplate
{
private:
	SimpleBitmapTemplate(const SimpleBitmapTemplate<ComponentType,NumComponentPerPixel> &incoming);
	SimpleBitmapTemplate<ComponentType,NumComponentPerPixel> & operator=
	    (const SimpleBitmapTemplate<ComponentType,NumComponentPerPixel> &incoming);

private:
	int nx=0,ny=0;
	ComponentType *bmpPtr=nullptr;

protected:
	/*! Returns pointer to top of the horizontal line. 
	    It must be a pointer to the first component of pixel at (0,y);

		&bmpPtr[0]                          ->First line
		&bmpPtr[nx*NumComponentPerPixel]    ->3nd line
		&bmpPtr[2*nx*NumComponentPerPixel]  ->2nd line
		    :
		    :
	*/
	ComponentType *LineTop(int y)
	{
		return bmpPtr+y*nx*NumComponentPerPixel;
	}
	const ComponentType *LineTop(int y) const
	{
		return bmpPtr+y*nx*NumComponentPerPixel;
	}

public:
	SimpleBitmapTemplate()
	{
	}
	~SimpleBitmapTemplate()
	{
		CleanUp();
	}
	void CleanUp(void)
	{
		delete [] bmpPtr;
		bmpPtr=nullptr;
		nx=0;
		ny=0;
	}

	/*! This function, you will populate in the assignment. 
	    This function cuts out part of the bitmap into destination bitmap.
	    The part on this bitmap is (thisX0,thisY0)-(thisX0+wid-1,thisY0+hei-1).
	    If a pixel coordinate in this bitmap that is to be copied to the destination is outside of
	    this bitmap, all components of the destination pixel must be clearColor.
	*/
	void CutOut(
	    SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &destination,
	    int thisX0,int thisY0,int wid,int hei,ComponentType clearColor) const
	{
		// Create and clear the destination bitmap.
		destination.Create(wid,hei);
		for(int i=0; i<destination.GetTotalNumComponent(); ++i)
		{
			destination.bmpPtr[i]=clearColor;
		}

		// Fill your code here.
	}

	/*! Returns the total number of components in the bitmap of given width and height.
	    If, the number of components per pixel is 4, (100x100) bitmap has 4x100x100 total number of components. */
	static int GetTotalNumComponent(int wid,int hei)
	{
		return wid*hei*NumComponentPerPixel;
	}

	/*! Returns the total number of components in this bitmap. */
	int GetTotalNumComponent(void) const
	{
		return nx*ny*NumComponentPerPixel;
	}

	/*! Returns the total number of components in one single line for the given width.
	    If the width of the bitmap is 100, and the total number of components per pixel is 4,
	    4x100 components per line. */
	static int GetNumComponentPerLine(int wid)
	{
		return wid*NumComponentPerPixel;
	}

	/*! Returns the total number of components in one single line for this bitmap. */
	int GetNumComponentPerLine(void) const
	{
		return nx*NumComponentPerPixel;
	}

	/*! Make a copy of the incoming bitmap. */
	SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &CopyFrom(const SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &incoming)
	{
		if(&incoming!=this)
		{
			Create(incoming.nx,incoming.ny);
			for(int i=0; i<GetTotalNumComponent(); ++i)
			{
				bmpPtr[i]=incoming.bmpPtr[i];
			}
		}
		return *this;
	}

	/*! Moves the incoming bitmap to this bitmap. */
	SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &MoveFrom(SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &incoming)
	{
		if(&incoming!=this)
		{
			SetDirect(incoming.nx,incoming.ny,incoming.bmpPtr);
			incoming.bmpPtr=nullptr;
			incoming.nx=0;
			incoming.ny=0;
		}
		return *this;
	}

	/*! Creates a bitmap. */
	bool Create(int wid,int hei)
	{
		CleanUp();
		bmpPtr=new ComponentType [GetTotalNumComponent(wid,hei)];
		nx=wid;
		ny=hei;
		return true;
	}

	/*! Returns the width in the number of pixels. */
	int GetWidth(void) const
	{
		return nx;
	}

	/*! Returns the height in the number of pixels. */
	int GetHeight(void) const
	{
		return ny;
	}

	/*! Returns the pointer to the bitmap. 
		Similar to .data() in STL terminology.
	*/
	const ComponentType *GetBitmapPointer(void) const
	{
		return bmpPtr;
	}

	/*! Returns the pointer to the pixel. */
	const ComponentType *GetPixelPointer(int x,int y) const
	{
		// nx is width
		// ny is height
		return bmpPtr+(nx*y+x)*NumComponentPerPixel;
	}

	/*! Returns the mutable pointer to the bitmap. */
	ComponentType *GetEditableBitmapPointer(void)
	{
		return bmpPtr;
	}

	/*! Returns the mutable pointer to the pixel. */
	ComponentType *GetEditablePixelPointer(int x,int y)
	{
		// nx is width
		// ny is height
		return bmpPtr+(nx*y+x)*NumComponentPerPixel;
	}

	/*! Flips the image upside down.
	    May be frequently used because the origin in the .PNG data format is lower-left, and 
	    .BMP format is upper-left.
	    We can use either way, as long as we are consistent, but we may need to go back and forth
	    occasionally.
	*/
	void Invert(void)
	{
		for(int y=0; y<ny/2; ++y)
		{
			auto ptr1=LineTop(y);
			auto ptr2=LineTop(ny-1-y);
			for(int i=0; i<GetNumComponentPerLine(); ++i)
			{
				std::swap(ptr1[i],ptr2[i]);
			}
		}
	}

	/*! Sets an array and width and height dirctory.
	    After this function, this class takes an ownership of incomingBmpPtr.
	    It must not be deleted outside of this class. */
	void SetDirect(int wid,int hei,ComponentType *incomingBmpPtr)
	{
		CleanUp();
		this->bmpPtr=incomingBmpPtr;
		this->nx=wid;
		this->ny=hei;
	}

	/*! Returns true if (x,y) is inside the bitmap. */
	bool IsInRange(int x,int y) const
	{
		return 0<=x && 0<=y && x<nx && y<ny;
	}

	/*! Returns true if 0<=x && x<GetWidth(). */
	bool IsXInRange(int x) const
	{
		return 0<=x && x<nx;
	}

	/*! Returns true if 0<=y && y<GetHeight(). */
	bool IsYInRange(int y) const
	{
		return 0<=y && y<ny;
	}
};

#endif
