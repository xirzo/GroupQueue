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

    GQrunServer();

    GQdestroyServer();
    GQdestroyDB();
    return 0;
}

// TODO: load initial state with toml/json files

// TODO: to end in list

// TODO: rewrite all inline sql into schema.sql functions
// TODO: put all of the sql files into one schema

// TODO: admin functions
// TODO: auth system for admins
