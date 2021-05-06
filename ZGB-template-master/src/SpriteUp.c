#include "Banks/SetBank4.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Math.h"
#include "Scroll.h"
#include "Misc.h"

extern BOOLEAN canEnter;

void Start_SpriteUp()
{
}

void Update_SpriteUp()
{
    SPRITE_SET_DMG_PALETTE(THIS, 0);

    THIS->x = scroll_target->x + 5;
    THIS->y = scroll_target->y - 12;

    if (!canEnter)
    {
        SpriteManagerRemove(THIS_IDX);
    }
}

void Destroy_SpriteUp()
{
}
