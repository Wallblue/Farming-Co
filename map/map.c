//
// PHE Lina & BUSQUET Nicolas
// 27/12/2023
// But du programme :
// Gestion de la map
//

#include "map.h"


//Création de la templates des maps qui  fonctionnent avec les caractères ascii & l'utilisation
//d'une tilemap (ici tilemap de 16x16) -> se référencer sur map/sheets/utilisationSheets.txt

//fond de la map (herbe)
char* mapBg[] = {
        "0010:;0<1?0010:;0<1?0010:",
        "2:;0:205141001423:<10:<??",
        "100142300:;0<1?10:2050:<?",
        "000142312311302;0<1?10:<?",
        ":;10:<?0<101231130200010?",
        "2311:205141001423:<:2051?",
        "1001423311:20511231130200",
        "001231130200:;0<1?10:2051",
        "00:;0<1?101310:<?0200:;0<",
        "331231:;0000:;0<1?1302000",
        "423311:310:<?00:<?0200205",
        "331?10111:2051<1?10:20511",
        "<1?10:20500:;0<?10:21?101",
        "00:;0<000:;0<?10:21?11?10",
        "310:<?023311:20511003311:",
        "002?10:2:;0:20514:;0<1?10",
        "00:;0<1?2:;0:20:205<51410",
        "3311:205<1?10:20511:;0<1?",
        "00:;0<1?10310:<?02:;0<100",
        "0000:;0<1?:205<10:;0<1?10",
};

//objets sur la map (ex: clotures)
char* mapFg[] = {
        "/////////////////////////",
        "/////////////////////////",
        "///////////////1PPPPP3////",
        "///////////////://///:///",
        "///////////////://///:///",
        "///////////////://///:///",
        "///////////////EPP/PPG///",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
        "/////////////////////////",
};


//affichage d'une map
void printMap(SDL_Renderer *renderer, SDL_Texture *tileset, char **tab) {
    SDL_Rect dst;
    SDL_Rect src;
    src.w = tileHeightWidth;
    dst.w = dstHeightWidth;
    src.h = tileHeightWidth;
    dst.h = dstHeightWidth;

    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            dst.x = i * dstHeightWidth;
            dst.y = j * dstHeightWidth;
            src.x = (tab[j][i] - '0')%10 * tileHeightWidth;
            src.y = (tab[j][i] - '0')/10 * tileHeightWidth;
            SDL_RenderCopy(renderer, tileset, &src, &dst);
        }
    }
}

