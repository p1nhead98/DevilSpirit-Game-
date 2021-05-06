#include "ZGBMain.h"
#include "Math.h"

UINT8 next_state = StateGame;
extern UINT8 current_level;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateGame) {

		

		switch(current_level){
			case 0: *tile =2; break;
			case 1: *tile =0; break;
			case 2: *tile = 39; break;
			case 3: *tile = 0; break;
			case 4: *tile = 2; break;
			case 5: *tile = 18; break;
		}
		switch(*tile_ptr) {
				case 253: return SpriteSkeleton; break;
				case 252: return SpriteEye; break;
				case 251: return SpriteHellDog; break;
				case 250: return SpriteStoneMan; break;
				case 249: return SpriteNpc1; break;
				case 248: return SpriteSkelNpc1; break;
				case 247: return SpriteTdNpc2; break;
				case 246: return SpriteTinyDevil; break;
				case 245: return SpriteGhost; break;
				case 244: return SpriteMidBoss1; break;
				
		}
		*tile = *tile_ptr;
	}

	return 255u;
}