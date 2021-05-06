#include "Banks/SetBank4.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 soulOrb_1[] = {6, 0, 1, 2, 0, 1, 2};
extern BOOLEAN transf1;
void Start_SpriteSoulOrb()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  data->state = 0;
  data->accel_y = 0;
  data->canDo = FALSE;
  data->direction = FALSE;
  THIS->lim_y = -20;
}
void Update_SpriteSoulOrb()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  struct Sprite *spr = 0;
  UINT8 i = 0;
  SPRITE_SET_DMG_PALETTE(THIS, 0);
  SetSpriteAnim(THIS, soulOrb_1, 15);
  switch (data->state)
  {
  case 0:
    if (data->accel_y > -60)
    {
      data->accel_y -= 2;
    }
    if (THIS->y <= 19)
    {
      data->state = 1;
    }
    break;

  case 1:
    data->accel_y = 0;
    if (THIS->x < 73)
    {
      THIS->x++;
    }
    else if (THIS->x > 73)
    {
      THIS->x--;
    }
    else
    {
      data->state = 2;
    }
    break;

  case 2:
    if (data->accel_y < 60)
    {
      data->accel_y += 4;
    }
    if (THIS->y >= 112)
    {
      data->state = 3;
      data->canDo = TRUE;
    }
    break;
  case 3:
    data->accel_y = 0;

    break;
  }

  switch (THIS->anim_frame)
  {
  case 0:
    if (!data->direction)
    {
      data->direction = TRUE;
      SpriteManagerAdd(SpriteShine, THIS->x + 6, THIS->y + 10);
    }
    break;
  case 1:
    if (data->direction)
    {
      data->direction = FALSE;
      SpriteManagerAdd(SpriteShine, THIS->x, THIS->y + 10);
    }
    break;
  case 3:
    if (!data->direction)
    {
      data->direction = TRUE;
      SpriteManagerAdd(SpriteShine, THIS->x + 3, THIS->y + 10);
    }
    break;
  case 4:
    if (data->direction)
    {
      data->direction = FALSE;
      SpriteManagerAdd(SpriteShine, THIS->x + 9, THIS->y + 10);
    }
    break;
  }

  THIS->y = THIS->y + (data->accel_y >> 4);

  SPRITEMANAGER_ITERATE(i, spr)
  {

    if (spr->type == SpritePlayer)
    {
      if (CheckCollision(THIS, spr))
      {
        if(current_state == StateGame){
          transf1 = TRUE;
        }
        SpriteManagerRemove(THIS_IDX);
      }
    }
  }
}
void Destroy_SpriteSoulOrb()
{
}