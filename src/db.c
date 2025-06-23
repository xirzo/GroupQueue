#include "db.h"
#include <assert.h>
#include <errno.h>
#include <libpq-fe.h>
#include <stddef.h>
#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"

static PGconn *gConn;

gqbool GQinitDB(const char *conninfo) {
    assert(conn != NULL);

    gConn = PQconnectdb(conninfo);

    if (PQstatus(gConn) == CONNECTION_BAD) {
        LOG_ERROR(
            "Failed to open postgres connection %s", PQerrorMessage(gConn)
        );

        PQfinish(gConn);
        return gqfalse;
    }

    LOG_INFO("Initalized database");
    return gqtrue;
}

void GQdestroyDB() {
    PQfinish(gConn);
    LOG_INFO("PostgreSQL connection finished");
}

gqbool GQaddList(GQlist list) {
    const char *add_list_q = "SELECT add_list($1)";
    const char *params[1] = { list.name };
    PGresult   *r =
        PQexecParams(gConn, add_list_q, 1, NULL, params, NULL, NULL, 0);

    ExecStatusType add_list_stat = PQresultStatus(r);

    if (add_list_stat != PGRES_TUPLES_OK) {
        LOG_ERROR("Failed to add list: %s", PQerrorMessage(gConn));
        PQclear(r);
        return gqfalse;
    }

    char *id_str = PQgetvalue(r, 0, 0);
    char *id_str_end = NULL;

    errno = 0;
    long long list_id = strtoll(id_str, &id_str_end, 10);

    if (id_str == id_str_end) {
        LOG_ERROR("Invalid number format: '%s'", id_str);
        PQclear(r);
        return gqfalse;
    } else if (errno != 0) {
        LOG_ERROR("Conversion error: %s ('%s')", strerror(errno), id_str);
        PQclear(r);
        return gqfalse;
    } else if (*id_str_end != '\0') {
        LOG_ERROR("Trailing characters in ID: '%s'", id_str_end);
        PQclear(r);
        return gqfalse;
    } else if (list_id <= 0) {
        LOG_ERROR("Returned ID is negative: %lld", list_id);
        PQclear(r);
        return gqfalse;
    }

    LOG_INFO("Successfully added list with ID: %lld", list_id);

    const char *add_users_q = "CALL add_list_users($1)";

    char list_name[16];
    snprintf(list_name, sizeof(list_name), "%lld", list_id);
    const char *add_users_params[1] = { list_name };
    PQexecParams(gConn, add_users_q, 1, NULL, add_users_params, NULL, NULL, 0);

    ExecStatusType add_users_stat = PQresultStatus(r);

    if (add_users_stat != PGRES_TUPLES_OK) {
        LOG_ERROR("Failed to add list users to list %s", PQerrorMessage(gConn));
        PQclear(r);
        return gqfalse;
    }

    PQclear(r);
    return gqtrue;
}

gqbool GQaddUser(GQuser user) {
    const char *q = "SELECT add_user($1, $2, $3, $4, $5)";

    char telegram_id_str[32];
    char is_admin_str[2] = "f";

    if (user.is_admin) {
        is_admin_str[0] = 't';
    }

    snprintf(
        telegram_id_str, sizeof(telegram_id_str), "%lld", user.telegram_id
    );

    const char *params[5] = { telegram_id_str,
                              user.first_name,
                              user.surname ? user.surname : "",
                              user.last_name ? user.last_name : "",
                              is_admin_str };

    PGresult *r = PQexecParams(gConn, q, 5, NULL, params, NULL, NULL, 0);

    ExecStatusType stat = PQresultStatus(r);

    if (stat != PGRES_TUPLES_OK) {
        LOG_ERROR("Failed to add user %s", PQerrorMessage(gConn));
        PQclear(r);
        return gqfalse;
    }

    char *id_str = PQgetvalue(r, 0, 0);
    char *id_str_end = NULL;

    errno = 0;
    long long user_id = strtoll(id_str, &id_str_end, 10);

    if (id_str == id_str_end) {
        LOG_ERROR("Invalid number format: '%s'", id_str);
        PQclear(r);
        return gqfalse;
    } else if (errno != 0) {
        LOG_ERROR("Conversion error: %s ('%s')", strerror(errno), id_str);
        PQclear(r);
        return gqfalse;
    } else if (*id_str_end != '\0') {
        LOG_ERROR("Trailing characters in ID: '%s'", id_str_end);
        PQclear(r);
        return gqfalse;
    } else if (user_id <= 0) {
        LOG_ERROR("Returned ID is negative: %lld", user_id);
        PQclear(r);
        return gqfalse;
    }

    LOG_INFO("Succesfully added user with ID: %lld", user_id);
    PQclear(r);
    return gqtrue;
}
