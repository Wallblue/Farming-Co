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

void moveLeft(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, const int *countY, char **tab, unsigned char **objects, char *zone) {
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

void moveRight(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, const int *countY, char **tab, unsigned char **objects, char *zone) {
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

void moveUp(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, unsigned char **objects, char *zone) {
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

void moveDown(SDL_Rect *playerSrc, SDL_Rect *playerDst, int *countX, int *countY, char **tab, unsigned char **objects, char *zone){
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

unsigned char destroyObject(unsigned char nX, unsigned char nY, char zone, unsigned char **objectMap, Inventory inventory, Item *heldItem) {
    sqlite3* db;
    sqlite3_stmt* res;
    int rc, objectLinkedToolAbility, objectLinkedItemId;

    if(openDb(&db) == FAILURE) return FAILURE;

    if(prepareRequest(db, "SELECT tool.ability, objectItem.itemId FROM item as objectItem, item as tool WHERE objectItem.linkedObjectSpriteRef = ?1 AND objectItem.linkedTool = tool.itemId;", &res) == FAILURE){
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_text(res, 1, (char*) objectMap[nY] + nX, 1, NULL);
    rc = sqlite3_step(res);
    if(rc != SQLITE_ROW){
        sqlite3_close(db);
        sqlite3_finalize(res);
        fprintf(stderr, "Can't get tool. %c", objectMap[nY][nX]);
        return FAILURE;
    }
    objectLinkedToolAbility = sqlite3_column_int(res, 0);
    objectLinkedItemId = sqlite3_column_int(res, 1);
    sqlite3_finalize(res);

    if(objectLinkedToolAbility != heldItem->ability) return 2;
    if(addItem(objectLinkedItemId, 1, inventory) == FAILURE) return 3;

    //delete object in db if()
    if(prepareRequest(db, "DELETE FROM object WHERE x = ?1 AND y = ?2 AND zone = ?3", &res) == FAILURE){
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, nX);
    sqlite3_bind_int(res, 2, nY);
    sqlite3_bind_int(res, 3, zone);
    rc = sqlite3_step(res);
    if(rc != SQLITE_DONE){
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }
    objectMap[nY][nX] = '/';

    sqlite3_finalize(res);
    sqlite3_close(db);
    return SUCCESS;
}
