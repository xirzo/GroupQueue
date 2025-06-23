#include "db.h"
#include <assert.h>
#include <libpq-fe.h>
#include <stddef.h>
#include <logger.h>
#include "bool.h"

gqbool GQinitDB(const char *conninfo) {
    assert(conn != NULL);
    LOG_INFO("Stated database init");

    gConn = PQconnectdb(conninfo);

    if (PQstatus(gConn) == CONNECTION_BAD) {
        LOG_ERROR(
            "Failed to open postgres connection %s", PQerrorMessage(gConn)
        );

        PQfinish(gConn);
        return gqfalse;
    }

    LOG_INFO("Succesfully inited database");
    return gqtrue;
}

void GQdestroyDB() {
    PQfinish(gConn);
    LOG_INFO("PostgreSQL connection finished");
}

gqbool GQaddList(const char *list_name) {
    char     *q = "SELECT * FROM list";
    PGresult *r = PQexec(gConn, q);

    ExecStatusType stat = PQresultStatus(r);

    switch (stat) {
        case PGRES_COMMAND_OK:
        case PGRES_TUPLES_OK:
            LOG_INFO("Succesfully added list");
            return gqtrue;

        default:
            LOG_ERROR("Failed to add list %s", PQerrorMessage(gConn));
            return gqfalse;
    }
}
