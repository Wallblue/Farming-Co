#include "inventory.h"
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include <string.h>

void init_inventory(Inventory inventory){
    unsigned char i;
    for(i = 0; i < INVENTORY_SIZE; i++){
        inventory[i].id = 0;
        strcpy(inventory[i].itemName, "");
        inventory[i].quantity = 0;
        strcpy(inventory[i].sprite, "");
        strcpy(inventory[i].itemType, "");
    }
}

short item_find(int id, Inventory inventory){
    short i;
    for(i = 0; i < INVENTORY_SIZE; i++)
        if(inventory[i].id == id)
            return i;
    return -1;
}

unsigned char isInventoryFull(Inventory inventory){
    return 0;
}

unsigned char item_set(Item item, Inventory inventory){
    //if(isInventoryFull(inventory)) return 1;


    return 0;
}

unsigned char save_inventory(Inventory inventory){
    char* jsonString;
    FILE* fp;

    fp = fopen("inventorySave.json", "w");
    if(fp == NULL) {
        fprintf(stderr, "\nError while saving inventory.\n");
        return 1;
    }
    jsonString = jsonifyInventory(inventory);
    fwrite(jsonString, strlen(jsonString), 1, fp);
    fclose(fp);
    free(jsonString);
    return 0;
}

char* jsonifyInventory(Inventory inventory){
    cJSON *jsonInventory = cJSON_CreateArray();
    cJSON *jsonItem;
    char *jsonString;
    int i;

    for(i = 0; i < INVENTORY_SIZE; i++){
        if(inventory[i].quantity == 0) break;
        jsonItem = cJSON_CreateObject();
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