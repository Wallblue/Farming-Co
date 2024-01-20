/*
 * BUSQUET Nicolas & PHE Lina
 * 18/01/202
 * But du programme : Gestion des items
 */

#include "items.h"

void affectItem(Item *item, int id, const char *name, unsigned char quantity, const char *type, const char *description,
                unsigned short energyBonus, unsigned char ability, unsigned char growTime, const char *sprite,
                unsigned char objectSpriteRef, int evolution, int linkedTool) {
    item->id = id;
    strcpy(item->name, name);
    item->quantity = quantity;
    strcpy(item->type, type);
    strcpy(item->description, description);
    item->energyBonus = energyBonus;
    item->ability = ability;
    item->growTime = growTime;
    strcpy(item->sprite, sprite);
    item->objectSpriteRef = objectSpriteRef;
    item->evolution = evolution;
    item->linkedTool = linkedTool;
}

void resetItem(Item* item){
    affectItem(item, 0, "", 0, "", "", 0, 0, 0, "", 0, 0, 0);
}

unsigned char addItemsToDatabase(){
    cJSON* jsonContent = NULL;
    cJSON* jsonObject;
    char* sqlReq;
    sqlite3* db;

    if(parseJsonFile("../items/items.json", &jsonContent) == FAILURE) return FAILURE;

    if(openDb(&db) == FAILURE) return FAILURE;

    cJSON_ArrayForEach(jsonObject, jsonContent){
        cJSON* id = cJSON_GetObjectItemCaseSensitive(jsonObject, "id");
        cJSON* name = cJSON_GetObjectItemCaseSensitive(jsonObject, "name");
        cJSON* type = cJSON_GetObjectItemCaseSensitive(jsonObject, "type");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(jsonObject, "description");
        cJSON* energyBonus = cJSON_GetObjectItemCaseSensitive(jsonObject, "energyBonus");
        cJSON* ability = cJSON_GetObjectItemCaseSensitive(jsonObject, "ability");
        cJSON* growTime = cJSON_GetObjectItemCaseSensitive(jsonObject, "growTime");
        cJSON* sprite = cJSON_GetObjectItemCaseSensitive(jsonObject, "sprite");
        cJSON* linkedSpriteRef = cJSON_GetObjectItemCaseSensitive(jsonObject, "linkedSpriteRef");
        cJSON* evolution = cJSON_GetObjectItemCaseSensitive(jsonObject, "evolution");
        cJSON* linkedTool = cJSON_GetObjectItemCaseSensitive(jsonObject, "linkedTool");

        sqlReq = malloc(700 * sizeof(char)); //Allocating max size bc we can't calculate it before
        if(sqlReq == NULL) {
            free(sqlReq);
            return FAILURE;
        }
        sqlReq[sprintf(sqlReq, "INSERT OR IGNORE INTO ITEM VALUES (%d, \"%s\", \"%s\", \"%s\", %hu, %hhu, NULL, \"%s\", %hhu, \"%c\", %d, %d, NULL, NULL);",
                id->valueint, name->valuestring, type->valuestring, description->valuestring, energyBonus->valueint, ability->valueint, sprite->valuestring,
                growTime->valueint, *(linkedSpriteRef->valuestring), evolution == NULL ? 0 : evolution->valueint, linkedTool->valueint) + 1] = '\0';

        if(executeSQL(db, sqlReq) == FAILURE){
            free(sqlReq);
            return FAILURE;
        }
        free(sqlReq);
    }

    sqlite3_close(db);
    return SUCCESS;
}

unsigned char getItem(int id, Item* dest, sqlite3* db){
    sqlite3_stmt* res;
    int rc;
    char homeMadeDb = 0;

    if(db == NULL) {
        if (openDb(&db) == FAILURE) return FAILURE;
        homeMadeDb = 1;
    }
    if(prepareRequest(db, "SELECT * FROM item WHERE itemId = ?1;", &res) == FAILURE) return FAILURE;
    sqlite3_bind_int(res, 1, id);
    rc = sqlite3_step(res);
    if(rc != SQLITE_ROW){
        fprintf(stderr, "Can't get item.");
        return FAILURE;
    }

    affectItem(dest, id, (char *) sqlite3_column_text(res, 1), 0, (char *) sqlite3_column_text(res, 2),
               (char *) sqlite3_column_text(res, 3),sqlite3_column_int(res, 4),
               sqlite3_column_int(res, 5), sqlite3_column_int(res, 8),(char *) sqlite3_column_text(res, 7),
               *sqlite3_column_text(res, 9), sqlite3_column_int(res, 10), sqlite3_column_int(res, 11));

    if(homeMadeDb == 1) sqlite3_close(db);
    sqlite3_finalize(res);
    return SUCCESS;
}

void printItem(const Item* item){
    FILE* fp = fopen("print.txt", "w");
    fprintf(fp, "id : %d\nname : %s\ntype : %s\ndescription : %s\nenergyBonus : %hu\nability : %hhu\ngrowTime : %hhu\nsprite : %s\nquantity : %hhu\nspriteRef : %c\nevolution ID: %d\nlinked tool ID : %d",
            item->id, item->name, item->type, item->description, item->energyBonus, item->ability, item->growTime, item->sprite, item->quantity, item->objectSpriteRef, item->evolution, item->linkedTool);
    fclose(fp);
}

void swapItems(Item* srcItem, Item* destItem){
    Item temp;

    if(srcItem == destItem) return;

    affectItem(&temp, srcItem->id, srcItem->name, srcItem->quantity, srcItem->type, srcItem->description, srcItem->energyBonus,
               srcItem->ability, srcItem->growTime, srcItem->sprite, srcItem->objectSpriteRef, srcItem->evolution, srcItem->linkedTool);

    affectItem(srcItem, destItem->id, destItem->name, destItem->quantity, destItem->type, destItem->description, destItem->energyBonus,
               destItem->ability, destItem->growTime, destItem->sprite, destItem->objectSpriteRef, destItem->evolution, destItem->linkedTool);

    affectItem(destItem, temp.id, temp.name, temp.quantity, temp.type, temp.description, temp.energyBonus,
               temp.ability, temp.growTime, temp.sprite, temp.objectSpriteRef, temp.evolution, temp.linkedTool);
}