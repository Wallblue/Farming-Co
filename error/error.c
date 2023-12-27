//
// PHE Lina & Nicolas Busquet
// 27/12/2023
// Gestion d'erreurs
//

#include "error.h"

void exitWithError(const char* message) {
    SDL_Log("ERREUR : %s : %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}