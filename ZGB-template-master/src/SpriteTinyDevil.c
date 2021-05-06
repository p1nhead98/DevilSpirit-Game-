#include "Banks/SetBank2.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 tdevil_1[] = {2, 0, 1};
const UINT8 tdevil_2[] = {1, 2};
const UINT8 tdevil_3[] = {1, 3};
const UINT8 tdevil_4[] = {6, 4, 5, 4, 5, 4, 5};
extern BOOLEAN start;
struct Sprite *tdevil_parent = 0;

void TinyDevilDeathSound()
{
    PlayFx(CHANNEL_4, 10, 0x3f, 0xf1, 0x6b, 0x80);
}
void TinyDevilJumpSfx()
{
    PlayFx(CHANNEL_1, 10, 0x37, 0xc9, 0xf2, 0x6d, 0x86);
}

void Start_SpriteTinyDevil()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    SetSpriteAnim(THIS, tdevil_1, 9);
    data->state = 0;
    if (THIS->x > scroll_target->x)
    {
        SPRITE_SET_VMIRROR(THIS);
    }
    else
    {
        SPRITE_UNSET_VMIRROR(THIS);
    }
    data->accel_y = 0;
    THIS->coll_x = 4;
    THIS->coll_y = 3;
    THIS->coll_w = 9;
    THIS->coll_h = 13;
    data->counter = 1;
}
void Update_SpriteTinyDevil()
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

        switch (data->state)
        {
        case 0:
            SetSpriteAnim(THIS, tdevil_1, 9);

            if (--data->counter == 0)
            {
                data->counter = 2;
                EnemyMovement(THIS, 1);
            }
            if (U_LESS_THAN(DISTANCE(THIS->x, scroll_target->x), 27))
            {

                Jump(data, 70);
                TinyDevilJumpSfx();
                if (THIS->x < scroll_target->x)
                {
                    data->direction = TRUE;
                }
                else
                {
                    data->direction = FALSE;
                }
                data->state = 1;
            }
            break;

        case 1:
            SetSpriteAnim(THIS, tdevil_2, 0);
            if (DISTANCE(THIS->x, scroll_target->x) < 2)
            {
                data->state = 2;
            }
            if (data->direction)
            {
                SPRITE_UNSET_VMIRROR(THIS);
                TranslateSprite(THIS, 1, 0);
            }
            else
            {
                SPRITE_SET_VMIRROR(THIS);
                TranslateSprite(THIS, -1, 0);
            }

            break;

        case 2:
            SetSpriteAnim(THIS, tdevil_3, 0);
            if (TranslateSprite(THIS, 0, 1))
            {
                data->state = 3;
            }
            break;

        case 3:
            SetSpriteAnim(THIS, tdevil_4, 7);
            if (THIS->anim_frame == 5)
            {
                data->state = 0;
            }
            break;
        }

        Gravity(data, tdevil_parent, 60, 4);

        SPRITEMANAGER_ITERATE(i, spr)
        {
            if (spr->type == SpriteChain)
            {
                if (CheckCollision(THIS, spr))
                {
                    DropItem(THIS);
                    TinyDevilDeathSound();
                    SpriteManagerRemove(THIS_IDX);
                    Shoot(THIS, SpriteExplosion);
                    // PlayFx(CHANNEL_4, 20, 0x3b, 0xd1, 0x79, 0x80);
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
void Destroy_SpriteTinyDevil()
{
}