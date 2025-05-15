#include <time.h>
#include <thread>
#include <chrono>
#include "fssimplewindow.h"
#include "ysglfontdata.h"

int main(void)
{
	FsOpenWindow(0,0,640,80,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		auto t=time(NULL);
		auto local=localtime(&t);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glRasterPos2i(16,64);
		YsGlDrawFontBitmap32x48(asctime(local));
		FsSwapBuffers();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
