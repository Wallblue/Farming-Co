//
// Created by Lina Phe on 21/01/2024.
//

#include "npc.h"

void
chat(SDL_Renderer *renderer, unsigned char npc, SDL_Texture *lightLayer, char *savedDialog, unsigned char hasInteracted,
     int *savedTrader) {
    SDL_Rect bgRect = {40, screenHeight/1.7, screenWidth-80, screenHeight/4};
    char npcName[10];
    unsigned char id;
    int trader;

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, 200);

    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &bgRect);

    switch(npc){
        case '0':
            sprintf(npcName, "%s", NPC1_NAME);
            id = 1;
            break;
        case '1':
            sprintf(npcName, "%s", NPC2_NAME);
            id = 2;
            break;
        case '2':
            sprintf(npcName, "%s", NPC3_NAME);
            id = 3;
            break;
        case '3':
            sprintf(npcName, "%s", NPC4_NAME);
            id = 4;
            break;
        case '4':
            sprintf(npcName, "%s", NPC5_NAME);
            id = 5;
            break;
    }

    SDL_Surface* nameSurface = loadItemSurface(nameSurface, npcName, 58, screenWidth-80);
    SDL_Texture* nameTexture = loadItemTexture(nameTexture, renderer, nameSurface);
    SDL_Rect nameRect = {60, screenHeight / 1.7 + 20, nameSurface->w, nameSurface->h };

    char *content;
    if(hasInteracted == 0){
        content = getContent(id);
        *savedTrader = isTrader(id);
        sprintf(savedDialog, "%s", content);
    }
    if(hasInteracted == 1)content = savedDialog;

    trader = *savedTrader;

    SDL_Surface* chatSurface = loadItemSurface(chatSurface, content, 42, screenWidth - 100);
    SDL_Texture* chatTexture = loadItemTexture(chatTexture, renderer, chatSurface);
    SDL_Rect chatRect = {60, screenHeight / 1.7 + 60, chatSurface->w, chatSurface->h };

    SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
    SDL_RenderCopy(renderer, chatTexture, NULL, &chatRect);

    if(trader == 1){
        SDL_Rect tradingRect = {screenWidth / 1.2 - 20, screenHeight / 2, screenWidth/7, screenHeight/12};

        SDL_SetRenderTarget(renderer, lightLayer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 10, 200);

        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, NULL);

        SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, lightLayer, NULL, &tradingRect);

        SDL_Surface* traderSurface = loadItemSurface(traderSurface, "Trade", 58, screenWidth - 80);
        SDL_Texture* traderTexture = loadItemTexture(traderTexture, renderer, traderSurface);
        SDL_Rect traderRect = {screenWidth / 1.2 - 6, screenHeight / 2 + 10, traderSurface->w, traderSurface->h };

        SDL_RenderCopy(renderer, traderTexture, NULL, &traderRect);

        SDL_FreeSurface(traderSurface);
        SDL_DestroyTexture(traderTexture);
    }

    SDL_FreeSurface(nameSurface);
    SDL_DestroyTexture(nameTexture);
    SDL_FreeSurface(chatSurface);
    SDL_DestroyTexture(chatTexture);

}

char* getContent(unsigned char id) {
    sqlite3_stmt* res;
    sqlite3* db;
    srand(time(NULL));
    int chosenDialog = rand() % 3 + 1;
    int i;

    if (openDb(&db) == FAILURE) {
        sqlite3_close(db);
        return NULL;
    }

    if (prepareRequest(db, "SELECT content FROM dialog WHERE npcId = ?1", &res) == FAILURE) {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return NULL;
    }

    sqlite3_bind_int(res, 1, id);

    for(i = 0; i<chosenDialog; i++) {
        sqlite3_step(res);
    }

    const char* content = (const char*)sqlite3_column_text(res, 0);
    char* result = strdup(content);
    sqlite3_finalize(res);
    sqlite3_close(db);

    return result;
}

int isTrader(unsigned char id){
    sqlite3_stmt* res;
    sqlite3* db;
    int rc;

    if(openDb(&db) == FAILURE){
        sqlite3_close(db);
        return FAILURE;
    }

    if(prepareRequest(db, "SELECT trader FROM npc WHERE npcId = ?1", &res) == FAILURE){
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }

    sqlite3_bind_int(res, 1, id);
    sqlite3_step(res);

    rc = sqlite3_column_int(res, 0);
    sqlite3_finalize(res);
    sqlite3_close(db);

    return rc;
}