#include "../menu.h"

unsigned char seeWallet(SDL_Renderer* renderer, SDL_Texture* lightLayer){
    sqlite3_stmt* res;
    sqlite3* db;
    int rc;

    SDL_Rect walletRect = {696, 64, 93, 36};

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 150);

    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &walletRect);

    if(openDb(&db) == FAILURE){
        sqlite3_close(db);
        return -1;
    }
    if(prepareRequest(db, "SELECT money FROM player WHERE playerId = 1", &res) == FAILURE){
        sqlite3_close(db);
        return -1;
    }

    rc = sqlite3_step(res);
    if(rc != SQLITE_ROW){
        fprintf(stderr, "Can't get current day");
        sqlite3_close(db);
        return -1;
    }
    rc = sqlite3_column_int(res, 0);
    sqlite3_finalize(res);
    sqlite3_close(db);

    char moneyText[20];
    sprintf(moneyText, "%d$", rc);

    SDL_Surface* moneySurface = loadItemSurface(moneySurface, moneyText, 48, 93);
    SDL_Texture* moneyTexture = loadItemTexture(moneyTexture, renderer, moneySurface);
    SDL_Rect nameRect = {696, 70, moneySurface->w, moneySurface->h };

    SDL_RenderCopy(renderer, moneyTexture, NULL, &nameRect);

    SDL_FreeSurface(moneySurface);
    SDL_DestroyTexture(moneyTexture);

}