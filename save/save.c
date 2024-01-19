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

unsigned char loadObjectsMaps(){
    sqlite3* db;
    sqlite3_stmt* res;

    if(openDb(&db) == FAILURE) return FAILURE;

    if(prepareRequest(db, "SELECT object.x, object.y, object.zone, item.linkedObjectSpriteRef, object.state FROM object, item WHERE item.itemId = object.itemId", &res) == FAILURE) return FAILURE;

    while(sqlite3_step(res) == SQLITE_ROW){
        switch(sqlite3_column_int(res, 2)){
            case 0:
                mapObjects1[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4) ;
                break;
            case 1:
                mapObjects2[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4);
                break;
            case 2:
                mapObjects3[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4);
                break;
            case 3:
                mapObjects4[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4);
                break;
            case 4:
                homeObjects[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4);
                if((char) *sqlite3_column_text(res, 3)=='J' || (char) *sqlite3_column_text(res, 3)=='I')
                    homeObjects[sqlite3_column_int(res, 1)+1][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3)+10;
                break;
        }
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
                   "INSERT INTO object (x, y, zone, growTime, poseDate, state, boosted, itemID) VALUES (%d, %d, %hhd, %hhu, %d, %hhd, %hhu, %d);",
                   object->x, object->y, object->zone, object->growTime, object->poseDate, object->state, object->boosted, object->itemId) + 1] = '\0';

    if (executeSQL(db, sqlReq) == FAILURE) {
        free(sqlReq);
        sqlite3_close(db);
        return FAILURE;
    }

    free(sqlReq);
    sqlite3_close(db);
    return SUCCESS;
}