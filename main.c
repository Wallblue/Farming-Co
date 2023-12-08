#include <stdio.h>
#include <SDL.h>
#include "database/database.h"

void exitWithError(const char*);

int main(int argc, char **argv){
    createDatabase();
    return 0;
}

void exitWithError(const char* message) {
    SDL_Log("ERREUR : %s : %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}