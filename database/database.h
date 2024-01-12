//
// BUSQUET Nicolas & PHE Lina
// 27/12/2023
// Création base de données
//
#ifndef FARMINGCO_DATABASE_H

#define FARMINGCO_DATABASE_H

#include <stdio.h>
#include <sqlite3.h>

int createDatabase();
unsigned char openDb(sqlite3** db);
unsigned char executeSQL(sqlite3* db, const char* request);
unsigned char prepareRequest(sqlite3* db, const char* request, sqlite3_stmt** res);

#endif //FARMINGCO_DATABASE_H
