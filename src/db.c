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
    const char *q = "SELECT add_list($1)";
    const char *params[1] = { list_name };
    PGresult   *r = PQexecParams(gConn, q, 1, NULL, params, NULL, NULL, 0);

    ExecStatusType stat = PQresultStatus(r);

    gqbool suc = gqfalse;

    switch (stat) {
        case PGRES_TUPLES_OK:
            LOG_INFO("Succesfully added list with ID: %d", PQgetvalue(r, 0, 0));
            suc = gqtrue;
            break;

        default:
            LOG_ERROR("Failed to add list %s", PQerrorMessage(gConn));
            break;
    }

    PQclear(r);
    return suc;
}
