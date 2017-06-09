#ifndef GBA_H
#define GBA_H

//TYPES
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef short              sint16;

typedef uint16 ScreenBlock[1024];
typedef uint16 Tile[32];
typedef Tile TileBlock[256];

typedef struct ObjectAttributes {
	uint16 attr0;
	uint16 attr1;
	uint16 attr2;
	uint16 pad;
} __attribute__((packed, aligned(4))) ObjectAttributes;

typedef struct AffineMatrix
{
    uint16 fill0[3];
    sint16 pa;
    uint16 fill1[3];
    sint16 pb;
    uint16 fill2[3];
    sint16 pc;
    uint16 fill3[3];
    sint16 pd;
} __attribute__((packed, aligned(4))) AffineMatrix;;

//DEFINES
#define SCREEN_W    	240
#define SCREEN_H        160

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

#define MEM_OAM            ((volatile ObjectAttributes *)0x07000000)

#define REG_VCOUNT_ADDR 0x04000006

//FUNCTIONS

inline void vsync()
{
    while (REG_VCOUNT >= 160);
    while (REG_VCOUNT < 160);
}

inline uint16 RGB15(uint32 red, uint32 green, uint32 blue)
{
    return red | (green<<5) | (blue<<10);
}


#endif
