#include "inventory.h"
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include <string.h>

void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* sprite, const char* type){
    item->id = id;
    strcpy(item->itemName, name);
    item->quantity = quantity;
    strcpy(item->sprite, sprite);
    strcpy(item->itemType, type);
}

void resetItem(Item* item){
    affectItem(item, 0, "", 0, "", "");
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

unsigned char itemSet(Item item, Inventory inventory){
    short index = firstEmptySlot(inventory);
    if(index == -1) return FAILURE; //If inventory is full

    affectItem(inventory + index, item.id, item.itemName, item.quantity, item.sprite, item.itemType);
    return SUCCESS;
}

unsigned char itemAdd(Item item, Inventory inventory){
    short index = itemFind(item.id, inventory);
    if(index == -1) //If item does not exist in this inventory
        return itemSet(item, inventory);
    //but if it does
    inventory[index].quantity += item.quantity;
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
    FILE* fp;
    char* save;
    cJSON* jsonInventory;
    cJSON* jsonItem;
    size_t fileSize;
    char i = 0;

    fp = fopen(INVENTORY_SAVE_FILE, "r");
    if(fp == NULL) return 2;

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);

    save = malloc((fileSize + 1) * sizeof(char));
    if(save == NULL) return FAILURE;
    fread(save, fileSize, 1, fp);
    save[fileSize] = '\0';


    jsonInventory = cJSON_Parse(save);
    free(save);
    if(jsonInventory == NULL) return FAILURE;

    cJSON_ArrayForEach(jsonItem, jsonInventory){
        cJSON* id = cJSON_GetObjectItemCaseSensitive(jsonItem, "id");
        cJSON* name = cJSON_GetObjectItemCaseSensitive(jsonItem, "name");
        cJSON* quantity = cJSON_GetObjectItemCaseSensitive(jsonItem, "quantity");
        cJSON* sprite = cJSON_GetObjectItemCaseSensitive(jsonItem, "sprite");
        cJSON* type = cJSON_GetObjectItemCaseSensitive(jsonItem, "type");

        affectItem(inventory + i, id->valueint, name->valuestring, quantity->valueint, sprite->valuestring, type->valuestring);
        ++i;
    }

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
        cJSON_AddStringToObject(jsonItem, "name", inventory[i].itemName);
        cJSON_AddNumberToObject(jsonItem, "quantity", inventory[i].quantity);
        cJSON_AddStringToObject(jsonItem, "sprite", inventory[i].sprite);
        cJSON_AddStringToObject(jsonItem, "type", inventory[i].itemType);

        cJSON_AddItemToArray(jsonInventory, jsonItem);
    }
    jsonString = cJSON_Print(jsonInventory);

    return jsonString;
}