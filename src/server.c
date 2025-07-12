#include "server.h"

#include <cjson/cJSON.h>
#include <event2/buffer.h>
#include <logger.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <evhttp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "db.h"
#include "types.h"

#define BUFFER_SIZE 128

static struct event_base *gBase;
static struct evhttp     *gServer;
static struct event      *sSigInt;

static void undefined_uri_callback(struct evhttp_request *req, void *ctx) {
    LOG_ERROR("Called callback of unset URI");
    evhttp_send_reply(req, HTTP_NOTFOUND, NULL, NULL);
}

static void health_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Health callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();
    evbuffer_add_printf(reply, "OK");
    evhttp_send_reply(req, HTTP_OK, NULL, reply);
    evbuffer_free(reply);
}

static void add_list_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Add list callback!, %s", req->uri);

    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);

    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at add list");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    LOG_INFO("Add list json parsed: %s", cJSON_Print(json));

    const cJSON *list_name =
        cJSON_GetObjectItemCaseSensitive(json, "list_name");

    if (!cJSON_IsString(list_name) || (list_name->valuestring == NULL)) {
        LOG_ERROR("Failed to get string from add list json");
        evbuffer_add_printf(
            reply, "Failed to get \"list_name\" string from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    LOG_INFO("Adding list with name %s", list_name->valuestring);

    if (GQaddList((GQlist){
            .list_name = list_name->valuestring,
        })
        == gqfalse) {
        LOG_ERROR("Failed to add list with name %s", list_name->valuestring);

        evbuffer_add_printf(
            reply, "Failed to add list, likely it is already present in the db"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    LOG_INFO("Added list with name %s", list_name->valuestring);

    evbuffer_add_printf(reply, "List was aRofdded");
    evhttp_send_reply(req, HTTP_OK, NULL, reply);

end:
    evbuffer_free(reply);
    cJSON_Delete(json);
}

static void get_list_id_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Get list ID callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);
    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at get list ID");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    const cJSON *list_name =
        cJSON_GetObjectItemCaseSensitive(json, "list_name");

    if (!cJSON_IsString(list_name) || (list_name->valuestring == NULL)) {
        LOG_ERROR("Failed to get string from get list ID json");
        evbuffer_add_printf(
            reply, "Failed to get \"list_name\" string from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    long long list_id = GQgetListId(list_name->valuestring);

    if (list_id == -1) {
        LOG_ERROR("Failed to get list ID for name %s", list_name->valuestring);
        evbuffer_add_printf(reply, "List not found");
        evhttp_send_reply(req, HTTP_NOTFOUND, NULL, reply);
        goto end;
    }

    cJSON *response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "list_id", (double)list_id);

    evbuffer_add_printf(reply, "%s", cJSON_Print(response));
    evhttp_send_reply(req, HTTP_OK, NULL, reply);
    cJSON_Delete(response);

end:
    evbuffer_free(reply);
    cJSON_Delete(json);
}

static void get_list_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Get list callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);
    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at get list");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    const cJSON *list_id_json =
        cJSON_GetObjectItemCaseSensitive(json, "list_id");

    if (!cJSON_IsNumber(list_id_json)) {
        LOG_ERROR("Failed to get list_id from get list json");
        evbuffer_add_printf(
            reply, "Failed to get \"list_id\" number from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    unsigned long long list_id = (unsigned long long)list_id_json->valuedouble;
    GQlist             list = GQgetList(list_id);

    if (list.list_id == (unsigned long long)-1) {
        LOG_ERROR("Failed to get list details for ID %llu", list_id);
        evbuffer_add_printf(reply, "List not found");
        evhttp_send_reply(req, HTTP_NOTFOUND, NULL, reply);
        goto end;
    }

    cJSON *response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "list_id", (double)list.list_id);
    cJSON_AddStringToObject(response, "list_name", list.list_name);

    evbuffer_add_printf(reply, "%s", cJSON_Print(response));
    evhttp_send_reply(req, HTTP_OK, NULL, reply);
    cJSON_Delete(response);

end:
    evbuffer_free(reply);
    cJSON_Delete(json);
}

static void get_list_users_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Get list users callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);
    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at get list users");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    const cJSON *list_id_json =
        cJSON_GetObjectItemCaseSensitive(json, "list_id");

    if (!cJSON_IsNumber(list_id_json)) {
        LOG_ERROR("Failed to get list_id from get list users json");
        evbuffer_add_printf(
            reply, "Failed to get \"list_id\" number from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    unsigned long long list_id = (unsigned long long)list_id_json->valuedouble;

    int         count = 0;
    GQlistUser *list_users = GQgetListUsers(list_id, &count);

    if (list_users == NULL) {
        LOG_ERROR("Failed to get users for list ID %llu", list_id);
        evbuffer_add_printf(reply, "Failed to get users for the list");
        evhttp_send_reply(req, HTTP_NOTFOUND, NULL, reply);
        goto end;
    }

    cJSON *response = cJSON_CreateObject();
    cJSON *users_array = cJSON_AddArrayToObject(response, "users");

    for (int i = 0; i < count; i++) {
        cJSON *user_obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(
            user_obj, "user_id", (double)list_users[i].user.user_id
        );
        cJSON_AddNumberToObject(
            user_obj, "telegram_id", (double)list_users[i].user.telegram_id
        );
        cJSON_AddStringToObject(
            user_obj, "first_name", list_users[i].user.first_name
        );
        cJSON_AddStringToObject(
            user_obj, "surname", list_users[i].user.surname
        );
        cJSON_AddStringToObject(
            user_obj, "last_name", list_users[i].user.last_name
        );
        cJSON_AddBoolToObject(
            user_obj, "is_admin", list_users[i].user.is_admin == gqtrue
        );
        cJSON_AddNumberToObject(user_obj, "user_order", list_users[i].order);
        cJSON_AddItemToArray(users_array, user_obj);
    }

    evbuffer_add_printf(reply, "%s", cJSON_Print(response));
    evhttp_send_reply(req, HTTP_OK, NULL, reply);

    cJSON_Delete(response);

    for (int i = 0; i < count; i++) {
        free(list_users[i].user.first_name);
        free(list_users[i].user.surname);
        free(list_users[i].user.last_name);
    }
    free(list_users);

end:
    evbuffer_free(reply);
    if (json) {
        cJSON_Delete(json);
    }
}

static void get_all_lists_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Get all lists callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    int     count = 0;
    GQlist *lists = GQgetAllLists(&count);

    if (lists == NULL) {
        LOG_ERROR("Failed to get lists");
        evbuffer_add_printf(reply, "Failed to get lists");
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end_no_lists;
    }

    cJSON *response = cJSON_CreateObject();
    cJSON *lists_array = cJSON_AddArrayToObject(response, "lists");

    for (int i = 0; i < count; i++) {
        cJSON *list_obj = cJSON_CreateObject();
        cJSON_AddNumberToObject(list_obj, "list_id", (double)lists[i].list_id);
        cJSON_AddStringToObject(list_obj, "list_name", lists[i].list_name);
        cJSON_AddItemToArray(lists_array, list_obj);
    }

    evbuffer_add_printf(reply, "%s", cJSON_Print(response));
    evhttp_send_reply(req, HTTP_OK, NULL, reply);
    cJSON_Delete(response);

    GQfreeAllLists(lists, count);

end_no_lists:
    evbuffer_free(reply);
}

