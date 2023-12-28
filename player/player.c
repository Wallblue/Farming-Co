//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion du joueur
//

#include "player.h"

void moveLeft(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab) {
    playerSrc->y = 112;
    if (*countX - 1 >= 0 && tab[*countY][*countX - 1] == '/') {
        playerDst->x -= 32;
        (*countX)--;
    }
}

void moveRight(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab) {
    playerSrc->y = 160;
    if(*countX+1 <= 24 && tab[*countY][*countX+1] == '/') {
        playerDst->x += 32;
        (*countX)++;
    }
}

void moveUp(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab) {
    playerSrc->y = 64;
    if(*countY-1 >= 0 && tab[*countY-1][*countX] == '/') {
        playerDst->y -= 32;
        (*countY)--;
    }
}

void moveDown(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab) {
    playerSrc->y = 16;
    if(*countY+1 <= 19 && tab[*countY+1][*countX] == '/') {
        playerDst->y += 32;
        (*countY)++;
    }
}
