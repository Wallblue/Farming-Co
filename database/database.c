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

unsigned char startGame(){
    sqlite3* db;
    if(openDb(&db) == FAILURE) return FAILURE;
    char* sqlReq = "INSERT OR IGNORE INTO PLAYER VALUES (1, \"Player\", DATE('now'), \"Farm\", 0, 10)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (6, 1, 1, 0)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (7, 1, 1, 1)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (8, 1, 1, 2)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (35, 1, 1, 3)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (32, 1, 1, 4)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (18, 1, 1, 5)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO PLAYER_OWN VALUES (19, 1, 1, 6)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (1, \"Explore my shop, it's filled with curated brilliance.\", 1)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (2, \"Discover intellect in every item at my shop. Take a look!\", 1)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (3, \"Smart choices await you in my updated collection. Care to browse?\", 1)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (4, \"Hello dear, take a peek at my updated shop - found some gems you might like!\", 2)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (5, \"Hi darling, check out the latest in my shop. Curated just for you!\", 2)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (6, \"New arrivals at my fantastic shop! Care to have a quick look?\", 2)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (7, \"I do not like chocolate\", 3)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (8, \"What a great place !\", 3)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (9, \"My name is Chocolat, but I'm not to be eaten\", 3)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (10, \"Check out my shop selected just for you!\", 4)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (11, \"Explore my shop's latest finds!\", 4)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (12, \"See what's new at my shop!\", 4)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (13, \"Do not be afraid, I do not jinx people\", 5)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (14, \"I'm craving chocolate ! But it might kill me\", 5)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlReq = "INSERT OR IGNORE INTO dialog VALUES (15, \"Meow meow meow meow meow (inaudible)\", 5)";
    if(executeSQL(db, sqlReq) == FAILURE) return FAILURE;
    sqlite3_close(db);
    return SUCCESS;
}

unsigned char returnProperly(sqlite3 *db, sqlite3_stmt *res, const char *errmsg, unsigned char returnValue) {
    if(res != NULL) sqlite3_finalize(res);
    if(returnValue == FAILURE) fprintf(stderr, "SQL Error : %s\n", sqlite3_errmsg(db));
    if(db != NULL) sqlite3_close(db);
    return returnValue;
}