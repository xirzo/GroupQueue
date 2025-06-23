#include "db.h"
#include <assert.h>
#include <libpq-fe.h>
#include <stddef.h>
#include <logger.h>

void GQinitDB(PGconn *conn) {
    assert(conn != NULL);
    gConn = conn;
    LOG_INFO("Database init");
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
