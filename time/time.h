//
// PHE Lina & BUSQUET Nicolas
// 07/01/2024
// Création du système de temps in game
//

#ifndef FARMINGCO_TIME_H
#define FARMINGCO_TIME_H

#define CLOCK_X 696
#define CLOCK_Y 14
#define CLOCK_WIDTH 93
#define CLOCK_HEIGHT 36

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <sqlite3.h>
#include "../database/database.h"
#include "../define.h"
#include "../error/error.h"
#include "../map/map.h"
#include "../colors.h"
#include "../main.h"

int day(void*);
void applyFilter(SDL_Renderer *, const int *, SDL_Texture *);
void seeTime(SDL_Renderer*, const int*);
void pauseMenu(SDL_Renderer *, SDL_Texture*);
int getDateInGame();
void getSleep(struct ThreadData *data);
unsigned char updateDate(int);
unsigned char updatePlants(int);
unsigned char updateSprite(sqlite3* db, int objectId, int state);

#endif //FARMINGCO_TIME_H
