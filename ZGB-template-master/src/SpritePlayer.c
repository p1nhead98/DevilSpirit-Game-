#include "Banks/SetBank2.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Math.h"
#include "Sound.h"
#include "Misc.h"
#include "TilesChange.h"
#include "..\res\src\pInWater.h"
#include "..\res\src\form1.h"
#include "..\res\src\player.h"
#include "Print.h"
#include <stdio.h>
#include <string.h>

///////////////// HUMAN ANIMATIONS /////////////////////////////////////
const UINT8 human_idle[] = {5, 0, 0, 0, 0, 1};
const UINT8 human_walk[] = {4, 4, 5, 6, 5};
const UINT8 human_jumping[] = {1, 2};
const UINT8 human_falling[] = {1, 3};
const UINT8 human_slide[] = {2, 7, 7};
const UINT8 human_atacking[] = {6, 8, 9, 9, 9, 9, 9};
const UINT8 human_ladders[] = {2, 10, 10};
const UINT8 human_hit[] = {2, 11, 11};
const UINT8 human_death[] = {4, 11, 12, 13, 13};
////////////////////////////////////////////////////////////////////////

///////////////// FORM 1 ANIMATIONS ////////////////////////////////////
const UINT8 form1_idle[] = {1, 14};
const UINT8 form1_walk[] = {4, 2, 4, 2, 0};
const UINT8 form1_jumping[] = {1, 6};
const UINT8 form1_attack[] = {8, 8, 10, 8, 10, 8, 10, 8, 10};
const UINT8 form1_attack2[] = {8, 8, 8, 8, 8, 8, 8, 8, 8};
const UINT8 form1_ladders[] = {2, 12, 12};
const UINT8 form1_hit[] = {2, 6, 6};
///////////////////////////////////////////////////////////////////////

//////////////// FORM 1 VARS //////////////////////////////////////////
BOOLEAN f1CanAttack;
//////////////////////////////////////////////////////////////////////

struct Sprite *player_parent = 0;

extern BOOLEAN start;
BOOLEAN dir = FALSE;
BOOLEAN inWater = FALSE;
BOOLEAN canEnter = FALSE;
INT8 player_state = 0;
BOOLEAN inmu = FALSE;
INT16 inmunity = 0;
extern INT8 current_life;
extern UINT8 max_life;
extern UINT8 current_level;
UINT8 player_counter = 0;
extern void RefreshLife();
extern INT8 transformation;
extern BOOLEAN transf1;
BOOLEAN canTransform = FALSE;

void transformAnimation(const UINT8 anim1[], const UINT8 anim2[], INT8 velocity)
{
  if (transformation == 0)
  {
    SetSpriteAnim(THIS, anim1, velocity);
  }
  else if (transformation == 1)
  {
    SetSpriteAnim(THIS, anim2, velocity);
  }
}

void ladderClimb(struct commonData *data)
{
  UINT8 i = 126;
  transformAnimation(human_ladders, form1_ladders, 12);
  if (KEY_PRESSED(J_UP))
  {
    if (THIS->anim_frame == 1)
    {
      SPRITE_SET_VMIRROR(THIS);
    }
    else
    {
      SPRITE_UNSET_VMIRROR(THIS);
    }

    data->collision = TranslateSprite(THIS, 0, -1 << delta_time);
    //CheckCollisionTile();
    i = GetScrollTile((THIS->x + THIS->coll_x) >> 3, (THIS->y + 15u) >> 3);
  }
  else if (KEY_PRESSED(J_DOWN))
  {
    if (THIS->anim_frame == 1)
    {
      SPRITE_SET_VMIRROR(THIS);
    }
    else
    {
      SPRITE_UNSET_VMIRROR(THIS);
    }

    data->collision = TranslateSprite(THIS, 0, 1 << delta_time);
    //CheckCollisionTile();
    i = GetScrollTile((THIS->x + THIS->coll_x) >> 3, (THIS->y + 16u) >> 3);
  }
  else
  {
    transformAnimation(human_ladders, form1_ladders, 1);
  }

  //Check the end of the ladder
  if (i != 126u && i != 1u && i != 2u)
  {
    //TranslateSprite(THIS, 0, 1 << delta_time);
    data->state = 0;
  }

  //Check jumping
  if (KEY_TICKED(J_A) && !KEY_PRESSED(J_UP))
  {
    //PlayFx(CHANNEL_1, 10, 0x66, 0x80, 0xf1, 0x74,86);
    Jump(data, 60);
    data->state = 1;
  }
}

