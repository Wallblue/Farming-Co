#include "inventory.h"
#include "../../define.h"
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include <string.h>

void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* type, const char* description, unsigned short energyBonus, unsigned char ability, unsigned char growTime, const char* sprite){
    item->id = id;
    strcpy(item->name, name);
    item->quantity = quantity;
    strcpy(item->type, type);
    strcpy(item->description, description);
    item->energyBonus = energyBonus;
    item->ability = ability;
    item->growTime = growTime;
    strcpy(item->sprite, sprite);
}

void resetItem(Item* item){
    affectItem(item, 0, "", 0, "", "", 0, 0, 0, "");
}

void initInventory(Inventory inventory){
    unsigned char i;
    for(i = 0; i < INVENTORY_SIZE; i++)
        resetItem(inventory + i);
}

short itemFind(int id, const Inventory inventory){
    short i;
    for(i = 0; i < INVENTORY_SIZE; i++)
        if(inventory[i].id == id)
            return i;
    return -1;
}

short firstEmptySlot(const Inventory inventory){
    short i;
    for(i = 0; i < INVENTORY_SIZE; i++)
        if(inventory[i].id == 0 && inventory[i].quantity == 0)
            return i;
    return -1;
}

unsigned char itemSet(const Item* item, Inventory inventory){
    short index = firstEmptySlot(inventory);
    if(index == -1) return FAILURE; //If inventory is full

    affectItem(inventory + index, item->id, item->name, item->quantity, item->type, item->description, item->energyBonus, item->ability, item->growTime, item->sprite);
    return SUCCESS;
}

unsigned char itemAdd(const Item* item, Inventory inventory){
    short index = itemFind(item->id, inventory);
    if(index == -1) //If item does not exist in this inventory
        return itemSet(item, inventory);
    //but if it does
    inventory[index].quantity += item->quantity;
    return SUCCESS;
}

unsigned char itemSubstract(int id, unsigned char quantity, Inventory inventory){
    short index = itemFind(id, inventory);
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
    if(fp == NULL) {
        fprintf(stderr, "\nError while saving inventory.\n");
        return FAILURE;
    }
    jsonString = jsonifyInventory(inventory);
    fwrite(jsonString, strlen(jsonString), 1, fp);
    fclose(fp);
    free(jsonString);
    return SUCCESS;
}

unsigned char loadInventory(Inventory inventory){
    cJSON* jsonInventory;
    cJSON* jsonItem;
    unsigned char i;

    if((i = parseJsonFile(INVENTORY_SAVE_FILE, &jsonInventory)) != SUCCESS) return i;

    cJSON_ArrayForEach(jsonItem, jsonInventory){
        cJSON* id = cJSON_GetObjectItemCaseSensitive(jsonItem, "id");
        cJSON* name = cJSON_GetObjectItemCaseSensitive(jsonItem, "name");
        cJSON* quantity = cJSON_GetObjectItemCaseSensitive(jsonItem, "quantity");
        cJSON* type = cJSON_GetObjectItemCaseSensitive(jsonItem, "type");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(jsonItem, "description");
        cJSON* energyBonus = cJSON_GetObjectItemCaseSensitive(jsonItem, "energyBonus");
        cJSON* ability = cJSON_GetObjectItemCaseSensitive(jsonItem, "ability");
        cJSON* growTime = cJSON_GetObjectItemCaseSensitive(jsonItem, "growTime");
        cJSON* sprite = cJSON_GetObjectItemCaseSensitive(jsonItem, "sprite");

        affectItem(inventory + i, id->valueint, name->valuestring, quantity->valueint, type->valuestring, description->valuestring,
                   energyBonus->valueint, ability->valueint, growTime->valueint ,sprite->valuestring);
        ++i;
    }

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
    int i;

    jsonInventory = cJSON_CreateArray();
    if(jsonInventory == NULL) return NULL;

    for(i = 0; i < INVENTORY_SIZE; i++){
        jsonItem = cJSON_CreateObject();
        if(jsonItem == NULL) return NULL;

        cJSON_AddNumberToObject(jsonItem, "id", inventory[i].id);
        cJSON_AddStringToObject(jsonItem, "name", inventory[i].name);
        cJSON_AddNumberToObject(jsonItem, "quantity", inventory[i].quantity);
        cJSON_AddStringToObject(jsonItem, "sprite", inventory[i].sprite);
        cJSON_AddStringToObject(jsonItem, "type", inventory[i].type);

        cJSON_AddItemToArray(jsonInventory, jsonItem);
    }
    jsonString = cJSON_Print(jsonInventory);

    return jsonString;
}