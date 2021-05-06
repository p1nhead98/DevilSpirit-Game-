#include "Banks/SetBank6.h"
#include "Keys.h"
#include <gb/gb.h>
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Palette.h"
#include "Misc.h"

const UINT8 mBoss_start[] = {2, 10, 10};
const UINT8 mBoss_start2[] = {3, 11, 11, 11};
const UINT8 mBoss_stand[] = {2, 0, 0};
const UINT8 mBoss_charging[] = {20, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
const UINT8 mBoss_attacking[] = {1, 4};
const UINT8 mBoss_jumping[] = {2, 6, 6};
const UINT8 mBoss_stomp[] = {1, 8};
const UINT8 mBoss_charging2[] = {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
BOOLEAN MB1CanHit = TRUE;
BOOLEAN MB1Death = TRUE;
struct Sprite *mb1_parent = 0;
UINT8 tile_collisionmb1;
extern BOOLEAN start;

void Mb1ChargeSfx()
{
  PlayFx(CHANNEL_1, 6, 0x45, 0x81, 0xd2, 0x05, 0x84);
}

void Mb1HitSfx()
{
  PlayFx(CHANNEL_4, 8, 0x3f, 0xf1, 0x6b, 0x80);
}
void Mb1FinalHitSfx()
{
  PlayFx(CHANNEL_4, 8, 0x3f, 0xf6, 0x5e, 0x80);
}

void Mb1DeathSfx()
{
  PlayFx(CHANNEL_4, 8, 0x3f, 0xf5, 0x6f, 0x80);
}
void Mb1ExplotionSfx()
{
  PlayFx(CHANNEL_4, 8, 0x3f, 0xf2, 0x6f, 0x80);
}

void mirrorMb()
{
  if (THIS->x < scroll_target->x)
  {
    SPRITE_UNSET_VMIRROR(THIS);
  }
  else
  {
    SPRITE_SET_VMIRROR(THIS);
  }
}

void midBoss1Shoot()
{
  struct Sprite *sprite_stone1 = SpriteManagerAdd(SpriteStone, THIS->x, THIS->y);
  sprite_stone1->flags = 0;
  sprite_stone1->unique_id = 1;
  struct Sprite *sprite_stone2 = SpriteManagerAdd(SpriteStone, THIS->x, THIS->y);
  sprite_stone2->flags = 0;
  sprite_stone2->unique_id = 2;

  struct Sprite *sprite_stone3 = SpriteManagerAdd(SpriteStone, THIS->x, THIS->y);
  sprite_stone3->flags = 32;
  sprite_stone3->unique_id = 1;
  struct Sprite *sprite_stone4 = SpriteManagerAdd(SpriteStone, THIS->x, THIS->y);
  sprite_stone4->flags = 32;
  sprite_stone4->unique_id = 2;
}

void mb1Jump(struct commonData *data, INT16 accelY, INT8 state)
{
  data->accel_y = -accelY;
  data->state = state;
}

void Start_SpriteMidBoss1()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  THIS->size = FRAME_16x16;
  SetSpriteAnim(THIS, mBoss_start, 2);
  data->state = 0;
  data->life = 18;
  data->counter = 0;
  data->canDo = TRUE;
  data->accel_y = 0;
  THIS->y = THIS->y + 1;
  MB1CanHit = TRUE;
  MB1Death = TRUE;
}
void Update_SpriteMidBoss1()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  struct Sprite *spr = 0;
  UINT8 i = 0;

  if (start == FALSE)
  {
    if (THIS->anim_speed == 0)
    {
      THIS->anim_speed = data->last_anim_speed;
    }
    if (data->counter < 20 && MB1CanHit == FALSE)
    {
      data->counter++;
      SPRITE_SET_PALETTE(THIS, 1);
      if (data->counter == 20)
      {
        data->counter = 0;
        MB1CanHit = TRUE;
        SPRITE_SET_PALETTE(THIS, 0);
      }
    }

    switch (data->state)
    {
    case 0:
      if (THIS->x < scroll_target->x)
      {
        SPRITE_UNSET_VMIRROR(THIS);
      }
      else
      {
        SPRITE_SET_VMIRROR(THIS);
      }
      data->accel_y = 0;
      if (THIS->anim_frame == 1)
      {
        mb1Jump(data, 90, 1);
      }
      break;
    case 1:
      SetSpriteAnim(THIS, mBoss_start2, 2);
      if (!(data->accel_y < 1))
      {

        data->state = 2;
      }
      break;
    case 2:
      THIS->size = FRAME_32x16;

      data->accel_y = 0;
      SetSpriteAnim(THIS, mBoss_start2, 1);
      if (THIS->anim_frame == 2)
      {
        data->state = 7;
      }
      break;
    case 3:
      SetSpriteAnim(THIS, mBoss_stand, 2);
      data->accel_y = 0;

      if (THIS->x < scroll_target->x)
      {
        SPRITE_UNSET_VMIRROR(THIS);
      }
      else
      {
        SPRITE_SET_VMIRROR(THIS);
      }
      if (THIS->anim_frame == 1)
      {
        data->state = 4;
      }
      break;
    case 4:
      SetSpriteAnim(THIS, mBoss_charging, 18);
      data->accel_y = 0;
      if (THIS->anim_frame % 2 == 0)
      {
        Mb1ChargeSfx();
        SPRITE_SET_DMG_PALETTE(THIS, 1);
      }
      else
      {

        SPRITE_SET_DMG_PALETTE(THIS, 0);
      }
      if (THIS->anim_frame == 9)
      {
        mb1Jump(data, 125, 5);
      }
      break;
    case 5:
      SPRITE_SET_DMG_PALETTE(THIS, 0);
      SetSpriteAnim(THIS, mBoss_jumping, 2);
      if (!(data->accel_y < 1))
      {
        data->state = 6;
      }
      break;
    case 6:
      SPRITE_SET_DMG_PALETTE(THIS, 0);
      SetSpriteAnim(THIS, mBoss_jumping, 2);
      data->accel_y = 0;
      THIS->x = scroll_target->x;
      if (THIS->anim_frame == 1)
      {
        data->state = 7;
      }
      break;
    case 7:

      SetSpriteAnim(THIS, mBoss_stomp, 1);

      if (TranslateSprite(THIS, 0, (data->accel_y >> 4)))
      {
        midBoss1Shoot();
        mirrorMb();
        ScreenShake(8);
        data->state = 8;
      }
      break;
    case 8:
      SetSpriteAnim(THIS, mBoss_charging, 8);

      if (THIS->anim_frame == 9)
      {
        data->state = 9;
      }
      break;
    case 9:
      SetSpriteAnim(THIS, mBoss_stand, 2);
      if (THIS->x < scroll_target->x)
      {
        SPRITE_UNSET_VMIRROR(THIS);
      }
      else
      {
        SPRITE_SET_VMIRROR(THIS);
      }
      if (THIS->anim_frame == 1)
      {
        data->state = 10;
      }
      break;
    case 10:
      SetSpriteAnim(THIS, mBoss_charging2, 18);
      data->accel_x = 0;
      if (THIS->anim_frame % 2 == 0)
      {
        Mb1ChargeSfx();
        SPRITE_SET_DMG_PALETTE(THIS, 1);
      }
      else
      {
        SPRITE_SET_DMG_PALETTE(THIS, 0);
      }
      if (THIS->anim_frame == 9)
      {
        data->state = 11;
      }
      break;
    case 11:
      SPRITE_SET_DMG_PALETTE(THIS, 0);
      SetSpriteAnim(THIS, mBoss_attacking, 1);
      if (SPRITE_GET_VMIRROR(THIS))
      {
        if (data->accel_x > -93)
        {
          data->accel_x = data->accel_x - 3;
        }

        if (THIS->x > 6)
        {
          TranslateSprite(THIS, (data->accel_x >> 4), 0);
        }
        else
        {
          ScreenShake(8);
          data->state = 3;
        }
      }
      else
      {
        if (data->accel_x < 93)
        {
          data->accel_x = data->accel_x + 3;
        }
        if (TranslateSprite(THIS, (data->accel_x >> 4), 0))
        {
          ScreenShake(8);
          data->state = 3;
        }
        else if (!scroll_collisions[GetScrollTile(((THIS->x + THIS->coll_x + THIS->coll_w) >> 3), (THIS->y >> 3) + 2u)] && !scroll_collisions_down[GetScrollTile(((THIS->x + THIS->coll_x + THIS->coll_w) >> 3), (THIS->y >> 3) + 2u)])
        {
          ScreenShake(8);
          data->state = 3;
        }
      }

      break;
    case 12:
      BGP_REG = PAL_DEF(0, 1, 2, 3);
      OBP0_REG = PAL_DEF(2, 0, 1, 3);
      OBP1_REG = PAL_DEF(0, 0, 0, 0);
      THIS->coll_x = 300;
      THIS->coll_y = 32;
      THIS->coll_w = 1;
      THIS->coll_h = 1;
      data->accel_y = 0;
      if (THIS->anim_frame % 2 == 0)
      {
        if (data->canDo)
        {
          data->canDo = FALSE;
          ScreenShake(6);
          Mb1ExplotionSfx();
          SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y - 16);
          SpriteManagerAdd(SpriteExplosion, THIS->x - 9, THIS->y);
        }
      }
      else
      {
        if (!data->canDo)
        {
          data->canDo = TRUE;
          ScreenShake(3);
          SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y);
          SpriteManagerAdd(SpriteExplosion, THIS->x + 8, THIS->y - 14);
        }
      }
      if (THIS->anim_frame == 19)
      {
        Mb1DeathSfx();
        SpriteManagerAdd(SpriteSoulOrb, THIS->x, THIS->y);
        SpriteManagerRemove(THIS_IDX);
        SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y);
        SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y - 16);
      }
      break;
    }

    if (mb1_parent == 0)
    {
      if (data->accel_y < 60)
      {
        data->accel_y += 4;
      }
      data->collision = TranslateSprite(THIS, 0, (data->accel_y >> (4 << delta_time)));
      if (!data->collision && delta_time != 0 && data->accel_y < 40)
      { //Do another iteration if there is no collision
        data->accel_y += 4 >> delta_time;

        data->collision = TranslateSprite(THIS, 0, (data->accel_y >> 4));
      }
      if (data->collision && !TranslateSprite(THIS, 0, (data->accel_y >> (-4 << delta_time))))
      {

        data->accel_y = 0;

        if (data->state == 5)
        {
          data->state = 3;
        }
        if (data->state == 7)
        {
          ScreenShake(8);
          midBoss1Shoot();
          data->state = 8;
        }

        //CheckCollisionTile();
      }
    }

    SPRITEMANAGER_ITERATE(i, spr)
    {

      if (spr->type == SpriteChain)
      {
        if (CheckCollision(THIS, spr))
        {
          if (data->life != 0)
          {
            if (MB1CanHit == TRUE)
            {
              Mb1HitSfx();
              MB1CanHit = FALSE;
              data->life--;
            }
          }
          else
          {
            if (MB1Death)
            {
              MB1Death = FALSE;
              BGP_REG = PAL_DEF(3, 3, 3, 3);
              OBP0_REG = PAL_DEF(0, 0, 0, 0);
              OBP1_REG = PAL_DEF(0, 0, 0, 0);
              Mb1FinalHitSfx();
              delay(500);
              SetSpriteAnim(THIS, mBoss_charging, 8);
              data->state = 12;
            }
          }
        }
      }
    }
  }
  else
  {
    if (THIS->anim_speed != 0)
    {
      data->last_anim_speed = THIS->anim_speed;
      THIS->anim_speed = 0;
    }
  }
}
void Destroy_SpriteMidBoss1()
{
}