//
// PHE Lina & BUSQUET Nicolas
// 07/01/2024
// Création du système de temps in game
//

#include "time.h"


int day(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;
    int* getHours = threadData->timeInGame;
    int* sleep = threadData->sleep;

    Uint64 startTime = SDL_GetTicks64();
    do {
        Uint64 currentTime = SDL_GetTicks64();
        Uint64 elapsedTime = currentTime - startTime;

        //remplacer 1000*60 par 1000 pour rester en secondes
        if ((elapsedTime / (1000*60)) == *getHours + 1) {
            (*getHours)++;
        }

        if(*getHours == 24){
            startTime = currentTime;
            *getHours = 0;
        }


        if(*sleep == 1){
            startTime = currentTime;
            *getHours = 0;
            *sleep = 0;
        }

    } while (*getHours < 25);

    return 0;
}

void applyFilter(SDL_Renderer * renderer, int * timeInGame, SDL_Texture *lightLayer){
    SDL_Rect gameRect = {0, 0, 800, 640};
    int nightFilter = 0;

    if(*timeInGame >= 12){
        nightFilter = 15 * (*timeInGame/2);
    }

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, nightFilter);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &gameRect);
}

void seeTime(SDL_Renderer *renderer, int* timeInGame) {
    TTF_Font* font = TTF_OpenFont("../assets/font/game.ttf", 64);
    if(!font) exitWithError("Erreur de chargement de la police");

    SDL_Color bgColor = {255, 255, 255, 100};
    SDL_Color textColor = {0, 0, 0};
    SDL_Rect bgRect = {696, 14, 93, 36};

    char text[20];
    if(*timeInGame < 17) {
        if(*timeInGame+7 > 9)
            snprintf(text, sizeof(text), "%d:00", *timeInGame + 7);
        else
            snprintf(text, sizeof(text), "0%d:00", *timeInGame + 7);
    }
    else
        snprintf(text, sizeof(text), "0%d:00", *timeInGame - 17);

    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if(!surface) exitWithError("Erreur d'initialisation de la surface");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) exitWithError("Erreur de chargement de la texture");

    SDL_Rect textRect = {700, 16, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}
