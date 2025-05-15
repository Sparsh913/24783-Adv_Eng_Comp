#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include "fssimplewindow.h"
#include "ysglfontdata.h"

void BubbleSortOneStep(int i,int N,int V[])
{
	if(i<N)
	{
		for(int j=i+1; j<N; ++j)
		{
			if(V[i]>V[j])
			{
				std::swap(V[i],V[j]);
			}
		}
	}
}

void Shuffle(int N,int V[])
{
	for(int i=0; i<N; ++i)
	{
		int j=rand()%N;
		std::swap(V[i],V[j]);
	}
}

const int N=30;

int main(void)
{
	FsOpenWindow(0,0,800,600,1);

	int V[N];
	for(int i=0; i<N; ++i)
	{
		V[i]=i;
	}
	Shuffle(N,V);

	int i=0;
	for(;;)
	{
		FsPollDevice();

		auto key=FsInkey();
		switch(key)
		{
		case FSKEY_SPACE:
			BubbleSortOneStep(i,N,V);
			++i;
			break;
		case FSKEY_S:
			Shuffle(N,V);
			i=0;
			break;
		}
		if(FSKEY_ESC==key)
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3f(0,1,0);
		glBegin(GL_QUADS);
		for(int i=0; i<N; ++i)
		{
			glVertex2i(0,i*20);
			glVertex2i(V[i]*20,i*20);
			glVertex2i(V[i]*20,i*20+20);
			glVertex2i(0,i*20+20);
		}
		glEnd();

		glColor3f(0,0,0);
		glRasterPos2i(0,32);
		YsGlDrawFontBitmap20x28("Bubble-Sort Visualization");
		glRasterPos2i(0,60);
		YsGlDrawFontBitmap20x28("ESC.....Close");
		glRasterPos2i(0,88);
		YsGlDrawFontBitmap20x28("SPACE...Bubble-Sort One Step");
		glRasterPos2i(0,116);
		YsGlDrawFontBitmap20x28("S.......Shuffle");

		FsSwapBuffers();
	}
}
