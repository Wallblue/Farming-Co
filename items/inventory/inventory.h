#ifndef INVENTORY_H
#define INVENTORY_H

    #include <cJSON.h>
    #define INVENTORY_MAX_SIZE 30
    #define INVENTORY_SAVE_FILE "../save/inventorySave.json"

//Structures :
    #include "../items.h"

    typedef Item Inventory[INVENTORY_MAX_SIZE];

//Includes:
    #include "../../define.h"
    #include "../../database/database.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sqlite3.h>

//Functions :
    void initInventory(Inventory inventory);

    short findItem(int id, const Inventory inventory); //Return a specified Item index in the inventory or -1
    unsigned char setItem(int id, unsigned char quantity, Inventory inventory); //Set a new item in the inventory and return 0 if it worked 1 else
    void swapItems(int srcSlot, int destSlot, Inventory inventory);
    unsigned char addItem(int id, unsigned char quantity, Inventory inventory); //Add parameter item to existing items or set it if no existing ones
    short firstEmptySlot(const Inventory inventory); //Return the first slot's index without item, -1 if not anymore

    unsigned char subtractItem(int id, unsigned char quantity, Inventory inventory); //Subtract a given quantity of an item

    unsigned char saveInventory(Inventory inventory); //Save inventory into a json file
    unsigned char loadInventory(Inventory inventory);
    char* jsonifyInventory(Inventory inventory); //Convert inventory in a json string
    unsigned char parseJsonFile(const char* fileName, cJSON** dest);
#endif