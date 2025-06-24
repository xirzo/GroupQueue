#include "server.h"

#include <cjson/cJSON.h>
#include <event2/buffer.h>
#include <logger.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <evhttp.h>
#include <signal.h>
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

/*

Example json

{
    "list_name" : "Rof
}

*/

// TODO: maybe return list id on creation
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
            .name = list_name->valuestring,
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

    evbuffer_add_printf(reply, "List was added");
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

    /* may faild if callback exists, but I don`t care */
    evhttp_set_gencb(gServer, undefined_uri_callback, NULL);
    evhttp_set_cb(gServer, "/health", health_callback, NULL);
    evhttp_set_cb(gServer, "/list/add", add_list_callback, NULL);

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
