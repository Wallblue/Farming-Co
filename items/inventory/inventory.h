#ifndef INVENTORY_H
#define INVENTORY_H

    #include <cJSON.h>
    #define INVENTORY_SIZE 30
    #define INVENTORY_SAVE_FILE "inventorySave.json"

//Structures :
    #include "../items.h"

    typedef Item Inventory[INVENTORY_SIZE];

//Functions :
    void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* type, const char* description, unsigned short energyBonus, unsigned char ability, unsigned char growTime, const char* sprite);
    void resetItem(Item* item);
    void initInventory(Inventory inventory);

    short itemFind(int id, const Inventory inventory); //Return a specified Item index in the inventory or -1
    unsigned char itemSet(const Item* item, Inventory inventory); //Set a new item in the inventory and return 0 if it worked 1 else
    unsigned char itemAdd(const Item* item, Inventory inventory); //Add parameter item to existing items or set it if no existing ones
    short firstEmptySlot(const Inventory inventory); //Return the first slot's index without item, -1 if not anymore

    unsigned char itemSubstract(int id, unsigned char quantity, Inventory inventory); //Substract a given quantity of an item

    unsigned char saveInventory(Inventory inventory); //Save inventory into a json file
    unsigned char loadInventory(Inventory inventory);
    char* jsonifyInventory(Inventory inventory); //Convert inventory in a json string
    unsigned char parseJsonFile(const char* fileName, cJSON** dest);
#endif