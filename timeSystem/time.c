//
// PHE Lina & BUSQUET Nicolas
// 07/01/2024
// Création du système de temps in game
//

#include "time.h"


int day(void* timeInGame) {
    int* getHours = (int*)timeInGame;

    Uint64 startTime = SDL_GetTicks64();

    do {
        Uint64 currentTime = SDL_GetTicks64();
        Uint64 elapsedTime = currentTime - startTime;

        if ((elapsedTime / (1000 * 60)) == *getHours + 1) {
            (*getHours)++;
            printf("\nIl est %d h", (*getHours)+7 );
        }

    } while (*getHours < 23);

    return 0;
}

void applyFilter(SDL_Renderer * renderer, int * timeInGame, SDL_Texture *lightLayer){
    SDL_Rect gameRect = {0, 0, 800, 640};
    int nightFilter = 0;

    if(*timeInGame >= 12){
        nightFilter = 11 * (*timeInGame/2);
    }

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, nightFilter);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &gameRect);
}