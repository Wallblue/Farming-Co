//
// BUSQUET Nicolas & PHE Lina
// 27/12/2023
// Création base de données
//

#include "database.h"

int createDatabase(){
    sqlite3 *db;
    FILE* fp;
    char* sql;
    size_t fileSize;

    if(openDb(&db) == FAILURE) return FAILURE;

    fp = fopen("../database/init.sql", "rb");
    if(fp == NULL) return FAILURE;
    fileSize = getFileSize(fp);

    sql = malloc(fileSize * sizeof(char) + 1);
    if(sql == NULL) return FAILURE;

    fread(sql, fileSize, 1, fp);
    sql[fileSize] = '\0';
    fclose(fp);

    if(executeSQL(db, sql) == FAILURE) return FAILURE;

    sqlite3_close(db);

    return 0;
}

unsigned char openDb(sqlite3** db){
    int rc = sqlite3_open("../database/farmingco.db", db);

    if(rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return FAILURE;
    }
    return SUCCESS;
}

unsigned char executeSQL(sqlite3* db, const char* request){
    char *err_msg = 0;
    int rc = sqlite3_exec(db, request, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);

        return FAILURE;
    }
    return SUCCESS;
}

unsigned char prepareRequest(sqlite3* db, const char* request, sqlite3_stmt** res){
    int rc = sqlite3_prepare_v2(db, request, -1, res, 0);

    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to fetch data %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return FAILURE;
    }
    return SUCCESS;
}

unsigned char addStartKitToDatabase(){
    cJSON* jsonContent = NULL;
    cJSON* jsonObject;
    char* sqlReq;
    sqlite3* db;

    if(parseJsonFile("../player/startKit.json", &jsonContent) == FAILURE) return FAILURE;

    if(openDb(&db) == FAILURE) return FAILURE;

    cJSON_ArrayForEach(jsonObject, jsonContent){
        cJSON* itemId = cJSON_GetObjectItemCaseSensitive(jsonObject, "itemId");
        cJSON* quantity = cJSON_GetObjectItemCaseSensitive(jsonObject, "quantity");
        cJSON* slot = cJSON_GetObjectItemCaseSensitive(jsonObject, "slot");

        sqlReq = malloc(100 * sizeof(char)); //Allocating max size bc we can't calculate it before
        if(sqlReq == NULL) {
            free(sqlReq);
            return FAILURE;
        }
        sqlReq[sprintf(sqlReq, "INSERT OR IGNORE INTO PLAYER_OWN (itemId, playerId, quantity, slot) VALUES (%d, %d, %d, %d);",
                       itemId->valueint, playerId, quantity->valueint, slot->valueint) + 1] = '\0';

        if(executeSQL(db, sqlReq) == FAILURE){
            free(sqlReq);
            return FAILURE;
        }
        free(sqlReq);
    }

    sqlite3_close(db);
    return SUCCESS;
}

unsigned char startGame(){
    sqlite3* db;
    if(openDb(&db) == FAILURE) return FAILURE;
    char* sqlReq = "INSERT OR IGNORE INTO PLAYER VALUES (1, \"Player\", DATE('now'), \"Farm\", 0, 10)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;

    if(sqlite3_changes(db) != 0)
        if(addStartKitToDatabase() == FAILURE) returnProperlyM(db, NULL, "Couldn't add startKit.\n", FAILURE);

    sqlite3_close(db);
    return SUCCESS;
}

unsigned char returnProperly(sqlite3 *db, sqlite3_stmt *res, const char *errmsg, unsigned char returnValue) {
    if(res != NULL) sqlite3_finalize(res);
    if(returnValue == FAILURE) fprintf(stderr, errmsg, sqlite3_errmsg(db));
    if(db != NULL) sqlite3_close(db);
    return returnValue;
}