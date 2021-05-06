#pragma bank 0
#include <gb/gb.h>
#include "MapInfo.h"
#include <BankManager.h>
#include <string.h>
#include "ZGBMain.h"
#include "levels.h"

#include "..\res\src\town1_1.h"
#include "..\res\src\town1_2.h"
#include "..\res\src\town1_3.h"
#include "..\res\src\town1_4.h"
#include "..\res\src\town1_5.h"
#include "..\res\src\town1_6.h"

const struct MapInfo* levels[] = {
  &town1_1,
  &town1_2,
  &town1_3,
  &town1_4,
  &town1_5,
  &town1_6,
};


void MapClone(struct MapInfo* sourceMap, struct MapInfo* destinationMap)
{
    PUSH_BANK(sourceMap->bank);
    {
        struct MapInfoInternal* destinationMap_data = destinationMap->data;
        struct MapInfoInternal const* sourceMap_Data = sourceMap->data;

        unsigned char map_width = sourceMap_Data->width;
        unsigned char map_height = sourceMap_Data->height;

        memcpy(destinationMap_data->data, sourceMap_Data->data, map_width * map_height);

        destinationMap_data->width = map_width;
        destinationMap_data->height = map_height;

        memcpy(destinationMap_data->attributes, sourceMap_Data->attributes, map_width * map_height);
        destinationMap_data->tiles = sourceMap_Data->tiles;
    }
    POP_BANK;
}

UINT8 current_level = 0;
UINT8 last_level = 0;
const UINT8 num_levels = 1;
UINT8 last_state = 0;
