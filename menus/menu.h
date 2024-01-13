#ifndef MENU_H
#define MENU_H

#include <SDL.h>

#define HOTBAR_LEN 10
#define HOTBAR_HEIGHT 50
#define HOTBAR_WIDTH (10 * (HOTBAR_HEIGHT + 10))

unsigned char printHotbarHUD(SDL_Renderer* renderer, SDL_Texture* hotbarTexture, unsigned char selectedSlot);

#endif