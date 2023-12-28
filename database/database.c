//
// BUSQUET Nicolas & PHE Lina
// 27/12/2023
// Création base de données
//

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

    char *sql = "CREATE TABLE cooperative("
                 "coopId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "coopName VARCHAR(25),"
                 "startTime DATETIME, "
                 "maxMembers INTEGER, "
                 "level INTEGER, "
                 "money INTEGER);"

                 "CREATE TABLE player("
                 "playerId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "pseudo VARCHAR(25), "
                 "startDate DATETIME, "
                 "farmName VARCHAR(25),"
                 "timeInGame INTEGER,"
                 "maxEnergy INTEGER,"
                 "coopId INTEGER, "
                 "FOREIGN KEY(coopId) REFERENCES cooperative(coopId));"

                 "CREATE TABLE object("
                 "objectId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "x INTEGER, "
                 "y INTEGER, "
                 "w INTEGER, "
                 "h INTEGER, "
                 "name VARCHAR(25), "
                 "poseDate INT, "
                 "growDate INT, "
                 "sprite VARCHAR(128),"
                 "state INTEGER,"
                 "boosted BOOLEAN, "
                 "playerId INTEGER, "
                 "FOREIGN KEY(playerId) REFERENCES player(playerId));"

                 "CREATE TABLE place("
                 "placeId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "name VARCHAR(50), "
                 "type INTEGER);"

                 "CREATE TABLE npc("
                 "npcId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "firstName VARCHAR(25), "
                 "lastName VARCHAR(25), "
                 "skin VARCHAR(128), "
                 "tradable BOOLEAN, "
                 "placeId INTEGER, "
                 "FOREIGN KEY(placeId) REFERENCES place(placeId));"

                 "CREATE TABLE item("
                 "itemId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "itemName VARCHAR(25), "
                 "type INTEGER, "
                 "description TEXT, "
                 "energyBonus INTEGER, "
                 "ability INTEGER, "
                 "quantity INTEGER, "
                 "sprite VARCHAR(128), "
                 "growTime INTEGER, "
                 "ownerId INTEGER, "
                 "npcId INTEGER, "
                 "FOREIGN KEY(ownerId) REFERENCES player(playerId), "
                 "FOREIGN KEY(npcId) REFERENCES npc(npcId));"

                 "CREATE TABLE dialog("
                 "dialogId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                 "content TEXT, "
                 "npcId INTEGER, "
                 "FOREIGN KEY(npcId) REFERENCES npc(npcId));";


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