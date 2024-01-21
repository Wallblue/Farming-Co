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
#include <sqlite3.h>
#include "../define.h"
#include "../error/error.h"
#include "../items/items.h"
#include "../database/database.h"
#include "../map/map.h"
#include "../save/save.h"
#include <time.h>


void moveLeft(SDL_Rect *, SDL_Rect *, int *, const int *, char **, unsigned char **, char*);
void moveRight(SDL_Rect *, SDL_Rect *, int *, const int *, char **, unsigned char **, char*);
void moveUp(SDL_Rect *, SDL_Rect *,  int *, int *, char **, unsigned char **, char*);
void moveDown(SDL_Rect *, SDL_Rect *,  int *, int *, char **, unsigned char **, char*);
char destroyObject(unsigned char nX, unsigned char nY, char zone, unsigned char **objectMap, Inventory* inventory, Item *heldItem);
unsigned char inputObject(int xMouse, int yMouse, unsigned char **tab, char **mapFg, unsigned char** soiledFloor, char zone, int todayDate, Item *heldItem, Inventory* inventory);
void soilFloor(unsigned char, unsigned char, unsigned char **, Item *);

#endif //FARMINGCO_PLAYER_H
