#include "items.h"
#include "inventory/inventory.h"
#include "../define.h"
#include "../database/database.h"
#include <cJSON.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

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

        sqlReq = malloc(454 * sizeof(char)); //Allocating max size bc we can't calculate it before
        sqlReq[sprintf(sqlReq, "INSERT INTO ITEM VALUES (%d, \"%s\", \"%s\", \"%s\", %hu, %hhu, NULL, \"%s\", %hhu, NULL, NULL)",
                id->valueint, name->valuestring, type->valuestring, description->valuestring, energyBonus->valueint, ability->valueint, sprite->valuestring, growTime->valueint) + 1] = '\0';

        if(executeSQL(db, sqlReq) == FAILURE){
            free(sqlReq);
            return FAILURE;
        }
        free(sqlReq);
    }

    sqlite3_close(db);
    return SUCCESS;
}