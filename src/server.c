#include "server.h"

#include <logger.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <evhttp.h>

static struct event_base *base;
static struct evhttp     *server;

static void list_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("List callback!");
}

static void undefined_uri_callback(struct evhttp_request *req, void *ctx) {
    LOG_ERROR("Called callback of unset URI");
}

gqbool GQinitServer(const char *ip, const unsigned short port) {
    base = event_base_new();
    server = evhttp_new(base);

    if (evhttp_bind_socket(server, ip, port) == -1) {
        LOG_ERROR("Failed to bind socket");
        return gqfalse;
    }

    /* may faild if callback exists, but I don`t care */
    evhttp_set_cb(server, "/list", list_callback, NULL);
    evhttp_set_gencb(server, undefined_uri_callback, NULL);

    LOG_INFO("Initialized HTTP server");
    return gqtrue;
}

void GQrunServer() {
    event_base_dispatch(base);
}

void GQdestroyServer() {
    evhttp_free(server);
    event_base_free(base);
    LOG_INFO("Destroyed HTTP server");
}
