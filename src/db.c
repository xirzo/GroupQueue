#include "db.h"
#include <assert.h>
#include <errno.h>
#include <libpq-fe.h>
#include <stddef.h>
#include <logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "types.h"

static PGconn *gConn;

static long long str_to_ll(const char *str) {
    errno = 0;
    char     *str_end = NULL;
    long long list_id = strtoll(str, &str_end, 10);

    if (str == str_end) {
        LOG_ERROR("Invalid number format: '%s'", str);
        return -1;
    } else if (errno != 0) {
        LOG_ERROR("Conversion error: %s ('%s')", strerror(errno), str);
        return -1;
    } else if (*str_end != '\0') {
        LOG_ERROR("Trailing characters in ID: '%s'", str_end);
        return -1;
    } else if (list_id <= 0) {
        LOG_ERROR("Returned ID is negative: %lld", list_id);
        return -1;
    }

    return list_id;
}

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

// FIX: add transcation for adding list_user and list
gqbool GQaddList(GQlist list) {
    const char *add_list_q = "SELECT add_list($1)";
    const char *params[1] = { list.list_name };
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

long long GQgetListId(const char *name) {
    const char *q = "SELECT list_id FROM list WHERE list_name = '$1'";
    const char *params[1] = { name };

    PGresult *r = PQexecParams(gConn, q, 1, NULL, params, NULL, NULL, 0);

    ExecStatusType stat = PQresultStatus(r);

    if (stat != PGRES_TUPLES_OK) {
        LOG_ERROR(
            "Failed to get list with name %s, %s", name, PQerrorMessage(gConn)
        );
        PQclear(r);
        return -1;
    }

    if (PQntuples(r) == 0) {
        LOG_ERROR("No list found with name %s", name);
        PQclear(r);
        return -1;
    }

    char     *id_str = PQgetvalue(r, 0, 0);
    long long list_id = str_to_ll(id_str);

    if (list_id == -1) {
        PQclear(r);
        return gqfalse;
    }

    PQclear(r);
    return list_id;
}

GQlist GQgetList(unsigned long long id) {
    const char *q = "SELECT * FROM list WHERE list_id = $1";

    GQlist result = {
        .list_id = -1,
        .list_name = NULL,
    };

    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%lld", id);
    const char *params[1] = { id_str };

    PGresult *r = PQexecParams(gConn, q, 1, NULL, params, NULL, NULL, 0);

    ExecStatusType stat = PQresultStatus(r);

    if (stat != PGRES_TUPLES_OK) {
        LOG_ERROR(
            "Failed to get list with id %s, %s", id_str, PQerrorMessage(gConn)
        );
        PQclear(r);
        return result;
    }

    if (PQntuples(r) == 0) {
        LOG_ERROR("No list found with id %s", id_str);
        PQclear(r);
        return result;
    }

    result.list_id = id;

    int name_col = PQfnumber(r, "list_name");

    if (name_col == -1) {
        LOG_ERROR("\"list_name\" column not found");
        PQclear(r);
        return result;
    }

    const char *list_name = PQgetvalue(r, 0, name_col);

    if (list_name == NULL) {
        LOG_ERROR("List name is NULL");
        PQclear(r);
        return result;
    }

    result.list_name = malloc(strlen(list_name) + 1);
    if (result.list_name == NULL) {
        LOG_ERROR("Failed to allocate memory for list name");
        PQclear(r);
        return result;
    }

    strcpy(result.list_name, list_name);

    PQclear(r);
    return result;
}

GQuser *GQgetUsersInList(unsigned long long id, int *count) {
    const char *q =
        "SELECT * FROM \"user\" WHERE user_id IN (SELECT user_id FROM list_user WHERE list_id = $1)";

    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%llu", id);

    const char *params[1] = { id_str };

    PGresult *r = PQexecParams(gConn, q, 1, NULL, params, NULL, NULL, 0);

    ExecStatusType stat = PQresultStatus(r);

    if (stat != PGRES_TUPLES_OK) {
        LOG_ERROR(
            "Failed to commit get list_users for list %llu: %s",
            id,
            PQerrorMessage(gConn)
        );
        PQclear(r);
        return NULL;
    }

    *count = PQntuples(r);
    GQuser *users = malloc(sizeof(GQuser) * *count);

    int user_id = PQfnumber(r, "user_id");
    int telegram_id = PQfnumber(r, "telegram_id");
    int first_name = PQfnumber(r, "first_name");
    int surname = PQfnumber(r, "surname");
    int last_name = PQfnumber(r, "last_name");
    int is_admin = PQfnumber(r, "is_admin");

    for (int i = 0; i < *count; i++) {
        GQuser *user = &users[i];
        user->user_id = str_to_ll(PQgetvalue(r, i, user_id));
        user->telegram_id = str_to_ll(PQgetvalue(r, i, telegram_id));

        user->first_name = strdup(PQgetvalue(r, i, first_name));
        user->surname = strdup(PQgetvalue(r, i, surname));
        user->last_name = strdup(PQgetvalue(r, i, last_name));

        char *is_adm = PQgetvalue(r, i, is_admin);

        if (is_adm[0] == 't') {
            user->is_admin = gqtrue;
        } else {
            user->is_admin = gqfalse;
        }
    }

    LOG_INFO("Successfully got list_users for list %llu", id);
    PQclear(r);
    return users;
}

void GQfreeUsersInList(GQuser *users, int count) {
    if (!users) {
        return;
    }

    for (int i = 0; i < count; i++) {
        GQuser *user = &users[i];
        free(user->first_name);
        free(user->last_name);
        free(user->surname);
    }

    free(users);
}

GQlist *GQgetAllLists(int *count) {
    const char *q = "SELECT * FROM list";

    PGresult *r = PQexec(gConn, q);

    ExecStatusType stat = PQresultStatus(r);

    if (stat != PGRES_TUPLES_OK) {
        LOG_ERROR("Failed to get all lists %s", PQerrorMessage(gConn));
        PQclear(r);
        return NULL;
    }

    int num_rows = PQntuples(r);

    GQlist *lists = malloc(sizeof(GQlist) * num_rows);

    if (!lists) {
        LOG_ERROR("Failed to allocate memory for lists");
        PQclear(r);
        return NULL;
    }

    int id_col = PQfnumber(r, "list_id");
    int name_col = PQfnumber(r, "list_name");

    if (name_col == -1) {
        LOG_ERROR("\"list_name\" column not found");
        PQclear(r);
        return NULL;
    }

    if (id_col == -1) {
        LOG_ERROR("\"list_id\" column not found");
        PQclear(r);
        return NULL;
    }

    for (int i = 0; i < num_rows; i++) {
        lists[i].list_name = NULL;

        const char *id_str = PQgetvalue(r, i, id_col);
        lists[i].list_id = str_to_ll(id_str);

        if (lists[i].list_id == -1) {
            LOG_ERROR("Failed to convert list id");
            continue;
        }

        const char *name = PQgetvalue(r, i, name_col);
        lists[i].list_name = strdup(name);

        if (!lists[i].list_name) {
            LOG_ERROR("Failed to allocate memory for list name");

            for (int j = 0; j < i; j++) {
                free(lists[j].list_name);
            }

            free(lists);
            PQclear(r);
            return NULL;
        }
    }

    *count = num_rows;
    PQclear(r);
    return lists;
}

void GQfreeAllLists(GQlist *lists, int count) {
    if (!lists) {
        return;
    }

    for (int i = 0; i < count; i++) {
        if (lists[i].list_name) {
            free(lists[i].list_name);
        }
    }

    free(lists);
}

gqbool GQdeleteList(unsigned long long id) {
    PGresult *tr = PQexec(gConn, "BEGIN");

    if (PQresultStatus(tr) != PGRES_COMMAND_OK) {
        LOG_ERROR("Failed to start transaction: %s", PQerrorMessage(gConn));
        PQclear(tr);
        return gqfalse;
    }

    PQclear(tr);

    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%llu", id);
    const char *params[1] = { id_str };

    const char *q_users = "DELETE FROM list_user WHERE list_id = $1";

    PGresult *r_users =
        PQexecParams(gConn, q_users, 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(r_users) != PGRES_COMMAND_OK) {
        LOG_ERROR("Failed to delete list users: %s", PQerrorMessage(gConn));
        PQexec(gConn, "ROLLBACK");
        PQclear(r_users);
        return gqfalse;
    }

    char *users_affected = PQcmdTuples(r_users);
    int   users_count = atoi(users_affected);
    PQclear(r_users);

    const char *q = "DELETE FROM list WHERE list_id = $1";
    PGresult   *r = PQexecParams(gConn, q, 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(r) != PGRES_COMMAND_OK) {
        LOG_ERROR("Failed to delete list: %s", PQerrorMessage(gConn));
        PQexec(gConn, "ROLLBACK");
        PQclear(r);
        return gqfalse;
    }

    char *rows_affected = PQcmdTuples(r);
    int   list_affected = atoi(rows_affected);
    PQclear(r);

    if (list_affected == 0) {
        LOG_INFO("No list found with id %llu to delete", id);
        PQexec(gConn, "ROLLBACK");
        return gqfalse;
    }

    PGresult *commit = PQexec(gConn, "COMMIT");
    if (PQresultStatus(commit) != PGRES_COMMAND_OK) {
        LOG_ERROR("Failed to commit transaction: %s", PQerrorMessage(gConn));
        PQclear(commit);
        return gqfalse;
    }

    PQclear(commit);

    LOG_INFO(
        "Deleted list with id %llu and %d associated users", id, users_count
    );

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

    char     *id_str = PQgetvalue(r, 0, 0);
    long long user_id = str_to_ll(id_str);

    if (user_id == -1) {
        PQclear(r);
        return gqfalse;
    }

    LOG_INFO("Succesfully added user with ID: %lld", user_id);
    PQclear(r);
    return gqtrue;
}

gqbool GQswapListUsers(
    unsigned long long list_id,
    unsigned long long user_a_id,
    unsigned long long user_b_id
) {
    PGresult *tr = PQexec(gConn, "BEGIN");

    if (PQresultStatus(tr) != PGRES_COMMAND_OK) {
        LOG_ERROR("Failed to start transaction: %s", PQerrorMessage(gConn));
        PQclear(tr);
        return gqfalse;
    }

    PQclear(tr);

    const char *q =
        "WITH orders AS ("
        "  SELECT a.user_order AS order_a, b.user_order AS order_b "
        "  FROM list_user a, list_user b "
        "  WHERE a.list_id = $1 AND b.list_id = $1 "
        "    AND a.user_id = $2 AND b.user_id = $3"
        ")"
        "UPDATE list_user "
        "SET user_order = CASE "
        "  WHEN user_id = $2 THEN (SELECT order_b FROM orders) "
        "  WHEN user_id = $3 THEN (SELECT order_a FROM orders) "
        "  ELSE user_order "
        "END "
        "WHERE list_id = $1 AND user_id IN ($2, $3) "
        "RETURNING list_user_id";

    char list_id_str[32];
    char user_a_str[32];
    char user_b_str[32];

    snprintf(list_id_str, sizeof(list_id_str), "%llu", list_id);
    snprintf(user_a_str, sizeof(user_a_str), "%llu", user_a_id);
    snprintf(user_b_str, sizeof(user_b_str), "%llu", user_b_id);

    const char *params[3] = { list_id_str, user_a_str, user_b_str };

    PGresult *r = PQexecParams(gConn, q, 3, NULL, params, NULL, NULL, 0);

    ExecStatusType stat = PQresultStatus(r);

    if (stat != PGRES_TUPLES_OK) {
        LOG_ERROR("Failed to swap user orders: %s", PQerrorMessage(gConn));
        PQexec(gConn, "ROLLBACK");
        PQclear(r);
        return gqfalse;
    }

    int rows_updated = PQntuples(r);
    PQclear(r);

    if (rows_updated != 2) {
        LOG_ERROR(
            "Expected to update 2 rows, but updated %d rows", rows_updated
        );
        PQexec(gConn, "ROLLBACK");
        return gqfalse;
    }

    PGresult *commit = PQexec(gConn, "COMMIT");

    if (PQresultStatus(commit) != PGRES_COMMAND_OK) {
        LOG_ERROR("Failed to commit transaction: %s", PQerrorMessage(gConn));
        PQclear(commit);
        return gqfalse;
    }

    PQclear(commit);

    LOG_INFO(
        "Successfully swapped orders for users %llu and %llu in list %llu",
        user_a_id,
        user_b_id,
        list_id
    );

    return gqtrue;
}
