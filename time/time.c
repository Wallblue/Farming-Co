//
// PHE Lina & BUSQUET Nicolas
// 07/01/2024
// Création du système de temps in game
//

#include "time.h"


int day(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;
    int* getHours = threadData->timeInGame;
    int* sleep = threadData->sleep;
    char* pause = threadData->pause;

    Uint64 startTime = SDL_GetTicks64();
    Uint64 pauseTime = 0;
    do {
        SDL_Delay(1); //For CPU's sake

        Uint64 currentTime = SDL_GetTicks64();
        Uint64 elapsedTime = currentTime - startTime;

        if(*pause == 1 && startTime){
            pauseTime = SDL_GetTicks64() - startTime;
            startTime = 0;
        }

        if(*pause == 0) {
            if(pauseTime){
                startTime = currentTime - pauseTime;
                pauseTime = 0;
            }
            //remplacer 1000*60 par 1000 pour rester en secondes
            if ((elapsedTime / (1000*60)) == *getHours + 1) {
                (*getHours)++;
            }

            if (*getHours == 24) {
                *sleep = 1;
            }

            if (*sleep == 1) {
                if(getSleep(data)== FAILURE)exitWithError("Database couldn't update today's date.");
                startTime = currentTime;
            }
        }

    } while (*getHours < 25);

    return 0;
}

void applyFilter(SDL_Renderer * renderer, const int * timeInGame, SDL_Texture *lightLayer){
    SDL_Rect gameRect = {0, 0, screenWidth, screenHeight};
    int nightFilter = 0;

    if(*timeInGame >= 12){
        nightFilter = 15 * (*timeInGame/2);
    }

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, nightFilter);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &gameRect);
}

void seeTime(SDL_Renderer *renderer, const int* timeInGame) {
    TTF_Font* font = loadFont(64);

    SDL_Color textColor = {BLACK};
    SDL_Rect bgRect = {696, 14, 93, 36};

    char text[20];
    if(*timeInGame < 17) {
        if(*timeInGame+7 > 9)
            snprintf(text, sizeof(text), "%d:00", *timeInGame + 7);
        else
            snprintf(text, sizeof(text), "0%d:00", *timeInGame + 7);
    }
    else
        snprintf(text, sizeof(text), "0%d:00", *timeInGame - 17);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if(!surface) exitWithError("Erreur d'initialisation de la surface");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) exitWithError("Erreur de chargement de la texture");

    SDL_Rect textRect = {700, 16, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void pauseMenu(SDL_Renderer *renderer, SDL_Texture* lightLayer){
    /* Création du fond noir en transparence*/
    SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};

    SDL_SetRenderTarget(renderer, lightLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 10, 100);

    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, lightLayer, NULL, &bgRect);

    /* Création du carré du menu*/
    SDL_Rect menuRect = {screenWidth/4+48, screenHeight/6+32, 300, 300};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &menuRect);

    /*Texte du menu*/
    TTF_Font* font = loadFont(64);
    SDL_Color color = {BLACK};

    SDL_Surface * pauseSurface = TTF_RenderText_Solid(font, "Pause", color);
    if(!pauseSurface) exitWithError("Erreur d'initialisation de la surface");

    SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
    if (!pauseTexture) exitWithError("Erreur de chargement de la texture");

    SDL_Rect pauseRect = {360, screenHeight/6 + 48, pauseSurface->w, pauseSurface->h};

    SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);

    /*Boutton du menu*/
    //Resume
    SDL_Rect pauseButton = {300, screenHeight/3+16, 200, 50};

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &pauseButton);

    SDL_Surface* resumeSurface = TTF_RenderText_Solid(font, "Resume", color);
    if(!resumeSurface)exitWithError("Erreur d'initialisation de la surface");

    SDL_Texture* resumeTexture = SDL_CreateTextureFromSurface(renderer, resumeSurface);
    if(!resumeTexture)exitWithError("Erreur d'initialisation de la texture");

    SDL_Rect resumeRect = {350, screenHeight/3+22, resumeSurface->w, resumeSurface->h};

    SDL_RenderCopy(renderer, resumeTexture, NULL, &resumeRect);

    //Quit
    SDL_Rect quitButton = {300, screenHeight/2+16, 200, 50};

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &quitButton);

    SDL_Surface* quitSurface = TTF_RenderText_Solid(font, "Quit", color);
    if(!quitSurface)exitWithError("Erreur d'initialisation de la surface");

    SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);
    if(!quitTexture)exitWithError("Erreur d'initialisation de la texture");

    SDL_Rect quitRect = {364 , screenHeight/2+22, quitSurface->w, quitSurface->h};

    SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

    SDL_DestroyTexture(pauseTexture);
    SDL_FreeSurface(pauseSurface);
    SDL_DestroyTexture(resumeTexture);
    SDL_FreeSurface(resumeSurface);
    TTF_CloseFont(font);

}


