#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 fireball_1[] = {2, 0, 1};
extern BOOLEAN start;

void Start_SpriteHDogFire()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    data->state = 0;
    data->accel_y = 50;
    THIS->coll_x = 4;
    THIS->coll_y = 4;
    THIS->coll_w = 8;
    THIS->coll_h = 8;
    THIS->y = THIS->y + 16;
    SPRITE_SET_DMG_PALETTE(THIS, 0);
    SetSpriteAnim(THIS, fireball_1, 15);
}
void Update_SpriteHDogFire()
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
        SPRITE_SET_DMG_PALETTE(THIS, 0);
        if (data->accel_y > -60)
        {
            data->accel_y = data->accel_y - 2;
        }

        THIS->y = THIS->y + (data->accel_y >> 4);

        if (SPRITE_GET_VMIRROR(THIS))
        {
            THIS->x = THIS->x + 2;
            DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x - 8, THIS->y - scroll_y, 64);
        }
        else
        {

            THIS->x = THIS->x - 2;
            DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x + 8, THIS->y - scroll_y, 32);
        }
        SPRITEMANAGER_ITERATE(i, spr)
        {

            if (spr->type == SpriteChain)
            {
                if (CheckCollision(THIS, spr))
                {
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
        if (SPRITE_GET_VMIRROR(THIS))
        {
            DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x - 8, THIS->y - scroll_y, 64);
        }
        else
        {
            DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x + 8, THIS->y - scroll_y, 32);
        }
    }
}
void Destroy_SpriteHDogFire()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
}