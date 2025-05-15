#include <iostream>
#include "simplebitmap.h"
#include "yspng.h"
#include "fssimplewindow.h"

void MainLoop(SimpleBitmap &bmp)
{
	FsOpenWindow(0,0,bmp.GetWidth(),bmp.GetHeight(),1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glRasterPos2i(0,bmp.GetHeight()-1);
		glDrawPixels(bmp.GetWidth(),bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmp.GetBitmapPointer());
		FsSwapBuffers();
	}
}

void TestMain(char filename[])
{
	SimpleBitmap bmp,bmp3;
	if(true==bmp.LoadPng(filename))
	{
		bmp.Invert();

		SimpleBitmap bmp2=bmp;
		std::cout << "Copy!\n";

		bmp3=bmp2;
		std::cout << "Copy Again!\n";

		MainLoop(bmp3);
	}
	else
	{
		std::cout << "Cannot open " << filename << "\n";
	}
}

int main(int ac,char *av[])
{
	std::cout << "Starting.\n";
	if(ac<2)
	{
		std::cout << "Specify a PNG file name.\n";
		return 1;
	}
	else
	{
		TestMain(av[1]);
	}
	std::cout << "Done.\n";
	return 0;
}
