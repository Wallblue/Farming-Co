//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion du joueur
//

#include "player.h"

void moveLeft(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, int *zone) {
    playerSrc->y = 112;

    switch(*zone) {
        case 0:
            if (*countX - 1 >= 0 && tab[*countY][*countX - 1] == '/') {
                playerDst->x -= 32;
                (*countX)--;
            }
            break;
        case 1:
            if (*countX - 1 >= 0) {
                playerDst->x -= 32;
                (*countX)--;
            }

            if (*countX == 0) {
                playerDst->x = 768;
                *countX = 24;
                *zone = 0;
            }
            break;
    }
}

void moveRight(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, int *zone) {
    playerSrc->y = 160;

    switch(*zone){
        case 0:
            if(tab[*countY][*countX+1] == '/') {
                playerDst->x += 32;
                (*countX)++;
            }

            if(*countX == 25) {
                playerDst->x = 0;
                *countX = 0;
                *zone = 1;
            }

            break;
        case 1 :
            if(*countX+1 <= 24) {
                playerDst->x += 32;
                (*countX)++;
            }
            break;
    }


}

void moveUp(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, int *zone) {
    playerSrc->y = 64;
    if(*countY-1 >= 0 && tab[*countY-1][*countX] == '/') {
        playerDst->y -= 32;
        (*countY)--;
    }
}

void moveDown(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, int *zone) {
    playerSrc->y = 16;
    if(*countY+1 <= 19 && tab[*countY+1][*countX] == '/') {
        playerDst->y += 32;
        (*countY)++;
    }
}
