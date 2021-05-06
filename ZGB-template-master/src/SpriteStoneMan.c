#include "Banks/SetBank3.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Math.h"
#include "Scroll.h"
#include "Sound.h"
#include "Misc.h"

const UINT8 stoneman_1[] = {4, 0, 2, 0, 4};
const UINT8 stoneman_2[] = {8, 4,4,4,4,4,4,4,4};
extern BOOLEAN start;

void StoneManExplosion()
{
    PlayFx(CHANNEL_4, 8, 0x3f, 0xf2, 0x6f, 0x80);
}

void Start_SpriteStoneMan()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    THIS->coll_x = 3;
    THIS->coll_y = -16;
    THIS->coll_w = 11;
    THIS->coll_h = 32;
    THIS->size = FRAME_32x16;
    data->life = 8;
    if (THIS->x > scroll_target->x)
    {
        SPRITE_SET_VMIRROR(THIS);
    }
    else
    {
        SPRITE_UNSET_VMIRROR(THIS);
    }
    data->state = 0;
    data->counter = 0;
    data->direction = FALSE;
    data->canDo = FALSE;
    SetSpriteAnim(THIS, stoneman_1, 7);
}
void Update_SpriteStoneMan()
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
            SPRITE_SET_DMG_PALETTE(THIS, 0);

            if (THIS->anim_frame == 1 || THIS->anim_frame == 3)
            {
                if (data->canDo == TRUE)
                {
                    ScreenShake(10);
                    PlayFx(CHANNEL_1, 7, 0x1b, 0x94, 0xc1, 0xab, 0x83);
                    data->canDo = FALSE;
                    EnemyMovement(THIS, 5);
                }
            }
            else
            {
                data->canDo = TRUE;
            }
            break;

        case 1:
            if (data->counter <= 18)
            {
                data->counter++;

                SPRITE_SET_DMG_PALETTE(THIS, 1);
                if (data->counter == 18)
                {
                    data->state = 0;
                    data->counter = 0;
                    data->direction = FALSE;
                }
            }
            break;

        case 2:
            SPRITE_SET_DMG_PALETTE(THIS, 1);
            SetSpriteAnim(THIS, stoneman_2, 20);
            THIS->coll_x = 33;
            THIS->coll_y = 0;
            THIS->coll_w = 0;
            THIS->coll_h = 0;
            // if (data->counter <= 20)
            // {
            // if (data->direction == TRUE)
            // {
            //     data->direction = FALSE;
            //     PlayFx(CHANNEL_4, 2, 0x3f, 0xf1, 0x5f, 0xc0);
            // }
            // data->counter++;

            if (THIS->anim_frame % 2 == 0)
            {

                if (data->canDo == TRUE)
                {
                    StoneManExplosion();
                    ScreenShake(1);
                    data->canDo = FALSE;
                    SpriteManagerAdd(SpriteExplosion, THIS->x + 8, THIS->y);
                    SpriteManagerAdd(SpriteExplosion, THIS->x - 8, THIS->y - 16);
                }
            }
            else
            {
                if (data->canDo == FALSE)
                {

                    SpriteManagerAdd(SpriteExplosion, THIS->x - 8, THIS->y);
                    SpriteManagerAdd(SpriteExplosion, THIS->x + 8, THIS->y - 16);
                    data->canDo = TRUE;
                }
            }
            if (THIS->anim_frame == 7)
            {
                DropItem(THIS);
                PlayFx(CHANNEL_4, 8, 0x3b, 0xd1, 0x79, 0x80);
                SpriteManagerRemove(THIS_IDX);
                SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y);
                SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y - 16);
            }

            // }
            break;
        }

        SPRITEMANAGER_ITERATE(i, spr)
        {
            if (spr->type == SpriteChain)
            {
                if (CheckCollision(THIS, spr))
                {

                    if (data->life > 0 && data->direction == FALSE)
                    {
                        data->direction = TRUE;
                        if (spr->type == SpriteChain)
                        {
                            data->life -= 1;
                        }
                        PlayFx(CHANNEL_4, 8, 0x3f, 0xf1, 0x6b, 0x80);
                        data->state = 1;
                    }
                    else if (data->life <= 1)
                    {

                        data->canDo = TRUE;
                        data->direction = TRUE;
                        data->counter = 0;
                        data->state = 2;
                    }
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
void Destroy_SpriteStoneMan()
{
}