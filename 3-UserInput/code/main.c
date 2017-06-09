#include "input.h"
#include <string.h>
#include "tiles.h"

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;

typedef uint16 ScreenBlock[1024];
typedef uint16 Tile[32];
typedef Tile TileBlock[256];

#define VIDEOMODE_0    0x0000
#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040
#define BACKGROUND_0   0x0100

#define REG_DISPLAYCONTROL     *((volatile uint16*)(0x04000000))
#define REG_VCOUNT             *((volatile uint16*)(0x04000006))
#define REG_BG0_CONTROL        *((volatile uint32*)(0x04000008))

#define MEM_VRAM                ((volatile uint32*)0x6000000)
#define MEM_TILE		        ((TileBlock*)0x6000000)
#define MEM_SCREENBLOCKS        ((ScreenBlock*)0x6000000)

#define MEM_BG_PALETTE          ((uint16*)(0x05000000))
#define MEM_PALETTE             ((uint16*)(0x05000200))
#define MEM_OAM            		((volatile ObjectAttributes *)0x07000000)

typedef struct ObjectAttributes {
	uint16 attr0;
	uint16 attr1;
	uint16 attr2;
	uint16 pad;
} __attribute__((packed, aligned(4))) ObjectAttributes;

inline void vsync()
{
  while (REG_VCOUNT >= 160);
  while (REG_VCOUNT < 160);
}

inline uint16 RGB15(uint32 red, uint32 green, uint32 blue)
{
    return red | (green<<5) | (blue<<10);
}

void LoadTileData()
{
    //each letter is 32 tiles
    memcpy(MEM_PALETTE, Pal,  PalLen );
    memcpy(&MEM_TILE[4][0], ATiles, TileLen);
    memcpy(&MEM_TILE[4][32], BTiles, TileLen);
    memcpy(&MEM_TILE[4][64], SelectTiles, TileLen);
    memcpy(&MEM_TILE[4][96], StartTiles, TileLen);
    memcpy(&MEM_TILE[4][128], RIGHTTiles, TileLen);
    memcpy(&MEM_TILE[4][160], LEFTTiles, TileLen);
    memcpy(&MEM_TILE[4][192], UPTiles, TileLen);
    memcpy(&MEM_TILE[4][224], DOWNTiles, TileLen);
    memcpy(&MEM_TILE[5][0], LTiles, TileLen);
    memcpy(&MEM_TILE[5][32], RTiles, TileLen);

    volatile ObjectAttributes *spriteAttribs = &MEM_OAM[0];

    spriteAttribs->attr0 = 0x602F;
    spriteAttribs->attr1 = 0xC04F;
    spriteAttribs->attr2 = 0;
}
void ClearSprite()
{
    volatile ObjectAttributes *spriteAttribs = &MEM_OAM[0];
    spriteAttribs->attr0 = 0x60AF;
    MEM_BG_PALETTE[0] = 0;

}

void DrawSprite(uint16 key_code)
{
    const uint16 keys[] = {KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_L, KEY_R};
    const uint16 bgCols[] = {RGB15(16,0,0), RGB15(0,16,0), RGB15(0,0,16),RGB15(16,16,0),RGB15(16,16,16),RGB15(32,16,0),RGB15(32,0,16),RGB15(16,0,32),RGB15(16,32,0),RGB15(32,0,32)};

	int idx = 0;
    for (int i = 0; i < 10; ++i)
    {
        if (keys[i] == key_code)
        {
            idx = i;
            break;
        }
    }

    volatile ObjectAttributes *spriteAttribs = &MEM_OAM[0];
    spriteAttribs->attr0 = 0x602F;
    spriteAttribs->attr1 = 0xC04F;
    spriteAttribs->attr2 = idx * 32 * 2;

    MEM_BG_PALETTE[0] = bgCols[idx];

}

void CreateBackground()
{
    MEM_BG_PALETTE[0] = RGB15(0,0,0);

    uint8 tile[64];
    for (int j = 0; j < 64; j++)
    {
        tile[j] = 0;
    }
    memcpy(MEM_TILE[0][0], tile, 64);

    uint16 screenBlock[1024];
    for (int i = 0; i < 1024; i++)
    {
        screenBlock[i] = 0;
    }

    memcpy(MEM_SCREENBLOCKS[1], &screenBlock[0], 2048);

    REG_BG0_CONTROL = 0x0180;

}

int main()
{
    CreateBackground();
    LoadTileData();

    REG_DISPLAYCONTROL =  VIDEOMODE_0 | ENABLE_OBJECTS | BACKGROUND_0 | MAPPINGMODE_1D;
    key_poll();
    ClearSprite();

    while(1)
    {
        vsync();
        key_poll();

        const uint16 keys[] = {KEY_A, KEY_B, KEY_SELECT,
                                KEY_START, KEY_RIGHT, KEY_LEFT,
                                KEY_UP, KEY_DOWN, KEY_L, KEY_R};
        for (int i = 0; i < 10; ++i)
        {
            if (getKeyState(keys[i]))
            {
                DrawSprite(keys[i]);
            }
        }
    }

    return 0;
}
