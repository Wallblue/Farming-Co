#include "inventory.h"


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

unsigned char setItem(int id, unsigned char quantity, Inventory inventory){
    Item item;
    short index = firstEmptySlot(inventory);

    if(index == -1) return FAILURE; //If inventory is full

    getItem(id, &item, NULL);
    affectItem(inventory + index, item.id, item.name, quantity, item.type, item.description, item.energyBonus, item.ability, item.growTime, item.sprite, item.objectSpriteRef, item.linkedTool);
    return SUCCESS;
}

unsigned char addItem(int id, unsigned char quantity, Inventory inventory){
    short index = findItem(id, inventory);

    if(index == -1) //If item does not exist in this inventory
        return setItem(id, quantity, inventory);
    //but if it does
    inventory[index].quantity += quantity;
    return SUCCESS;
}

unsigned char subtractItem(int id, unsigned char quantity, Inventory inventory){
    short index = findItem(id, inventory);
    if(index == -1 || inventory[index].quantity < quantity) return FAILURE;

    if(inventory[index].quantity == quantity)
        resetItem(inventory + index);
    else
        inventory[index].quantity -= quantity;

    return SUCCESS;
}

void swapItems(int srcSlot, int destSlot, Inventory inventory){
    Item temp;
    affectItem(&temp, inventory[srcSlot].id, inventory[srcSlot].name, inventory[srcSlot].quantity, inventory[srcSlot].type, inventory[srcSlot].description, inventory[srcSlot].energyBonus, inventory[srcSlot].ability, inventory[srcSlot].growTime, inventory[srcSlot].sprite, inventory[srcSlot].objectSpriteRef, inventory[srcSlot].linkedTool);
    affectItem(inventory + srcSlot, inventory[destSlot].id, inventory[destSlot].name, inventory[destSlot].quantity, inventory[destSlot].type, inventory[destSlot].description, inventory[destSlot].energyBonus, inventory[destSlot].ability, inventory[destSlot].growTime, inventory[destSlot].sprite, inventory[destSlot].objectSpriteRef, inventory[destSlot].linkedTool);
    affectItem(inventory + destSlot, temp.id, temp.name, temp.quantity, temp.type, temp.description, temp.energyBonus, temp.ability, temp.growTime, temp.sprite, temp.objectSpriteRef, temp.linkedTool);
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
    unsigned char i;

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
        if(getItem(id->valueint, inventory + i, db) == FAILURE) return FAILURE;
        inventory[i].quantity = quantity->valueint;

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