#include "sprite.h"
#include <string.h>

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;

typedef uint32 Tile[16];
typedef Tile TileBlock[256];

#define VIDEOMODE_0    0x0000
#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040

#define REG_DISPLAYCONTROL     *((volatile uint16*)(0x04000000))

#define MEM_VRAM        ((volatile uint32*)0x6000000)
#define MEM_TILE		((TileBlock*)0x6000000 )
#define MEM_PALETTE   ((uint16*)(0x05000200))
#define SCREEN_W    	240
#define SCREEN_H         160

typedef struct ObjectAttributes {
	uint16 attr0;
	uint16 attr1;
	uint16 attr2;
	uint16 pad;
} __attribute__((packed, aligned(4))) ObjectAttributes;

#define MEM_OAM            ((volatile ObjectAttributes *)0x07000000)

#define REG_VCOUNT_ADDR 0x04000006
#define REG_VCOUNT      (* (volatile uint16*) REG_VCOUNT_ADDR)

inline void vsync()
{
  while (REG_VCOUNT >= 160);
  while (REG_VCOUNT < 160);
}

int main()
{
	memcpy(MEM_PALETTE, spritePal,  spritePalLen );
    memcpy(&MEM_TILE[4][1], spriteTiles, spriteTilesLen);

    volatile ObjectAttributes *spriteAttribs = &MEM_OAM[0];

    spriteAttribs->attr0 = 0x2032; // 8bpp tiles, SQUARE shape, at y coord 50
    spriteAttribs->attr1 = 0x4064; // 16x16 size when using the SQUARE shape
    spriteAttribs->attr2 = 2;      // Start at the first tile in tile

	REG_DISPLAYCONTROL =  VIDEOMODE_0 | ENABLE_OBJECTS | MAPPINGMODE_1D;//0x1000 | 0x0040;

	int x = -50;
    while(1)
	{
		vsync();
		x = (x+1) % (SCREEN_W);
		spriteAttribs->attr1 = 0x4000 | (0x1FF & x);

	}
    return 0;
}
