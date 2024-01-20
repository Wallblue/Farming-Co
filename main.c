#include <stdio.h>
#include <time.h>
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
#include "menus/menu.h"

SDL_Window *initWindow();

void gameLoop(SDL_Renderer *, SDL_Texture *, SDL_Texture *, SDL_Texture *, SDL_Texture *, struct ThreadData* , Inventory inventory);

SDL_Renderer *initRenderer(SDL_Window *);

SDL_Texture *loadTexture(SDL_Renderer *, const char *);

void makeHudDisappear(SDL_Renderer* renderer, SDL_Texture* floorTexture, SDL_Texture* furnitureTexture, SDL_Texture* playerTexture,
                      char** mapBg, char** mapFg, unsigned char** mapObjects, SDL_Rect* playerSrc, SDL_Rect* playerDst, char zone);

int main(int argc, char **argv) {
    int timeInGame = 0;
    int sleep = 0;
    char pause = 0;
    int todayDate;
    srand(time(NULL));

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

    if (createDatabase() == FAILURE) exitWithError("Database creation error.");
    if (startGame() == FAILURE) exitWithError("Couldn't start game.");
    if (addItemsToDatabase() == FAILURE) exitWithError("Items loading in database impossible");

    todayDate = getDateInGame();
    struct ThreadData threadData;
    threadData.timeInGame = &timeInGame;
    threadData.sleep = &sleep;
    threadData.pause = &pause;
    threadData.todayDate = &todayDate;

    SDL_Thread *threadID = SDL_CreateThread(day, "LazyThread", (void *) (&threadData));

    if (initObjectMaps() == FAILURE) exitWithError("Can't initialize maps.");
    if (loadObjectsMaps() == FAILURE) exitWithError("Can't load maps.");

    if ((err = loadInventory(inventory)) != SUCCESS) {
        if (err == 2) initInventory(inventory);
        else exitWithError("Can't load saved inventory.");
    }

    gameLoop(renderer, floorTexture, playerTexture, furnitureTexture, lightLayer, &threadData ,inventory);

    if(saveInventory(inventory) == FAILURE) exitWithError("Couldn't save properly.");

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

void gameLoop(SDL_Renderer *renderer, SDL_Texture *floorTexture, SDL_Texture *playerTexture, SDL_Texture *furnitureTexture, SDL_Texture *lightLayer, struct ThreadData* data, Inventory inventory) {
    // Boucle principale du jeu
    int endGame = 0;
    int countX = 18;
    int countY = 5;
    char zone = 0;
    char err;
    unsigned char currentSlot = 1;
    char **mapBg;
    char **mapFg;
    unsigned char **mapObjects;
    unsigned char lastMovement;
    unsigned char interactedWith;
    unsigned char **soiledFloor;


    SDL_Texture* hotbarTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, HOTBAR_WIDTH, SLOT_SIDE);
    SDL_Event event;
    SDL_Rect playerDst;
    SDL_Rect playerSrc;

    Inventory tempInventory = {0};

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
                soiledFloor = soiledFloor3;
                break;
            case 3:
                mapBg = fourthZoneBg;
                mapFg = fourthZoneFg;
                mapObjects = mapObjects4;
                soiledFloor = soiledFloor4;
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
        if(zone == 2)printMap(renderer, floorTexture,(char **) soiledFloor);
        if(zone == 3)printMap(renderer, floorTexture,(char **) soiledFloor);
        printMap(renderer, furnitureTexture, (char**)mapObjects);
        SDL_RenderCopy(renderer, playerTexture, &playerSrc, &playerDst);
        seeTime(renderer, data->timeInGame);
        applyFilter(renderer,  data->timeInGame, lightLayer);
        if (printHotbarHUD(renderer, hotbarTexture, currentSlot, inventory) == FAILURE)
            exitWithError("Can't load hotbar");

        if(*data->pause == 1)pauseMenu(renderer, lightLayer);

        SDL_RenderPresent(renderer);
        SDL_Delay(FPS_LIMIT / 1000);

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    endGame = 1;
                    break;

                case SDL_KEYDOWN: //détecte quand on appuie sur une touche
                    if (*data->pause == 0 || event.key.keysym.sym == SDLK_ESCAPE) {
                        switch (event.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                if (*data->pause == 0) {
                                    *data->pause = 1;
                                } else
                                    *data->pause = 0;
                                break;

                            //0: gauche, 1: droite, 2: haut, 3: bas
                            case SDLK_LEFT: case SDLK_q:
                                moveLeft(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                lastMovement = 0;
                                break;

                            case SDLK_RIGHT: case SDLK_d:
                                moveRight(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                lastMovement = 1;
                                break;

                            case SDLK_UP: case SDLK_z:
                                moveUp(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                lastMovement = 2;
                                break;

                            case SDLK_DOWN: case SDLK_s:
                                moveDown(&playerSrc, &playerDst, &countX, &countY, mapFg, mapObjects, &zone);
                                lastMovement = 3;
                                break;

                            case SDLK_f:
                                switch(lastMovement){
                                    case 0:
                                        interactedWith = mapObjects[countY][countX-1];
                                        break;
                                    case 1:
                                        interactedWith = mapObjects[countY][countX+1];
                                        break;
                                    case 2:
                                        interactedWith = mapObjects[countY-1][countX];
                                        break;
                                    case 3:
                                        interactedWith = mapObjects[countY+1][countX];
                                        break;
                                }

                                switch(interactedWith){
                                    case 'J' : case 'I' : case 'T': case 'S':
                                        *data->sleep = 1;
                                        updateSoil();
                                        break;

                                    case 'P':
                                        makeHudDisappear(renderer, floorTexture, furnitureTexture, playerTexture, mapBg, mapFg, mapObjects, &playerSrc, &playerDst, zone);
                                        if(inventoryEventLoop(renderer, inventory, tempInventory) == -1) endGame = 1;
                                        break;
                                }
                                break;

                            case SDLK_e:
                                makeHudDisappear(renderer, floorTexture, furnitureTexture, playerTexture, mapBg, mapFg, mapObjects, &playerSrc, &playerDst, zone);

                                if (inventoryEventLoop(renderer, inventory, NULL) == -1) endGame = 1;
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
                                switch (event.button.button) {
                                    case SDL_BUTTON_LEFT:
                                        SDL_GetMouseState(&x, &y);

                                        if(*data->pause == 0) {

                                            if(mapObjects[y/32][x/32] != '/') {
                                                err = destroyObject(x / 32, y / 32, zone, mapObjects, inventory,inventory + (currentSlot - 1));
                                                if (err == 3)
                                                    fprintf(stderr, "Inventory full !\n");
                                                else if (err == 2)
                                                    fprintf(stderr, "Wrong tool !\n");
                                                else if (err == FAILURE)
                                                    exitWithError("Error while trying to break object.");
                                            }else if (inventory[currentSlot - 1].id != 0 && inventory[currentSlot - 1].objectSpriteRef != '/' && mapFg[y / 32][x / 32] == '/')
                                                inputObject(x, y, mapObjects, mapFg, soiledFloor, zone, *data->todayDate,inventory + (currentSlot - 1), inventory);
                                            else if (mapObjects[y / 32][x / 32] == '/' && mapFg[y / 32][x / 32] == '/' && zone == 2 || zone == 3)
                                                soilFloor(x / 32, y / 32, soiledFloor, inventory + (currentSlot - 1));

                                        }else if (*data->pause == 1) {
                                            if (x >= 300 && y >= screenHeight / 3 + 16 && x <= 500 && y <= screenHeight / 3 + 66) *data->pause = 0;
                                            if (x >= 300 && y >= screenHeight / 2 + 16 && x <= 500 && y <= screenHeight / 2 + 66) endGame = 1;
                                        }
                                    break;
                                }
                        break;

                        case SDL_MOUSEWHEEL:
                            if (event.wheel.y > 0) ++currentSlot;
                            if (event.wheel.y < 0) --currentSlot;
                            if (event.wheel.x > 0) ++currentSlot;
                            if (event.wheel.x < 0) --currentSlot;
                            currentSlot = currentSlot == 0 ? 10 : currentSlot;
                            currentSlot = currentSlot == 11 ? 1 : currentSlot;
                            break;
                    }
            }
        }
    }
    SDL_DestroyTexture(hotbarTexture);
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

SDL_Texture* saveRendererToTexture(SDL_Renderer* renderer){
    int pitch = sizeof(Uint32) * screenWidth;
    Uint32* pixels = malloc(pitch * screenHeight);
    SDL_Surface* surface = NULL;
    SDL_Texture* texture = NULL;

    if(SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, pitch) < 0) {
        free(pixels);
        return NULL;
    }

    surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, screenWidth, screenHeight, 32, pitch, SDL_PIXELFORMAT_RGBA8888);
    if(surface < 0){
        free(pixels);
        SDL_FreeSurface(surface);
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    free(pixels);
    SDL_FreeSurface(surface);

    return texture;
}

void makeHudDisappear(SDL_Renderer* renderer, SDL_Texture* floorTexture, SDL_Texture* furnitureTexture, SDL_Texture* playerTexture,
                      char** mapBg, char** mapFg, unsigned char** mapObjects, SDL_Rect* playerSrc, SDL_Rect* playerDst, char zone){
    SDL_RenderClear(renderer);

    printMap(renderer, floorTexture, mapBg);
    printMap(renderer, floorTexture, mapFg);

    if (zone == 0)printMap(renderer, floorTexture, houseRoof);

    printMap(renderer, furnitureTexture, (char**)mapObjects);
    SDL_RenderCopy(renderer, playerTexture, playerSrc, playerDst);
}

size_t getFileSize(FILE* fp){
    size_t fileSize;
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    return fileSize;
}


