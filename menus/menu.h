#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <sqlite3.h>
#include "../../define.h"
#include "../items/inventory/inventory.h"
#include "../main.h"
#include "../player/player.h"
#include "../time/time.h"
#include <string.h>


#define HOTBAR_LEN 10
#define SLOT_SIDE 50
#define HOTBAR_WIDTH (10 * (SLOT_SIDE + 10))
#define INV_LEFT_RIGHT_PADDING 30
#define INV_TOP_BOT_PADDING 30
#define INV_SPACE_BTWN_LINES 15
#define INV_SPACE_BTWN_SLOTS 10
#define INVENTORY_HUD_HEIGHT (3*SLOT_SIDE + 2*INV_SPACE_BTWN_LINES + 2*INV_TOP_BOT_PADDING) //3 inventory bars + 2 between spaces of 5px + 2 padding of 20px
#define INVENTORY_HUD_WIDTH (HOTBAR_WIDTH + 2*INV_LEFT_RIGHT_PADDING) //2 * padding of 20px
#define DESC_BOX_WIDTH (INVENTORY_HUD_WIDTH / 4)

#define WALLET_X CLOCK_X
#define WALLET_Y 64
#define WALLET_WIDTH CLOCK_WIDTH
#define WALLET_HEIGHT CLOCK_HEIGHT

unsigned char printHotbarHUD(SDL_Renderer* renderer, SDL_Texture* hotbarTexture, unsigned char selectedSlot, Item inventory[30]);
unsigned char printInventoryHUD(SDL_Renderer* renderer, Item inventory[30], char draggedItemIndex, int xHud, int yHud);
SDL_Texture* loadItemTextureFromBMP(char* sprite, SDL_Renderer* renderer);
unsigned char insertItemInSlot(SDL_Renderer* renderer, Item* item, SDL_Rect* slotRect);

char inventoryEventLoop(SDL_Renderer* renderer, Inventory* inventory, Inventory* secondInventory);
SDL_bool isMouseOnSlot(int xMouse, int yMouse, int xHud, int yHud);
unsigned char highlightSlot(SDL_Renderer* renderer, unsigned char nX, unsigned char nY, int r, int g, int b, int xHud, int yHud);
unsigned char dragItem(SDL_Renderer* renderer, int xMouse, int yMouse, Item inventory[30], char draggedItemIndex);
unsigned char refreshInventory(SDL_Renderer *renderer, SDL_Texture *rendererSave, Inventory* inventory, Inventory* secondInventory, Inventory* heldInventory,
                 int xHud, int yHud, int xMouse, int yMouse, char draggedItemIndex);
void displayDescriptionBox(SDL_Renderer* renderer, unsigned char nX, unsigned char nY, int xHud, int yHud);
void seeItemData(SDL_Renderer* renderer, Item *heldItem, unsigned char nX, unsigned char nY, int xHud, int yHud);

unsigned char seeWallet(SDL_Renderer* renderer, SDL_Texture* lightLayer);
unsigned char buyItem(Inventory* playerInventory, Inventory* npcInventory, char index);
unsigned char sellItem(Inventory* playerInventory, char index);
#endif