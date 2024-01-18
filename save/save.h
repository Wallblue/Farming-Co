//
// BUSQUET Nicolas & PHE Lina
// 10/01/2024
// Sauvegarde de la partie
//

#ifndef FARMINGCO_SAVE_H
#define FARMINGCO_SAVE_H

#include <stdio.h>
#include <SDL.h>
#include <sqlite3.h>
#include "../map/map.h"
#include "../define.h"
#include "../database/database.h"

unsigned char loadObjectsMaps();
unsigned char saveObject(const Object* object);

#endif //FARMINGCO_SAVE_H
