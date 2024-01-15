#include "menu.h"
#include "../colors.h"
#include "../define.h"
#include "../items/inventory/inventory.h"
#include <SDL.h>

unsigned char printHotbarHUD(SDL_Renderer* renderer, SDL_Texture* hotbarTexture, unsigned char selectedSlot){
    SDL_Rect hotbar[HOTBAR_LEN];
    SDL_Rect hotbarDest = {(gameWidth - HOTBAR_WIDTH) / 2, gameHeight - HOTBAR_HEIGHT - 10, HOTBAR_WIDTH, HOTBAR_HEIGHT};
    unsigned char i;

    if(SDL_SetRenderTarget(renderer, hotbarTexture) < 0) return FAILURE;

    SDL_SetRenderDrawColor(renderer, GREY, 128);
    for(i = 0; i < HOTBAR_LEN; i++){
        hotbar[i].w = HOTBAR_HEIGHT;
        hotbar[i].h = HOTBAR_HEIGHT;
        hotbar[i].x = (HOTBAR_HEIGHT + 10) * i;
        hotbar[i].y = 0;
    }
    SDL_RenderFillRects(renderer, hotbar, HOTBAR_LEN);
    SDL_SetRenderDrawColor(renderer, RED, 255);
    SDL_RenderDrawRect(renderer, hotbar + selectedSlot - 1);

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_SetTextureBlendMode(hotbarTexture, SDL_BLENDMODE_BLEND) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, hotbarTexture, NULL, &hotbarDest) < 0) return FAILURE;
    return SUCCESS;
}

unsigned char printInventoryHUD(SDL_Renderer* renderer, SDL_Texture* inventoryTexture, Inventory inventory){
    SDL_Rect inventorySlots[INVENTORY_MAX_SIZE];
    SDL_Rect hudLoc = {(gameWidth - INVENTORY_HUD_WIDTH) / 2, (gameHeight - INVENTORY_HUD_HEIGHT) / 2, INVENTORY_HUD_WIDTH, INVENTORY_HUD_HEIGHT};
    unsigned char i, j;

    if(SDL_SetRenderTarget(renderer, inventoryTexture) < 0) return FAILURE;

    SDL_SetRenderDrawColor(renderer, GREY, 128);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, DARK_GREY, 128);
    for(i = 0; i < INVENTORY_MAX_SIZE / HOTBAR_LEN; i++){
        for(j = 0; j < HOTBAR_LEN; j++){
            inventorySlots[i*10+j].w = HOTBAR_HEIGHT;
            inventorySlots[i*10+j].h = HOTBAR_HEIGHT;
            inventorySlots[i*10+j].x = INV_LEFT_RIGHT_PADDING + (HOTBAR_HEIGHT + 10) * j;
            inventorySlots[i*10+j].y = INV_TOP_BOT_PADDING + i * (HOTBAR_HEIGHT + INV_BETWEEN_LINES);
        }
    }
    SDL_RenderFillRects(renderer, inventorySlots, INVENTORY_MAX_SIZE);

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_SetTextureBlendMode(inventoryTexture, SDL_BLENDMODE_BLEND) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, inventoryTexture, NULL, &hudLoc) < 0) return FAILURE;
    return SUCCESS;
}