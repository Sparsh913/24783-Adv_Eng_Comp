#include <stdio.h>
#include "fssimplewindow.h"
#include "tiles.h"
#include "map.h"

// // Window size
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int TILE_SIZE = 40;
const int VIEW_WIDTH = 25;
const int VIEW_HEIGHT = 20;


class Application {
private:
    int topLeftX, topLeftY;
    bool needQuit;
public:
    Application() : topLeftX(0), topLeftY(0), needQuit(false) {}
    bool NeedQuit() const { return needQuit; }
    void Interval() {
        FsPollDevice();
        int key = FsInkey();
        switch (key) {
            case FSKEY_LEFT:
                if (topLeftX - 4 >= 0) topLeftX -= 4;
                break;
            case FSKEY_RIGHT:
                if (topLeftX + 4 < mapW - VIEW_WIDTH) topLeftX += 4;
                break;
            case FSKEY_DOWN:
                if (topLeftY + 4 < mapH - VIEW_HEIGHT) topLeftY += 4;
                break;
            case FSKEY_UP:
                if (topLeftY - 4 >= 0) topLeftY -= 4;
                break;
            case FSKEY_ENTER:
                topLeftX = 0;
                topLeftY = 0;
                break;
            case FSKEY_ESC:
                needQuit = true;
                break;
        }
    }
    void Draw() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int by = 0; by < VIEW_HEIGHT; ++by) {
            for (int bx = 0; bx < VIEW_WIDTH; ++bx) {
                int mapX = topLeftX + bx;
                int mapY = topLeftY + by;
                if (mapX >= 0 && mapX < mapW && mapY >= 0 && mapY < mapH) {
                    int tileIndex = map[mapY * mapW + mapX];
                    glRasterPos2i(bx * TILE_SIZE, by * TILE_SIZE);
                    glDrawPixels(TILE_SIZE, TILE_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, tiles[tileIndex]);
                }
            }
        }
        FsSwapBuffers();
    }
};

int main(void)
{
    FsOpenWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
    Application app;
    while (!app.NeedQuit()) {
        app.Interval();
        app.Draw();
    }
    return 0;
}
