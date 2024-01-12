//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion du joueur
//


#ifndef FARMINGCO_PLAYER_H
#define FARMINGCO_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "../define.h"
#include "../error/error.h"

void moveLeft(SDL_Rect *, SDL_Rect *, int *, const int *, char **, char **, int*);
void moveRight(SDL_Rect *, SDL_Rect *, int *, const int *, char **, char **, int*);
void moveUp(SDL_Rect *, SDL_Rect *, const int *, int *, char **, char **, int*);
void moveDown(SDL_Rect *, SDL_Rect *, const int *, int *, char **, char **, int*);

#endif //FARMINGCO_PLAYER_H
