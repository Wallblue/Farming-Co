//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion de la map
//

#ifndef FARMINGCO_MAP_H
#define FARMINGCO_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "../define.h"
#include "../error/error.h"
#include "../save/save.h"
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

void printMap(SDL_Renderer *, SDL_Texture *, char **);
void inputObject(int, int, unsigned char**, char **, int, int, Item*);
unsigned char initObjectMaps();
unsigned char initLine(unsigned char** line, const char* defaultLine);
void freeObjectMaps();
unsigned char setObject(int x, int y, int zone, int todayDate, int growTime, unsigned char spriteRef);

#endif //FARMINGCO_MAP_H
