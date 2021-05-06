#include <string.h>
#include "ZGBMain.h"
#include "SpriteManager.h"
#include <BankManager.h>
#include "TilesChange.h"
#include "Scroll.h"



void SpriteSetTiles(struct TilesInfo *data, UINT8 first_tile, UINT8 last_tile)
{
	PushBank(data->bank);
	set_sprite_data(first_tile, last_tile, &data->data->data[0]);
	POP_BANK;
}

void Tile_Anim(UINT8 x, int tilesAmount, struct TilesInfo *tile, UINT8 first_tile)
{
	unsigned char tileData[16];
	PUSH_BANK(tile->bank);
	memcpy(tileData, &tile->data->data[16 * (x % tilesAmount)], 16);
	set_bkg_data(first_tile, 1, tileData);
	POP_BANK;
}
void SetTileCross(RestoreTileType* newTile, UINT8 bkgPalette)
{
    static unsigned char _newTiles[1];
#ifdef CGB
    if (_cpu == CGB_TYPE) {
        _newTiles[0] = bkgPalette;
        VBK_REG = 1;
        set_bkg_tiles(newTile->x, newTile->y, 1, 1, _newTiles);
        VBK_REG = 0;
    }
#endif
    _newTiles[0] = newTile->type;
    set_bkg_tiles(newTile->x, newTile->y, 1, 1, _newTiles);//gb.h
}
void SetScrollCollisionTile(UINT16 x, UINT16 y, UINT8 tile)
{
	UINT8 *ret;
	PUSH_BANK(scroll_bank);
	ret = GetScrollTilePtr(x, y); //Scroll.c
	*ret = tile;
	POP_BANK;
}
