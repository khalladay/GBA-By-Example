#include <string.h>
#include "tiles.h"
#include "bg.h"

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;

#define SCREEN_W    	240
#define SCREEN_H        160

typedef uint16 ScreenBlock[1024];
typedef uint16 Tile[32];
typedef Tile TileBlock[256];

#define VIDEOMODE_0    0x0000
#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040
#define BACKGROUND_0   0x0100
#define BACKGROUND_1   0x0200

#define REG_DISPLAYCONTROL     *((volatile uint16*)(0x04000000))
#define REG_VCOUNT             *((volatile uint16*)(0x04000006))

#define REG_BG0_CONTROL        *((volatile uint32*)(0x04000008))
#define REG_BG1_CONTROL        *((volatile uint32*)(0x0400000A))
#define REG_BG0_SCROLL_H       *((volatile uint16*)(0x04000010))
#define REG_BG0_SCROLL_V       *((volatile uint16*)(0x04000012))
#define REG_BG1_SCROLL_H	   *((volatile uint16*)(0x04000014))
#define REG_BG1_SCROLL_V	   *((volatile uint16*)(0x04000016))


#define MEM_VRAM                ((volatile uint32*)0x6000000)
#define MEM_TILE		        ((TileBlock*)0x6000000)
#define MEM_SCREENBLOCKS        ((ScreenBlock*)0x6000000)

#define MEM_BG_PALETTE          ((uint16*)(0x05000000))
#define MEM_PALETTE             ((uint16*)(0x05000200))

typedef struct ObjectAttributes {
	uint16 attr0;
	uint16 attr1;
	uint16 attr2;
	uint16 pad;
} __attribute__((packed, aligned(4))) ObjectAttributes;

#define MEM_OAM            ((volatile ObjectAttributes *)0x07000000)

#define REG_VCOUNT_ADDR 0x04000006

inline void vsync()
{
  while (REG_VCOUNT >= 160);
  while (REG_VCOUNT < 160);
}


inline uint16 RGB15(uint32 red, uint32 green, uint32 blue)
{
    return red | (green<<5) | (blue<<10);
}


int main()
{
    //load data
	memcpy(MEM_BG_PALETTE, bgPal, bgPalLen );
	memcpy(&MEM_TILE[0][0], bgTiles, bgTilesLen);
	memcpy(&MEM_SCREENBLOCKS[1], checkerBg, checkerBgLen);

    //generate data

    //we've uploaded 4 colours to palette memory
	//so make sure we don't overwrite those
    for (uint16 i = 0; i < 32; i++)
    {
        *((uint16*)(MEM_BG_PALETTE+(4+i))) = RGB15(i,i,i);
    }

    //every tile is 64 palette indices
	//we have 32 grayscale values from above
	//so we need to make 32 tiles, each one a solid
	//gray
    uint8 tile[64];
    for (uint16 i = 0; i < 32; ++i)
    {
        for (int j = 0; j < 64; j++)
        {
            tile[j] = 4 + (i);
        }
        memcpy(MEM_TILE[1][i], tile, 64);
    }

    //generate 2 screen blocks,
	//each gray value getting two tiles of width
	for (int block = 0; block < 2; ++block)
	{
		uint16 screenBlock[1024];
		for (uint16 i = 0; i < 32; ++i)
		{
			for (uint16 j = 0; j < 32; ++j)
			{
				screenBlock[i * 32 + j] =  (j/2) + (block*16);
			}
		}
		memcpy(MEM_SCREENBLOCKS[block+2], &screenBlock[0], 2048);

	}


    REG_BG0_CONTROL = 0x0180;// 0000 0001 1000 0000;
    REG_BG1_CONTROL = 0x4285; // 0100 0010 1000 0101
	REG_DISPLAYCONTROL =  VIDEOMODE_0 | BACKGROUND_0 | BACKGROUND_1;
    int hScroll = 0;
    int h2Scroll = 0;
    while(1)
	{
		vsync();
        vsync();

        REG_BG0_SCROLL_H = -hScroll;
        REG_BG1_SCROLL_H = h2Scroll;
        h2Scroll +=2;
        hScroll = h2Scroll/3;
	}
    return 0;
}
