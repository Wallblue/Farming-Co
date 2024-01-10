//
// BUSQUET Nicolas & PHE Lina
// 10/01/2024
// Sauvegarde de la partie
//

#ifndef FARMINGCO_SAVE_H
#define FARMINGCO_SAVE_H

#include <stdio.h>

unsigned char saveObjectMaps();
unsigned char loadObjectsMaps();
unsigned char loadMapLine(char** line, FILE* fp);

#endif //FARMINGCO_SAVE_H
