#include <iostream>
#include "simplebitmap.h"
#include <cmath>
#include "fssimplewindow.h"

using namespace std;

// Complete MakeBitmapC function so that the difference is highlighted when the user presses the 2 key.

SimpleBitmap LoadPNG(std::string fileName)
{
	SimpleBitmap b;
	b.LoadPng(fileName);
	std::cout << b.GetWidth() << " " << b.GetHeight() << std::endl;
	return b;
}

SimpleBitmap MakeBitmapC(const SimpleBitmap &bmpA,const SimpleBitmap &bmpB)
{
	SimpleBitmap bmpC;
	bmpC.Create(bmpA.GetWidth(),bmpA.GetHeight());
	for(int y=0; y<bmpA.GetHeight(); ++y)
	{
		for(int x=0; x<bmpA.GetWidth(); ++x)
		{
			if(true==bmpB.IsInRange(x,y))
			{
				auto pixA=bmpA.GetPixelPointer(x,y);
				auto pixB=bmpB.GetPixelPointer(x,y);
				auto pixC=bmpC.GetEditablePixelPointer(x,y);

				// Compare R,G,B values of pixel A and pixel B, and make pixel C red, if at least one of the components
				// have 60 or greater, or white otherwise.
				if(abs(pixA[0]-pixB[0])>=60 || abs(pixA[1]-pixB[1])>=60 || abs(pixA[2]-pixB[2])>=60)
				{
					pixC[0]=255;
					pixC[1]=0;
					pixC[2]=0;
					pixC[3]=255;
				}
				else
				{
					pixC[0]=255;
					pixC[1]=255;
					pixC[2]=255;
					pixC[3]=255;
				}
			}
		}
	}
	return bmpC;
}

int main(int argc,char *argv[])
{
	if(argc<3)
	{
		std::cout << "Usage: ps5 1 pictureA.png pictureB.png" << std::endl;
		return 1;
	}

	auto bmpA=LoadPNG(argv[1]);
	auto bmpB=LoadPNG(argv[2]);
	bmpA.Invert();
	bmpB.Invert();

	if(0==bmpA.GetWidth() || 0==bmpA.GetHeight() ||
	   0==bmpB.GetWidth() || 0==bmpB.GetHeight())
	{
		std::cout << "Cannot open input file." << std::endl;
		return 1;
	}

	auto bmpC=MakeBitmapC(bmpA,bmpB);

	int mode=0;

	FsOpenWindow(0,0,bmpA.GetWidth(),bmpA.GetHeight(),1);

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}
		if(FSKEY_0<=key && key<=FSKEY_2)
		{
			mode=key-FSKEY_0;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		switch(mode)
		{
		case 0:
			glRasterPos2i(0,bmpA.GetHeight()-1);
			glDrawPixels(bmpA.GetWidth(),bmpA.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmpA.GetBitmapPointer());
			break;
		case 1:
			glRasterPos2i(0,bmpB.GetHeight()-1);
			glDrawPixels(bmpB.GetWidth(),bmpB.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmpB.GetBitmapPointer());
			break;
		case 2:
			glRasterPos2i(0,bmpC.GetHeight()-1);
			glDrawPixels(bmpC.GetWidth(),bmpC.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmpC.GetBitmapPointer());
			break;
		}
		FsSwapBuffers();
	}

	return 0;
}
