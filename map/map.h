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
extern char* mapBg[];
extern char* mapFg[];

void printMap(SDL_Renderer *, SDL_Texture *, char **);
#endif //FARMINGCO_MAP_H
