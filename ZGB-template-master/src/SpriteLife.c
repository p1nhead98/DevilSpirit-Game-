#include "Banks/SetBank4.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Palette.h"
#include "Misc.h"

const UINT8 life_1[] = {2, 0, 0};
const UINT8 life_2[] = {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const UINT8 life_3[] = {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const UINT8 life_4[] = {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
extern INT8 current_life;
extern UINT8 max_life;
struct Sprite *life_parent = 0;

void DestroyLifeSound()
{
  PlayFx(CHANNEL_1, 10, 0x35, 0xbd, 0xf7, 0xac, 0x86);
}
void Start_SpriteLife()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  data->canDo = FALSE;
  data->accel_y = -30;
  data->state = 0;
  SPRITE_SET_DMG_PALETTE(THIS, 0);
}
void Update_SpriteLife()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  struct Sprite *spr = 0;
  UINT8 i = 0;

  if (data->accel_y > 1)
  {
    if (!data->canDo)
    {
      data->canDo = TRUE;
    }
  }
  switch (data->state)
  {
  case 0:
    SetSpriteAnim(THIS, life_1, 2);
    SPRITE_SET_DMG_PALETTE(THIS, 0);
    if (THIS->anim_frame == 1)
    {
      data->state = 1;
    }
    break;
  case 1:
    SetSpriteAnim(THIS, life_2, 10);
    if (THIS->anim_frame % 2 == 0)
    {
      SPRITE_SET_DMG_PALETTE(THIS, 1);
    }
    else
    {
      SPRITE_SET_DMG_PALETTE(THIS, 0);
    }
    if (THIS->anim_frame == 9)
    {
      data->state = 2;
    }
    break;
  case 2:
    SetSpriteAnim(THIS, life_3, 20);
    if (THIS->anim_frame % 2 == 0)
    {
      SPRITE_SET_DMG_PALETTE(THIS, 1);
    }
    else
    {
      SPRITE_SET_DMG_PALETTE(THIS, 0);
    }
    if (THIS->anim_frame == 9)
    {
      data->state = 3;
    }
    break;
  case 3:
    SetSpriteAnim(THIS, life_4, 30);
    if (THIS->anim_frame % 2 == 0)
    {
      SPRITE_SET_DMG_PALETTE(THIS, 1);
    }
    else
    {
      SPRITE_SET_DMG_PALETTE(THIS, 0);
    }
    if (THIS->anim_frame == 19)
    {
      SpriteManagerRemove(THIS_IDX);
    }
    break;
  }

  if (life_parent == 0)
  {
    if (data->accel_y < 60)
    {
      data->accel_y += 4;
    }
    data->collision = TranslateSprite(THIS, 0, (data->accel_y >> (4 << delta_time)));
    if (!data->collision && delta_time != 0 && data->accel_y < 40)
    {
      data->accel_y += 4 >> delta_time;
      data->collision = TranslateSprite(THIS, 0, (data->accel_y >> 4));
    }
    if (data->collision && !TranslateSprite(THIS, 0, (data->accel_y >> (-4 << delta_time))))
    {
      data->accel_y = 0;
    }
  }
  SPRITEMANAGER_ITERATE(i, spr)
  {
    if (spr->type == SpritePlayer)
    {
      if (CheckCollision(THIS, spr))
      {
        if (data->canDo)
        {
          current_life += 2;
          if (current_life > max_life)
          {
            current_life = max_life;
          }
          RefreshLife();
          DestroyLifeSound();
          SpriteManagerRemove(THIS_IDX);
        }
      }
    }
  }
}
void Destroy_SpriteLife()
{
}