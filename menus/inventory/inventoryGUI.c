#include "../menu.h"
#include "../../colors.h"
#include "../../define.h"
#include <SDL.h>

char inventoryEventLoop(SDL_Renderer* renderer, Inventory inventory){
    SDL_Texture* inventoryTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, INVENTORY_HUD_WIDTH, INVENTORY_HUD_HEIGHT);
    SDL_Event event;
    char exit = 0;

    if(printInventoryHUD(renderer, inventoryTexture, inventory) == FAILURE){
        SDL_Log("Couldn't open inventory : %s", SDL_GetError());
        exit = 1;
    }
    SDL_RenderPresent(renderer);

    while(exit == 0){
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit = -1;
                    break;

                case SDL_KEYDOWN: //détecte quand on appuie sur une touche
                    switch (event.key.keysym.sym) {
                        case SDLK_e: case SDLK_ESCAPE: //quitte si la touche est la lettre H
                            exit = 1;
                            break;
                    }
            }
        }
    }

    SDL_DestroyTexture(inventoryTexture);
    return exit;
}