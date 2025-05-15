#include <fssimplewindow.h>
#include <vector>

class Point
{
public:
	int xy[2];
};

class ApplicationMain
{
public:
	std::vector <Point> points;
	bool mustQuit=false;

	ApplicationMain()
	{
	}
	bool MustQuit(void) const
	{
		return mustQuit;
	}
	void Interval(void)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			mustQuit=true;
		}

		int mx,my,lb,mb,rb;
		auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
		if(FSMOUSEEVENT_LBUTTONDOWN==evt)
		{
			Point p;
			p.xy[0]=mx;
			p.xy[1]=my;
			points.push_back(p);
		}
	}
	void Draw(void) const
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glBegin(GL_LINE_STRIP);
		for(auto p : points)
		{
			glVertex2iv(p.xy);
		}
		glEnd();
		FsSwapBuffers();
	}
};

int main(void)
{
	FsOpenWindow(0,0,800,600,1);
	class ApplicationMain app;
	while(true!=app.MustQuit())
	{
		app.Interval();
		app.Draw();
	}
    return 0;
}
