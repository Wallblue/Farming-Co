//
// Main.h file
//

#ifndef FARMINGCO_MAIN_H
#define FARMINGCO_MAIN_H

#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include "database/database.h"
#include "map/map.h"
#include "player/player.h"
#include "error/error.h"
#include "save/save.h"
#include "time/time.h"
#include "items/items.h"
#include "items/inventory/inventory.h"
#include <SDL_ttf.h>
#include "menus/menu.h"
#include "npc/npc.h"

SDL_Window *initWindow();
void gameLoop(SDL_Renderer *renderer, SDL_Texture *floorTexture, SDL_Texture *playerTexture, SDL_Texture *furnitureTexture, SDL_Texture* npcTexture, SDL_Texture *lightLayer, struct ThreadData* data, Inventory* inventory);
SDL_Renderer *initRenderer(SDL_Window *);
SDL_Texture *loadTexture(SDL_Renderer *, const char *);
void makeHudDisappear(SDL_Renderer* renderer, SDL_Texture* floorTexture, SDL_Texture* furnitureTexture, SDL_Texture* playerTexture,
                      char** mapBg, char** mapFg, unsigned char** mapObjects, SDL_Rect* playerSrc, SDL_Rect* playerDst, char zone, unsigned char** soiledFloor, SDL_Texture* npcTexture);
TTF_Font *loadFont(int size);
SDL_Surface *loadItemSurface(SDL_Surface* surface, char* text, int size, int wrapLength);
SDL_Texture *loadItemTexture(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Surface* surface);


#endif //FARMINGCO_MAIN_H