void CheckCollisionTile(struct commonData *data)
{
  UINT8 colision = GetScrollTile(((THIS->x + THIS->coll_x) + 4) >> 3, (THIS->y + 15u) >> 3);

  if (colision == 114)
  {
    if (!canEnter)
    {
      canEnter = TRUE;
      Shoot(THIS, SpriteUp);
    }
    if (KEY_TICKED(J_UP))
    {
      current_level++;
      SetState(current_state);
    }
  }
  else if (colision == 113)
  {
    THIS->y++;
    current_life = 0;
    RefreshLife();
  }
  else
  {
    canEnter = FALSE;
  }

  if (current_state == StateGame && current_level == 0)
  {

    if ((colision == 45 || colision == 28) && data->state != 6 && data->state != 7)
    {
      current_life = 0;
      dir = TRUE;

      RefreshLife();
      SetSpriteAnim(THIS, human_hit, 6);
      Jump(data, 35);
      data->state = 6;
    }
    else if ((colision == 36 || colision == 38 || colision == 89))
    {

      if (!inWater)
      {
        if (data->accel_y > 1)
        {
          if (TranslateSprite(THIS, 0, 2))
          {
            inWater = TRUE;
            SpriteSetTiles(&pInWater, 0, 56);
            Shoot(THIS, SpriteWaterSplash);
            PlayFx(CHANNEL_4, 20, 0x3f, 0xe3, 0x11, 0x80);
          }
        }
      }
    }
    else
    {
      if (inWater)
      {
        player_counter = 1;
        inWater = FALSE;
        SpriteSetTiles(&player, 0, 56);
        Shoot(THIS, SpriteWaterSplash);
        PlayFx(CHANNEL_4, 27, 0x3f, 0xe3, 0x11, 0x80);
      }
    }
  }
}

void playerPostHit(struct commonData *data)
{
  if (inmunity == 0)
  {
    if (current_life <= 0)
    {
      // if (lives >= 1)
      // {
      data->state = 7;
      //}
      // else
      // {
      //   gameOver = TRUE;
      // }
    }
    else
    {
      data->state = 1;
      SPRITE_SET_PALETTE(THIS, 1);
      inmunity = 100;
      inmu = TRUE;
    }
  }
}

void playerHit(BOOLEAN dir, struct commonData *data)
{

  if (dir == FALSE)
  {
    if (THIS->x > 0)
    {
      TranslateSprite(THIS, -1, 0);
    }
    SPRITE_UNSET_VMIRROR(THIS);
  }
  else
  {
    if (THIS->x < (scroll_w - 15))
    {
      TranslateSprite(THIS, 1, 0);
    }
    SPRITE_SET_VMIRROR(THIS);
  }

  if (THIS->anim_frame == 1)
  {

    playerPostHit(data);
  }
}

void Start_SpritePlayer()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  THIS->coll_x = 3;
  THIS->coll_y = 2;
  THIS->coll_w = 10;
  THIS->coll_h = 13;
  RefreshLife();
  playerCollisions(data);
  data->accel_y = 0;
  data->state = 0;
  start = FALSE;
  inmu = FALSE;
  inmunity = 0;
  inWater = FALSE;
  f1CanAttack = TRUE;
  player_counter = 1;
}

