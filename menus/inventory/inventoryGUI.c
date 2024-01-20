#include "../menu.h"
#include "../../colors.h"
#include "../../define.h"
#include "../../time/time.h"
#include <SDL.h>
#include <SDL_ttf.h>

char inventoryEventLoop(SDL_Renderer* renderer, Inventory inventory, Inventory secondInventory){
    SDL_Event event;
    char exit = 0;

    int xMouse, yMouse, xHud = (screenWidth - INVENTORY_HUD_WIDTH) / 2, yHud = (screenHeight - INVENTORY_HUD_HEIGHT) / 2, temp;
    char draggedItem = -1;
    char index;
    Item* heldInventory = inventory, * inventoryPointer;
    char partOfScreen;

    SDL_Texture* rendererSave = saveRendererToTexture(renderer);

    if(secondInventory != NULL){
        yHud = (screenHeight - 2 * INVENTORY_HUD_HEIGHT) / 3;
    }

    while(exit == 0){
        refreshInventory(renderer, rendererSave, inventory, secondInventory, heldInventory, xHud, yHud, xMouse, yMouse, draggedItem);
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit = -1;
                    break;

                case SDL_KEYDOWN: //détecte quand on appuie sur une touche
                    switch (event.key.keysym.sym) {
                        case SDLK_e: case SDLK_ESCAPE:
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
                    if(secondInventory != NULL && yMouse > screenHeight / 2){
                        inventoryPointer = secondInventory;
                        temp = 2 * yHud + INVENTORY_HUD_HEIGHT;
                    } else {
                        inventoryPointer = inventory;
                        temp = yHud;
                    }
                    if(isMouseOnSlot(xMouse, yMouse, xHud, temp) == SDL_TRUE){
                        index = (char) ((yMouse - temp - INV_TOP_BOT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_LINES) * 10 + (xMouse - xHud - INV_LEFT_RIGHT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_SLOTS));
                        if(draggedItem != -1){
                            swapItems(heldInventory + draggedItem, inventoryPointer + index);
                            draggedItem = -1;
                        } else if(inventoryPointer[index].id != 0) {
                            draggedItem = index;
                            heldInventory = inventoryPointer;
                        }
                    }
                    break;
            }
        }
    }

    return exit;
}

unsigned char refreshInventory(SDL_Renderer *renderer, SDL_Texture *rendererSave, Inventory inventory, Inventory secondInventory, Inventory heldInventory,
                               int xHud, int yHud, int xMouse, int yMouse, char draggedItemIndex) {
    int nX, nY;

    SDL_SetTextureBlendMode(rendererSave, SDL_BLENDMODE_BLEND);

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, rendererSave, NULL, NULL) < 0) return FAILURE;

    if(secondInventory == NULL){
        if(printInventoryHUD(renderer, inventory, draggedItemIndex, xHud, yHud) == FAILURE) return FAILURE;
    }else if(heldInventory == inventory) {
        if (printInventoryHUD(renderer, inventory, draggedItemIndex, xHud, yHud) == FAILURE) return FAILURE;
        if(printInventoryHUD(renderer, secondInventory, -1, xHud, 2 * yHud + INVENTORY_HUD_HEIGHT) == FAILURE) return FAILURE;
    }else{
        if (printInventoryHUD(renderer, inventory, -1, xHud, yHud) == FAILURE) return FAILURE;
        if (printInventoryHUD(renderer, secondInventory, draggedItemIndex, xHud, 2 * yHud + INVENTORY_HUD_HEIGHT) == FAILURE) return FAILURE;
    }

    if(isMouseOnSlot(xMouse, yMouse, xHud, yHud) == SDL_TRUE) {
        nX = (xMouse - xHud - INV_LEFT_RIGHT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_SLOTS);
        nY = (yMouse - yHud - INV_TOP_BOT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_LINES);
        highlightSlot(renderer, nX, nY, YELLOW, xHud, yHud);
        if(inventory[nY * 10 + nX].id != 0) displayDescriptionBox(renderer, nX, nY, xHud, yHud);
    }
    if(secondInventory != NULL && isMouseOnSlot(xMouse, yMouse, xHud, 2 * yHud + INVENTORY_HUD_HEIGHT) == SDL_TRUE) {
        nX = (xMouse - xHud - INV_LEFT_RIGHT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_SLOTS);
        nY = (yMouse - (2 * yHud + INVENTORY_HUD_HEIGHT) - INV_TOP_BOT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_LINES);
        highlightSlot(renderer, nX, nY, YELLOW, xHud, 2 * yHud + INVENTORY_HUD_HEIGHT);
        if(secondInventory[nY * 10 + nX].id != 0) displayDescriptionBox(renderer, nX, nY, xHud, 2 * yHud + INVENTORY_HUD_HEIGHT);
    }

    if(draggedItemIndex != -1)
        if (dragItem(renderer, xMouse, yMouse, heldInventory, draggedItemIndex) == FAILURE) return FAILURE;

    SDL_RenderPresent(renderer);
    SDL_Delay(FPS_LIMIT / 1000);

    return SUCCESS;
}

SDL_bool isMouseOnSlot(int xMouse, int yMouse, int xHud, int yHud){
    xMouse -= (xHud + INV_LEFT_RIGHT_PADDING); //This simplifies conditions below
    yMouse -= (yHud + INV_TOP_BOT_PADDING);
    int xGap = SLOT_SIDE + INV_SPACE_BTWN_SLOTS; //These are the distance on x and y between slots upperleft corner
    int yGap = SLOT_SIDE + INV_SPACE_BTWN_LINES;
    int xMax = 10 * xGap - INV_SPACE_BTWN_SLOTS; //The max values of x and y for each slot range
    int yMax = 3 * yGap - INV_SPACE_BTWN_LINES;

    if(xMouse < 0 || yMouse < 0 || xMouse > xMax || yMouse > yMax || xMouse % xGap > SLOT_SIDE || yMouse % yGap > SLOT_SIDE){
        return SDL_FALSE;
    }
    return SDL_TRUE;
}

unsigned char highlightSlot(SDL_Renderer* renderer, unsigned char nX, unsigned char nY, int r, int g, int b, int xHud, int yHud){
    int xFirstSlot = xHud + INV_LEFT_RIGHT_PADDING;
    int yFirstSlot = yHud + INV_TOP_BOT_PADDING;
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

unsigned char displayDescriptionBox(SDL_Renderer* renderer, unsigned char nX, unsigned char nY, int xHud, int yHud){
    int xFirstSlotEnd = xHud + INV_LEFT_RIGHT_PADDING; //x and y coordinate of the first slot right upper corner except for last slots of range
    xFirstSlotEnd += (nX+1) % 10 == 0 ? -DESC_BOX_WIDTH : SLOT_SIDE;
    int yFirstSlot = yHud + INV_TOP_BOT_PADDING;
    SDL_Rect descriptionBox = {xFirstSlotEnd + nX * (SLOT_SIDE + INV_SPACE_BTWN_SLOTS), yFirstSlot + nY * (SLOT_SIDE + INV_SPACE_BTWN_LINES), DESC_BOX_WIDTH, INVENTORY_HUD_HEIGHT};
    SDL_Texture* boxTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DESC_BOX_WIDTH, INVENTORY_HUD_HEIGHT);
    TTF_Font* font = loadFont(64);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, DARK_GREY, 200);
    SDL_RenderFillRect(renderer, &descriptionBox);
    SDL_SetRenderDrawColor(renderer, BLACK, 255);
    SDL_RenderDrawRect(renderer, &descriptionBox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}