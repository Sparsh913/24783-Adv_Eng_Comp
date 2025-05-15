#include <iostream>
#include "fssimplewindow.h"

#include "simplebitmap.h"
#include "hashtable.h"
#include <algorithm>

// Complete a hash function for SimpleBitmap so that you can make a hash table that maps a bitmap to an integer.
template <>
struct ysHash<SimpleBitmap>
{
	std::size_t operator()(const SimpleBitmap &bmp) const
	{
		// std::size_t sum=0;
		// return sum;
		const unsigned char *ptr=bmp.GetBitmapPointer();
		const int len = bmp.GetTotalNumComponent();

		std::size_t hash=0;
		std::size_t primes[] = {2,3,5,7,11};
		for(int i=0; i<len; ++i)
		{
			hash += ptr[i] * primes[i%5];
		}
		return hash;
	}
};


// Complete a hash function for an unsigned integer.
template <>
struct ysHash<unsigned int>
{
	std::size_t operator()(unsigned int id) const
	{
		return id;
	}
};

int main(int argc,char *argv[])
{
    if(argc<2)
    {
        std::cout << "You need to specify a PNG file name." << std::endl;
        return 1;
    }

    SimpleBitmap bmp;
    if(!bmp.LoadPng(argv[1]))
    {
        std::cout << "Failed to open the PNG file." << std::endl;
        return 1;
    }

    ysHashTable<SimpleBitmap, unsigned int> bmp2Id;
    ysHashTable<unsigned int, SimpleBitmap> id2Bmp;

    const int tileSize = 16;
    const int mapWidth = bmp.GetWidth()/tileSize;
    const int mapHeight = bmp.GetHeight()/tileSize;

    SimpleBitmapTemplate<unsigned int,1> tileMap;
    tileMap.Create(mapWidth, mapHeight);

    unsigned int nextTileId = 0;

    // Fill tile map and hash tables
    unsigned int* tileMapPtr = tileMap.GetEditableBitmapPointer();
    for (int y=0; y<mapHeight; ++y)
	{
		for (int x=0; x<mapWidth; ++x)
		{
			SimpleBitmap tile = bmp.CutOut(x*tileSize, y*tileSize, tileSize, tileSize);

			auto iter = bmp2Id.find(tile);
			unsigned int tileId;

			if (iter != bmp2Id.end())
			{
				tileId = iter->second;
			}
			else
			{
				tileId = nextTileId++;
				bmp2Id.insert(tile, tileId);
				id2Bmp.insert(tileId, tile);
			}
			
			tileMap.GetEditableBitmapPointer()[y * mapWidth + x] = tileId;
		}
	}


    FsOpenWindow(0,0,800,640,1);

    int drawX=0, drawY=0;

    // Helper Draw Function
    auto DrawBitmap = [](const SimpleBitmap& bmp, int posX, int posY)
    {
        glRasterPos2i(posX, posY + bmp.GetHeight());
        glDrawPixels(bmp.GetWidth(), bmp.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, bmp.GetBitmapPointer());
    };

    for(auto iter = id2Bmp.begin(); iter != id2Bmp.end(); ++iter)
    {
        iter->second.Invert();
        DrawBitmap(iter->second, drawX, drawY);
        drawX += tileSize;
        if(drawX >= 800)
        {
            drawX = 0;
            drawY += tileSize;
        }
    }

    FsSwapBuffers();

    while(1)
    {
        FsPollDevice();
        if(FsInkey() == FSKEY_ESC)
            break;
    }

    int offsetX=0, offsetY=0;
    const int screenTileWidth = 800/tileSize;
    const int screenTileHeight = 640/tileSize;

    bool terminate = false;
    while (!terminate)
    {
        FsPollDevice();
        auto key = FsInkey();
        switch (key)
        {
        case FSKEY_ESC:
            terminate = true;
            break;
        case FSKEY_LEFT:
            offsetX = std::max(offsetX-4, 0);
            break;
        case FSKEY_RIGHT:
            offsetX = std::min(offsetX+4, mapWidth - screenTileWidth);
            break;
        case FSKEY_UP:
            offsetY = std::max(offsetY-4, 0);
            break;
        case FSKEY_DOWN:
            offsetY = std::min(offsetY+4, mapHeight - screenTileHeight);
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        for(int y=0; y<screenTileHeight; ++y)
        {
            for(int x=0; x<screenTileWidth; ++x)
            {
                unsigned int tileId = tileMapPtr[(y+offsetY) * mapWidth + (x+offsetX)];
                auto tileIter = id2Bmp.find(tileId);
                if(tileIter != id2Bmp.end())
                {
                    DrawBitmap(tileIter->second, x * tileSize, y * tileSize);
                }
            }
        }
        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}

