#include <stdio.h>
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

SDL_Window *initWindow();

void gameLoop(SDL_Renderer *, SDL_Texture *, SDL_Texture *, SDL_Texture *, int *, SDL_Texture *, int *, int *);


SDL_Renderer *initRenderer(SDL_Window *);

SDL_Texture *loadTexture(SDL_Renderer *, const char *);


int main(int argc, char **argv) {
    int timeInGame = 0;
    int sleep = 0;
    int pause = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)exitWithError("Erreur d'initialisation");
    if (TTF_Init() != 0)exitWithError("Erreur d'initialisation");

    SDL_Window *window = initWindow();
    SDL_Renderer *renderer = initRenderer(window);

    SDL_Texture *floorTexture = loadTexture(renderer, "../assets/sheets/floors.bmp");
    SDL_Texture *playerTexture = loadTexture(renderer, "../assets/sheets/player.bmp");
    SDL_Texture *furnitureTexture = loadTexture(renderer, "../assets/sheets/furniture.bmp");
    SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    unsigned char err;
    Inventory inventory;

    struct ThreadData threadData;
    threadData.timeInGame = &timeInGame;
    threadData.sleep = &sleep;
    threadData.pause = &pause;

    SDL_Thread *threadID = SDL_CreateThread(day, "LazyThread", (void *) (&threadData));

    if (createDatabase() == FAILURE) exitWithError("Database creation error.");
    if (addItemsToDatabase() == FAILURE) exitWithError("Items loading on database impossible");

    if ((err = loadObjectsMaps()) != SUCCESS) {
        if (err == 2) err = initObjectMaps();
        if (err == FAILURE) exitWithError("Can't load map.");
    }

    if ((err = loadInventory(inventory)) != SUCCESS) {
        if (err == 2) initInventory(inventory);
        else exitWithError("Can't load saved inventory.");
    }

    gameLoop(renderer, floorTexture, playerTexture, furnitureTexture, &timeInGame, lightLayer, threadData.sleep,
             threadData.pause);

    err = saveObjectMaps();
    if (err == FAILURE || saveInventory(inventory) == FAILURE) exitWithError("Couldn't save properly.");

    // Libération des ressources
    TTF_Quit();
    SDL_DestroyTexture(floorTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    freeObjectMaps();

    return EXIT_SUCCESS;
}


SDL_Window *initWindow() {
    // Initialisation de la fenêtre
    SDL_Window *window = SDL_CreateWindow("FarmingCo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screenWidth, screenHeight,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        exitWithError("Erreur de création de la fenêtre");
    }

    return window;
}

void
gameLoop(SDL_Renderer *renderer, SDL_Texture *floorTexture, SDL_Texture *playerTexture, SDL_Texture *furnitureTexture,
         int *timeInGame, SDL_Texture *lightLayer, int *sleep, int *pause) {
    // Boucle principale du jeu
    int endGame = 0;
    int countX = 18;
    int countY = 5;
    int zone = 0;
    char **mapBg;
    char **mapFg;
    char **mapObjects;

    SDL_Event event;
    SDL_Rect playerDst;
    SDL_Rect playerSrc;

    playerSrc.w = tileHeightWidth;
    playerSrc.h = tileHeightWidth;
    playerDst.w = dstHeightWidth;
    playerDst.h = dstHeightWidth;
    playerSrc.x = tileHeightWidth;
    playerSrc.y = tileHeightWidth;
    playerDst.x = 576;
    playerDst.y = 160;
    int x, y;

    while (!endGame) {
        switch (zone) {
            case 0:
                mapBg = firstZoneBg;
                mapFg = firstZoneFg;
                mapObjects = mapObjects1;
                break;
            case 1:
                mapBg = secondZoneBg;
                mapFg = secondZoneFg;
                mapObjects = mapObjects2;
                break;
            case 2:
                mapBg = thirdZoneBg;
                mapFg = thirdZoneFg;
                mapObjects = mapObjects3;
                break;
            case 3:
                mapBg = fourthZoneBg;
                mapFg = fourthZoneFg;
                mapObjects = mapObjects4;
                break;
            case 4:
                mapBg = home;
                mapFg = homeFg;
                mapObjects = homeObjects;
                break;
        }

        SDL_RenderClear(renderer);

        printMap(renderer, floorTexture, mapBg);
        printMap(renderer, floorTexture, mapFg);

        if (zone == 0)printMap(renderer, floorTexture, houseRoof);

        printMap(renderer, furnitureTexture, mapObjects);
        SDL_RenderCopy(renderer, playerTexture, &playerSrc, &playerDst);
        seeTime(renderer, timeInGame);
        applyFilter(renderer, timeInGame, lightLayer);

        if(*pause == 1)pauseMenu(renderer, lightLayer);

        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    endGame = 1;
                    break;

                case SDL_KEYDOWN: //détecte quand on appuie sur une touche
                    if (*pause == 0 || event.key.keysym.sym == SDLK_ESCAPE) {
                        switch (event.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                if (*pause == 0) {
                                    *pause = 1;
                                }else
                                    *pause = 0;
                                break;

                            case SDLK_LEFT:
                                moveLeft(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                break;

                            case SDLK_RIGHT:
                                moveRight(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                break;

                            case SDLK_UP:
                                moveUp(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                break;

                            case SDLK_DOWN:
                                moveDown(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                break;

                            case SDLK_s:
                                *sleep = 1;
                                break;

                        }
                    }

                case SDL_MOUSEBUTTONUP:
                    if (*pause == 0) {
                        switch (event.button.button) {
                            case SDL_BUTTON_LEFT:
                                SDL_GetMouseState(&x, &y);
                                inputObject(x, y, mapObjects, mapFg, &zone);
                                break;
                        }
                    }else if(*pause == 1){
                        switch (event.button.button) {
                            case SDL_BUTTON_LEFT:
                                SDL_GetMouseState(&x, &y);
                                if(x>=300 && y>= screenHeight/3+16 && x<=500 && y<=screenHeight/3+66)*pause = 0;

                                if(x>=300 && y>=screenHeight/2+16 && x<=500 && y<=screenHeight/2+66)endGame = 1;
                                break;
                        }
                    }
            }
        }
    }
}

SDL_Renderer *initRenderer(SDL_Window *window) {
    // Initialisation du rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        exitWithError("Erreur de création du rendu");
    }

    return renderer;
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *imagePath) {
    // Chargement d'une texture à partir d'une image BMP
    SDL_Surface *surface = SDL_LoadBMP(imagePath);
    if (surface == NULL) {
        exitWithError("Echec de chargement du tileset");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        exitWithError("Erreur de création du tileset");
    }

    SDL_FreeSurface(surface);

    return texture;
}

size_t getFileSize(FILE *fp) {
    size_t fileSize;
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    return fileSize;
}


