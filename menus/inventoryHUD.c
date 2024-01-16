#include "menu.h"
#include "../colors.h"
#include "../define.h"
#include <SDL.h>
#include <string.h>

unsigned char printHotbarHUD(SDL_Renderer* renderer, SDL_Texture* hotbarTexture, unsigned char selectedSlot, Inventory inventory){
    SDL_Rect hotbarSlot;
    SDL_Rect hotbarDest = {(gameWidth - HOTBAR_WIDTH) / 2, gameHeight - HOTBAR_HEIGHT - 10, HOTBAR_WIDTH, HOTBAR_HEIGHT};
    unsigned char i;

    if(SDL_SetRenderTarget(renderer, hotbarTexture) < 0) return FAILURE;

    SDL_SetRenderDrawColor(renderer, GREY, 128);
    for(i = 0; i < HOTBAR_LEN; i++){
        hotbarSlot.w = HOTBAR_HEIGHT;
        hotbarSlot.h = HOTBAR_HEIGHT;
        hotbarSlot.x = (HOTBAR_HEIGHT + 10) * i;
        hotbarSlot.y = 0;
        SDL_RenderFillRect(renderer, &hotbarSlot);

        if(inventory[i].id != 0) //If there's an item in this slot we load the sprite
            if(insertItemInSlot(renderer, inventory + i, &hotbarSlot) == FAILURE) return FAILURE;

        if(i == selectedSlot){
            SDL_SetRenderDrawColor(renderer, RED, 255);
            SDL_RenderDrawRect(renderer, &hotbarSlot);
            SDL_SetRenderDrawColor(renderer, GREY, 128);
        }
    }

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_SetTextureBlendMode(hotbarTexture, SDL_BLENDMODE_BLEND) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, hotbarTexture, NULL, &hotbarDest) < 0) return FAILURE;
    return SUCCESS;
}

unsigned char printInventoryHUD(SDL_Renderer* renderer, SDL_Texture* inventoryTexture, Inventory inventory){
    SDL_Rect inventorySlot;
    SDL_Rect hudLoc = {(gameWidth - INVENTORY_HUD_WIDTH) / 2, (gameHeight - INVENTORY_HUD_HEIGHT) / 2, INVENTORY_HUD_WIDTH, INVENTORY_HUD_HEIGHT};
    unsigned char i, j;

    if(SDL_SetRenderTarget(renderer, inventoryTexture) < 0) return FAILURE;

    SDL_SetRenderDrawColor(renderer, GREY, 128);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, DARK_GREY, 128);
    for(i = 0; i < INVENTORY_MAX_SIZE / HOTBAR_LEN; i++){
        for(j = 0; j < HOTBAR_LEN; j++){
            inventorySlot.w = HOTBAR_HEIGHT;
            inventorySlot.h = HOTBAR_HEIGHT;
            inventorySlot.x = INV_LEFT_RIGHT_PADDING + (HOTBAR_HEIGHT + 10) * j;
            inventorySlot.y = INV_TOP_BOT_PADDING + i * (HOTBAR_HEIGHT + INV_BETWEEN_LINES);
            SDL_RenderFillRect(renderer, &inventorySlot);

            if(inventory[i*10+j].id != 0) //If there's an item in this slot we load the sprite
                if(insertItemInSlot(renderer, inventory + (i*10+j), &inventorySlot) == FAILURE) return FAILURE;
        }
    }

    if(SDL_SetRenderTarget(renderer, NULL) < 0) return FAILURE;
    if(SDL_SetTextureBlendMode(inventoryTexture, SDL_BLENDMODE_BLEND) < 0) return FAILURE;
    if(SDL_RenderCopy(renderer, inventoryTexture, NULL, &hudLoc) < 0) return FAILURE;
    return SUCCESS;
}

SDL_Texture* loadTextureFromBMP(char* sprite, SDL_Renderer* renderer){
    SDL_Surface *surface = SDL_LoadBMP(sprite);
    if (surface == NULL) return NULL;

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    if(format == NULL) return NULL;
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(format, WHITE));

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL) return NULL;

    SDL_FreeFormat(format);
    SDL_FreeSurface(surface);
    return texture;
}

unsigned char insertItemInSlot(SDL_Renderer* renderer, Item* item, SDL_Rect* slotRect){
    SDL_Texture* sprite;
    char* spritePath;

    //Size of path is : spritesRoot + itemType + / + spriteName + \0
    spritePath = malloc((strlen("../assets/sprites/") + strlen(item->type) + 1 + strlen(item->sprite) + 1) * sizeof(char));
    strcpy(spritePath, "../assets/sprites/");
    strcat(strcat(strcat(spritePath, item->type), "/"), item->sprite);

    sprite = loadTextureFromBMP(spritePath, renderer);
    free(spritePath);
    if(sprite == NULL) return FAILURE;

    if(SDL_SetTextureBlendMode(sprite, SDL_BLENDMODE_BLEND) < 0) return FAILURE;
    if (SDL_RenderCopy(renderer, sprite, NULL, slotRect) < 0) return FAILURE;

    SDL_DestroyTexture(sprite);
    return SUCCESS;
}