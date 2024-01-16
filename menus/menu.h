#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "../items/inventory/inventory.h"

#define HOTBAR_LEN 10
#define HOTBAR_HEIGHT 50
#define HOTBAR_WIDTH (10 * (HOTBAR_HEIGHT + 10))
#define INV_LEFT_RIGHT_PADDING 30
#define INV_TOP_BOT_PADDING 30
#define INV_BETWEEN_LINES 15
#define INVENTORY_HUD_HEIGHT (3*HOTBAR_HEIGHT + 2*INV_BETWEEN_LINES + 2*INV_TOP_BOT_PADDING) //3 inventory bars + 2 between spaces of 5px + 2 padding of 20px
#define INVENTORY_HUD_WIDTH (HOTBAR_WIDTH + 2*INV_LEFT_RIGHT_PADDING) //2 * padding of 20px

unsigned char printHotbarHUD(SDL_Renderer* renderer, SDL_Texture* hotbarTexture, unsigned char selectedSlot, Inventory inventory);
unsigned char printInventoryHUD(SDL_Renderer* renderer, SDL_Texture* inventoryTexture, Inventory inventory);
SDL_Texture* loadTextureFromBMP(char* sprite, SDL_Renderer* renderer);
unsigned char insertItemInSlot(SDL_Renderer* renderer, Item* item, SDL_Rect* slotRect);

char inventoryEventLoop(SDL_Renderer* renderer, Inventory inventory);

#endif