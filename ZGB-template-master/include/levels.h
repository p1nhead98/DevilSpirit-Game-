#ifndef LEVELS_H
#define LEVELS_H

#include <gb/gb.h>
#include "ZGBMain.h"
#include <BankManager.h>
#include "Sprite.h"
#include "TilesInfo.h"
#include "MapInfo.h"


void MapClone(struct MapInfo* sourceMap, struct MapInfo* destinationMap);

#endif