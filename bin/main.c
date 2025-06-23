#include "bool.h"
#include "db.h"

int main(void) {
    const char *conninfo =
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234";

    if (GQinitDB(conninfo) == gqfalse) {
        return 1;
    }

    GQaddUser((GQuser){
        .telegram_id = 618214141,
        .first_name = "Aleks",
        .surname = "Lol",
        .last_name = "Kekovich",
        .is_admin = gqtrue,
    });

    GQaddUser((GQuser){
        .telegram_id = 618211245,
        .first_name = "Bebr",
        .surname = "Kek",
        .last_name = "Lolovich",
        .is_admin = gqfalse,
    });

    GQaddList((GQlist){ .name = "Another List 1" });
    GQaddList((GQlist){ .name = "Another List 2" });

    GQdestroyDB();
    return 0;
}

// TODO: load initial state with toml files
// TODO: remove list
