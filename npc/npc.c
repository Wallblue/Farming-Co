//
// Created by Lina Phe on 21/01/2024.
//

#include "npc.h"

void chat(SDL_Renderer *renderer, unsigned char npc, SDL_Texture* lightLayer){
    SDL_Rect bgRect = {40, screenHeight/1.7, screenWidth-80, screenHeight/4};
    char npcName[10];

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, 200);

    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &bgRect);

    switch(npc){
        case '0':
            sprintf(npcName, "%s", NPC1_NAME);
            break;
        case '1':
            sprintf(npcName, "%s", NPC2_NAME);
            break;
        case '2':
            sprintf(npcName, "%s", NPC3_NAME);
            break;
        case '3':
            sprintf(npcName, "%s", NPC4_NAME);
            break;
        case '4':
            sprintf(npcName, "%s", NPC5_NAME);
            break;
    }

    SDL_Surface* nameSurface = loadItemSurface(nameSurface, npcName, 58, screenWidth-80);
    SDL_Texture* nameTexture = loadItemTexture(nameTexture, renderer, nameSurface);
    SDL_Rect nameRect = {60, screenHeight / 1.7 + 20, nameSurface->w, nameSurface->h };


    SDL_Surface* chatSurface = loadItemSurface(chatSurface, "Bonjour je suis un NPC", 42, screenWidth - 80);
    SDL_Texture* chatTexture = loadItemTexture(chatTexture, renderer, chatSurface);
    SDL_Rect chatRect = {60, screenHeight / 1.7 + 60, chatSurface->w, chatSurface->h };

    SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
    SDL_RenderCopy(renderer, chatTexture, NULL, &chatRect);

    SDL_FreeSurface(nameSurface);
    SDL_DestroyTexture(nameTexture);
    SDL_FreeSurface(chatSurface);
    SDL_DestroyTexture(chatTexture);
}