void Update_SpritePlayer()
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

    if (KEY_TICKED(J_SELECT))
    {
      if (transf1 && canTransform)
      {
        transformation = 1;
        THIS->size = FRAME_32x16;
        canTransform = FALSE;
        SpriteSetTiles(&form1, 0, 67);
      }
      else if (transf1 && !canTransform)
      {
        transformation = 0;
        THIS->size = FRAME_16x16;
        canTransform = TRUE;
        SpriteSetTiles(&player, 0, 67);
      }
    }

    CheckCollisionTile(data);

    switch (data->state)
    {

    case 0: //idle
      PlayerMovement(data);
      playerLaddersCol(data);
      if (KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT))
      {
        transformAnimation(human_walk, form1_walk, 9);
      }
      else if (!KEY_PRESSED(J_RIGHT) && !KEY_PRESSED(J_LEFT))
      {
        transformAnimation(human_idle, form1_idle, 8);
      }

      if (KEY_TICKED(J_A) && !KEY_PRESSED(J_DOWN) && !KEY_TICKED(J_B))
      {

        Jump(data, 60);
        data->state = 1;
      };
      if (KEY_PRESSED(J_DOWN) && KEY_TICKED(J_A) && transformation == 0)
      { //slide
        PlayFx(CHANNEL_4, 1, 0x28, 0x81, 0x21, 0x80);
        data->state = 3;
      }

      if (KEY_TICKED(J_B) && !KEY_TICKED(J_A))
      { // main weapon attack idle
        if (transformation == 0)
        {
          PlayFx(CHANNEL_4, 6, 0x3a, 0xf1, 0x11, 0xc0);
          Shoot(THIS, SpriteChain);
          SetSpriteAnim(THIS, human_atacking, 30);
          data->state = 4;
        }
        else if (transformation == 1 && f1CanAttack)
        {
          data->state = 8;
        }
      }

      if (KEY_TICKED(J_B) && KEY_TICKED(J_A) && transformation == 0)
      { // main weapon attack jumping
        PlayFx(CHANNEL_4, 6, 0x3a, 0xf1, 0x11, 0xc0);
        Jump(data, 60);
        Shoot(THIS, SpriteChain);
        SetSpriteAnim(THIS, human_atacking, 30);
        data->state = 5;
      }

      break;

    case 1: //jumping
      if (data->accel_y < 1)
      {
        transformAnimation(human_jumping, form1_jumping, 1);
      }
      else
      {
        transformAnimation(human_falling, form1_jumping, 1);
      }
      if (data->accel_y < 0 && !KEY_PRESSED(J_A))
      { //Si no se mantiene "A" se deja de saltar
        data->accel_y = 0;
      }
      if (KEY_TICKED(J_B) && !KEY_TICKED(J_A))
      { // main weapon attack idle
        if (transformation == 0)
        {
          PlayFx(CHANNEL_4, 6, 0x3a, 0xf1, 0x11, 0xc0);
          Shoot(THIS, SpriteChain);
          SetSpriteAnim(THIS, human_atacking, 30);
          data->state = 5;
        }
        else if (transformation == 1 && f1CanAttack)
        {
          data->state = 8;
          f1CanAttack = FALSE;
        }
      }
      PlayerMovement(data);
      playerLaddersCol(data);
      break;

    case 2: //ladders
      ladderClimb(data);
      break;

    case 3: //slide

      SetSpriteAnim(THIS, human_slide, 7);
      Slide(THIS);
      playerCollisions(data);
      if (THIS->anim_frame == 1)
      {
        data->state = 0;
        playerCollisions(data);
      }
      break;

    case 4: //attack idle

      if (THIS->anim_frame == 5)
      {
        data->state = 0;
      }
      break;
    case 5: //attack jumping

      PlayerMovement(data);
      if (THIS->anim_frame == 5)
      {
        data->state = 1;
      }
      break;

    case 6: //HIT
      playerHit(dir, data);
      break;

    case 7: //Death
      SetSpriteAnim(THIS, human_death, 6);
      if (THIS->anim_frame == 3)
      {
        current_life = max_life;
        SetState(current_state);
      }
      break;
    case 8: // form 1 attack

      if (f1CanAttack)
      {
        SetSpriteAnim(THIS, form1_attack, 25);
      }
      else if(f1CanAttack == FALSE)
      {
        data->accel_y = 0;
        SetSpriteAnim(THIS, form1_attack2, 25);
      }

      if (THIS->x > 2 && THIS->x <= (scroll_w - 17))
      {
        if (SPRITE_GET_VMIRROR(THIS))
        {
          if (THIS->anim_frame % 2 == 0)
          {
            DrawFrame(FRAME_8x16, 56, THIS->x - scroll_x - 6, THIS->y - scroll_y - 2, THIS->flags);
            DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x + 12, THIS->y - scroll_y - 16, THIS->flags);
            DrawFrame(FRAME_8x16, THIS->frame + 4, THIS->x - scroll_x + 12, THIS->y - scroll_y, THIS->flags);
          }
          else
          {
            DrawFrame(FRAME_8x16, 56, THIS->x - scroll_x - 6, THIS->y - scroll_y - 3, THIS->flags);
          }
          TranslateSprite(THIS, -2, 0);
        }
        else
        {

          if (THIS->anim_frame % 2 == 0)
          {
            DrawFrame(FRAME_8x16, 56, THIS->x - scroll_x + 14, THIS->y - scroll_y - 2, THIS->flags);
            DrawFrame(FRAME_8x16, THIS->frame, THIS->x - scroll_x - 4, THIS->y - scroll_y - 16, THIS->flags);
            DrawFrame(FRAME_8x16, THIS->frame + 4, THIS->x - scroll_x - 4, THIS->y - scroll_y, THIS->flags);
          }
          else
          {
            DrawFrame(FRAME_8x16, 56, THIS->x - scroll_x + 14, THIS->y - scroll_y - 3, THIS->flags);
          }
          TranslateSprite(THIS, 2, 0);
        }
      }
      if (THIS->anim_frame == 7)
      {
        data->state = 0;
      }

      break;
    case 9:
       if (KEY_TICKED(J_B) && !KEY_TICKED(J_A))
      { 
          data->state = 8;
          f1CanAttack = FALSE;
        
      }

      SetSpriteAnim(THIS, form1_hit, 8);
      if (dir)
      {
        TranslateSprite(THIS, 1, 0);
      }
      else
      {
        TranslateSprite(THIS, -1, 0);
      }
      if (THIS->anim_frame == 1)
      {
        f1CanAttack = FALSE;
        data->state = 1;
      }
      break;
    }

    if (data->state != 2)
    {

      if (player_parent == 0)
      {

        if (data->accel_y < 60)
        {
          data->accel_y += 4;
        }

        data->collision = TranslateSprite(THIS, 0, (data->accel_y >> (4 << delta_time)));
        if (!data->collision && delta_time != 0 && data->accel_y < 40)
        { //Do another iteration if there is no collision
          data->accel_y += 4 >> delta_time;

          data->collision = TranslateSprite(THIS, 0, (data->accel_y >> 4));
        }
        if (data->collision && !TranslateSprite(THIS, 0, (data->accel_y >> (-4 << delta_time))))
        {

          data->accel_y = 0;

          if (data->state == 1)
          {
              f1CanAttack = TRUE;
            PlayFx(CHANNEL_4, 1, 0x28, 0x81, 0x21, 0x80);
            data->state = 0;
          }
          if (data->state == 5)
          {

            PlayFx(CHANNEL_4, 1, 0x28, 0x81, 0x21, 0x80);
            data->state = 4;
          }
        }
      }
      if (data->state == 0 && data->accel_y >= 20)
      {
        data->state = 1;
      }
    }

    player_state = data->state;

    if (inmunity != 0)
    {
      inmunity -= (1 << delta_time);
      if ((inmunity % 2 == 0))
      {
        SPRITE_SET_DMG_PALETTE(THIS, 1);
      }
      else
      {
        SPRITE_SET_DMG_PALETTE(THIS, 0);
      }
      if (inmunity < 1)
      {
        inmunity = 0;
        inmu = FALSE;
        SPRITE_SET_DMG_PALETTE(THIS, 0);
      }
    }

    if (inWater && data->state != 1 && data->state != 5)
    {
      if (--player_counter == 0 && start == FALSE)
      {
        player_counter = 2;
        TranslateSprite(THIS, -1, 0);
      }
    }

    SPRITEMANAGER_ITERATE(i, spr)
    {

      if (spr->type != SpriteChain && spr->type != SpriteExplosion && spr->type != SpritePlayer && spr->type != SpriteNpc1 && spr->type != SpriteWaterSplash && spr->type != SpriteUp && spr->type != SpriteLife && spr->type != SpriteShine &&
          spr->type != SpriteSoulOrb)
      {
        if (CheckCollision(THIS, spr))
        {
          if (data->state != 6 && data->state != 7 && data->state != 8 && data->state != 9 && inmu == FALSE)
          {
            if (THIS->x < spr->x)
            {
              dir = FALSE;
            }
            else
            {
              dir = TRUE;
            }
            current_life--;
            SetSpriteAnim(THIS, human_hit, 6);
            Jump(data, 35);
            RefreshLife();
            PlayFx(CHANNEL_4, 4, 0x3f, 0xf1, 0x30, 0x80);
            data->state = 6;
          }
          else if (data->state == 8 && data->state != 9)
          {
            if (SPRITE_GET_VMIRROR(THIS))
            {
              if (THIS->x < spr->x)
              {
                dir = FALSE;
                current_life--;
                SetSpriteAnim(THIS, human_hit, 6);
                Jump(data, 35);
                RefreshLife();
                PlayFx(CHANNEL_4, 4, 0x3f, 0xf1, 0x30, 0x80);
                data->state = 6;
              }
              else
              {
                dir = TRUE;
                Jump(data, 35);
                data->state = 9;
              }
            }
            else
            {
              if (THIS->x < spr->x)
              {
                dir = FALSE;
                Jump(data, 35);
                data->state = 9;
              }
              else
              {
                dir = TRUE;
                current_life--;
                SetSpriteAnim(THIS, human_hit, 6);
                Jump(data, 35);
                RefreshLife();
                PlayFx(CHANNEL_4, 4, 0x3f, 0xf1, 0x30, 0x80);
                data->state = 6;
              }
            }
          }
        }
      }
      // else if (spr->type == SpriteMovePlataform)
      // {
      //   if (CheckCollision(THIS, spr) && data->accel_y > 15)
      //   {
      //     if (THIS->y + 4 < spr->y)
      //     {
      //       THIS->y = spr->y - 15;
      //       THIS->x = spr->x;
      //       data->accel_y = 15;
      //       if (data->state == 1)
      //       {
      //         data->state = 0;
      //       }
      //     }
      //   }
      // }
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

void Destroy_SpritePlayer()
{
  struct commonData *data = (struct commonData *)THIS->custom_data;
  if (current_life == 0)
  {
    current_life = max_life;
    SetState(current_state);
  }
}