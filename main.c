#include <stdio.h>
#include <SDL.h>
#include "database/database.h"
#include "map/map.h"
#include "error/error.h"

SDL_Window* initWindow();
void gameLoop(SDL_Renderer*, SDL_Texture*, SDL_Texture*);

int main(int argc, char **argv){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        exitWithError("Erreur d'initialisation");
    }

    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRenderer(window);

    SDL_Texture* grassTexture = loadTexture(renderer, "../map/sheets/grass.bmp");
    SDL_Texture* fencesTexture = loadTexture(renderer, "../map/sheets/fences.bmp");

    //createDatabase();
    gameLoop(renderer, grassTexture, fencesTexture);

    // Libération des ressources
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(fencesTexture);
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

void gameLoop(SDL_Renderer* renderer, SDL_Texture* grassTexture, SDL_Texture* fencesTexture) {
    // Boucle principale du jeu
    int endGame = 0;
    SDL_Event event;

    while (!endGame) {
        printMap(renderer, grassTexture, mapBg);
        printMap(renderer, fencesTexture, mapFg);
        SDL_RenderPresent(renderer);

        if (SDL_WaitEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    endGame = 1;
                    break;

                case SDL_KEYDOWN: //détecte quand on appuie sur une touche
                    switch (event.key.keysym.sym) {
                        case SDLK_h: //quitte si la touche est la lettre H
                            endGame = 1;
                            break;
                    }
            }
        }
    }
}
