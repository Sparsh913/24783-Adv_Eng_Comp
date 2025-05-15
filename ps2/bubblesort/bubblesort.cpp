#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include "fssimplewindow.h"
#include "ysglfontdata.h"

const int N = 30;

void BubbleSortOneStep(int i, int N, int V[]) {
    if (i < N) {
        for (int j = i + 1; j < N; ++j) {
            if (V[i] > V[j]) {
                std::swap(V[i], V[j]);
            }
        }
    }
}

void Shuffle(int N, int V[]) {
    for (int i = 0; i < N; ++i) {
        int j = rand() % N;
        std::swap(V[i], V[j]);
    }
}

class ApplicationMain {
private:
    int V[N];
    int currentStep;
    bool needQuit;

public:
    ApplicationMain() : currentStep(0), needQuit(false) {
        for (int i = 0; i < N; ++i) V[i] = i;
        Shuffle(N, V);
    }

    bool NeedQuit() const { return needQuit; }

    void Interval() {
        FsPollDevice();
        auto key = FsInkey();

        switch (key) {
            case FSKEY_ESC:
                needQuit = true;
                break;
            case FSKEY_SPACE:
                if (currentStep < N) {
                    BubbleSortOneStep(currentStep, N, V);
                    currentStep++;
                }
                break;
            case FSKEY_S:
                Shuffle(N, V);
                currentStep = 0;
                break;
        }
    }

    void Draw() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw bars
        glColor3f(0, 1, 0);
        glBegin(GL_QUADS);
        for (int i = 0; i < N; ++i) {
            int x = V[i] * 20;
            glVertex2i(0, i * 20);
            glVertex2i(x, i * 20);
            glVertex2i(x, i * 20 + 20);
            glVertex2i(0, i * 20 + 20);
        }
        glEnd();

        // Draw text
        glColor3f(0, 0, 0);
        glRasterPos2i(0, 32);
        YsGlDrawFontBitmap20x28("Bubble-Sort Visualization");
        glRasterPos2i(0, 60);
        YsGlDrawFontBitmap20x28("ESC.....Close");
        glRasterPos2i(0, 88);
        YsGlDrawFontBitmap20x28("SPACE...Bubble-Sort One Step");
        glRasterPos2i(0, 116);
        YsGlDrawFontBitmap20x28("S.......Shuffle");

        FsSwapBuffers();
    }
};

int main(void) {
    FsOpenWindow(0, 0, 800, 600, 1);
    ApplicationMain app;
    while (false == app.NeedQuit()) {
        app.Interval();
        app.Draw();
    }
    return 0;
}