#include <iostream>
#include "simplebitmap.h"
#include "yspng.h"
#include "yspngenc.h"

SimpleBitmap::SimpleBitmap(const SimpleBitmap &incoming)
{
	CopyFrom(incoming);
}

SimpleBitmap &SimpleBitmap::operator=(const SimpleBitmap &from)
{
	CopyFrom(from);
	return *this;
}

bool SimpleBitmap::LoadPng(const char filename[])
{
	YsRawPngDecoder png;
	png.Decode(filename);

	std::cout << png.wid << " " << png.hei << "\n";

	if(0!=png.wid && 0!=png.hei)
	{
		this->SetDirect(png.wid,png.hei,png.rgba);

		png.rgba=nullptr;
		png.wid=0;
		png.hei=0;

		return true;
	}

	return false;
}

bool SimpleBitmap::LoadPng(std::string filename)
{
	YsRawPngDecoder png;
	png.Decode(filename.c_str());

	std::cout << png.wid << " " << png.hei << "\n";

	if(0!=png.wid && 0!=png.hei)
	{
		this->SetDirect(png.wid,png.hei,png.rgba);

		png.rgba=nullptr;
		png.wid=0;
		png.hei=0;

		return true;
	}

	return false;
}

void SimpleBitmap::Clear(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
	auto rgba=GetEditableBitmapPointer();
	for(int i=0; i<GetTotalNumComponent(); i+=4)
	{
		rgba[i  ]=r;
		rgba[i+1]=g;
		rgba[i+2]=b;
		rgba[i+3]=a;
	}
}

SimpleBitmap SimpleBitmap::CutOut(int x0,int y0,int wid,int hei) const
{
	SimpleBitmap cut;
	SimpleBitmapTemplate <unsigned char,4>::CutOut(cut,x0,y0,wid,hei,0);
	return cut;
}

bool SimpleBitmap::SavePng(FILE *fp) const
{
	YsRawPngEncoder encoder;
	return (YSOK==encoder.EncodeToFile(fp,GetWidth(),GetHeight(),8,6,GetBitmapPointer()));
}

bool SimpleBitmap::operator==(const SimpleBitmap &bitmapB) const
{
	if(GetWidth()!=bitmapB.GetWidth() ||
	   GetHeight()!=bitmapB.GetHeight())
	{
		return false;
	}

	auto ptr1=GetBitmapPointer();
	auto ptr2=bitmapB.GetBitmapPointer();
	for(int i=0; i<GetTotalNumComponent(); i+=4)
	{
		if(ptr1[i]!=ptr2[i])
		{
			return false;
		}
	}
	return true;
}

bool SimpleBitmap::operator!=(const SimpleBitmap &bitmapB) const
{
	if(GetWidth()!=bitmapB.GetWidth() ||
	   GetHeight()!=bitmapB.GetHeight())
	{
		return true;
	}

	auto ptr1=GetBitmapPointer();
	auto ptr2=bitmapB.GetBitmapPointer();
	for(int i=0; i<GetTotalNumComponent(); i+=4)
	{
		if(ptr1[i]!=ptr2[i])
		{
			return true;
		}
	}
	return false;
}
