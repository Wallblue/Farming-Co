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

#include <SDL.h>
#include <stdio.h>
#include <sqlite3.h>
#include "save.h"
#include "../define.h"
#include "../map/map.h"
#include "../database/database.h"

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
    int rc;
    int i;

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

    if(openDb(&db) == FAILURE) return FAILURE;

    if(prepareRequest(db, "SELECT * FROM object", &res) == FAILURE) return FAILURE;

    while(sqlite3_step(res) == SQLITE_ROW){
        switch(sqlite3_column_int(res, 5)){
            case 0:
                mapObjects1[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char) *sqlite3_column_text(res, 9);
                break;
            case 1:
                mapObjects2[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char) *sqlite3_column_text(res, 9);
                break;
            case 2:
                mapObjects3[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char) *sqlite3_column_text(res, 9);
                break;
            case 3:
                mapObjects4[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char) *sqlite3_column_text(res, 9);
                break;
            case 4:
                homeObjects[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char) *sqlite3_column_text(res, 9);
                break;
        }
        i++;
    }

    return SUCCESS;
}