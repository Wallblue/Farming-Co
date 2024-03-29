//
// BUSQUET Nicolas & PHE Lina
// 27/12/2023
// Création base de données
//
#ifndef FARMINGCO_DATABASE_H

#define FARMINGCO_DATABASE_H

#include <stdio.h>
#include <sqlite3.h>
#include "../define.h"
#include <stdlib.h>
#include <cJSON.h>
#include "../items/inventory/inventory.h"


int createDatabase();
unsigned char openDb(sqlite3** db);
unsigned char startGame();
unsigned char executeSQL(sqlite3* db, const char* request);
unsigned char prepareRequest(sqlite3* db, const char* request, sqlite3_stmt** res);
unsigned char returnProperly(sqlite3 *db, sqlite3_stmt *res, const char *errmsg, unsigned char returnValue);
unsigned char addStartKitToDatabase();

#endif //FARMINGCO_DATABASE_H
