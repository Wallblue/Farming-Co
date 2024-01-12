#ifndef ITEMS_H
#define ITEMS_H

#define MAX_SPRITE_LEN 128
#define MAX_STR 25
#define DESC_MAX 255

struct Item{
    int id;
    char name[MAX_STR];
    char type[MAX_STR];
    char description[DESC_MAX];
    unsigned short energyBonus;
    unsigned char ability;
    unsigned char growTime;
    char sprite[MAX_SPRITE_LEN];
    unsigned char quantity;
};
typedef struct Item Item;

//Functions :
void affectItem(Item* item, int id, const char* name, unsigned char quantity, const char* type, const char* description, unsigned short energyBonus, unsigned char ability, unsigned char growTime, const char* sprite);
void resetItem(Item* item);
unsigned char addItemsToDatabase();
void printItem(const Item* item);
unsigned char dejsonifyItems();

#endif