#include "server.h"

#include <crow/common.h>

#include "io.h"

ServerIO::ServerIO(std::size_t port, std::function<std::string(gq::InputType)> callback)
    : port_(port), callback_(std::move(callback)) {}

ServerIO::~ServerIO() {
    stopListening();

    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

void ServerIO::startListening() {
    if (running_.load()) {
        return;
    }

    running_.store(true);

    CROW_ROUTE(app_, "/get_list_users").methods(crow::HTTPMethod::GET)([this]() {
        return crow::response{callback_(gq::InputType::GET_LIST_USERS)};
    });

    CROW_ROUTE(app_, "/get_lists").methods(crow::HTTPMethod::GET)([this]() {
        return crow::response{callback_(gq::InputType::GET_LISTS)};
    });

    CROW_ROUTE(app_, "/add_list")
        .methods(crow::HTTPMethod::POST)([this](const crow::request&) {
            return crow::response{callback_(gq::InputType::ADD_LIST)};
        });

    CROW_ROUTE(app_, "/delete_list")
        .methods(crow::HTTPMethod::DELETE)([this](const crow::request&) {
            return crow::response{callback_(gq::InputType::DELETE_LIST)};
        });

    server_thread_ = std::thread([this]() { app_.port(port_).multithreaded().run(); });
}

void ServerIO::stopListening() {
    if (!running_.load()) {
        return;
    }

    running_.store(false);

    app_.stop();

    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}
