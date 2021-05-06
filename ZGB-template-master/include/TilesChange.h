#ifndef TILESCHANGE_H
#define TILESCHANGE_H

#include <gb/gb.h>
#include <string.h>
#include "ZGBMain.h"
#include "SpriteManager.h"
#include <BankManager.h>
#include "Scroll.h"


typedef struct RestoreTileTypeStruct
{
	UINT16 x;
	UINT16 y;
	UINT8 type;
} RestoreTileType;


void SpriteSetTiles(struct TilesInfo* data, UINT8 first_tile, UINT8 last_tile);
void Tile_Anim(UINT8 x, int tilesAmount, struct TilesInfo* tile, UINT8 first_tile);
void SetTileCross(RestoreTileType* newTile, UINT8 bkgPalette);
void SetScrollCollisionTile(UINT16 x, UINT16 y, UINT8 tile);


#endif