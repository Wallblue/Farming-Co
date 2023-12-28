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

void moveLeft(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab);
void moveRight(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab);
void moveUp(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab);
void moveDown(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab);

#endif //FARMINGCO_PLAYER_H
