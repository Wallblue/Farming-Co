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
        "//{}~}///////////////////",
        "/}//////x/////////|xyy///",
        "///////////////////Uyz///",
        "/////////////////{/z/xV//",
        "///////////////////////~/",
        "//////////////////_{/////",
        "///////{_////////////////",
        "/////z///{y//////////////",
        "////~//////xz////////////",
        "/////////////////////////",
        "/////////////////////{}//",
        "///////////////////xy|/_",
        "///////////////////Uzy///",
        "/////////////////////////",
        "////y}///////////////////",
        "///zy|x//////////////////",
        "///yz////////////////////",
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
unsigned char** npcMap;

//Création de la localisation des NPC
int npc1loc1[] = NPC1_LOC1;
int npc1loc2[] = NPC1_LOC2;
int npc1loc3[] = NPC1_LOC3;

npcLocation npc1 = {npc1loc1, npc1loc2, npc1loc3};

int npc2loc1[] = NPC2_LOC1;
int npc2loc2[] = NPC2_LOC2;
int npc2loc3[] = NPC2_LOC3;

npcLocation npc2 = {npc2loc1, npc2loc2, npc2loc3};

int npc3loc1[] = NPC3_LOC1;
int npc3loc2[] = NPC3_LOC2;
int npc3loc3[] = NPC3_LOC3;

npcLocation npc3 = {npc3loc1, npc3loc2, npc3loc3};

int npc4loc1[] = NPC4_LOC1;
int npc4loc2[] = NPC4_LOC2;
int npc4loc3[] = NPC4_LOC3;

npcLocation npc4 = {npc4loc1, npc4loc2, npc4loc3};

int npc5loc1[] = NPC5_LOC1;
int npc5loc2[] = NPC5_LOC2;
int npc5loc3[] = NPC5_LOC3;

npcLocation npc5 = {npc5loc1, npc5loc2, npc5loc3};



//affichage d'une mapdz
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
    npcMap = malloc(mapHeight * sizeof(char *));
    if(npcMap == NULL) return FAILURE;

    char* defaultLine = "//////////////////////////";

    for (int i = 0; i < mapHeight; i++){
        if(initLine(mapObjects1 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(mapObjects2 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(mapObjects3 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(mapObjects4 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(homeObjects + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(soiledFloor3 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(soiledFloor4 + i, defaultLine) == FAILURE) return FAILURE;
        if(initLine(npcMap + i, defaultLine) == FAILURE) return FAILURE;
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
    free(npcMap);
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

unsigned char deleteObjectByCoordinates(int x, int y, char zone, sqlite3* db){
    sqlite3_stmt* res;
    int rc;
    char homemadeDb = 0;

    if(db == NULL) {
        if (openDb(&db) == FAILURE) return FAILURE;
        homemadeDb = 1;
    }
    if(prepareRequest(db, "DELETE FROM object WHERE x = ?1 AND y = ?2 AND zone = ?3", &res) == FAILURE){
        sqlite3_close(db);
        return FAILURE;
    }
    sqlite3_bind_int(res, 1, x);
    sqlite3_bind_int(res, 2, y);
    sqlite3_bind_int(res, 3, zone);
    rc = sqlite3_step(res);
    sqlite3_finalize(res);

    if(rc != SQLITE_DONE){
        fprintf(stderr, "Erreur : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return FAILURE;
    }

    if(homemadeDb == 1) sqlite3_close(db);

    return SUCCESS;
}

void updateSoil(){
    int i;
    int j;
    for(i = 0; i < mapHeight; i++){
        for(j = 0; j < mapWidth; j++){
            if(mapObjects3[i][j] == '/' && soiledFloor3[i][j] != '/')
                soiledFloor3[i][j] = '/';
            if(mapObjects4[i][j] == '/' && soiledFloor4[i][j] != '/')
                soiledFloor4[i][j] = '/';
        }
    }
}

unsigned char updateMisc(int todayDate){
    int i;
    int j;
    int count = 0;
    int id;
    Object newObject;
    for(i = 0; i<mapHeight; i++){
        for(j = 0; j < mapWidth; j++){
            if(mapObjects1[i][j] == 'X' || mapObjects1[i][j] == 'Y' || mapObjects1[i][j] == 'Z' || mapObjects1[i][j] == '[') count++;
        }
    }

    if(count < 15){
        srand(time(NULL));
        int randomNum = rand() % 15 + 1;
        int randomMisc;
        int randomHeight;
        int randomWidth;

        while(randomNum != 0) {
            randomMisc = rand() % 3;

            randomHeight = rand() % mapHeight;
            randomWidth = rand() % mapWidth;

            while (!(mapObjects1[randomHeight][randomWidth] == '/' && firstZoneFg[randomHeight][randomWidth] == '/' && houseRoof[randomHeight][randomWidth] == '/')){
                randomHeight = rand() % mapHeight;
                randomWidth = rand() % mapWidth;
            }

            unsigned char randomChar = 'X' + randomMisc;
            mapObjects1[randomHeight][randomWidth] = randomChar;

            switch(randomChar){
                case 'X':
                    id = 2;
                    break;
                case 'Y':
                    id = 3;
                    break;
                case 'Z':
                    id = 4;
                    break;
                case '[':
                    id = 5;
                    break;
            }


            affectObject(&newObject, randomWidth, randomHeight, 0, 0, todayDate,id);
            if (saveObject(&newObject) == FAILURE) return FAILURE;

            randomNum--;
        }
    }

    return SUCCESS;
}



void updateNPC(){
    int i;
    int j;

    for(i = 0; i<mapHeight; i++){
        for(j = 0; j<mapWidth; j++){
            if(npcMap[i][j] != '/')npcMap[i][j] = '/';
        }
    }

    srand(time(NULL));
    int npc1Loc = rand() % 3;
    int npc2Loc = rand() % 3;
    int npc3Loc = rand() % 3;
    int npc4Loc = rand() % 3;
    int npc5Loc = rand() % 3;

    npcMap[npc1.loc[npc1Loc][0]][npc1.loc[npc1Loc][1]] = '0';
    npcMap[npc2.loc[npc2Loc][0]][npc2.loc[npc2Loc][1]] = '1';
    npcMap[npc3.loc[npc3Loc][0]][npc3.loc[npc3Loc][1]] = '2';
    npcMap[npc4.loc[npc4Loc][0]][npc4.loc[npc4Loc][1]] = '3';
    npcMap[npc5.loc[npc5Loc][0]][npc5.loc[npc5Loc][1]] = '4';
}