static void delete_list_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Delete list callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);
    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at delete list");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    const cJSON *list_id_json =
        cJSON_GetObjectItemCaseSensitive(json, "list_id");

    if (!cJSON_IsNumber(list_id_json)) {
        LOG_ERROR("Failed to get list_id from delete list json");
        evbuffer_add_printf(
            reply, "Failed to get \"list_id\" number from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    unsigned long long list_id = (unsigned long long)list_id_json->valuedouble;

    if (GQdeleteList(list_id) == gqfalse) {
        LOG_ERROR("Failed to delete list with ID %llu", list_id);
        evbuffer_add_printf(reply, "Failed to delete list, it may not exist");
        evhttp_send_reply(req, HTTP_NOTFOUND, NULL, reply);
        goto end;
    }

    evbuffer_add_printf(reply, "List was deleted");
    evhttp_send_reply(req, HTTP_OK, NULL, reply);

end:
    evbuffer_free(reply);
    cJSON_Delete(json);
}

static void add_user_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Add user callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);
    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at add user");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    const cJSON *first_name =
        cJSON_GetObjectItemCaseSensitive(json, "first_name");
    const cJSON *surname = cJSON_GetObjectItemCaseSensitive(json, "surname");
    const cJSON *last_name =
        cJSON_GetObjectItemCaseSensitive(json, "last_name");
    const cJSON *telegram_id =
        cJSON_GetObjectItemCaseSensitive(json, "telegram_id");
    const cJSON *is_admin = cJSON_GetObjectItemCaseSensitive(json, "is_admin");

    if (!cJSON_IsString(first_name) || (first_name->valuestring == NULL)) {
        LOG_ERROR("Failed to get first_name from add user json");
        evbuffer_add_printf(
            reply, "Failed to get \"first_name\" string from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    if (!cJSON_IsNumber(telegram_id)) {
        LOG_ERROR("Failed to get telegram_id from add user json");
        evbuffer_add_printf(
            reply, "Failed to get \"telegram_id\" number from json"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    GQuser user = { 0 };
    user.telegram_id = (long long)telegram_id->valuedouble;
    user.first_name = strdup(first_name->valuestring);

    if (cJSON_IsString(surname) && surname->valuestring != NULL) {
        user.surname = strdup(surname->valuestring);
    } else {
        user.surname = strdup("");
    }

    if (cJSON_IsString(last_name) && last_name->valuestring != NULL) {
        user.last_name = strdup(last_name->valuestring);
    } else {
        user.last_name = strdup("");
    }

    if (cJSON_IsBool(is_admin)) {
        user.is_admin = cJSON_IsTrue(is_admin) ? gqtrue : gqfalse;
    } else {
        user.is_admin = gqfalse;
    }

    LOG_INFO("Adding user with first name %s", user.first_name);

    if (GQaddUser(user) == gqfalse) {
        LOG_ERROR("Failed to add user with first name %s", user.first_name);
        evbuffer_add_printf(
            reply, "Failed to add user, likely it is already present in the db"
        );
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);

        free(user.first_name);
        free(user.surname);
        free(user.last_name);
        goto end;
    }

    LOG_INFO("Added user with first name %s", user.first_name);

    evbuffer_add_printf(reply, "User was added");
    evhttp_send_reply(req, HTTP_OK, NULL, reply);

    free(user.first_name);
    free(user.surname);
    free(user.last_name);

end:
    evbuffer_free(reply);
    cJSON_Delete(json);
}

static void swap_list_users_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Swap list users callback!, %s", req->uri);
    struct evbuffer *reply = evbuffer_new();

    char    read_buffer[BUFFER_SIZE];
    ssize_t bytes_read = evbuffer_copyout(
        req->input_buffer, (void *)read_buffer, sizeof(read_buffer)
    );
    read_buffer[bytes_read] = '\0';

    cJSON *json = cJSON_Parse(read_buffer);
    if (json == NULL) {
        LOG_ERROR("Could not parse JSON at swap list users");
        const char *error_ptr = cJSON_GetErrorPtr();

        if (error_ptr != NULL) {
            LOG_ERROR("JSON parser error at: %s\n", error_ptr);
            evbuffer_add_printf(
                reply, "Failed to parse json, error at: %s", error_ptr
            );
        } else {
            evbuffer_add_printf(reply, "Failed to parse json");
        }

        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    const cJSON *list_id_json =
        cJSON_GetObjectItemCaseSensitive(json, "list_id");
    const cJSON *user_a_id_json =
        cJSON_GetObjectItemCaseSensitive(json, "user_a_id");
    const cJSON *user_b_id_json =
        cJSON_GetObjectItemCaseSensitive(json, "user_b_id");

    if (!cJSON_IsNumber(list_id_json) || !cJSON_IsNumber(user_a_id_json)
        || !cJSON_IsNumber(user_b_id_json)) {
        LOG_ERROR("Failed to get required IDs from swap list users json");
        evbuffer_add_printf(reply, "Failed to get required IDs from json");
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    unsigned long long list_id = (unsigned long long)list_id_json->valuedouble;
    unsigned long long user_a_id =
        (unsigned long long)user_a_id_json->valuedouble;
    unsigned long long user_b_id =
        (unsigned long long)user_b_id_json->valuedouble;

    if (GQswapListUsers(list_id, user_a_id, user_b_id) == gqfalse) {
        LOG_ERROR(
            "Failed to swap users %llu and %llu in list %llu",
            user_a_id,
            user_b_id,
            list_id
        );
        evbuffer_add_printf(reply, "Failed to swap users");
        evhttp_send_reply(req, HTTP_BADREQUEST, NULL, reply);
        goto end;
    }

    LOG_INFO(
        "Swapped users %llu and %llu in list %llu",
        user_a_id,
        user_b_id,
        list_id
    );
    evbuffer_add_printf(reply, "Users were swapped");
    evhttp_send_reply(req, HTTP_OK, NULL, reply);

end:
    evbuffer_free(reply);
    cJSON_Delete(json);
}

static void signal_cb(evutil_socket_t fd, short event, void *arg) {
    LOG_INFO("%s signal received", strsignal(fd));
    event_base_loopbreak(arg);
}

gqbool GQinitServer(const char *ip, const unsigned short port) {
    gBase = event_base_new();
    gServer = evhttp_new(gBase);

    if (evhttp_bind_socket(gServer, ip, port) == -1) {
        LOG_ERROR("Failed to bind socket");
        return gqfalse;
    }

    evhttp_set_gencb(gServer, undefined_uri_callback, NULL);
    evhttp_set_cb(gServer, "/health", health_callback, NULL);
    evhttp_set_cb(gServer, "/list/add", add_list_callback, NULL);
    evhttp_set_cb(gServer, "/list/id", get_list_id_callback, NULL);
    evhttp_set_cb(gServer, "/list/get", get_list_callback, NULL);
    evhttp_set_cb(gServer, "/list/users/get", get_list_users_callback, NULL);
    evhttp_set_cb(gServer, "/list/all", get_all_lists_callback, NULL);
    evhttp_set_cb(gServer, "/list/delete", delete_list_callback, NULL);
    evhttp_set_cb(gServer, "/user/add", add_user_callback, NULL);
    evhttp_set_cb(gServer, "/list/users/swap", swap_list_users_callback, NULL);

    sSigInt = evsignal_new(gBase, SIGINT, signal_cb, gBase);
    event_add(sSigInt, NULL);

    LOG_INFO("Initialized HTTP server, listening on %s:%d", ip, port);
    return gqtrue;
}

void GQrunServer() {
    event_base_dispatch(gBase);
    evhttp_free(gServer);
    event_free(sSigInt);
    event_base_free(gBase);
}

void GQdestroyServer() {
    evhttp_free(gServer);
    event_base_free(gBase);
    LOG_INFO("Destroyed HTTP server");
}
