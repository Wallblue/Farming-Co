#ifndef INVENTORY_H
#define INVENTORY_H

    #define INVENTORY_SIZE 30
    #define MAX_SPRITE_LEN 128
    #define MAX_STR 25
    #define SUCCESS 0
    #define FAILURE 1

//Structures :
    struct Item{
      int id;
      char itemName[MAX_STR];
      unsigned char quantity;
      char sprite[MAX_SPRITE_LEN];
      char itemType[MAX_STR];
    };
    typedef struct Item Item;

    typedef Item Inventory[INVENTORY_SIZE];

//Functions :
    void init_inventory(Inventory inventory);
    void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* sprite, const char* type);

    short item_find(int id, const Inventory inventory); //Return a specified Item index in the inventory or -1
    unsigned char item_set(Item item, Inventory inventory); //Set a new item in the inventory and return 0 if it worked 1 else
    unsigned char item_add(Item item, Inventory inventory); //Add parameter item to existing items or set it if no existing ones
    short firstEmptySlot(const Inventory inventory); //Return the first slot's index without item, -1 if not anymore

    unsigned char item_has(int id, unsigned char quantity, Inventory inventory); //Return true if an item exists at a given quantity in inventory, false else
    unsigned char item_remove(int id, Inventory inventory); // Remove all of a given item in inventory
    unsigned char item_substract(int id, unsigned char quantity, Inventory inventory); //Substract a given quantity of an item

    unsigned char save_inventory(Inventory inventory); //Save inventory into a json file
    char* jsonifyInventory(Inventory inventory); //Convert inventory in a json string
#endif