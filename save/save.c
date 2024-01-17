//
// BUSQUET Nicolas & PHE Lina
// 10/01/2024
// Sauvegarde de la partie
//

/*
 * Ce qui est à sauvegarder :
 *  -> maps
 *  -> inventaires
 *  -> argent
 *  -> temps de pousse des plantes
 * */

#include "save.h"

unsigned char saveObjectMaps(){
    FILE* fp;
    unsigned char i;

    fp = fopen("../save/objectMapsSave.dat", "wb");
    if(fp == NULL) return FAILURE;

    for(i = 0; i < mapHeight; i++) {
        fwrite(mapObjects1[i], mapWidth, 1, fp);
        fwrite(mapObjects2[i], mapWidth, 1, fp);
        fwrite(mapObjects3[i], mapWidth, 1, fp);
        fwrite(mapObjects4[i], mapWidth, 1, fp);
        fwrite(homeObjects[i], mapWidth, 1, fp);
    }
    fclose(fp);

    return SUCCESS;
}

unsigned char loadObjectsMaps(){
    FILE* fp;
    unsigned char i;

    fp = fopen("../save/objectMapsSave.dat", "rb");
    if(fp == NULL) return 2;

    mapObjects1 = malloc(mapHeight * sizeof(char *));
    if(mapObjects1 == NULL) return FAILURE;
    mapObjects2 = malloc(mapHeight * sizeof(char *));
    if(mapObjects2 == NULL) return FAILURE;
    mapObjects3 = malloc(mapHeight * sizeof(char *));
    if(mapObjects3 == NULL) return FAILURE;
    mapObjects4 = malloc(mapHeight * sizeof(char *));
    if(mapObjects4 == NULL) return FAILURE;
    homeObjects = malloc(mapHeight * sizeof(char *));
    if(homeObjects == NULL) return FAILURE;

    for (i = 0; i < mapHeight; i++) {
        if(loadMapLine(mapObjects1 + i, fp) == FAILURE) return FAILURE;
        if(loadMapLine(mapObjects2 + i, fp) == FAILURE) return FAILURE;
        if(loadMapLine(mapObjects3 + i, fp) == FAILURE) return FAILURE;
        if(loadMapLine(mapObjects4 + i, fp) == FAILURE) return FAILURE;
        if(loadMapLine(homeObjects + i, fp) == FAILURE) return FAILURE;
    }

    fclose(fp);
    return SUCCESS;
}

unsigned char loadMapLine(unsigned char** line, FILE* fp){
    *line = malloc(mapWidth * sizeof(char));
    if(*line == NULL) return FAILURE;

    fread(*line, mapWidth, 1, fp);

    return SUCCESS;
}

unsigned char loadMapV2(){
    sqlite3* db;
    sqlite3_stmt* res;
    int i;

    if(openDb(&db) == FAILURE) return FAILURE;

    if(prepareRequest(db, "SELECT object.x, object.y, object.zone, item.linkedObjectSpriteRef FROM object, item WHERE item.itemId = object.itemId", &res) == FAILURE) return FAILURE;

    while(sqlite3_step(res) == SQLITE_ROW){
        switch(sqlite3_column_int(res, 2)){
            case 0:
                mapObjects1[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                break;
            case 1:
                mapObjects2[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                break;
            case 2:
                mapObjects3[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                break;
            case 3:
                mapObjects4[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                break;
            case 4:
                homeObjects[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                if((char) *sqlite3_column_text(res, 3)=='J' || (char) *sqlite3_column_text(res, 3)=='I')
                    homeObjects[sqlite3_column_int(res, 1)+1][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3)+10;
                break;
        }
        i++;
    }
    sqlite3_finalize(res);
    sqlite3_close(db);
    return SUCCESS;
}

unsigned char saveObject(const Object* object) {
    sqlite3* db;
    char* sqlReq;
    if(openDb(&db) == FAILURE) return FAILURE;

    sqlReq = malloc(160 * sizeof(char)); //Allocating max size bc we can't calculate it before
    if(sqlReq == NULL){
        free(sqlReq);
        sqlite3_close(db);
        return FAILURE;
    }
    sqlReq[sprintf(sqlReq,
                   "INSERT INTO object (x, y, zone, growTime, growDate, state, boosted, itemID) VALUES (%d, %d, %hhd, %hhu, %d, %hhd, %hhu, %d);",
                   object->x, object->y, object->zone, object->growTime, object->growDate, object->state, object->boosted, object->itemId) + 1] = '\0';

    if (executeSQL(db, sqlReq) == FAILURE) {
        free(sqlReq);
        sqlite3_close(db);
        return FAILURE;
    }

    free(sqlReq);
    sqlite3_close(db);
    return SUCCESS;
}