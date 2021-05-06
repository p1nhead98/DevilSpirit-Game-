#include "Banks/SetBank4.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 skel_1[] = {4, 0, 1, 0, 2};
const UINT8 skel_2[] = {3, 3, 4, 4};

extern BOOLEAN start;
extern INT8 player_state;

void SkelDeathSound()
{
  PlayFx(CHANNEL_4, 8, 0x3b, 0xd1, 0x79, 0x80);
}

void Start_SpriteSkeleton()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  data->state = 0;
  data->counter = 0;
  THIS->coll_x = 3;
  THIS->coll_y = 2;
  THIS->coll_w = 10;
  THIS->coll_h = 13;
  if (THIS->x > scroll_target->x)
  {
    SPRITE_SET_VMIRROR(THIS);
  }
  else
  {
    SPRITE_UNSET_VMIRROR(THIS);
  }
}

void Update_SpriteSkeleton()
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
    switch (data->state)
    {

    case 0:
      SetSpriteAnim(THIS, skel_1, 9);
      if (data->counter <= 60)
      {
        data->counter++;
        if (data->counter % 2 == 0)
        {
          EnemyMovement(THIS, 1);
        }
        if (data->counter == 60)
        {
          data->state = 1;
          if (THIS->x > scroll_target->x)
          {
            SPRITE_SET_VMIRROR(THIS);
          }
          else
          {
            SPRITE_UNSET_VMIRROR(THIS);
          }
        }
      }

      break;

    case 1:
      SetSpriteAnim(THIS, skel_2, 9);

      if (THIS->anim_frame == 1 && data->counter == 60)
      {
        Shoot(THIS, SpriteBone);
        data->counter = 0;
      }
      if (THIS->anim_frame == 2)
      {
        data->state = 0;
      }
      break;

    case 2:
      DropItem(THIS);
      Shoot(THIS, SpriteExplosion);
      SpriteManagerRemove(THIS_IDX);
      break;
    }

    SPRITEMANAGER_ITERATE(i, spr)
    {

      if (spr->type == SpriteChain)
      {
        if (CheckCollision(THIS, spr))
        {
          SkelDeathSound();
          data->state = 2;
        }
      }
      else if (spr->type == SpritePlayer && player_state == 8)
      {
        if (CheckCollision(THIS, spr))
        {
          if (SPRITE_GET_VMIRROR(spr))
          {
            if (THIS->x < spr->x)
            {
              ScreenShake(10);
              SkelDeathSound();
              data->state = 2;
            }
          }
          else
          {
            if (THIS->x > spr->x)
            {
              ScreenShake(10);
              SkelDeathSound();
              data->state = 2;
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

void Destroy_SpriteSkeleton()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
}