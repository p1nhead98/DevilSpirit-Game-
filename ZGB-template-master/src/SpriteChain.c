#include "Banks/SetBank4.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Sound.h"
#include "Scroll.h"
#include "Palette.h"
#include "Misc.h"
const UINT8 sword_anim1[] = {2, 9, 9};
const UINT8 sword_anim2[] = {5, 0, 4, 8, 8, 8};
extern INT8 player_state;
UINT8 sword_state = 0;
extern BOOLEAN start;
void SwordSound()
{
    PlayFx(CHANNEL_4, 6, 0x3d, 0xa1, 0x02, 0x80);
}
void Start_SpriteChain()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    sword_state = 0;
    SetSpriteAnim(THIS, sword_anim1, 30);
    SPRITE_SET_DMG_PALETTE(THIS, 0);
}

void Update_SpriteChain()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    SPRITE_SET_DMG_PALETTE(THIS, 0);

    if (start == FALSE)
    {
        if (THIS->anim_speed == 0)
        {
            THIS->anim_speed = data->last_anim_speed;
        }
        switch (sword_state)
        {
        case 0:
            THIS->coll_x = 0;
            THIS->coll_y = 17;
            THIS->coll_w = 0;
            THIS->coll_h = 0;
            if (SPRITE_GET_VMIRROR(THIS))
            {
                THIS->y = scroll_target->y + 4;
                THIS->x = scroll_target->x + 10;
            }
            else
            {
                THIS->y = scroll_target->y + 4;
                THIS->x = scroll_target->x - 4;
            }
            if (THIS->anim_frame == 1)
            {
                SwordSound();
                sword_state = 1;
            }
            break;
        case 1:

            SetSpriteAnim(THIS, sword_anim2, 30);

            if (SPRITE_GET_VMIRROR(THIS))
            {
                THIS->y = scroll_target->y - 4;
                THIS->x = scroll_target->x - 7;
                THIS->coll_x = -4;
                THIS->coll_y = 0;
                THIS->coll_w = 12;
                THIS->coll_h = 16;
            }
            else
            {
                THIS->coll_x = 0;
                THIS->coll_y = 0;
                THIS->coll_w = 16;
                THIS->coll_h = 16;
                THIS->y = scroll_target->y - 4;
                THIS->x = scroll_target->x + 15;
            }
            if (THIS->anim_frame < 2)
            {
                if (SPRITE_GET_VMIRROR(THIS))
                {
                    DrawFrame(FRAME_8x16, THIS->frame + 2, THIS->x - scroll_x - 8, THIS->y - scroll_y, THIS->flags);
                    DrawFrame(FRAME_8x16, THIS->frame + 4, THIS->x - scroll_x, THIS->y - scroll_y + 16, THIS->flags);
                    DrawFrame(FRAME_8x16, THIS->frame + 6, THIS->x - scroll_x - 8, THIS->y - scroll_y + 16, THIS->flags);
                }
                else
                {
                    DrawFrame(FRAME_8x16, THIS->frame + 2, THIS->x - scroll_x + 8, THIS->y - scroll_y, THIS->flags);
                    DrawFrame(FRAME_8x16, THIS->frame + 4, THIS->x - scroll_x, THIS->y - scroll_y + 16, THIS->flags);
                    DrawFrame(FRAME_8x16, THIS->frame + 6, THIS->x - scroll_x + 8, THIS->y - scroll_y + 16, THIS->flags);
                }
            }
            if (THIS->anim_frame == 4 || player_state != 5 && player_state != 4)
            {
                SpriteManagerRemove(THIS_IDX);
            }
            break;
        }
    }
    else
    {
        if (THIS->anim_speed != 0)
        {
            data->last_anim_speed = THIS->anim_speed;
            THIS->anim_speed = 0;
        }
        if (THIS->anim_frame < 2)
        {
            if (SPRITE_GET_VMIRROR(THIS))
            {
                DrawFrame(FRAME_8x16, THIS->frame + 2, THIS->x - scroll_x - 8, THIS->y - scroll_y, THIS->flags);
                DrawFrame(FRAME_8x16, THIS->frame + 4, THIS->x - scroll_x, THIS->y - scroll_y + 16, THIS->flags);
                DrawFrame(FRAME_8x16, THIS->frame + 6, THIS->x - scroll_x - 8, THIS->y - scroll_y + 16, THIS->flags);
            }
            else
            {
                DrawFrame(FRAME_8x16, THIS->frame + 2, THIS->x - scroll_x + 8, THIS->y - scroll_y, THIS->flags);
                DrawFrame(FRAME_8x16, THIS->frame + 4, THIS->x - scroll_x, THIS->y - scroll_y + 16, THIS->flags);
                DrawFrame(FRAME_8x16, THIS->frame + 6, THIS->x - scroll_x + 8, THIS->y - scroll_y + 16, THIS->flags);
            }
        }
    }
}

void Destroy_SpriteChain()
{
}