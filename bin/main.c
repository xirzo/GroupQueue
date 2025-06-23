#include "bool.h"
#include "db.h"
#include "types.h"

int main(void) {
    const char *conninfo =
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234";

    if (GQinitDB(conninfo) == gqfalse) {
        return 1;
    }

    GQaddList((GQlist){ .name = "Test List" });

    GQaddUser((GQuser){
        .telegram_id = 618214141,
        .first_name = "Aleks",
        .surname = "Lol",
        .last_name = "Kekovich",
        .is_admin = gqtrue,
    });

    GQdestroyDB();
    return 0;
}

// TODO: load initial state with toml files
