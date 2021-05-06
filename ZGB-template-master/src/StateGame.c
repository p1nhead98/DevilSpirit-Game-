#include "Banks/SetBank2.h"

#include "..\res\src\towntiles.h"
#include "..\res\src\housetiles.h"
#include "..\res\src\town1_3.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Palette.h"
#include "Print.h"
#include "Sprite.h"
#include "main.h"
#include "Keys.h"
#include "MapInfo.h"
#include "levels.h"
#include "gbt_player.h"

#include "TilesChange.h"
#include "../res/src/font.h"
#include "../res/src/waterAnim.h"
#include "../res/src/waterCrash.h"
#include "../res/src/sky1.h"
#include "../res/src/sky2.h"

#include "../res/src/window.h"

UINT8 col_tiles_town[] = {1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 31, 32, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 0};
UINT8 col_tiles_house[] = {2, 3, 4, 5, 12, 11, 19, 20, 23, 24, 25, 26, 27, 28, 34, 0};
UINT8 col_down_town[] = {126, 127, 0};

extern UINT8 current_level;
extern UINT8 last_level;
extern const UINT8 num_levels;
extern const struct MapInfo *levels[];
UINT8 x = 0;
UINT8 x2 = 0;
UINT8 count = 1;
UINT8 count2 = 1;

extern BOOLEAN start;
extern BOOLEAN skNpc1;
extern BOOLEAN tdNpc2;
extern BOOLEAN canSpawn;
extern UINT8 max_life;
extern INT8 current_life;
extern BOOLEAN canEnter;
extern UINT8 *tema1_mod_Data[];

BOOLEAN canEffect = FALSE;
// unsigned char map_clone_data[34 * 34];
// unsigned char map_clone_data_attribs[34 * 34];
// struct MapInfoInternal map_clone_internal = {map_clone_data, 0, 0, map_clone_data_attribs, 0}; //data, w, h, attributes, tiles
// struct MapInfo map_clone = {0, &map_clone_internal};										   //bank, MapInfoInternal

void Start_StateGame()
{
	const struct MapInfo *level = levels[current_level];
	UINT8 i = 0;
	SPRITES_8x16;
	canEffect = FALSE;
	INIT_FONT(font, PRINT_WIN);
	WX_REG = 7;
	WY_REG = (144 - (2 << 3));
	scroll_h_border = 2 << 3;
	InitWindow(0, 0, &window);
	SHOW_WIN;

	OBP0_REG = PAL_DEF(2, 0, 1, 3);
	OBP1_REG = PAL_DEF(0, 0, 0, 0);
	canEnter = FALSE;
	max_life = 8;
	SpriteManagerLoad(0);
	SpriteManagerLoad(1);
	SpriteManagerLoad(7);
	SpriteManagerLoad(15);
	SpriteManagerLoad(18);
	switch (current_level)
	{
	case 0:

		SpriteManagerLoad(2);
		SpriteManagerLoad(3);
		SpriteManagerLoad(4);
		SpriteManagerLoad(5);
		SpriteManagerLoad(6);
		SpriteManagerLoad(8);
		PlayMusic(tema1_mod_Data, 5, 1);
		current_life = max_life;
		skNpc1 = FALSE;

		scroll_target = SpriteManagerAdd(SpritePlayer, 24, 96);
		InitScroll(level, col_tiles_town, col_down_town);
		canEffect = TRUE;
		break;

	case 1:
		SpriteManagerLoad(2);
		SpriteManagerLoad(3);
		SpriteManagerLoad(8);
		SpriteManagerLoad(11);

		scroll_target = SpriteManagerAdd(SpritePlayer, 24, 352);
		InitScroll(level, col_tiles_house, col_down_town);
		break;
	case 2:
		SpriteManagerLoad(12);
		SpriteManagerLoad(13);
		SpriteManagerLoad(14);

		tdNpc2 = TRUE;

		scroll_target = SpriteManagerAdd(SpritePlayer, 72, 88);
		canSpawn = TRUE;
		//MapClone(level, &map_clone);
		InitScroll(level, col_tiles_town, col_down_town);
		canEffect = TRUE;
		break;
	case 3:
		SpriteManagerLoad(8);
		SpriteManagerLoad(9);
		SpriteManagerLoad(10);
		SpriteManagerLoad(11);
		SpriteManagerLoad(13);

		scroll_target = SpriteManagerAdd(SpritePlayer, 40, 64);
		InitScroll(level, col_tiles_house, col_down_town);
		break;
	case 4:
		SpriteManagerLoad(2);
		SpriteManagerLoad(3);
		SpriteManagerLoad(8);
		SpriteManagerLoad(11);
		SpriteManagerLoad(14);

		scroll_target = SpriteManagerAdd(SpritePlayer, 32, 40);
		InitScroll(level, col_tiles_town, col_down_town);
		canEffect = TRUE;
		break;
	case 5:
		scroll_target = SpriteManagerAdd(SpritePlayer, 8, 128);
		SpriteManagerLoad(16);
		SpriteManagerLoad(17);
		SpriteManagerLoad(19);
		SpriteManagerLoad(20);
		InitScroll(level, col_tiles_town, col_down_town);
		canEffect = TRUE;
		break;
	}

	SHOW_BKG;

	SHOW_SPRITES;
	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
}

