#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 skel_npc1_anim[] = {2, 0, 1};
extern BOOLEAN start;
extern BOOLEAN skNpc1;

void Skel1Colision()
{
    UINT8 colision = GetScrollTile(((THIS->x + THIS->coll_x) + 4) >> 3, (THIS->y + 15u) >> 3);
    if (colision == 28)
    {
        SpriteManagerRemove(THIS_IDX);
        Shoot(THIS, SpriteWaterSplash);
        skNpc1 = TRUE;
    }
}

void Start_SpriteSkelNpc1()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    THIS->y = THIS->y + 10;
    if (skNpc1)
    {
        SpriteManagerRemove(THIS_IDX);
    }
    SetSpriteAnim(THIS, skel_npc1_anim, 7);
}

void Update_SpriteSkelNpc1()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    if (start == FALSE)
    {
        if (THIS->anim_speed == 0)
        {
            THIS->anim_speed = data->last_anim_speed;
        }
        Skel1Colision();
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

void Destroy_SpriteSkelNpc1()
{
}