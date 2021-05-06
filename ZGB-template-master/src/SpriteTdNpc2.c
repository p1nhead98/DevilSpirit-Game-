#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 td_npc2_1[] = {7, 0, 0, 5, 6, 7, 6, 5};
const UINT8 td_npc2_2[] = {6, 1, 1, 1, 1, 1, 1};
const UINT8 td_npc2_3[] = {2, 2, 3};
const UINT8 td_npc2_4[] = {1, 4};
extern BOOLEAN start;
extern BOOLEAN tdNpc2;
struct Sprite *td_npc_parent = 0;
// void Skel1Colision()
// {
//     UINT8 colision = GetScrollTile(((THIS->x + THIS->coll_x) + 4) >> 3, (THIS->y + 15u) >> 3);
//     if (colision == 28)
//     {
//         SpriteManagerRemove(THIS_IDX);
//         Shoot(THIS, SpriteWaterSplash);
//         skNpc1 = TRUE;
//     }
// }

void TdNpcSfx1()
{
    PlayFx(CHANNEL_1, 7, 0x3d, 0xc6, 0xa2, 0x94, 0x87);
}

void TdNpcSfx2()
{
    PlayFx(CHANNEL_1, 10, 0x37, 0xc9, 0xf2, 0x6d, 0x86);
}

void Start_SpriteTdNpc2()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    if (!tdNpc2)
    {
        SpriteManagerRemove(THIS_IDX);
    }
    SetSpriteAnim(THIS, td_npc2_1, 9);
    data->state = 0;
    data->accel_y = 0;
    THIS->coll_x = 4;
    THIS->coll_y = 3;
    THIS->coll_w = 9;
    THIS->coll_h = 13;
}

void Update_SpriteTdNpc2()
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
            if (U_LESS_THAN(DISTANCE(THIS->x, scroll_target->x), 40))
            {
                TdNpcSfx1();
                data->state = 1;
            }
            break;
        case 1:
            SetSpriteAnim(THIS, td_npc2_2, 40);
            if (THIS->anim_frame % 2 == 0)
            {
                THIS->x++;
            }
            else
            {
                THIS->x--;
            }
            if (THIS->anim_frame == 5)
            {
                data->state = 2;
            }
            break;
        case 2:
            SetSpriteAnim(THIS, td_npc2_3, 15);
            TranslateSprite(THIS, 1 << delta_time, 0);
            Gravity(data, td_npc_parent, 60, 4);
            if (!scroll_collisions[GetScrollTile(((THIS->x + THIS->coll_x + THIS->coll_w) >> 3), (THIS->y >> 3) + 2u)] && !scroll_collisions_down[GetScrollTile(((THIS->x + THIS->coll_x + THIS->coll_w) >> 3), (THIS->y >> 3) + 2u)])
            {
                data->state = 3;
                SetSpriteAnim(THIS, td_npc2_4, 0);
                TdNpcSfx2();
                Jump(data, 20);
                tdNpc2 = FALSE;
            }

            break;
        case 3:
            THIS->x++;
            if (data->accel_y < 60)
            {
                data->accel_y = data->accel_y + 2;
            }

            THIS->y = THIS->y + (data->accel_y >> 4);
            break;
        }

        // Skel1Colision();
        // THIS->x--;
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

void Destroy_SpriteTdNpc2()
{
}