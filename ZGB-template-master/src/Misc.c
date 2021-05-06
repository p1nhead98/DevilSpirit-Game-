#include "Banks/SetBank0.h"
#include "Misc.h"
#include <string.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sound.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "SpriteManager.h"
#include <BankManager.h>
#include "Print.h"
#include "rand.h"

BOOLEAN start = FALSE;
BOOLEAN skNpc1 = FALSE;
BOOLEAN tdNpc2 = FALSE;
BOOLEAN canSpawn = FALSE;
UINT8 max_life = 8;
INT8 current_life = 8;
INT8 transformation = 0;
BOOLEAN transf1 = TRUE;

///////////////////////PLAYER MOVEMENTS
void PlayerMovement(struct commonData *data)
{

  if (KEY_PRESSED(J_RIGHT) && THIS->x <= (scroll_w - 17))
  {

    TranslateSprite(THIS, 1, 0);
    if (data->state != 5)
    {
      SPRITE_UNSET_VMIRROR(THIS);
    }
  }
  else if (KEY_PRESSED(J_LEFT) && THIS->x > 2)
  {
    TranslateSprite(THIS, -1, 0);
    if (data->state != 5)
    {
      SPRITE_SET_VMIRROR(THIS);
    }
  }
}

void playerLaddersCol(struct commonData *data)
{
  if (KEY_PRESSED(J_UP))
  {
    UINT8 tile = GetScrollTile((THIS->x + THIS->coll_x) >> 3, (THIS->y + 15u) >> 3);
    if (tile == 126u)
    {
      THIS->x = ((THIS->x + THIS->coll_x) >> 3) << 3;
      data->accel_y = 0;
      data->state = 2;
    }
  }
  if (KEY_PRESSED(J_DOWN))
  {
    UINT8 tile = GetScrollTile((THIS->x + THIS->coll_x) >> 3, (THIS->y + 16u) >> 3);
    if (tile == 126u)
    {
      THIS->x = ((THIS->x + THIS->coll_x) >> 3) << 3;
      THIS->y = THIS->y + 1u;
      data->accel_y = 0;
      data->state = 2;
    }
  }
}

void playerCollisions(struct commonData *data)
{
  if (data->state == 3)
  {
    THIS->coll_x = 3;
    THIS->coll_y = 8;
    THIS->coll_w = 10;
    THIS->coll_h = 8;
  }
  else
  {
    THIS->coll_x = 3;
    THIS->coll_y = 1;
    THIS->coll_w = 10;
    THIS->coll_h = 15;
  }
}

void Gravity(struct commonData *data, struct Sprite *parent, INT16 maxGravity, INT16 gravity)
{
  //data->accel_y = data->accel_y + gravity;

  if (parent == 0)
  {

    if (maxGravity < 0)
    {
      if (data->accel_y > maxGravity)
      {
        data->accel_y += gravity;
      }
    }
    else
    {
      if (data->accel_y < maxGravity)
      {
        data->accel_y += gravity;
      }
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
        data->state = 0;
      }
    }
  }
  if (data->state == 0 && data->accel_y == 20)
  {
    data->state = 1;
  }
}

void Jump(struct commonData *data, UINT8 force)
{
  data->accel_y = -force;
}

void Slide(struct Sprite *sprite)
{
  if (sprite->x > 2 && sprite->x <= (scroll_w - 17))

    if (SPRITE_GET_VMIRROR(sprite))
    {
      TranslateSprite(sprite, -2, 0);
    }
    else
    {
      TranslateSprite(sprite, 2, 0);
    }
}

void Shoot(struct Sprite *this, UINT8 weapon)
{
  struct Sprite *sprite_weapon;
  if (this->size == FRAME_32x32 && SPRITE_GET_VMIRROR(this))
  {
    sprite_weapon = SpriteManagerAdd(weapon, this->x + 32, this->y);
    sprite_weapon->flags = this->flags;
  }
  else
  {
    sprite_weapon = SpriteManagerAdd(weapon, this->x, this->y);
    sprite_weapon->flags = this->flags;
  }
}

void EnemyMovement(struct Sprite *this, UINT8 velocity)
{
  if (SPRITE_GET_VMIRROR(this))
  {
    if (TranslateSprite(this, -velocity << delta_time, 0))
    {
      SPRITE_UNSET_VMIRROR(this);
    }
    else if (!scroll_collisions[GetScrollTile(((this->x + this->coll_x) >> 3), (this->y >> 3) + 2u)] && !scroll_collisions_down[GetScrollTile(((this->x + this->coll_x) >> 3), (this->y >> 3) + 2u)])
    {
      SPRITE_UNSET_VMIRROR(this);
    }
  }
  else
  {
    if (TranslateSprite(this, +velocity << delta_time, 0))
    {
      SPRITE_SET_VMIRROR(this);
    }
    else if (!scroll_collisions[GetScrollTile(((this->x + this->coll_x + this->coll_w) >> 3), (this->y >> 3) + 2u)] && !scroll_collisions_down[GetScrollTile(((this->x + this->coll_x + this->coll_w) >> 3), (this->y >> 3) + 2u)])
    {
      SPRITE_SET_VMIRROR(this);
    }
  }
}

void RefreshLife()
{
  const UINT8 HEART_TILE = 125;
  const UINT8 HEART_TILE2 = 124;
  const UINT8 EMPTY_HEART_TILE = 123;
  UINT8 i = 0;
  UINT8 last_tile = 0;

  if (current_life >= 0)
  {

    last_tile = (current_life + 1) / 2;
    for (i = 0; i != (max_life / 2); ++i)
    {
      set_win_tiles(3 + i, 0, 1, 1, &EMPTY_HEART_TILE);
    }
    if (current_life % 2 == 0)
    {
      for (i = 0; i != (current_life / 2); ++i)
      {
        set_win_tiles(3 + i, 0, 1, 1, &HEART_TILE);
      }
      for (; i != (max_life / 2); ++i)
      {
        set_win_tiles(3 + i, 0, 1, 1, &EMPTY_HEART_TILE);
      }
    }
    else
    {
      for (i = 0; i != ((current_life - 1) / 2); ++i)
      {
        set_win_tiles(3 + i, 0, 1, 1, &HEART_TILE);
      }
      if (i == ((current_life - 1) / 2))
      {
        set_win_tiles(3 + i, 0, 1, 1, &HEART_TILE2);
      }
    }
  }
}

void ScreenShake(INT8 x)
{
  scroll_x -= x;
}

void DeatSFX1()
{
  PlayFx(CHANNEL_4, 8, 0x3b, 0xd1, 0x79, 0x80);
}

void DropItem(struct Sprite *this)
{
  UINT16 random = rand();

  if ((random > 0 && random < 10) || (random > 30 && random < 40) || (random > 60 && random < 70))
  {
    Shoot(THIS, SpriteLife);
  }
}