#include <libpq-fe.h>
#include <logger.h>
#include "db.h"

int main(void) {
    PGconn *conn = PQconnectdb(
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234"
    );

    if (PQstatus(conn) == CONNECTION_BAD) {
        LOG_ERROR(
            "Failed to open postgres connection %s", PQerrorMessage(conn)
        );
        PQfinish(conn);
        return 1;
    }

    GQinitDB(conn);
    LOG_INFO("PostgreSQL connection success");

    GQaddList("Test List Name");

    PQfinish(conn);
    LOG_INFO("PostgreSQL connection finished");
    return 0;
}

// TODO: load initial state with toml files
