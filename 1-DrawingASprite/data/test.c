
//{{BLOCK(test)

//======================================================================
//
//	test, 8x8@8, 
//	+ palette 3 entries, not compressed
//	+ 1 tiles not compressed
//	Total size: 6 + 64 = 70
//
//	Time-stamp: 2017-04-05, 06:01:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned short testTiles[32] __attribute__((aligned(4)))=
{
	0x0000,0x0000,0x0000,0x0000,0x0001,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0102,0x0202,0x0202,0x0202,0x0000,0x0000,0x0000,0x0000,
};

const unsigned short testPal[4] __attribute__((aligned(4)))=
{
	0x001F,0x03E0,0x7C00,
};

//}}BLOCK(test)
