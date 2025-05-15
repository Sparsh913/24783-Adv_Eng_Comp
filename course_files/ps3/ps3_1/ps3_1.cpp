#include <fssimplewindow.h>
#include "tiles.h"


int main(void)
{
	FsOpenWindow(0,0,1000,800,1);

	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();

		if(FSKEY_ESC==key)
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		for(int i=0; i<numTiles; ++i)
		{
			int x=(i%16)*40;
			int y=(i/16)*40;

			glRasterPos2i(x,y+40);
			glDrawPixels(40,40,GL_RGBA,GL_UNSIGNED_BYTE,tiles[i]);
		}
		FsSwapBuffers();
	}
}
