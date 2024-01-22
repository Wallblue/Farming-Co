#ifndef INVENTORY_H
#define INVENTORY_H

    #include <cJSON.h>
    #define INVENTORY_MAX_SIZE 30

//Structures :

    #include "../items.h"
    struct Inventory{ //Owner type : 1 - Object | 2 - NPC | else Player
        char ownerType;
        int ownerId;
        Item slots[INVENTORY_MAX_SIZE];
    };
    typedef struct Inventory Inventory;

//Includes:
    #include "../../define.h"
    #include "../../database/database.h"
    #include "../../save/save.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sqlite3.h>

//Functions :
    void initInventory(Item inventory[INVENTORY_MAX_SIZE]);

    char findItem(int id, const Item inventory[30]); //Return a specified Item index in the inventory or -1
    unsigned char setItem(int id, unsigned char quantity, Inventory* inventory); //Set a new item in the inventory and return 0 if it worked 1 else
    unsigned char addItem(int id, unsigned char quantity, Inventory* inventory); //Add parameter item to existing items or set it if no existing ones
    short firstEmptySlot(const Item inventory[INVENTORY_MAX_SIZE]); //Return the first slot's index without item, -1 if not anymore
    unsigned char subtractItem(int id, unsigned char quantity, Inventory* inventory); //Subtract a given quantity of an item

    unsigned char loadInventory(Inventory* inventory);
    unsigned char parseJsonFile(const char* fileName, cJSON** dest);
    unsigned char swapInventoryItems(Inventory* srcInventory, char srcSlot, Inventory* destInventory, char destSlot);
#endif