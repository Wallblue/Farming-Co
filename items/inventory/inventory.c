#include "inventory.h"
#include "../../define.h"
#include "../../database/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include <string.h>
#include <sqlite3.h>

void initInventory(Inventory inventory){
    unsigned char i;
    for(i = 0; i < INVENTORY_MAX_SIZE; i++)
        resetItem(inventory + i);
}

short findItem(int id, const Inventory inventory){
    short i;
    for(i = 0; i < INVENTORY_MAX_SIZE; i++)
        if(inventory[i].id == id)
            return i;
    return -1;
}

short firstEmptySlot(const Inventory inventory){
    short i;
    for(i = 0; i < INVENTORY_MAX_SIZE; i++)
        if(inventory[i].id == 0 && inventory[i].quantity == 0)
            return i;
    return -1;
}

unsigned char setItem(const Item* item, Inventory inventory){
    short index = firstEmptySlot(inventory);
    if(index == -1) return FAILURE; //If inventory is full

    affectItem(inventory + index, item->id, item->name, item->quantity, item->type, item->description, item->energyBonus, item->ability, item->growTime, item->sprite);
    return SUCCESS;
}

unsigned char addItem(const Item* item, Inventory inventory){
    short index = findItem(item->id, inventory);
    if(index == -1) //If item does not exist in this inventory
        return setItem(item, inventory);
    //but if it does
    inventory[index].quantity += item->quantity;
    return SUCCESS;
}

unsigned char substractItem(int id, unsigned char quantity, Inventory inventory){
    short index = findItem(id, inventory);
    if(index == -1 || inventory[index].quantity < quantity) return FAILURE;

    if(inventory[index].quantity == quantity)
        resetItem(inventory + index);
    else
        inventory[index].quantity -= quantity;

    return SUCCESS;
}

unsigned char saveInventory(Inventory inventory){
    char* jsonString;
    FILE* fp;

    fp = fopen(INVENTORY_SAVE_FILE, "w");
    if(fp == NULL) return FAILURE;

    jsonString = jsonifyInventory(inventory);
    if(jsonString == NULL){
        fclose(fp);
        return FAILURE;
    }

    fwrite(jsonString, strlen(jsonString), 1, fp);
    fclose(fp);
    free(jsonString);
    return SUCCESS;
}

unsigned char loadInventory(Inventory inventory){
    cJSON* jsonInventory;
    cJSON* jsonItem;
    sqlite3* db;
    sqlite3_stmt* res;
    unsigned char i;
    int rc;

    if(openDb(&db) == FAILURE) return FAILURE;

    if((i = parseJsonFile(INVENTORY_SAVE_FILE, &jsonInventory)) != SUCCESS) return i;

    cJSON_ArrayForEach(jsonItem, jsonInventory){
        //On récupère l'id et la quantité dans la save
        cJSON* id = cJSON_GetObjectItemCaseSensitive(jsonItem, "id");
        cJSON* quantity = cJSON_GetObjectItemCaseSensitive(jsonItem, "quantity");

        if(id->valueint == 0){
            resetItem(inventory + i++);
            continue;
        }

        //On récupère le reste dans la bdd
        if(prepareRequest(db, "SELECT * FROM item WHERE itemId = ?1", &res) == FAILURE) return FAILURE;
        sqlite3_bind_int(res, 1, id->valueint);
        rc = sqlite3_step(res);
        if(rc != SQLITE_ROW){
            fprintf(stderr, "Can't load saved item.");
            return FAILURE;
        }


        affectItem(inventory + i, id->valueint, (char*)sqlite3_column_text(res, 1), quantity->valueint, (char*)sqlite3_column_text(res, 2), (char*)sqlite3_column_text(res, 3),
                   sqlite3_column_int(res, 4), sqlite3_column_int(res, 5), sqlite3_column_int(res, 6) ,(char*)sqlite3_column_text(res, 7));

        sqlite3_finalize(res);
        ++i;
    }

    sqlite3_close(db);
    return SUCCESS;
}

unsigned char parseJsonFile(const char* fileName, cJSON** dest){
    FILE* fp;
    char* jsonFileContent;
    size_t fileSize;

    fp = fopen(fileName, "r");
    if(fp == NULL) return 2;

    fileSize = getFileSize(fp);

    jsonFileContent = malloc((fileSize + 1) * sizeof(char));
    if(jsonFileContent == NULL) return FAILURE;
    fread(jsonFileContent, fileSize, 1, fp);
    jsonFileContent[fileSize] = '\0';
    fclose(fp);

    *dest = cJSON_Parse(jsonFileContent);
    free(jsonFileContent);
    if(*dest == NULL) return FAILURE;

    return SUCCESS;
}

char* jsonifyInventory(Inventory inventory){
    cJSON *jsonInventory;
    cJSON *jsonItem;
    char *jsonString;
    unsigned char i;

    jsonInventory = cJSON_CreateArray();
    if(jsonInventory == NULL) return NULL;

    for(i = 0; i < INVENTORY_MAX_SIZE; i++){
        jsonItem = cJSON_CreateObject();
        if(jsonItem == NULL) return NULL;

        cJSON_AddNumberToObject(jsonItem, "id", inventory[i].id);
        cJSON_AddNumberToObject(jsonItem, "quantity", inventory[i].quantity);

        cJSON_AddItemToArray(jsonInventory, jsonItem);
    }
    jsonString = cJSON_Print(jsonInventory);

    return jsonString;
}