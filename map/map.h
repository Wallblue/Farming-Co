//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion de la map
//

#ifndef FARMINGCO_MAP_H
#define FARMINGCO_MAP_H

//Structures
struct Object{
    int x;
    int y;
    char zone;
    unsigned char growTime;
    int poseDate;
    char state;
    unsigned char boosted;
    int itemId;
};
typedef struct Object Object;

struct npcLocation{
    int* loc[3];
};
typedef struct npcLocation npcLocation;

//Includes
#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include "../items/items.h"
#include "../items/inventory/inventory.h"
#include "../define.h"
#include "../player/player.h"


//références les deux tableaux qui sont utilisé sur le main (pour l'instant possible que ça change)
extern char* firstZoneBg[];
extern char* firstZoneFg[];
extern char* secondZoneBg[];
extern char* secondZoneFg[];
extern char* thirdZoneBg[];
extern char* thirdZoneFg[];
extern char* fourthZoneBg[];
extern char* fourthZoneFg[];
extern char* houseRoof[];
extern char* home[];
extern char* homeFg[];
extern unsigned char** mapObjects1;
extern unsigned char** mapObjects2;
extern unsigned char** mapObjects3;
extern unsigned char** mapObjects4;
extern unsigned char** homeObjects;
extern unsigned char** soiledFloor3;
extern unsigned char** soiledFloor4;
extern unsigned char** npcMap;

extern npcLocation npc1;
extern npcLocation npc2;
extern npcLocation npc3;
extern npcLocation npc4;
extern npcLocation npc5;

//Functions
void printMap(SDL_Renderer *, SDL_Texture *, char **);
unsigned char initObjectMaps();
unsigned char initLine(unsigned char** line, const char* defaultLine);
void freeObjectMaps();
void affectObject(Object* object, int x, int y, char zone, int growTime, int growDate, int itemId);
unsigned char deleteObjectByCoordinates(int x, int y, char zone, sqlite3* db);
int getObjectIdByCoordinates(int x, int y, char zone, sqlite3* db);
void updateSoil();
unsigned char updateMisc(int todayDate);
void updateNPC();

#endif //FARMINGCO_MAP_H
