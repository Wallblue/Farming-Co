//
// Created by Lina Phe on 08/12/2023.
//

#include <stdio.h>
#include <sqlite3.h>
#include "database.h"

int createDatabase(){
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("../database/farmingco.db", &db);

    if(rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char *sql = "CREATE TABLE cooperative(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, coopName VARCHAR(50), numMembers INTEGER, rank INTEGER, level INTEGER, money INTEGER);"
                "CREATE TABLE player(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, pseudo VARCHAR(25), startDate DATETIME, coopId INTEGER, FOREIGN KEY(coopId) REFERENCES cooperative(id));"
                "CREATE TABLE object(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, x INTEGER, y INTEGER, w INTEGER, h INTEGER, name VARCHAR(25), poseDate DATETIME, growDate DATETIME, sprite VARCHAR(50));"
                "CREATE TABLE place(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(50), type INTEGER);"
                "CREATE TABLE npc(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, firstName VARCHAR(25), lastName VARCHAR(25), skin VARCHAR(128), tradable BOOLEAN, placeId INTEGER, FOREIGN KEY(placeId) REFERENCES place(id));"
                "CREATE TABLE item(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, itemName VARCHAR(25), description TEXT, energyBonus INTEGER, ability INTEGER, quantity INTEGER, sprite VARCHAR(50), ownerId INTEGER, npcId INTEGER, FOREIGN KEY(ownerId) REFERENCES player(id), FOREIGN KEY(npcId) REFERENCES npc(id));"
                "CREATE TABLE dialog(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, description TEXT, npcId INTEGER, FOREIGN KEY(npcId) REFERENCES npc(id));";



    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    sqlite3_close(db);

    return 0;


}