#include "server.h"

#include <logger.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <evhttp.h>
#include <signal.h>
#include <string.h>

static struct event_base *gBase;
static struct evhttp     *gServer;
static struct event      *sSigInt;

static void add_list_callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Add list callback!, %s", req->uri);
}

static void undefined_uri_callback(struct evhttp_request *req, void *ctx) {
    LOG_ERROR("Called callback of unset URI");
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
    evhttp_set_cb(gServer, "/list/add", add_list_callback, NULL);
    evhttp_set_gencb(gServer, undefined_uri_callback, NULL);

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
