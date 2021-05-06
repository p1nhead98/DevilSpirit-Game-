#include "Banks/SetBank2.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Palette.h"
#include "Misc.h"

const UINT8 water_splash[] = {3, 0, 1, 2};
extern BOOLEAN start;

void Start_SpriteWaterSplash()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    SetSpriteAnim(THIS, water_splash, 14);

    THIS->y = THIS->y - 1;
}
void Update_SpriteWaterSplash()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    if (start == FALSE)
    {
        if (THIS->anim_speed == 0)
        {
            THIS->anim_speed = data->last_anim_speed;
        }
        SPRITE_SET_DMG_PALETTE(THIS, 0);
        if (THIS->anim_frame == 2)
        {
            SpriteManagerRemove(THIS_IDX);
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
void Destroy_SpriteWaterSplash()
{
}