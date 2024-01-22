//
// Created by Lina Phe on 21/01/2024.
//

#ifndef FARMINGCO_NPC_H
#define FARMINGCO_NPC_H

//Structures
struct Npc{
    char* name;
    int id;
};
typedef struct Npc Npc;

#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include "../colors.h"
#include "../main.h"
void
chat(SDL_Renderer *renderer, unsigned char npcChar, SDL_Texture *lightLayer, char **savedDialog, unsigned char hasInteracted, int *savedTrader);
char* getContent(unsigned char id);
int isTrader(unsigned char id);
Npc* getNpcByChar(const unsigned char npcChar);
#endif //FARMINGCO_NPC_H