void Update_StateGame()
{

	if (KEY_TICKED(J_START) && start == FALSE)
	{

		OBP0_REG = PAL_DEF(0, 1, 2, 3);
		OBP1_REG = PAL_DEF(1, 1, 1, 1);
		BGP_REG  = PAL_DEF(1, 2, 3, 3);
		NR52_REG = 0x80; //Enables sound, you should always setup this first
		NR51_REG = 0xFF; //Enables all channels (left and right)
		NR50_REG = 0x10;
		start = TRUE;
	}
	else if (KEY_TICKED(J_START) && start == TRUE)
	{
		OBP0_REG = PAL_DEF(2, 0, 1, 3);
		OBP1_REG = PAL_DEF(0, 0, 0, 0);
		BGP_REG  = PAL_DEF(0, 1, 2, 3);
		NR52_REG = 0x80; //Enables sound, you should always setup this first
		NR51_REG = 0xFF; //Enables all channels (left and right)
		NR50_REG = 0x77;
		start = FALSE;
	}

	if (current_level == 4 && scroll_target->x > 620)
	{
		current_level++;
		SetState(current_state);
	}

	if ((current_level == 2 && scroll_target->x > 503 && scroll_target->x < 816 && canSpawn == TRUE) || (current_level == 4 && scroll_target->x > 503 && canSpawn == TRUE))
	{
		if (SPRITE_GET_VMIRROR(scroll_target))
		{
			SpriteManagerAdd(SpriteGhost, scroll_x - 16, scroll_target->y - 30);
		}
		else
		{
			SpriteManagerAdd(SpriteGhost, scroll_x + 176, scroll_target->y - 30);
		}
		canSpawn = FALSE;
	}

	if (KEY_PRESSED(J_DOWN) &&  KEY_TICKED(J_SELECT))
	{
		current_level++;
		SetState(current_state);
	}
	if (--count2 == 0 && start == FALSE && current_level == 0 && canEffect)
	{
		count2 = 4;
		x++;
		Tile_Anim(x, 8, &waterAnim, 37);
		Tile_Anim(x, 2, &waterCrash, 38);
	}
	else if (--count == 0 && start == FALSE && current_level != 1 && current_level != 3 && canEffect)
	{
		count = 2;
		x2--;
		Tile_Anim(x2, 16, &sky1, 21);
		Tile_Anim(x2, 16, &sky2, 27);
	}
	else
	{
		count = 1;
		count2 = 1;
	}
}