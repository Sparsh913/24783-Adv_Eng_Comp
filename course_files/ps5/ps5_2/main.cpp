#include <iostream>
#include "fssimplewindow.h"

#include "simplebitmap.h"
#include "hashtable.h"

// Complete a hash function for SimpleBitmap so that you can make a hash table that maps a bitmap to an integer.
template <>
struct ysHash<SimpleBitmap>
{
	std::size_t operator()(const SimpleBitmap &bmp) const
	{
		std::size_t sum=0;
		return sum;
	}
};

// Complete a hash function for an unsigned integer.
template <>
struct ysHash<unsigned int>
{
	std::size_t operator()(unsigned int id) const
	{
		return 0;
	}
};

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		std::cout << "You need to specify a PNG file name in the command argument." << std::endl;
		return 1;
	}


	SimpleBitmap bmp;
	if(true!=bmp.LoadPng(argv[1]))
	{
		std::cout << "Failed to open the PNG file." << std::endl;
		return 1;
	}

	FsOpenWindow(0,0,800,600,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		FsSwapBuffers();
	}

	return 0;
}
