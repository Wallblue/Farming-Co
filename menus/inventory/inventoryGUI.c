#include "../menu.h"
#include "../../colors.h"
#include "../../define.h"
#include <SDL.h>

char inventoryEventLoop(SDL_Renderer* renderer, Inventory inventory){
    SDL_Event event;
    char exit = 0;

    int x = 11;
    int y = 11;
    int xGap;
    int yGap;
    int xMax;
    int yMax;

    SDL_Texture* inventoryTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, INVENTORY_HUD_WIDTH, INVENTORY_HUD_HEIGHT);
    SDL_Texture* rendererSave = saveRendererToTexture(renderer);

    while(exit == 0){
        refreshInventory(renderer, rendererSave, inventoryTexture, inventory, x, y);
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
                    break;

                case SDL_MOUSEMOTION:
                    x = event.motion.x - (INVENTORY_HUD_X + INV_LEFT_RIGHT_PADDING);
                    y = event.motion.y - (INVENTORY_HUD_Y + INV_TOP_BOT_PADDING);

                    xGap = SLOT_SIDE + INV_SPACE_BTWN_SLOTS;
                    yGap = SLOT_SIDE + INV_SPACE_BTWN_LINES;

                    xMax = 10 * xGap - INV_SPACE_BTWN_SLOTS;
                    yMax = 3 * yGap - INV_SPACE_BTWN_LINES;

                    if(x < 0 || y < 0 || x > xMax || y > yMax || x % xGap > SLOT_SIDE || y % yGap > SLOT_SIDE){
                        x = 11;
                        y = 11;
                        break;
                    }

                    x = x / xGap;
                    y = y / yGap;

                    break;
            }
        }
    }

    SDL_DestroyTexture(inventoryTexture);
    return exit;
}

unsigned char highlightSlot(SDL_Renderer* renderer, unsigned char nX, unsigned char nY, int r, int g, int b){
    int xFirstSlot = INVENTORY_HUD_X + INV_LEFT_RIGHT_PADDING;
    int yFirstSlot = INVENTORY_HUD_Y + INV_TOP_BOT_PADDING;
    SDL_Rect highlighting = {xFirstSlot + nX * (SLOT_SIDE + INV_SPACE_BTWN_SLOTS), yFirstSlot + nY * (SLOT_SIDE + INV_SPACE_BTWN_LINES), SLOT_SIDE, SLOT_SIDE};

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawRect(renderer, &highlighting);

    return SUCCESS;
}

unsigned char refreshInventory(SDL_Renderer* renderer, SDL_Texture* rendererSave, SDL_Texture* inventoryTexture, Inventory inventory, int mouseX, int mouseY){
    SDL_SetTextureBlendMode(rendererSave, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(inventoryTexture, SDL_BLENDMODE_BLEND);

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, rendererSave, NULL, NULL) < 0) return FAILURE;
    if(printInventoryHUD(renderer, inventoryTexture, inventory) == FAILURE) return FAILURE;

    if(mouseX != 11 && mouseY != 11) highlightSlot(renderer, mouseX, mouseY, YELLOW);

    SDL_RenderPresent(renderer);

    return SUCCESS;
}