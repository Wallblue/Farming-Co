//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion de la map
//

#include "map.h"
#include "../save/save.h"
#include "../define.h"
#include <stdio.h>
#include <stdlib.h>

//Création de la templates des maps qui  fonctionnent avec les caractères ascii & l'utilisation
//d'une tilemap (ici tilemap de 16x16) -> se référencer sur map/sheets/utilisationSheets.txt

//fond de la map (herbe)
char* firstZoneBg[] = {
        "0010:;0<1?0010:;0<1?0010:",
        "2:;0:205141001423:<10:<??",
        "100142300:;0<1?10:2050:<?",
        "000142312311302;0<1?10:<?",
        ":;10:<?0<101231130200010?",
        "2311:205141001423:<:2051?",
        "1001423311:20511231130200",
        "001231130200:;0<1?10:2051",
        "00:;0<1?101310:<?0200:;0<",
        "331231:;0000:;0<1?1302000",
        "423311:310:<?00:<?0200205",
        "331?10111:2051<1?10:20511",
        "<1?10:20500:;0<?10:21?101",
        "00:;0<000:;0<?10:21?11?10",
        "310:<?023311:20511003311:",
        "002?10:2:;0:20514:;0<1?10",
        "00:;0<1?2:;0:20:205<51410",
        "3311:205<1?10:20511:;0<1?",
        "00:;0<1?10310:<?02:;0<100",
        "0000:;0<1?:205<10:;0<1?10",
};

char* secondZoneBg[] = {
        "331?10111:2051<1?10:20511",
        "<1?10:20500:;0<?10:21?101",
        "00:;0<000:;0<?10:21?11?10",
        "310:<?023311:20511003311:",
        "002?10:2:;0:20514:;0<1?10",
        "00:;0<1?2:;0:20:205<51410",
        "3311:205<1?10:20511:;0<1?",
        "00:;0<1?10310:<?02:;0<100",
        "2311:205141001423:<:2051?",
        "1001423311:20511231130200",
        "001231130200:;0<1?10:2051",
        "331231:;0000:;0<1?1302000",
        "423311:310:<?00:<?0200205",
        "331?10111:2051<1?10:20511",
        "<1?10:20500:;0<?10:21?101",
        "0010:;0<1?0010:;0<1?0010:",
        "2311:205141001423:<:2051?",
        "1001423311:20511231130200",
        "001231130200:;0<1?10:2051",
        "00:;0<1?101310:<?0200:;0<",
};

char* thirdZoneBg[] = {
    "<1?10:20500:;0<?10:21?101",
    "001231130200:;0<1?10:2051",
    "00:;0<1?101310:<?0200:;0<",
    "331231:;0000:;0<1?1302000",
    "423311:310:<?00:<?0200205",
    "0010:;0<1?0010:;0<1?0010:",
    "2311:205141001423:<:2051?",
    "1001423311:20511231130200",
    "001231130200:;0<1?10:2051",
    "00:;0<1?101310:<?0200:;0<",
    "423311:310:<?00:<?0200205",
    "0000:;0<1?:205<10:;0<1?10",
    "000142312311302;0<1?10:<?",
    ":;10:<?0<101231130200010?",
    "00:;0<1?10310:<?02:;0<100",
    "2311:205141001423:<:2051?",
    "1001423311:20511231130200",
    "001231130200:;0<1?10:2051",
    "2311:205141001423:<:2051?",
    "1001423311:20511231130200",
};

char* fourthZoneBg[] = {
        "001231130200:;0<1?10:2051",
        "2311:205141001423:<:2051?",
        "000142312311302;0<1?10:<?",
        "423311:310:<?00:<?0200205",
        "331?10111:2051<1?10:20511",
        "423311:310:<?00:<?0200205",
        ":;10:<?0<101231130200010?",
        "00:;0<1?10310:<?02:;0<100",
        "001231130200:;0<1?10:2051",
        "00:;0<1?101310:<?0200:;0<",
        "0010:;0<1?0010:;0<1?0010:",
        "2:;0:205141001423:<10:<??",
        "100142300:;0<1?10:2050:<?",
        "2311:205141001423:<:2051?",
        "1001423311:20511231130200",
        "423311:310:<?00:<?0200205",
        "331231:;0000:;0<1?1302000",
        "0010:;0<1?0010:;0<1?0010:",
        "2311:205141001423:<:2051?",
        "1001423311:20511231130200",
};

char* home[] = {
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@YYYYYYYYYYYYYYYYY@@@@",
        "@@@@@@@@@@@@Y@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@@@@@@@@@@@@@@@@@@@@@@@@@",

};

