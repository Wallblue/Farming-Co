#include <stdio.h>
#include <SDL.h>
#include "database/database.h"
#include "map/map.h"
#include "player/player.h"
#include "error/error.h"

SDL_Window* initWindow();
void gameLoop(SDL_Renderer*, SDL_Texture*, SDL_Texture*, SDL_Texture*);
SDL_Renderer* initRenderer(SDL_Window* );
SDL_Texture* loadTexture(SDL_Renderer*, const char*);

int main(int argc, char **argv){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        exitWithError("Erreur d'initialisation");
    }

    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRenderer(window);

    SDL_Texture* grassTexture = loadTexture(renderer, "../map/sheets/grass.bmp");
    SDL_Texture* fencesTexture = loadTexture(renderer, "../map/sheets/fences.bmp");
    SDL_Texture* playerTexture = loadTexture(renderer, "../player/sheets/player.bmp");

    //createDatabase();
    gameLoop(renderer, grassTexture, fencesTexture, playerTexture);

    // Libération des ressources
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(fencesTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Window* initWindow() {
    // Initialisation de la fenêtre
    SDL_Window *window = SDL_CreateWindow("FarmingCo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 640,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        exitWithError("Erreur de création de la fenêtre");
    }

    return window;
}

void gameLoop(SDL_Renderer* renderer, SDL_Texture* grassTexture, SDL_Texture* fencesTexture, SDL_Texture* playerTexture) {
    // Boucle principale du jeu
    int endGame = 0;
    int countX = 0;
    int countY = 0;

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

    while (!endGame) {
        printMap(renderer, grassTexture, mapBg);
        printMap(renderer, fencesTexture, mapFg);
        SDL_RenderCopy(renderer, playerTexture, &playerSrc, &playerDst);
        SDL_RenderPresent(renderer);

        if (SDL_WaitEvent(&event)) {
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
                            moveLeft(&playerSrc, &playerDst, &countX, &countY, mapFg);
                            break;

                        case SDLK_RIGHT:
                            moveRight(&playerSrc, &playerDst, &countX, &countY, mapFg);
                            break;

                        case SDLK_UP:
                            moveUp(&playerSrc, &playerDst, &countX, &countY, mapFg);
                            break;

                        case SDLK_DOWN:
                            moveDown(&playerSrc, &playerDst, &countX, &countY, mapFg);
                            break;
                    }
            }
        }
    }
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