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
    int* pause = threadData->pause;

    Uint64 startTime = SDL_GetTicks64();
    Uint64 pauseTime = 0;
    do {
        Uint64 currentTime = SDL_GetTicks64();
        Uint64 elapsedTime = currentTime - startTime;

        if(*pause == 1 && startTime){
            pauseTime = SDL_GetTicks64() - startTime;
            startTime = 0;
        }

        if(*pause == 0) {
            if(pauseTime){
                startTime = currentTime - pauseTime;
                pauseTime = 0;
            }
            //remplacer 1000*60 par 1000 pour rester en secondes
            if ((elapsedTime / (1000)) == *getHours + 1) {
                (*getHours)++;
            }

            if (*getHours == 24) {
                startTime = currentTime;
                *getHours = 0;
            }

            if (*sleep == 1) {
                startTime = currentTime;
                *getHours = 0;
                *sleep = 0;
            }
        }


    } while (*getHours < 25);

    return 0;
}

void applyFilter(SDL_Renderer * renderer, int * timeInGame, SDL_Texture *lightLayer){
    SDL_Rect gameRect = {0, 0, screenWidth, screenHeight};
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
    TTF_Font* font = loadFont();

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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
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

void pauseMenu(SDL_Renderer *renderer, SDL_Texture* lightLayer){
    /* Création du fond noir en transparence*/
    SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, 100);

    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &bgRect);

    /* Création du carré du menu*/
    SDL_Rect menuRect = {screenWidth/4, screenHeight/6, 400, 400};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &menuRect);

    /*Texte du menu*/
    TTF_Font* font = loadFont();
    SDL_Color color = {0,0,0};

    SDL_Surface * surface = TTF_RenderText_Solid(font, "Pause", color);
    if(!surface) exitWithError("Erreur d'initialisation de la surface");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) exitWithError("Erreur de chargement de la texture");

    SDL_Rect textRect = {360, screenHeight/6 + 16, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &textRect);


    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

}

TTF_Font *loadFont(){
    TTF_Font* font = TTF_OpenFont("../assets/font/game.ttf", 64);
    if(!font) exitWithError("Erreur de chargement de la police");

    return font;
}