//objets sur la map (ex: eau)
char* firstZoneFg[] = {
        "/////////////////////////",
        "//////////////NOOOOOOOP//",
        "//////////////X///////Z//",
        "//////////////X///////Z//",
        "//////////////bcccQcccd//",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "////6666/////////////////",
        "///6666666///////////////",
        "//666666666//////////////",
        "///6666666///////////////",
        "////66666////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",

};

char* houseRoof[] = {
        "//////////////HIIIIIIIJ//",
        "//////////////RSSSSSSST//",
        "//////////////\\]]]]]]]^//",
        "//////////////fgggggggh//",
        "//////////////pqqqqqqqr//",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
};

char* secondZoneFg[] = {
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
};

char* homeFg[] = {
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "@@@@@@@@@@@@/@@@@@@@@@@@@",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
};

char* thirdZoneFg[] = {
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
};

char* fourthZoneFg[] = {
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
};

unsigned char** mapObjects1;
unsigned char** mapObjects2;
unsigned char** mapObjects3;
unsigned char** mapObjects4;
unsigned char** homeObjects;
unsigned char** soiledFloor3;
unsigned char** soiledFloor4;

unsigned char inputObject(int xMouse, int yMouse, unsigned char** tab, char **mapFg, unsigned char **soiledFloor, char zone, int todayDate, Item* heldItem, Inventory inventory){
    Object newObject;
    char success = 0;
    yMouse = yMouse/32;
    xMouse = xMouse/32;

    switch(zone){
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
        case 2: case 3:
            if(houseRoof[yMouse][xMouse] == '/' && strcmp(heldItem->type, "furn") != 0 && soiledFloor[yMouse][xMouse] != '/') {
                tab[yMouse][xMouse] = (char) heldItem->objectSpriteRef;
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

//affichage d'une map
void printMap(SDL_Renderer *renderer, SDL_Texture *tileset, char **tab) {
    SDL_Rect dst;
    SDL_Rect src;
    src.w = tileHeightWidth;
    dst.w = dstHeightWidth;
    src.h = tileHeightWidth;
    dst.h = dstHeightWidth;

    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            dst.x = i * dstHeightWidth;
            dst.y = j * dstHeightWidth;
            src.x = (tab[j][i] - '0')%10 * tileHeightWidth;
            src.y = (tab[j][i] - '0')/10 * tileHeightWidth;
            SDL_RenderCopy(renderer, tileset, &src, &dst);
        }
    }
}

unsigned char initObjectMaps(){
    mapObjects1 = malloc(mapHeight * sizeof(char *));
    if(mapObjects1 == NULL) return FAILURE;
    mapObjects2 = malloc(mapHeight * sizeof(char *));
    if(mapObjects2 == NULL) return FAILURE;
    mapObjects3 = malloc(mapHeight * sizeof(char *));
    if(mapObjects3 == NULL) return FAILURE;
    mapObjects4 = malloc(mapHeight * sizeof(char *));
    if(mapObjects4 == NULL) return FAILURE;
    homeObjects = malloc(mapHeight * sizeof(char *));
    if(homeObjects == NULL) return FAILURE;
    soiledFloor3 = malloc(mapHeight * sizeof(char *));
    if(soiledFloor3 == NULL) return FAILURE;
    soiledFloor4 = malloc(mapHeight * sizeof(char *));
    if(soiledFloor4 == NULL) return FAILURE;

    char* defaultLine = "//////////////////////////";

    for (int i = 0; i < mapHeight; i++){
        if(initLine(mapObjects1 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(mapObjects2 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(mapObjects3 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(mapObjects4 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(homeObjects + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(soiledFloor3 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(soiledFloor4 + i, defaultLine) == FAILURE) return FAILURE;
    }

    return SUCCESS;
}

unsigned char initLine(unsigned char** line, const char* defaultLine){
    *line = malloc(mapWidth * sizeof(char));
    if(*line == NULL) return FAILURE;
    strcpy((char*)*line, defaultLine);

    return SUCCESS;
}

void freeObjectMaps(){
    free(mapObjects1);
    free(mapObjects2);
    free(mapObjects3);
    free(mapObjects4);
    free(homeObjects);
    free(soiledFloor3);
    free(soiledFloor4);
}

void affectObject(Object* object, int x, int y, char zone, int growTime, int poseDate, int itemId){
    object->x = x;
    object->y = y;
    object->zone = zone;
    object->growTime = growTime;
    object->poseDate = poseDate;
    object->state = 0;
    object->boosted = 0;
    object->itemId = itemId;
}