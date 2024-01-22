#include "../menu.h"

char tradingEventLoop(SDL_Renderer* renderer, Inventory* npcInventory, Inventory* playerInventory){
    SDL_Event event;
    char exit = 0;

    int xMouse, yMouse, xHud, yHud, temp;
    char draggedItem = -1, index;
    Inventory* heldInventory = npcInventory, * inventoryPointer;
    SDL_Texture* rendererSave = saveRendererToTexture(renderer);

    /*if(npcInventory == NULL || playerInventory == NULL || npcInventory->ownerType != 2){
        fprintf(stderr, "Trading must receive both and only npc and player inventory\n");
        exit = 2;
    }*/

    xHud = (screenWidth - INVENTORY_HUD_WIDTH) / 2;
    yHud = (screenHeight - 2 * INVENTORY_HUD_HEIGHT) / 3;

    while(exit == 0){
        refreshInventory(renderer, rendererSave, npcInventory, playerInventory, heldInventory, xHud, yHud, xMouse, yMouse, draggedItem);
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
                    if(yMouse > screenHeight / 2){
                        inventoryPointer = playerInventory;
                        temp = 2 * yHud + INVENTORY_HUD_HEIGHT;
                    } else {
                        inventoryPointer = npcInventory;
                        temp = yHud;
                    }
                    if(isMouseOnSlot(xMouse, yMouse, xHud, temp) == SDL_TRUE){
                        index = (char) ((yMouse - temp - INV_TOP_BOT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_LINES) * 10 + (xMouse - xHud - INV_LEFT_RIGHT_PADDING) / (SLOT_SIDE + INV_SPACE_BTWN_SLOTS));
                        if(draggedItem != -1){
                            if(heldInventory != inventoryPointer) {
                                sellItem(playerInventory, npcInventory, draggedItem);
                                if(playerInventory->slots[draggedItem].id == 0) draggedItem = -1;
                            }else {
                                if (draggedItem != index)
                                    swapInventoryItems(heldInventory, draggedItem, inventoryPointer, index);
                                draggedItem = -1;
                            }
                        } else if(inventoryPointer->slots[index].id != 0){
                            if(inventoryPointer == playerInventory) {
                                draggedItem = index;
                                heldInventory = inventoryPointer;
                            } else
                                buyItem(playerInventory, npcInventory, index);
                        }
                    }
                    break;

            }
        }
    }
    SDL_DestroyTexture(rendererSave);
    return exit;
}

unsigned char buyItem(Inventory* playerInventory, Inventory* npcInventory, char index){
    if(!isWalletFilledEnough(npcInventory->slots[index].price, NULL)){
        fprintf(stderr, "Not enough money\n");
        return 2;
    }
    if(alterMoneyAmount(-npcInventory->slots[index].price, NULL) == FAILURE) return FAILURE;
    if(addItem(npcInventory->slots[index].id, 1, playerInventory) == FAILURE) return FAILURE;
    if(subtractItem(npcInventory->slots[index].id, 1, npcInventory) == FAILURE) return FAILURE;
    return SUCCESS;
}

unsigned char sellItem(Inventory* playerInventory, Inventory* npcInventory, char index){
    if(alterMoneyAmount(playerInventory->slots[index].price, NULL) == FAILURE) return FAILURE;
    if(subtractItem(playerInventory->slots[index].id, 1, playerInventory) == FAILURE) return FAILURE;
    return SUCCESS;
}