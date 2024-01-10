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

//références les deux tableaux qui sont utilisé sur le main (pour l'instant possible que ça change)
extern char* firstZoneBg[];
extern char* firstZoneFg[];
extern char* secondZoneBg[];
extern char* secondZoneFg[];
extern char* thirdZoneBg[];
extern char* thirdZoneFg[];
extern char* fourthZoneBg[];
extern char* fourthZoneFg[];
extern char** mapObjects1;
extern char** mapObjects2;
extern char** mapObjects3;
extern char** mapObjects4;


void printMap(SDL_Renderer *, SDL_Texture *, char **);
void inputObject(int, int, char**);
void initObjectMap(char*** map);
void initObjectMaps();
void freeObjectMaps();

#endif //FARMINGCO_MAP_H
