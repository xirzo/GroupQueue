#include <logger.h>
#include "bool.h"
#include "db.h"
#include "server.h"

#define IP   "127.0.0.1"
#define PORT 5000

int main(void) {
    const char *conninfo =
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234";

    if (GQinitDB(conninfo) == gqfalse || GQinitServer(IP, PORT) == gqfalse) {
        return 1;
    }

    GQdeleteList(25);
    GQrunServer();

    GQdestroyServer();
    GQdestroyDB();
    return 0;
}

// TODO: load initial state with toml/json files

// TODO: swap users in list
// TODO: to end in list

//  TODO: add server endpoints for all of the new db functions

// TODO: rewrite all inline sql into schema.sql functions
