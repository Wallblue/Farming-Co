//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion du joueur
//

#include "player.h"


/*
 * zone 0 = maison
 * zone 1 = magasin
 * zone 2 = ferme des plantes
 * zone 3 = ferme des arbres
 */

void moveLeft(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, const int *countY, char **tab, unsigned char **objects, int *zone) {
    playerSrc->y = 112;

    switch(*zone) {
        case 0:

            if (*countX - 1 >= 0 && tab[*countY][*countX - 1] == '/' && objects[*countY][*countX - 1] == '/' ) {
                playerDst->x -= 32;
                (*countX)--;
            }
            break;

        case 1:

            //magasin -> maison
            if (*countX-1 == -1) {
                playerDst->x = 768;
                *countX = 24;
                *zone = 0;
                break;
            }

            if (tab[*countY][*countX - 1] == '/'  && objects[*countY][*countX - 1] == '/') {
                playerDst->x -= 32;
                (*countX)--;
            }
            break;

        case 2:

            if(*countX-1>= 0 && tab[*countY][*countX-1] == '/'  && objects[*countY][*countX - 1] == '/') {
                playerDst->x -= 32;
                (*countX)--;
            }
            break;

        case 3:
            //arbres -> plantes
            if(*countX-1 == -1){
                playerDst->x = 768;
                *countX = 24;
                *zone = 2;
                break;
            }

            if(*countX-1>= 0 && tab[*countY][*countX-1] == '/'  && objects[*countY][*countX - 1] == '/') {
                playerDst->x -= 32;
                (*countX)--;
            }
            break;
        case 4:
            if(*countX-1>= 4 && tab[*countY][*countX-1] == '/'  && objects[*countY][*countX - 1] == '/') {
                playerDst->x -= 32;
                (*countX)--;
            }
            break;
    }
}

void moveRight(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, const int *countY, char **tab, unsigned char **objects, int *zone) {
    playerSrc->y = 160;

    switch(*zone){
        case 0:

            //maison -> magasin
            if(*countX+1 == 25){
                playerDst->x = 0;
                *countX = 0;
                *zone = 1;
                break;
            }

            if(tab[*countY][*countX+1] == '/' && objects[*countY][*countX+1] == '/') {
                playerDst->x += 32;
                (*countX)++;
            }
            break;

        case 1 :

            if(*countX+1 <= 24 && tab[*countY][*countX+1] == '/' && objects[*countY][*countX+1] == '/') {
                playerDst->x += 32;
                (*countX)++;
            }
            break;

        case 2:

            //plantes -> arbres
            if(*countX+1 == 25){
                playerDst->x = 0;
                *countX = 0;
                *zone = 3;
                break;
            }

            if(*countX+1 <= 24 && tab[*countY][*countX+1] == '/' && objects[*countY][*countX+1] == '/') {
                playerDst->x += 32;
                (*countX)++;
            }
            break;

        case 3:

            if(*countX+1 <= 24 && tab[*countY][*countX+1] == '/' && objects[*countY][*countX+1] == '/') {
                playerDst->x += 32;
                (*countX)++;
            }
            break;

        case 4:
            if(*countX+1 <= 20 && tab[*countY][*countX+1] == '/' && objects[*countY][*countX+1] == '/') {
                playerDst->x += 32;
                (*countX)++;
            }
            break;
    }


}

void moveUp(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, unsigned char **objects, int *zone) {
    playerSrc->y = 64;

    switch (*zone) {
        case 0:

            if(tab[*countY-1][*countX] == 'Q'){
                *zone = 4;
                *countY = 14;
                *countX = 12;
                playerDst->y = 448;
                playerDst->x = 384;
                break;
            }
            if (*countY - 1 >= 0 && tab[*countY - 1][*countX] == '/' && objects[*countY - 1][*countX] == '/' ) {
                playerDst->y -= 32;
                (*countY)--;
            }


            break;

        case 1:

            if (*countY - 1 >= 0 && tab[*countY - 1][*countX] == '/' && objects[*countY - 1][*countX] == '/') {
                playerDst->y -= 32;
                (*countY)--;
            }
            break;

        case 2:

            //plantes -> maison
            if(*countY-1 == -1){
                playerDst->y = 608;
                *countY = 19;
                *zone = 0;
                break;
            }

            if (*countY - 1 >= 0 && tab[*countY - 1][*countX] == '/' && objects[*countY - 1][*countX] == '/') {
                playerDst->y -= 32;
                (*countY)--;
            }


            break;

        case 3:

            //arbres -> magasin
            if(*countY-1 == -1){
                playerDst->y = 608;
                *countY = 19;
                *zone = 1;
                break;
            }

            if (*countY - 1 >= 0 && tab[*countY - 1][*countX] == '/' && objects[*countY - 1][*countX] == '/') {
                playerDst->y -= 32;
                (*countY)--;
            }

            break;

        case 4:
            if (*countY - 1 >= 5 && tab[*countY - 1][*countX] == '/' && objects[*countY - 1][*countX] == '/') {
                playerDst->y -= 32;
                (*countY)--;
            }
            break;
    }
}

void moveDown(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, unsigned char **objects, int *zone){
    playerSrc->y = 16;

    switch(*zone){
        case 0:

            //maison -> plantes
            if(*countY+1 == 20){
                playerDst->y = 0;
                *countY = 0;
                *zone = 2;
                break;
            }
            if(*countY+1 <= 19 && tab[*countY+1][*countX] == '/' && objects[*countY+1][*countX] == '/') {
                playerDst->y += 32;
                (*countY)++;
            }

            break;
        case 1:

            //magasin -> arbres
            if(*countY+1 == 20){
                playerDst->y = 0;
                *countY = 0;
                *zone = 3;
                break;
            }
            if(*countY+1 <= 19 && tab[*countY+1][*countX] == '/' && objects[*countY+1][*countX] == '/') {
                playerDst->y += 32;
                (*countY)++;
            }

            break;

        case 2:
            if(*countY+1 <= 19 && tab[*countY+1][*countX] == '/' && objects[*countY+1][*countX] == '/') {
                playerDst->y += 32;
                (*countY)++;
            }
            break;

        case 3:
            if(*countY+1 <= 19 && tab[*countY+1][*countX] == '/' && objects[*countY+1][*countX] == '/') {
                playerDst->y += 32;
                (*countY)++;
            }
            break;
        case 4:
            if(*countY+1 == 15){
                playerDst->y = 160;
                playerDst->x = 576;
                *countY = 5;
                *countX = 18;
                *zone = 0;
                break;
            }
            if(*countY+1 <= 14 && tab[*countY+1][*countX] == '/' && objects[*countY+1][*countX] == '/') {
                playerDst->y += 32;
                (*countY)++;
            }

            break;
    }

}
