#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "Scroll.h"
#include "Misc.h"


void Start_SpriteMovePlataform(){
    struct commonData *data = (struct commonData *)THIS->custom_data;
    THIS->y = THIS->y + 12;
    data->direction = TRUE;
    THIS->lim_x = scroll_w;
}
void Update_SpriteMovePlataform(){
    struct commonData *data = (struct commonData *)THIS->custom_data;


    UINT8 colision = GetScrollTile(((THIS->x + THIS->coll_x) + 4) >> 3, (THIS->y + 8u) >> 3);
    if ((colision == 91) )
    {
      if(data->direction == FALSE){
          THIS->x--;
      }else{
          THIS->x++;
      }
    }
    else 
    {
        if(data->direction == FALSE){
          data->direction = TRUE;
          THIS->x = THIS->x + 1; 
      }else{
          data->direction = FALSE;
         THIS->x = THIS->x - 1; 
      }
     
    }
    
  
}
void Destroy_SpriteMovePlataform(){

}