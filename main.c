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
#include "menus/menu.h"
#include <SDL_ttf.h>

SDL_Window* initWindow();

void gameLoop(SDL_Renderer*, SDL_Texture* , SDL_Texture* , SDL_Texture* , SDL_Texture* , int * , SDL_Texture *, int *, Inventory inventory);

SDL_Renderer* initRenderer(SDL_Window* );
SDL_Texture* loadTexture(SDL_Renderer*, const char*);

int main(int argc, char **argv){
    int timeInGame = 0;
    int sleep = 0;
    TTF_Init();
    TTF_Quit();
    struct ThreadData threadData;
    threadData.timeInGame = &timeInGame;
    threadData.sleep = &sleep;

    SDL_Thread* threadID = SDL_CreateThread( day, "LazyThread", (void*)(&threadData));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        exitWithError("Erreur d'initialisation");
    }

    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRenderer(window);

    SDL_Texture* grassTexture = loadTexture(renderer, "../map/sheets/grass.bmp");
    SDL_Texture* fencesTexture = loadTexture(renderer, "../map/sheets/fences.bmp");
    SDL_Texture* playerTexture = loadTexture(renderer, "../player/sheets/player.bmp");
    SDL_Texture* furnitureTexture = loadTexture(renderer, "../map/sheets/furniture.bmp");
    SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    unsigned char err;
    Inventory inventory;

    if(createDatabase() == FAILURE) exitWithError("Database creation error.");
    if(addItemsToDatabase() == FAILURE) exitWithError("Items loading on database impossible");

    if((err = loadObjectsMaps()) != SUCCESS){
        if(err == 2) err = initObjectMaps();
        if(err == FAILURE) exitWithError("Can't load map.");
    }

    if((err = loadInventory(inventory)) != SUCCESS){
        if(err == 2) initInventory(inventory);
        else exitWithError("Can't load saved inventory.");
    }


    gameLoop(renderer, grassTexture, fencesTexture, playerTexture, furnitureTexture, &timeInGame, lightLayer, threadData.sleep, inventory);

    err = saveObjectMaps();
    if(saveInventory(inventory) == FAILURE || err == FAILURE) exitWithError("Couldn't save properly.");

    // Libération des ressources
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(fencesTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    freeObjectMaps();

    return EXIT_SUCCESS;
}


SDL_Window* initWindow() {
    // Initialisation de la fenêtre
    SDL_Window *window = SDL_CreateWindow("FarmingCo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          gameWidth, gameHeight,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        exitWithError("Erreur de création de la fenêtre");
    }

    return window;
}

void gameLoop(SDL_Renderer* renderer, SDL_Texture* grassTexture, SDL_Texture* fencesTexture, SDL_Texture* playerTexture, SDL_Texture* furnitureTexture ,int * timeInGame, SDL_Texture *lightLayer, int *sleep, Inventory inventory) {
    // Boucle principale du jeu
    int endGame = 0;
    int countX = 0;
    int countY = 0;
    int zone = 0;
    unsigned char currentSlot = 1;
    char **mapBg;
    char **mapFg;
    char **mapObjects;

    SDL_Texture* hotbarTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, HOTBAR_WIDTH, HOTBAR_HEIGHT);
    SDL_Texture* inventoryTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, INVENTORY_HUD_WIDTH, INVENTORY_HUD_HEIGHT);

    SDL_Event event;
    SDL_Rect playerDst;
    SDL_Rect playerSrc;

    playerSrc.w = tileHeightWidth;
    playerSrc.h = tileHeightWidth;
    playerDst.w = dstHeightWidth;
    playerDst.h = dstHeightWidth;
    playerSrc.x = tileHeightWidth;
    playerSrc.y = tileHeightWidth;
    playerDst.x = 0;
    playerDst.y = 0;
    int x, y;


    while (!endGame) {
        switch(zone){
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
            default:
                mapBg = firstZoneBg;
                mapFg = firstZoneFg;
                mapObjects = mapObjects1;
                break;
        }

        printMap(renderer, grassTexture, mapBg);
        printMap(renderer, fencesTexture, mapFg);
        printMap(renderer, furnitureTexture, mapObjects);

        SDL_RenderCopy(renderer, playerTexture, &playerSrc, &playerDst);

        applyFilter(renderer, timeInGame, lightLayer);
        printHotbarHUD(renderer, hotbarTexture, currentSlot);
        printInventoryHUD(renderer, inventoryTexture, inventory);
        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    endGame = 1;
                    break;

                case SDL_KEYDOWN: //détecte quand on appuie sur une touche
                    switch( event.key.keysym.sym ) {
                        case SDLK_h: //quitte si la touche est la lettre H
                            endGame =1;
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

                        case SDLK_1:
                            currentSlot = 1;
                            break;
                        case SDLK_2:
                            currentSlot = 2;
                            break;
                        case SDLK_3:
                            currentSlot = 3;
                            break;
                        case SDLK_4:
                            currentSlot = 4;
                            break;
                        case SDLK_5:
                            currentSlot = 5;
                            break;
                        case SDLK_6:
                            currentSlot = 6;
                            break;
                        case SDLK_7:
                            currentSlot = 7;
                            break;
                        case SDLK_8:
                            currentSlot = 8;
                            break;
                        case SDLK_9:
                            currentSlot = 9;
                            break;
                        case SDLK_0:
                            currentSlot = 10;
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    switch(event.button.button){
                        case SDL_BUTTON_LEFT:
                            SDL_GetMouseState(&x, &y);
                            inputObject(x, y, mapObjects);
                            break;
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0) ++currentSlot;
                    if(event.wheel.y < 0) --currentSlot;
                    if(event.wheel.x > 0) ++currentSlot;
                    if(event.wheel.x < 0) --currentSlot;
                    currentSlot = currentSlot == 0 ? 10 : currentSlot;
                    currentSlot = currentSlot == 11 ? 1 : currentSlot;
                    break;
            }
        }
    }

    SDL_DestroyTexture(inventoryTexture);
    SDL_DestroyTexture(hotbarTexture);
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    // Initialisation du rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        exitWithError("Erreur de création du rendu");
    }

    return renderer;
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath) {
    // Chargement d'une texture à partir d'une image BMP
    SDL_Surface *surface = SDL_LoadBMP(imagePath);
    if (surface == NULL) {
        exitWithError("Echec de chargement du tileset");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL) {
        exitWithError("Erreur de création du tileset");
    }

    SDL_FreeSurface(surface);

    return texture;
}

size_t getFileSize(FILE* fp){
    size_t fileSize;
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    return fileSize;
}
