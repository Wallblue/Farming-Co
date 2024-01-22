//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du fichier :
// DEFINE de tous les éléments qui seront utilisés pour la map, le player... (pour meilleure clarté)
//
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#ifndef FARMINGCO_DEFINE_H
#define FARMINGCO_DEFINE_H

#define tileHeightWidth 16

#define dstHeightWidth 32

#define mapWidth 25
#define mapHeight 20

#define screenWidth (mapWidth*dstHeightWidth)
#define screenHeight (mapHeight*dstHeightWidth)

#define FAILURE 1
#define SUCCESS 0

#define FPS_LIMIT 60

#define NPC1_LOC1 {1, 4}
#define NPC1_LOC2 {2, 10}
#define NPC1_LOC3 {2, 6}
#define NPC1_NAME "Pumpkin"

#define NPC2_LOC1 {3, 20}
#define NPC2_LOC2 {4, 21}
#define NPC2_LOC3 {1, 17}
#define NPC2_NAME "Bianca"

#define NPC3_LOC1 {9, 4}
#define NPC3_LOC2 {7, 7}
#define NPC3_LOC3 {8, 8}
#define NPC3_NAME "Chocolat"

#define NPC4_LOC1 {12, 18}
#define NPC4_LOC2 {9, 19}
#define NPC4_LOC3 {13, 22}
#define NPC4_NAME "Penny"

#define NPC5_LOC1 {13, 6}
#define NPC5_LOC2 {15, 8}
#define NPC5_LOC3 {17, 4}
#define NPC5_NAME "Cosmos"

#define returnProperlyM return returnProperly

extern int playerId;

//Functions
size_t getFileSize(FILE* fp);
SDL_Texture* saveRendererToTexture(SDL_Renderer* renderer);

//Structures
struct ThreadData{
    int *timeInGame;
    int *sleep;
    char *pause;
    int *todayDate;
};

#endif //FARMINGCO_DEFINE_H
