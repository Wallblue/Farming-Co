#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "../../define.h"
#include "../items/inventory/inventory.h"

#define HOTBAR_LEN 10
#define SLOT_SIDE 50
#define HOTBAR_WIDTH (10 * (SLOT_SIDE + 10))
#define INV_LEFT_RIGHT_PADDING 30
#define INV_TOP_BOT_PADDING 30
#define INV_SPACE_BTWN_LINES 15
#define INV_SPACE_BTWN_SLOTS 10
#define INVENTORY_HUD_HEIGHT (3*SLOT_SIDE + 2*INV_SPACE_BTWN_LINES + 2*INV_TOP_BOT_PADDING) //3 inventory bars + 2 between spaces of 5px + 2 padding of 20px
#define INVENTORY_HUD_WIDTH (HOTBAR_WIDTH + 2*INV_LEFT_RIGHT_PADDING) //2 * padding of 20px
#define INVENTORY_HUD_X ((gameWidth - INVENTORY_HUD_WIDTH) / 2)
#define INVENTORY_HUD_Y ((gameHeight - INVENTORY_HUD_HEIGHT) / 2)

unsigned char printHotbarHUD(SDL_Renderer* renderer, SDL_Texture* hotbarTexture, unsigned char selectedSlot, Inventory inventory);
unsigned char printInventoryHUD(SDL_Renderer* renderer, SDL_Texture* inventoryTexture, Inventory inventory, char draggedItemIndex);
SDL_Texture* loadItemTextureFromBMP(char* sprite, SDL_Renderer* renderer);
unsigned char insertItemInSlot(SDL_Renderer* renderer, Item* item, SDL_Rect* slotRect);

char inventoryEventLoop(SDL_Renderer* renderer, Inventory inventory);
SDL_bool isMouseOnSlot(int xMouse, int yMouse);
unsigned char highlightSlot(SDL_Renderer* renderer, unsigned char nX, unsigned char nY, int r, int g, int b);
unsigned char dragItem(SDL_Renderer* renderer, int xMouse, int yMouse, Inventory inventory, char draggedItemIndex);
unsigned char refreshInventory(SDL_Renderer* renderer, SDL_Texture* rendererSave, SDL_Texture* inventoryTexture, Inventory inventory, int xMouse, int yMouse, char draggedItemIndex);

#endif