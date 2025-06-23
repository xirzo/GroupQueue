#include "bool.h"
#include "db.h"

int main(void) {
    const char *conninfo =
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234";

    if (GQinitDB(conninfo) == gqfalse) {
        return 1;
    }

    GQaddList("Test List Name");

    GQdestroyDB();
    return 0;
}

// TODO: load initial state with toml files