int getDateInGame(){
    sqlite3_stmt* res;
    sqlite3* db;
    int rc;
    if(openDb(&db) == FAILURE){
        sqlite3_close(db);
        return -1;
    }

    if(prepareRequest(db, "SELECT timeInGame FROM player WHERE playerId = 1", &res) == FAILURE){
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
    return rc;
}

unsigned char getSleep(struct ThreadData* data){
    *data->sleep = 0;
    (*data->todayDate)++;
    *data->timeInGame = 0;
    if(updateDate(*data->todayDate)== FAILURE) return FAILURE;
    if(updatePlants(*data->todayDate)==FAILURE) return FAILURE;
    if(updateMisc(*data->todayDate) == FAILURE) return FAILURE;
    return SUCCESS;
}

unsigned char updateDate(int todayDate){
    sqlite3_stmt* res;
    int rc;
    sqlite3* db;

    if(openDb(&db) == FAILURE) {
        fprintf(stderr, "Couldn't open Database");
        return FAILURE;
    }

    if(prepareRequest(db, "UPDATE player SET timeInGame = ?1", &res) == FAILURE) {
        sqlite3_close(db);
        return FAILURE;
    }

    sqlite3_bind_int(res, 1, todayDate);
    rc = sqlite3_step(res);

    if(rc != SQLITE_DONE){
        fprintf(stderr, "Couldn't update Database");
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
    return SUCCESS;
}

unsigned char updatePlants(int todayDate) {
    sqlite3* db;
    sqlite3_stmt* res;
    int objectId, nextState, poseDate, growTime;

    if (openDb(&db) == FAILURE) {
        fprintf(stderr, "Couldn't open Database");
        return FAILURE;
    }

    const char* query = "SELECT object.objectId, object.x, object.y, object.zone, object.growTime, object.poseDate, object.state, item.linkedObjectSpriteRef FROM object, item WHERE object.growTime > 0 AND item.itemId = object.itemId";

    if (prepareRequest(db, query, &res) == FAILURE) {
        fprintf(stderr, "Couldn't prepare query: %s", query);
        sqlite3_close(db);
        return FAILURE;
    }

    while (sqlite3_step(res) == SQLITE_ROW) {
        nextState = sqlite3_column_int(res, 6) + 1;
        poseDate = sqlite3_column_int(res, 5);
        growTime = sqlite3_column_int(res, 4);

        if(sqlite3_column_int(res, 6) == 3 || //If the plant is already on final nextState we skip it or
        ((todayDate - poseDate) * 3 < nextState * growTime)) // if the plant has not reached the next state
            continue;

        objectId = sqlite3_column_int(res, 0);

        if (updateSprite(db, objectId, nextState) == FAILURE) {
            fprintf(stderr, "Failed to updateSprite for objectId: %d\n", objectId);
            sqlite3_finalize(res);
            sqlite3_close(db);
            return FAILURE;
        }

        switch (sqlite3_column_int(res, 3)) {
           case 2:
                mapObjects3[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char)(*sqlite3_column_text(res, 7) + nextState);
                break;
            case 3:
                mapObjects4[sqlite3_column_int(res, 2)][sqlite3_column_int(res, 1)] = (char)(*sqlite3_column_text(res, 7) + nextState);
                break;
        }
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
    return SUCCESS;
}

unsigned char updateSprite(sqlite3* db, int objectId, int state) {
    sqlite3_stmt* res;
    int rc;

    const char* query = "UPDATE object SET state = ?1 WHERE objectId = ?2";

    if (prepareRequest(db, query, &res) == FAILURE) {
        fprintf(stderr, "Couldn't prepare query: %s", query);
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }

    sqlite3_bind_int(res, 1, state);
    sqlite3_bind_int(res, 2, objectId);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Couldn't update object state in Database: %s", sqlite3_errmsg(db));
        sqlite3_finalize(res);
        sqlite3_close(db);
        return FAILURE;
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
    return SUCCESS;
}

