//
// PHE Lina & BUSQUET Nicolas
// 07/01/2024
// Création du système de temps in game
//

#ifndef FARMINGCO_TIME_H
#define FARMINGCO_TIME_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "../define.h"
#include "../error/error.h"

int day(void*);
void applyFilter(SDL_Renderer *, int *, SDL_Texture *);

#endif //FARMINGCO_TIME_H
