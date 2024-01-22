#include "player.h"

unsigned char alterMoneyAmount(int quantity, sqlite3* db){ //Add or subtract quantity depending on mode
    sqlite3_stmt* res;
    int rc;
    char homeMadeDb = 0;

    if(db == NULL){
        if(openDb(&db) == FAILURE) returnProperlyM(db, res, "SQL Error : %s\n", FAILURE);
        homeMadeDb = 1;
    }

    if(prepareRequest(db, "UPDATE player SET money = money + ?1 WHERE playerId = ?2;", &res) == FAILURE) returnProperlyM(db, res, "SQL Error : %s\n", FAILURE);
    sqlite3_bind_int(res, 1, quantity);
    sqlite3_bind_int(res, 2, playerId);

    rc = sqlite3_step(res);
    if(rc != SQLITE_DONE) returnProperlyM(db, res, "SQL Error : %s\n", FAILURE);

    if(homeMadeDb == 1) sqlite3_close(db);
    returnProperly(NULL, res, NULL, SUCCESS);
}

int getWallet(sqlite3* db){
    int rc;
    sqlite3_stmt* res;
    char homeMadeDb = 0;

    if(db == NULL){
        if(openDb(&db) == FAILURE) returnProperlyM(db, NULL, "SQL Error : %s\n", FAILURE);
        homeMadeDb = 1;
    }

    if(prepareRequest(db, "SELECT money FROM player WHERE playerId = ?1;", &res) == FAILURE) returnProperlyM(db, res, "SQL error : %s\n", FAILURE);
    sqlite3_bind_int(res, 1, playerId);
    rc = sqlite3_step(res);
    if(rc != SQLITE_ROW) returnProperlyM(db, res,"SQL error : %s\n", FAILURE);
    rc = sqlite3_column_int(res, 0);
    sqlite3_finalize(res);
    if(homeMadeDb == 1) sqlite3_close(db);
    return rc;
}

char isWalletFilledEnough(int quantity, sqlite3* db){
    return (char) (getWallet(db) - quantity >= 0);
}