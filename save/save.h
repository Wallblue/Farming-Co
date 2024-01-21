//
// BUSQUET Nicolas & PHE Lina
// 10/01/2024
// Sauvegarde de la partie
//

#ifndef FARMINGCO_SAVE_H
#define FARMINGCO_SAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>
#include <sqlite3.h>
#include "../map/map.h"
#include "../define.h"
#include "../database/database.h"

unsigned char loadObjectsMaps();
unsigned char saveObject(const Object* object);
unsigned char saveNewItem(Inventory* inventory, int slot, sqlite3* db); //Owner type : 1 - Object | 2 - NPC | else Player
unsigned char deleteSavedItem(Inventory* inventory, char slot, sqlite3* db);
unsigned char alterItemQuantity(Inventory* inventory, char slot, unsigned char quantity, char mode);
unsigned char updateItemSlot(Inventory* inventory, char srcSlot, char destSlot, sqlite3* db);
#endif //FARMINGCO_SAVE_H
