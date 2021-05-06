#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 ghost_anim[] = {2, 0, 1, 2};

extern BOOLEAN start;
extern BOOLEAN canSpawn;

void GhostDeathSound()
{
    PlayFx(CHANNEL_4, 10, 0x3f, 0xf1, 0x6b, 0x80);
}

void Start_SpriteGhost()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    SetSpriteAnim(THIS, ghost_anim, 10);
    data->accel_y = 0;
    data->accel_x = 0;
    data->state = 0;
    if (THIS->x > scroll_target->x)
    {
        data->direction = FALSE;
        SPRITE_SET_VMIRROR(THIS);
    }
    else
    {
        data->direction = TRUE;
    }
    data->canDo = FALSE;
    data->counter = 1;
    THIS->coll_x = 5;
    THIS->coll_y = 5;
    THIS->coll_w = 7;
    THIS->coll_h = 7;
}
void Update_SpriteGhost()
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

        if (--data->counter == 0)
        {
            data->counter = 1;

            if (data->direction == FALSE)
            {
                THIS->x--;
            }
            else
            {
                THIS->x++;
            }

            if (data->canDo == FALSE)
            {
                if (data->accel_y < 44)
                {
                    data->accel_y = data->accel_y + 2;
                }
                else
                {
                    data->canDo = TRUE;
                }
            }
            else
            {
                if (data->accel_y > -30)
                {
                    data->accel_y = data->accel_y - 2;
                }
                else
                {
                    data->canDo = FALSE;
                }
            }
            THIS->y = THIS->y + (data->accel_y >> 4);
        }

        SPRITEMANAGER_ITERATE(i, spr)
        {

            if (spr->type == SpriteChain)
            {
                if (CheckCollision(THIS, spr))
                {
                    DropItem(THIS);
                    GhostDeathSound();
                    Shoot(THIS, SpriteExplosion);
                    SpriteManagerRemove(THIS_IDX);
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
void Destroy_SpriteGhost()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    canSpawn = TRUE;
}