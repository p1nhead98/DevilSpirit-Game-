#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"

struct Sprite *stone_parent = 0;

extern BOOLEAN start;

void Start_SpriteStone()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
    data->state = 0;
    data->accel_y = 0;
    THIS->coll_x = 6;
    THIS->coll_y = 6;
    THIS->coll_w = 6;
    THIS->coll_h = 6;
}
void Update_SpriteStone()
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
            if (THIS->unique_id == 1)
            {
                Jump(data, 30);
            }
            else
            {
                Jump(data, 40);
            }
            if (SPRITE_GET_VMIRROR(THIS))
            {
                data->state = 1;
            }
            else
            {
                data->state = 2;
            }
            break;
        case 1:
            THIS->x--;
            break;
        case 2:
            THIS->x++;
            break;
        }

        if (data->accel_y < 70)
        {
            data->accel_y = data->accel_y + 2;
        }

        THIS->y = THIS->y + (data->accel_y >> 4);
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
void Destroy_SpriteStone()
{
    struct commonData *data = (struct commonData *)THIS->custom_data;
}