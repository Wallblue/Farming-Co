#ifndef ITEMS_H
#define ITEMS_H

#define MAX_SPRITE_LEN 128
#define MAX_STR 25
#define DESC_MAX 255

#include <sqlite3.h>

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
    unsigned char objectSpriteRef;
    int evolution;
};
typedef struct Item Item;

//Functions :
void affectItem(Item *item, int id, const char *name, unsigned char quantity, const char *type, const char *description,
                unsigned short energyBonus, unsigned char ability, unsigned char growTime, const char *sprite,
                unsigned char objectSpriteRef, int evolution);
void resetItem(Item* item);
unsigned char addItemsToDatabase();
unsigned char getItem(int id, Item* dest, sqlite3* db);
void printItem(const Item* item);

#endif