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

char destroyObject(unsigned char nX, unsigned char nY, char zone, unsigned char **objectMap, Inventory inventory, Item *heldItem) {
    sqlite3* db;
    sqlite3_stmt* res;
    int rc, objectLinkedToolAbility, objectLinkedItemId;

    if(openDb(&db) == FAILURE) return FAILURE;

    //if it's a bed (because takes 2 tiles)
    if(objectMap[nY][nX] == 'J' || objectMap[nY][nX] == 'I') {
        objectMap[nY + 1][nX] = '/';
        objectMap[nY][nX] = '/';
    }
    if(objectMap[nY][nX] == 'T' || objectMap[nY][nX] == 'S') {
        objectMap[nY - 1][nX] = '/';
        objectMap[nY][nX] = '/';
        nY--;
    }


    if(prepareRequest(db,
                      "SELECT tool.ability, objectItem.itemId, objectItem.evolution, object.state FROM object, item as objectItem, item as tool "
                      "WHERE object.x = ?1 AND object.y = ?2 AND object.zone = ?3 AND object.itemId = objectItem.itemId "
                      "AND objectItem.linkedTool = tool.itemId;",
                      &res) == FAILURE){
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, nX);
    sqlite3_bind_int(res, 2, nY);
    sqlite3_bind_int(res, 3, zone);
    rc = sqlite3_step(res);
    if(rc != SQLITE_ROW){
        sqlite3_close(db);
        sqlite3_finalize(res);
        fprintf(stderr, "Can't get tool. %s", sqlite3_errmsg(db));
        return FAILURE;
    }
    objectLinkedToolAbility = sqlite3_column_int(res, 0);
    objectLinkedItemId = sqlite3_column_int(res, 1);

    if(objectLinkedToolAbility != heldItem->ability) {
        sqlite3_close(db);
        sqlite3_finalize(res);
        return 2;
    }

    rc = 0;
    if(sqlite3_column_int(res, 3) == 3) { //This is for the cultivated loot
        if (addItem(sqlite3_column_int(res, 2), rand() % 4 + 1, inventory) == FAILURE) rc = 3;
        if (addItem(objectLinkedItemId, rand() % 3 + 1, inventory) == FAILURE) rc = 3;
    }else //Classic loot
        if(addItem(objectLinkedItemId, 1, inventory) == FAILURE) rc = 3;

    sqlite3_finalize(res);
    if(rc == 3){
        sqlite3_close(db);
        return 3;
    }

    if(deleteObjectByCoordinates(nX, nY, zone, db) == FAILURE) return FAILURE;

    if(objectMap[nY][nX] == 'J' || objectMap[nY][nX] == 'I')
        objectMap[nY+1][nX] = '/';
    objectMap[nY][nX] = '/';

    sqlite3_close(db);
    return SUCCESS;
}

unsigned char inputObject(int xMouse, int yMouse, unsigned char** tab, char **mapFg, unsigned char **soiledFloor, char zone, int todayDate, Item* heldItem, Inventory inventory){
    Object newObject;
    char success = 0;
    yMouse = yMouse/32;
    xMouse = xMouse/32;

    switch(zone){
        case 0:
            if(houseRoof[yMouse][xMouse] == '/' && strcmp(heldItem->type, "furn") != 0 && strcmp(heldItem->type, "crops") != 0) {
                tab[yMouse][xMouse] = (char) heldItem->objectSpriteRef;
                success = 1;
            }
            break;
        case 2: case 3:
            if(strcmp(heldItem->type, "furn") != 0 && soiledFloor[yMouse][xMouse] != '/') {
                tab[yMouse][xMouse] = (char) heldItem->objectSpriteRef;
                success = 1;
            }
            break;
        case 4:
            if(yMouse >= 5 && yMouse<=13 && xMouse >= 4 && xMouse <= 20 && strcmp(heldItem->type, "crops") != 0) {
                tab[yMouse][xMouse] = (char) heldItem->objectSpriteRef;
                if((char) heldItem->objectSpriteRef == 'J' || (char) heldItem->objectSpriteRef == 'I') {
                    if (yMouse + 1 == 14) {
                        tab[yMouse][xMouse] = '/';
                        success = 0;
                        break;
                    }
                    tab[yMouse + 1][xMouse] = (char) heldItem->objectSpriteRef + 10;
                }
                success = 1;
            }
            break;
        default:
            if(mapFg[yMouse][xMouse] == '/'  && strcmp(heldItem->type, "furn") != 0 && strcmp(heldItem->type, "crops") != 0) {
                tab[yMouse][xMouse] = (char) heldItem->objectSpriteRef;
                success = 1;
            }
            break;
    }

    if(success == 1) {
        affectObject(&newObject, xMouse, yMouse, zone, heldItem->growTime, todayDate,
                     heldItem->id);
        if (saveObject(&newObject) == FAILURE) return FAILURE;

        subtractItem(heldItem->id, 1, inventory);
    }

    return SUCCESS;
}

void soilFloor(unsigned char nX, unsigned char nY, unsigned char **tab, Item *heldItem){
    srand(time(NULL));
    int randomNum = rand() % 3;

    if(heldItem->id == 7)
        tab[nY][nX] = 'A' + randomNum;
}