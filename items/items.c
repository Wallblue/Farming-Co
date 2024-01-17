#include "items.h"
#include "inventory/inventory.h"
#include "../define.h"
#include "../database/database.h"
#include <cJSON.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* type, const char* description, unsigned short energyBonus, unsigned char ability, unsigned char growTime, const char* sprite, unsigned char objectSpriteRef){
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
}

void resetItem(Item* item){
    affectItem(item, 0, "", 0, "", "", 0, 0, 0, "", 0);
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

        sqlReq = malloc(454 * sizeof(char)); //Allocating max size bc we can't calculate it before
        if(sqlReq == NULL){
            free(sqlReq);
            return FAILURE;
        }
        sqlReq[sprintf(sqlReq, "INSERT OR IGNORE INTO ITEM VALUES (%d, \"%s\", \"%s\", \"%s\", %hu, %hhu, NULL, \"%s\", %hhu, \"%c\", NULL, NULL);",
                id->valueint, name->valuestring, type->valuestring, description->valuestring, energyBonus->valueint, ability->valueint, sprite->valuestring, growTime->valueint, *(linkedSpriteRef->valuestring)) + 1] = '\0';

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

    if(db == NULL)
        if(openDb(&db) == FAILURE) return FAILURE;

    if(prepareRequest(db, "SELECT * FROM item WHERE itemId = ?1", &res) == FAILURE) return FAILURE;
    sqlite3_bind_int(res, 1, id);
    rc = sqlite3_step(res);
    if(rc != SQLITE_ROW){
        fprintf(stderr, "Can't get item.");
        return FAILURE;
    }

    affectItem(dest, id, (char*)sqlite3_column_text(res, 1), 0, (char*)sqlite3_column_text(res, 2), (char*)sqlite3_column_text(res, 3),
               sqlite3_column_int(res, 4), sqlite3_column_int(res, 5), sqlite3_column_int(res, 8) ,(char*)sqlite3_column_text(res, 7), *sqlite3_column_text(res, 9));

    if(db == NULL) sqlite3_close(db);
    sqlite3_finalize(res);
    return SUCCESS;
}

void printItem(const Item* item){
    FILE* fp = fopen("print.txt", "w");
    fprintf(fp, "id : %d\nname : %s\ntype : %s\ndescription : %s\nenergyBonus : %hu\nability : %hhu\ngrowTime : %hhu\nsprite : %s\nquantity : %hhu\nspriteRef : %c",
            item->id, item->name, item->type, item->description, item->energyBonus, item->ability, item->growTime, item->sprite, item->quantity, item->objectSpriteRef);
    fclose(fp);
}