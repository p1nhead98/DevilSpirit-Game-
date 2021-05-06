#ifndef MISC_H
#define MISC_H

#include <gb/gb.h>
#include "ZGBMain.h"
#include <BankManager.h>
#include "Sprite.h"
#include "TilesInfo.h"
#include <string.h>
struct commonData{
  INT16 accel_y;
  INT16 accel_x;
  INT8 state;
  INT8 life;
  UINT8 collision;
  UINT8 last_anim_speed;
  UINT8 counter;
  BOOLEAN direction;
  BOOLEAN canDo;
 
};

void Gravity(struct commonData *data, struct Sprite *parent, INT16 maxGravity, INT16 gravity );
void Jump(struct commonData * data, UINT8 force);
void Slide(struct Sprite* sprite);
void Shoot(struct Sprite* this, UINT8);
void EnemyMovement(struct Sprite *this, UINT8 velocity);
void ScreenShake(INT8 x);
void ScreenShakeY(INT8 y);
void DeatSFX1();
void DropItem(struct Sprite *this);
void PlayerMovement(struct commonData *data);
void playerLaddersCol(struct commonData *data);
void playerCollisions(struct commonData *data);

#endif