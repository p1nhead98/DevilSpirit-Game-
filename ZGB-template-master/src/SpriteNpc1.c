#include "Banks/SetBank4.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 npc1_anim[] = {4, 0, 1, 2, 3};
extern BOOLEAN start;
void Start_SpriteNpc1()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  if (THIS->x < scroll_target->x)
  {
    SpriteManagerRemove(THIS_IDX);
  }
  SetSpriteAnim(THIS, npc1_anim, 7);
}

void Update_SpriteNpc1()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  if (start == FALSE)
  {
    if (THIS->anim_speed == 0)
    {
      THIS->anim_speed = data->last_anim_speed;
    }
    THIS->x--;
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

void Destroy_SpriteNpc1()
{
}