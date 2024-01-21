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

    srand(time(NULL));
    int randomNum = rand() % 3;

    if(openDb(&db) == FAILURE) return FAILURE;

    if(prepareRequest(db, "SELECT object.x, object.y, object.zone, item.linkedObjectSpriteRef, object.state FROM object, item WHERE item.itemId = object.itemId", &res) == FAILURE) return FAILURE;

    while(sqlite3_step(res) == SQLITE_ROW){
        switch(sqlite3_column_int(res, 2)){
            case 0:
                mapObjects1[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                break;
            case 1:
                mapObjects2[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
                break;
            case 2:
                mapObjects3[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4) ;
                soiledFloor3[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = 'A' + randomNum;
                break;
            case 3:
                mapObjects4[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3) + (int) sqlite3_column_int(res, 4);
                soiledFloor4[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = 'A' + randomNum;
                break;
            case 4:
                homeObjects[sqlite3_column_int(res, 1)][sqlite3_column_int(res, 0)] = (char) *sqlite3_column_text(res, 3);
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

unsigned char saveNewItem(Inventory* inventory, int slot, sqlite3* db){
    sqlite3_stmt* res;
    int rc;
    char homeMadeDb = 0;

    if(db == NULL) {
        if (openDb(&db) == FAILURE) return FAILURE;
        homeMadeDb = 1;
    }

    switch(inventory->ownerType){
        case 1:
            rc = prepareRequest(db, "INSERT INTO OBJECT_OWN (itemId, objectId, quantity, slot) VALUES (?1, ?2, ?3, ?4);", &res);
            sqlite3_bind_int(res, 4, slot);
            break;
        case 2:
            rc = prepareRequest(db, "INSERT INTO NPC_OWN (itemId, npcId, quantity) VALUES (?1, ?2, ?3);", &res);
            break;
        default:
            rc = prepareRequest(db, "INSERT INTO PLAYER_OWN (itemId, playerId, quantity, slot) VALUES (?1, ?2, ?3, ?4);", &res);
            sqlite3_bind_int(res, 4, slot);
            break;
    }
    if(rc == FAILURE){
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, inventory->slots[slot].id);
    sqlite3_bind_int(res, 2, inventory->ownerId);
    sqlite3_bind_int(res, 3, inventory->slots[slot].quantity);

    rc = sqlite3_step(res);
    sqlite3_finalize(res);
    if(homeMadeDb == 1) sqlite3_close(db);

    if(rc != SQLITE_DONE) return FAILURE;
    return SUCCESS;
}

unsigned char deleteSavedItem(Inventory* inventory, char slot, sqlite3* db){
    sqlite3_stmt* res;
    int rc;
    char homeMadeDb = 0;

    if(db == NULL) {
        if (openDb(&db) == FAILURE) return FAILURE;
        homeMadeDb = 1;
    }

    switch(inventory->ownerType){
        case 1:
            rc = prepareRequest(db, "DELETE FROM OBJECT_OWN WHERE itemId = ?1 AND objectId = ?2 AND slot = ?3", &res);
            sqlite3_bind_int(res, 3, slot);
            break;
        case 2:
            rc = prepareRequest(db, "DELETE FROM NPC_OWN WHERE itemId = ?1 AND npcId = ?2", &res);
            break;
        default:
            rc = prepareRequest(db, "DELETE FROM PLAYER_OWN WHERE itemId = ?1 AND playerId = ?2 AND slot = ?3", &res);
            sqlite3_bind_int(res, 3, slot);
    }
    if(rc == FAILURE){
        sqlite3_finalize(res);
        if(homeMadeDb == 1) sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, inventory->slots[slot].id);
    sqlite3_bind_int(res, 2, inventory->ownerId);

    rc = sqlite3_step(res);
    sqlite3_finalize(res);
    if(homeMadeDb == 1) sqlite3_close(db);

    if(rc != SQLITE_DONE) return FAILURE;
    return SUCCESS;
}

unsigned char alterItemQuantity(Inventory* inventory, char slot, unsigned char quantity, char mode){ //Mode 1 : subtract, default : add
    sqlite3* db;
    sqlite3_stmt* res;
    int computeQuantity = quantity;
    int rc;
    if(openDb(&db) == FAILURE) return FAILURE;

    if(mode == 1) computeQuantity = -computeQuantity;

    switch(inventory->ownerType){
        case 1:
            rc = prepareRequest(db, "UPDATE OBJECT_OWN SET quantity = quantity + ?1 WHERE itemId = ?2 AND objectId = ?3 AND slot = ?4", &res);
            sqlite3_bind_int(res, 4, slot);
            break;
        case 2:
            rc = prepareRequest(db, "UPDATE NPC_OWN SET quantity = quantity + ?1 WHERE itemId = ?2 AND npcId = ?3", &res);
            break;
        default:
            rc = prepareRequest(db, "UPDATE PLAYER_OWN SET quantity = quantity  + ?1 WHERE itemId = ?2 AND playerId = ?3 AND slot = ?4", &res);
            sqlite3_bind_int(res, 4, slot);
    }
    if(rc == FAILURE){
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, computeQuantity);
    sqlite3_bind_int(res, 2, inventory->slots[slot].id);
    sqlite3_bind_int(res, 3, inventory->ownerId);

    rc = sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);
    if(rc != SQLITE_DONE) return FAILURE;

    return SUCCESS;
}

unsigned char updateItemSlot(Inventory* inventory, char srcSlot, char destSlot, sqlite3* db){
    sqlite3_stmt* res;
    int rc;
    char homeMadeDb = 0;

    if(db == NULL) {
        if (openDb(&db) == FAILURE) return FAILURE;
        homeMadeDb = 1;
    }

    switch(inventory->ownerType){
        case 1:
            rc = prepareRequest(db, "UPDATE OBJECT_OWN SET slot = ?1 WHERE itemId = ?2 AND objectId = ?3 AND slot = ?4", &res);
            break;
        case 2:
            if(homeMadeDb == 1) sqlite3_close(db);
            fprintf(stderr, "No slots management with NPC's inventory.");
            return FAILURE;
        default:
            rc = prepareRequest(db, "UPDATE PLAYER_OWN SET slot = ?1 WHERE itemId = ?2 AND playerId = ?3 AND slot = ?4", &res);
    }
    if(rc == FAILURE){
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, destSlot);
    sqlite3_bind_int(res, 2, inventory->slots[srcSlot].id);
    sqlite3_bind_int(res, 3, inventory->ownerId);
    sqlite3_bind_int(res, 4, srcSlot);

    rc = sqlite3_step(res);
    sqlite3_finalize(res);
    if(homeMadeDb == 1) sqlite3_close(db);
    if(rc != SQLITE_DONE) return FAILURE;

    return SUCCESS;
}
