#include "inventory.h"


void initInventory(Item inventory[INVENTORY_MAX_SIZE]){
    unsigned char i;
    for(i = 0; i < INVENTORY_MAX_SIZE; i++)
        resetItem(inventory + i);
}

char findItem(int id, const Item inventory[30]){
    char i;
    for(i = 0; i < INVENTORY_MAX_SIZE; i++)
        if(inventory[i].id == id)
            return i;
    return -1;
}

short firstEmptySlot(const Item inventory[INVENTORY_MAX_SIZE]){
    short i;
    for(i = 0; i < INVENTORY_MAX_SIZE; i++)
        if(inventory[i].id == 0 && inventory[i].quantity == 0)
            return i;
    return -1;
}

unsigned char setItem(int id, unsigned char quantity, Inventory* inventory){
    Item item;
    short index = firstEmptySlot(inventory->slots);

    if(index == -1) return FAILURE; //If inventory is full

    getItem(id, &item, NULL);
    affectItem(inventory->slots + index, item.id, item.name, quantity, item.type, item.description, item.energyBonus,
               item.ability, item.growTime, item.sprite, item.objectSpriteRef, 0, item.linkedTool);
    saveNewItem(inventory, index, NULL);
    return SUCCESS;
}

unsigned char addItem(int id, unsigned char quantity, Inventory* inventory){
    char index = findItem(id, inventory->slots);

    if(index == -1) //If item does not exist in this inventory
        return setItem(id, quantity, inventory);
    //but if it does
    inventory->slots[index].quantity += quantity;
    alterItemQuantity(inventory, index, quantity, 0);
    return SUCCESS;
}

unsigned char subtractItem(int id, unsigned char quantity, Inventory* inventory){
    char index = findItem(id, inventory->slots);
    if(index == -1 || inventory->slots[index].quantity < quantity) return FAILURE;

    if(inventory->slots[index].quantity == quantity) {
        if(deleteSavedItem(inventory, index, NULL) == FAILURE) return FAILURE;
        resetItem(inventory->slots + index);
    }
    else {
        if(alterItemQuantity(inventory, index, quantity, 1) == FAILURE) return FAILURE;
        inventory->slots[index].quantity -= quantity;
    }
    return SUCCESS;
}

unsigned char loadInventory(Inventory* inventory){
    sqlite3* db;
    sqlite3_stmt* res;
    int rc, index = -1;

    openDb(&db);
    switch(inventory->ownerType){
        case 1:
            rc = prepareRequest(db, "SELECT item.itemId, name, type, description, energyBonus, ability, growTime, sprite, OBJECT_OWN.quantity, "
                                    "linkedObjectSpriteRef, evolution, linkedTool, OBJECT_OWN.slot FROM item, OBJECT_OWN WHERE OBJECT_OWN.itemId = item.itemId AND OBJECT_OWN.objectId = ?1", &res);
            break;
        case 2:
            rc = prepareRequest(db, "SELECT item.itemId, name, type, description, energyBonus, ability, growTime, sprite, NPC_OWN.quantity, "
                                    "linkedObjectSpriteRef, evolution, linkedTool FROM item, NPC_OWN WHERE NPC_OWN.itemId = item.itemId AND NPC_OWN.objectId = ?1", &res);
            break;
        default:
            rc = prepareRequest(db,"SELECT item.itemId, name, type, description, energyBonus, ability, growTime, sprite, PLAYER_OWN.quantity, "
                                   "linkedObjectSpriteRef, evolution, linkedTool, PLAYER_OWN.slot FROM item, PLAYER_OWN WHERE PLAYER_OWN.itemId = item.itemId AND PLAYER_OWN.playerId = ?1", &res);
            break;
    }

    if(rc == FAILURE){
        sqlite3_finalize(res);
        sqlite3_close(db);
        fprintf(stderr, "Error : %s\n", sqlite3_errmsg(db));
        return FAILURE;
    }

    sqlite3_bind_int(res, 1, inventory->ownerId);
    while(sqlite3_step(res) == SQLITE_ROW){
        if(inventory->ownerType != 2) index = sqlite3_column_int(res, 12);
        else ++index;
        affectItem(inventory->slots + index, sqlite3_column_int(res, 0), (char*)sqlite3_column_text(res, 1), sqlite3_column_int(res, 8), (char*)sqlite3_column_text(res, 2),
                   (char*)sqlite3_column_text(res, 3), sqlite3_column_int(res, 4), sqlite3_column_int(res, 5), sqlite3_column_int(res, 6),
                   (char*)sqlite3_column_text(res, 7), *sqlite3_column_text(res, 9), sqlite3_column_int(res, 10), sqlite3_column_int(res, 11));
    }
    sqlite3_finalize(res);
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

unsigned char swapInventoryItems(Inventory* srcInventory, char srcSlot, Inventory* destInventory, char destSlot){
    sqlite3* db;
    if(openDb(&db) == FAILURE) return FAILURE;

    if(destInventory->slots[destSlot].id != 0 && srcInventory->slots[srcSlot].id == destInventory->slots[destSlot].id){
        if(deleteSavedItem(srcInventory, srcSlot, db) == FAILURE)
            return returnProperly(db, NULL, FAILURE);
        if(alterItemQuantity(destInventory, destSlot, srcInventory->slots[srcSlot].quantity, 0) == FAILURE)
            return returnProperly(db, NULL, FAILURE);
        resetItem(srcInventory->slots + srcSlot);

    } else if(srcInventory == destInventory) {
        if(updateItemSlot(srcInventory, srcSlot, destSlot, db) == FAILURE) return returnProperly(db, NULL, FAILURE);

        if(destInventory->slots[destSlot].id != 0)
            if(updateItemSlot(destInventory, destSlot, srcSlot, db) == FAILURE) return returnProperly(db, NULL, FAILURE);

        swapItems(srcInventory->slots + srcSlot, destInventory->slots + destSlot);
    }else {
        if(deleteSavedItem(srcInventory, srcSlot, db) == FAILURE) return returnProperly(db, NULL, FAILURE);
        if(destInventory->slots[destSlot].id != 0)
            if(deleteSavedItem(destInventory, destSlot, db) == FAILURE) return returnProperly(db, NULL, FAILURE);

        swapItems(srcInventory->slots + srcSlot, destInventory->slots + destSlot);

        if(srcInventory->slots[srcSlot].id != 0)
            if(saveNewItem(srcInventory, srcSlot, db) == FAILURE) return returnProperly(db, NULL, FAILURE);
        if(saveNewItem(destInventory, destSlot, db) == FAILURE) return returnProperly(db, NULL, FAILURE);
    }

    sqlite3_close(db);
    return SUCCESS;
}