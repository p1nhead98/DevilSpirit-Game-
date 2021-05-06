#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateGame)\
STATE_DEF_END

#define SPRITES \
_SPRITE_DMG(SpritePlayer, player)\
_SPRITE_DMG(SpriteChain, sword)\
_SPRITE_DMG(SpriteSkeleton, skeleton)\
_SPRITE_DMG(SpriteBone, bone)\
_SPRITE_DMG(SpriteNpc1, npc1)\
_SPRITE_DMG(SpriteWaterSplash, waterSplash)\
_SPRITE_DMG(SpriteSkelNpc1, skeletonNpc1)\
_SPRITE_DMG(SpriteExplosion, explosion)\
_SPRITE_DMG(SpriteEye, eye)\
_SPRITE_DMG(SpriteHellDog, hellDog)\
_SPRITE_DMG(SpriteHDogFire, fireBall)\
_SPRITE_DMG(SpriteStoneMan, stoneman)\
_SPRITE_DMG(SpriteTdNpc2, TdNpc)\
_SPRITE_DMG(SpriteTinyDevil, tinyDevil)\
_SPRITE_DMG(SpriteGhost, ghost)\
_SPRITE_DMG(SpriteUp, up)\
_SPRITE_DMG(SpriteMidBoss1, midBoss1)\
_SPRITE_DMG(SpriteStone, stone)\
_SPRITE_DMG(SpriteLife, life)\
_SPRITE_DMG(SpriteSoulOrb, soulOrb)\
_SPRITE_DMG(SpriteShine, shine)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif