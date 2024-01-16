#include "../menu.h"
#include "../../colors.h"
#include "../../define.h"
#include <SDL.h>

char inventoryEventLoop(SDL_Renderer* renderer, Inventory inventory){
    SDL_Event event;
    char exit = 0;

    int xMouse, yMouse;
    char draggedItem = -1;
    char index;

    SDL_Texture* inventoryTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, INVENTORY_HUD_WIDTH, INVENTORY_HUD_HEIGHT);
    SDL_Texture* rendererSave = saveRendererToTexture(renderer);

    while(exit == 0){
        refreshInventory(renderer, rendererSave, inventoryTexture, inventory, xMouse, yMouse, draggedItem);
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
                    xMouse = event.motion.x;
                    yMouse = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONUP:
                    xMouse = event.button.x;
                    yMouse = event.button.y;
                    if(isMouseOnSlot(xMouse, yMouse) == SDL_TRUE){
                        index = (char) ((yMouse - INVENTORY_HUD_Y - INV_TOP_BOT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_LINES) * 10 + (xMouse - INVENTORY_HUD_X - INV_LEFT_RIGHT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_SLOTS));
                        if(draggedItem != -1){
                            swapItems(draggedItem, index, inventory);
                            draggedItem = -1;
                        } else if(inventory[index].id != 0)
                            draggedItem = index;
                    }
                    break;
            }
        }
    }

    SDL_DestroyTexture(inventoryTexture);
    return exit;
}

SDL_bool isMouseOnSlot(int xMouse, int yMouse){
    xMouse -= (INVENTORY_HUD_X + INV_LEFT_RIGHT_PADDING); //This simplifies conditions below
    yMouse -= (INVENTORY_HUD_Y + INV_TOP_BOT_PADDING);
    int xGap = SLOT_SIDE + INV_SPACE_BTWN_SLOTS; //These are the distance on x and y between slots upperleft corner
    int yGap = SLOT_SIDE + INV_SPACE_BTWN_LINES;
    int xMax = 10 * xGap - INV_SPACE_BTWN_SLOTS; //The max values of x and y for each slot range
    int yMax = 3 * yGap - INV_SPACE_BTWN_LINES;

    if(xMouse < 0 || yMouse < 0 || xMouse > xMax || yMouse > yMax || xMouse % xGap > SLOT_SIDE || yMouse % yGap > SLOT_SIDE){
        return SDL_FALSE;
    }
    return SDL_TRUE;
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

unsigned char dragItem(SDL_Renderer* renderer, int xMouse, int yMouse, Inventory inventory, char draggedItemIndex){
    SDL_Rect destRect = {xMouse - SLOT_SIDE / 2, yMouse - SLOT_SIDE / 2, SLOT_SIDE, SLOT_SIDE};

    if(insertItemInSlot(renderer, inventory + draggedItemIndex, &destRect) == FAILURE) return FAILURE;

    return SUCCESS;
}

unsigned char refreshInventory(SDL_Renderer* renderer, SDL_Texture* rendererSave, SDL_Texture* inventoryTexture, Inventory inventory, int xMouse, int yMouse, char draggedItemIndex){
    SDL_SetTextureBlendMode(rendererSave, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(inventoryTexture, SDL_BLENDMODE_BLEND);

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, rendererSave, NULL, NULL) < 0) return FAILURE;
    if(printInventoryHUD(renderer, inventoryTexture, inventory, draggedItemIndex) == FAILURE) return FAILURE;

    if(isMouseOnSlot(xMouse, yMouse) == SDL_TRUE)
        highlightSlot(renderer,
                      (xMouse - INVENTORY_HUD_X - INV_LEFT_RIGHT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_SLOTS),
                      (yMouse - INVENTORY_HUD_Y - INV_TOP_BOT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_LINES),
                      YELLOW);

    if(draggedItemIndex != -1)
        if(dragItem(renderer, xMouse, yMouse, inventory, draggedItemIndex) == FAILURE) return FAILURE;

    SDL_RenderPresent(renderer);

    return SUCCESS;
}