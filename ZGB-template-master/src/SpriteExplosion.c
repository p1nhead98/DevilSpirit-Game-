#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "Scroll.h"

const UINT8 explosion1[] = {3, 0,1,2};
void Start_SpriteExplosion(){
    SetSpriteAnim(THIS, explosion1,25);
}
void Update_SpriteExplosion(){
    DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x + 8 , THIS->y - scroll_y ,  32);
    if(THIS->anim_frame == 2){
        SpriteManagerRemove(THIS_IDX);
    }
}
void Destroy_SpriteExplosion(){

}