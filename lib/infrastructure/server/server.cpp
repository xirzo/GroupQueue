#include "server.h"
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <exception>
#include <memory>

Server::Server(std::shared_ptr<Repository> repository)
    : r_(std::move(repository)) {
}

crow::json::wvalue Server::jsonFromList(const List &list) {
    crow::json::wvalue json;
    json["list_id"] = list.list_id;
    json["list_name"] = list.list_name;
    return json;
}

void Server::start(std::size_t port) {
    app_ = std::make_unique<crow::SimpleApp>();

    CROW_ROUTE((*app_), "/health").methods(crow::HTTPMethod::GET)([]() {
        return crow::response{200, "OK"};
    });

    CROW_ROUTE((*app_), "/list")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req) {
            try {
                crow::json::rvalue json = crow::json::load(req.body);

                if (!json.has("list_name")) {
                    return crow::response{400, "No list_name"};
                }

                r_->addList({json["list_name"].s()});
                return crow::response{200, "OK"};
            } catch (const std::exception &e) {
                return crow::response{400, e.what()};
            }
        });

    CROW_ROUTE((*app_), "/list/<int>")
        .methods(crow::HTTPMethod::GET)([this](int64_t list_id) {
            try {
                return crow::response{200, jsonFromList(r_->getList(list_id))};
            } catch (const std::exception &e) {
                return crow::response{400, e.what()};
            }
            return crow::response{200, "OK"};
        });

    app_->port(port).multithreaded().run();
}
