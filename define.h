//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du fichier :
// DEFINE de tous les éléments qui seront utilisés pour la map, le player... (pour meilleure clarté)
//
#include <stdio.h>

#ifndef FARMINGCO_DEFINE_H
#define FARMINGCO_DEFINE_H

#define tileHeightWidth 16

#define dstHeightWidth 32

#define mapWidth 25
#define mapHeight 20

#define FAILURE 1
#define SUCCESS 0

//Functions
size_t getFileSize(FILE* fp);

//Structures
struct ThreadData{
    int *timeInGame;
    int *sleep;
};

#endif //FARMINGCO_DEFINE_H
