#include "bool.h"
#include "db.h"
#include <event2/http.h>
#include <event2/http_struct.h>
#include <evhttp.h>
#include <logger.h>

#define IP   "127.0.0.1"
#define PORT 5000

void callback(struct evhttp_request *req, void *ctx) {
    LOG_INFO("Callback!");
}

int main(void) {
    const char *conninfo =
        "host=localhost port=5432 dbname=groupbotdb user=groupbot password=1234";

    if (GQinitDB(conninfo) == gqfalse) {
        return 1;
    }

    struct event_base *base = event_base_new();
    struct evhttp     *server = evhttp_new(base);

    evhttp_bind_socket(server, IP, PORT);
    evhttp_set_cb(server, "/list", callback, NULL);

    // a evhttp_set_gencb(); this callback will be invoked if no other callbacks
    // have been registered for a given URI.

    event_base_dispatch(base);

    evhttp_free(server);
    event_base_free(base);

    GQdestroyDB();
    return 0;
}

// TODO: load initial state with toml files
// TODO: remove list
// TODO: http server for serving db methods
