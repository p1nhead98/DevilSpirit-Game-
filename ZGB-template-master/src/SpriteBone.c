#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 bone_anim[] = {2, 0, 0};

struct Sprite *bone_parent = 0;

extern BOOLEAN start;

void Start_SpriteBone()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  data->state = 0;
  data->accel_y = 0;
  THIS->coll_x = 6;
  THIS->coll_y = 6;
  THIS->coll_w = 6;
  THIS->coll_h = 6;
}
void Update_SpriteBone()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;

  if (start == FALSE)
  {
    if (THIS->anim_speed == 0)
    {
      THIS->anim_speed = data->last_anim_speed;
    }

    switch (data->state)
    {
    case 0:
      Jump(data, 40);
      SetSpriteAnim(THIS, bone_anim, 11);
      if (SPRITE_GET_VMIRROR(THIS))
      {
        data->state = 1;
      }
      else
      {
        data->state = 2;
      }
      break;
    case 1:
      THIS->x--;
      if (THIS->anim_frame == 1)
      {
        SPRITE_SET_VMIRROR(THIS);
      }
      else
      {
        SPRITE_UNSET_VMIRROR(THIS);
      }
      break;
    case 2:
      THIS->x++;
      if (THIS->anim_frame == 1)
      {
        SPRITE_SET_VMIRROR(THIS);
      }
      else
      {
        SPRITE_UNSET_VMIRROR(THIS);
      }
      break;
    }

    if (data->accel_y < 60)
    {
      data->accel_y = data->accel_y + 2;
    }

    THIS->y = THIS->y + (data->accel_y >> 4);
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
void Destroy_SpriteBone()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
}