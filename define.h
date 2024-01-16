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

//Functions
size_t getFileSize(FILE* fp);
SDL_Texture* saveRendererToTexture(SDL_Renderer* renderer);

//Structures
struct ThreadData{
    int *timeInGame;
    int *sleep;
    int *pause;
};

#endif //FARMINGCO_DEFINE_H
