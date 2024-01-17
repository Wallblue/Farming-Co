//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion de la map
//

#ifndef FARMINGCO_MAP_H
#define FARMINGCO_MAP_H

#include <SDL.h>
#include "../items/items.h"

//références les deux tableaux qui sont utilisé sur le main (pour l'instant possible que ça change)
extern char* firstZoneBg[];
extern char* firstZoneFg[];
extern char* secondZoneBg[];
extern char* secondZoneFg[];
extern char* thirdZoneBg[];
extern char* thirdZoneFg[];
extern char* fourthZoneBg[];
extern char* fourthZoneFg[];
extern char* houseRoof[];
extern char* home[];
extern char* homeFg[];
extern unsigned char** mapObjects1;
extern unsigned char** mapObjects2;
extern unsigned char** mapObjects3;
extern unsigned char** mapObjects4;
extern unsigned char** homeObjects;

//Structures
struct Object{
    int x;
    int y;
    char zone;
    unsigned char growTime;
    int growDate;
    char state;
    unsigned char boosted;
    int itemId;
};
typedef struct Object Object;

//Functions
void printMap(SDL_Renderer *, SDL_Texture *, char **);
unsigned char inputObject(int xMouse, int yMouse, unsigned char **tab, char **mapFg, char zone, int todayDate, Item *heldItem);
unsigned char initObjectMaps();
unsigned char initLine(unsigned char** line, const char* defaultLine);
void freeObjectMaps();
void affectObject(Object* object, int x, int y, char zone, int growTime, int growDate, int itemId);

#endif //FARMINGCO_MAP_H
