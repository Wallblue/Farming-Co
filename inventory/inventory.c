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

void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* sprite, const char* type){
    item->id = id;
    strcpy(item->itemName, name);
    item->quantity = quantity;
    strcpy(item->sprite, sprite);
    strcpy(item->itemType, type);
}

short item_find(int id, const Inventory inventory){
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

unsigned char item_set(const Item item, Inventory inventory){
    short index = firstEmptySlot(inventory);
    if(index == -1) return FAILURE; //If inventory is full

    affectItem(inventory + index, item.id, item.itemName, item.quantity, item.sprite, item.itemType);
    return SUCCESS;
}

unsigned char item_add(Item item, Inventory inventory){
    short index = item_find(item.id, inventory);
    if(index == -1) //If item does not exist in this inventory
        return item_set(item, inventory);
    //but if it does
    inventory[index].quantity += item.quantity;
    return SUCCESS;
}

unsigned char save_inventory(Inventory inventory){
    char* jsonString;
    FILE* fp;

    fp = fopen("inventorySave.json", "w");
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

char* jsonifyInventory(Inventory inventory){
    cJSON *jsonInventory = cJSON_CreateArray();
    cJSON *jsonItem;
    char *jsonString;
    int i;

    for(i = 0; i < INVENTORY_SIZE; i++){
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