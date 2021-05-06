#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 eye_1[] = {5, 1, 1, 0, 0, 0};
const UINT8 eye_2[] = {5, 0, 0, 0, 0, 0};
const UINT8 eye_3[] = {3, 0, 0, 1};
const UINT8 eye_4[] = {1, 2};

struct Sprite *eye_parent = 0;

extern BOOLEAN start;
extern UINT8 current_level;

void EyeCollisionTile()
{
    UINT8 colision = GetScrollTile(((THIS->x + THIS->coll_x) + 4) >> 3, (THIS->y + 15u) >> 3);

    if ((colision == 36 || colision == 38 || colision == 89 || colision == 45 || colision == 28) && current_level == 0)
    {
        Shoot(THIS, SpriteExplosion);
        SpriteManagerRemove(THIS_IDX);
    }
}

void EyeDeathSound()
{
    PlayFx(CHANNEL_4, 8, 0x3f, 0xf1, 0x6b, 0x80);
}

void Start_SpriteEye()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    THIS->coll_x = 2;
    THIS->coll_y = 2;
    THIS->coll_w = 12;
    THIS->coll_h = 13;
    data->state = 2;
    data->accel_y = 0;
    if (THIS->x < scroll_target->x)
    {
        data->direction = TRUE;
        SPRITE_UNSET_VMIRROR(THIS);
    }
    else if (THIS->x > scroll_target->x)
    {
        data->direction = FALSE;
        SPRITE_SET_VMIRROR(THIS);
    }
}

void Update_SpriteEye()
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
            SetSpriteAnim(THIS, eye_1, 18);
            if (THIS->anim_frame == 4)
            {
                data->state = 2;
            }
            break;
        case 1:
            SetSpriteAnim(THIS, eye_4, 15);
            if (data->direction)
            {
                TranslateSprite(THIS, 1, 0);
            }
            else
            {
                TranslateSprite(THIS, -1, 0);
            }
            eye_parent = 0;
            break;

        case 2:
            SetSpriteAnim(THIS, eye_2, 15);
            if (THIS->x < scroll_target->x)
            {
                SPRITE_UNSET_VMIRROR(THIS);
            }
            else if (THIS->x > scroll_target->x)
            {
                SPRITE_SET_VMIRROR(THIS);
            }
            if (THIS->anim_frame == 4)
            {
                data->state = 3;
            }
            break;

        case 3:
            SetSpriteAnim(THIS, eye_3, 15);

            if (THIS->anim_frame == 2)
            {
                if (THIS->x < scroll_target->x)
                {
                    data->direction = TRUE;
                    SPRITE_UNSET_VMIRROR(THIS);
                }
                else if (THIS->x > scroll_target->x)
                {
                    data->direction = FALSE;
                    SPRITE_SET_VMIRROR(THIS);
                }
                PlayFx(CHANNEL_1, 10, 0x44, 0x9e, 0xf1, 0x42, 0xc3);
                data->state = 1;
                data->accel_y = -50;
            }
            else
            {
                if (THIS->x < scroll_target->x)
                {
                    SPRITE_UNSET_VMIRROR(THIS);
                }
                else if (THIS->x > scroll_target->x)
                {
                    SPRITE_SET_VMIRROR(THIS);
                }
            }
            break;
        }
        Gravity(data, eye_parent, 60, 4);

        SPRITEMANAGER_ITERATE(i, spr)
        {

            if (spr->type == SpriteChain)
            {
                if (CheckCollision(THIS, spr))
                {
                    DropItem(THIS);
                    EyeDeathSound();
                    Shoot(THIS, SpriteExplosion);
                    SpriteManagerRemove(THIS_IDX);
                }
            }
        }
        EyeCollisionTile();
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

void Destroy_SpriteEye()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
}