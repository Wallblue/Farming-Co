//
// BUSQUET Nicolas & PHE Lina
// 27/12/2023
// Création base de données
//

#include "database.h"
#include "../define.h"
#include <stdlib.h>

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

unsigned char startGame(){
    sqlite3* db;
    if(openDb(&db) == FAILURE) return FAILURE;
    char* sqlReq = "INSERT OR IGNORE INTO PLAYER VALUES (1, \"Player\", DATE('now'), \"Farm\", 0, 50, NULL)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlite3_close(db);
    return SUCCESS;
}