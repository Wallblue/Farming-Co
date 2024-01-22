//
// Created by Lina Phe on 21/01/2024.
//

#ifndef FARMINGCO_NPC_H
#define FARMINGCO_NPC_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include "../colors.h"
#include "../main.h"
void chat(SDL_Renderer *renderer, unsigned char npc, SDL_Texture* lightLayer, char *savedDialog, unsigned char hasInteracted);
char* getContent(unsigned char id);
#endif //FARMINGCO_NPC_H
