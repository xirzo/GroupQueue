#include <libpq-fe.h>
#include <logger.h>
#include "db.h"

int main(void) {
    PGconn *conn = PQconnectdb(
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234"
    );

    if (PQstatus(conn) == CONNECTION_BAD) {
        LOG_ERROR("failed to open postgres connection");
        PQfinish(conn);
        return 1;
    }

    LOG_INFO("connection success");

    PQfinish(conn);

    LOG_INFO("connection finished");
    return 0;
}

// TODO: load initial state with toml files
