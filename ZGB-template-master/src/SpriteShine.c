#include "Banks/SetBank3.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Math.h"
#include "Scroll.h"
#include "Misc.h"

extern BOOLEAN start;
const UINT8 shine_1[] = {3, 0, 0, 0};

void Start_SpriteShine()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
}

void Update_SpriteShine()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    THIS->y++;
    SetSpriteAnim(THIS, shine_1, 15);
    switch (THIS->anim_frame)
    {
    case 1:
        SPRITE_SET_DMG_PALETTE(THIS, 1);
        break;
    case 2:
        SpriteManagerRemove(THIS_IDX);
        break;
    }
}

void Destroy_SpriteShine()
{
}
