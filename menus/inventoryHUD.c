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

