#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"
const UINT8 helldog_0[] = {1, 0};
const UINT8 helldog_1[] = {3, 0, 0, 0};
const UINT8 helldog_2[] = {2, 1, 1};
const UINT8 helldog_3[] = {17, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
const UINT8 helldog_4[] = {17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
BOOLEAN HDCanShoot = TRUE;
BOOLEAN HDCanHit = TRUE;
extern UINT8 current_level;
extern BOOLEAN start;

void HelldogHitSfx(){
    PlayFx(CHANNEL_4, 8, 0x3f, 0xf1, 0x6b, 0x80);
}

void HelldogExplotionSfx()
{
    PlayFx(CHANNEL_4, 8, 0x3f, 0xf2, 0x6f, 0x80);
}

void HelldogFireSfx(){
    PlayFx(CHANNEL_4, 8, 0x3f, 0xf3, 0x74, 0x80);
}

void Start_SpriteHellDog()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    data->state = 0;
    data->life = 7;
    data->counter = 5;
    HDCanShoot = TRUE;
    HDCanHit = TRUE;
    if (current_level == 3 && THIS->x > 250)
    {
        SPRITE_SET_VMIRROR(THIS);
    }
}

void Update_SpriteHellDog()
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

        if (data->counter < 20 && HDCanHit == FALSE)
        {
            data->counter++;
            SPRITE_SET_PALETTE(THIS, 1);
            if (data->counter == 20)
            {
                data->counter = 0;
                HDCanHit = TRUE;
                SPRITE_SET_PALETTE(THIS, 0);
            }
        }

        switch (data->state)
        {
        case 0:
            SetSpriteAnim(THIS, helldog_0, 1);
            if (U_LESS_THAN(DISTANCE(THIS->x, scroll_target->x), 65))
            {
                data->state = 1;
            }
            break;
        case 1:

            SetSpriteAnim(THIS, helldog_1, 2);
            if (THIS->anim_frame == 2)
            {
                data->state = 2;
            }
            break;

        case 2:
            SetSpriteAnim(THIS, helldog_2, 4);
            if (THIS->anim_frame == 1)
            {

                data->state = 3;
            }
            break;

        case 3:
            SetSpriteAnim(THIS, helldog_3, 34);
            if (THIS->anim_frame == 1)
            {
                if (HDCanShoot == TRUE)
                {
                    HelldogFireSfx();
                    Shoot(THIS, SpriteHDogFire);
                    HDCanShoot = FALSE;
                }
            }
            else if (THIS->anim_frame == 3)
            {
                if (HDCanShoot == FALSE)
                {
                    HelldogFireSfx();
                    Shoot(THIS, SpriteHDogFire);
                    HDCanShoot = TRUE;
                }
            }
            else if (THIS->anim_frame == 5)
            {
                if (HDCanShoot == TRUE)
                {
                    HelldogFireSfx();
                    Shoot(THIS, SpriteHDogFire);
                    HDCanShoot = FALSE;
                }
            }

            if (THIS->anim_frame % 2 == 0)
            {

                THIS->x = THIS->x + 1;
            }
            else
            {
                THIS->x = THIS->x - 1;
            }

            if (THIS->anim_frame == 16)
            {
                THIS->x = THIS->x - 1;
                data->state = 0;
                HDCanShoot = TRUE;
            }
            break;

        case 4:
            SetSpriteAnim(THIS, helldog_4, 28);
            SPRITE_SET_PALETTE(THIS, 1);
            THIS->coll_x = 0;
            THIS->coll_y = 0;
            THIS->coll_w = 0;
            THIS->coll_h = 0;
            if (THIS->anim_frame % 2 == 0)
            {
                if (HDCanShoot == TRUE)
                {   
                    HelldogExplotionSfx();
                    ScreenShake(1);
                    HDCanShoot = FALSE;
                    SpriteManagerAdd(SpriteExplosion, THIS->x + 26, THIS->y + 15);
                }
            }
            else
            {
                if (HDCanShoot == FALSE)
                {
                    HDCanShoot = TRUE;
                    SpriteManagerAdd(SpriteExplosion, THIS->x, THIS->y + 24);
                }
            }
            if (THIS->anim_frame == 16)
            {
                DropItem(THIS);
                SpriteManagerRemove(THIS_IDX);
            }

            break;
        }

        SPRITEMANAGER_ITERATE(i, spr)
        {

            if (spr->type == SpriteChain)
            {
                if (CheckCollision(THIS, spr))
                {
                    if (data->life != 0)
                    {
                        if (HDCanHit == TRUE)
                        {
                            HelldogHitSfx();
                            HDCanHit = FALSE;
                            data->life--;
                        }
                    }
                    else
                    {
                        HDCanShoot = TRUE;
                        data->state = 4;
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

void Destroy_SpriteHellDog()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
}