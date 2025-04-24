#include "server.h"

#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_response.h>

#include <expected>
#include <nlohmann/json.hpp>

ServerIO::ServerIO(std::size_t port) noexcept : port_(port) {}

ServerIO::~ServerIO() {
    stopListening();

    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

std::expected<void, std::string> ServerIO::tryRegisterCommand(
    std::unique_ptr<Command> command) {
    if (!command) {
        return std::unexpected("Command is null");
    }

    Command* raw_cmd = command.get();
    std::type_index type_idx(typeid(*raw_cmd));

    if (commands_.find(type_idx) != commands_.end()) {
        return std::unexpected("Trying to add command that is already added");
    }

    commands_.emplace(type_idx, std::move(command));

    return {};
}

bool ServerIO::running() const {
    return running_.load();
}

std::expected<void, std::string> ServerIO::startListening() {
    if (running_.load()) {
        return std::unexpected("Server is already running");
    }

    if (commands_.empty()) {
        return std::unexpected("No commands added");
    }

    running_.store(true);

    for (const auto& [_, cmd] : commands_) {
        crow::HTTPMethod method;

        if (cmd->str().starts_with("/post")) {
            method = crow::HTTPMethod::POST;
        } else if (cmd->str().starts_with("/delete")) {
            method = crow::HTTPMethod::Delete;
        } else {
            method = crow::HTTPMethod::GET;
        }

        app_.route_dynamic(cmd->str())
            .methods(method)([&cmd](const crow::request& req, crow::response& res) {
                CommandRequest creq;
                try {
                    auto j = nlohmann::json::parse(req.body);
                    creq.text = j.value("text", "");
                }
                catch (const std::exception& e) {
                    res = crow::response(400, "Invalid JSON");
                    return;
                }
                res = crow::response{cmd->execute(creq)->text};
            });
    }

    server_thread_ = std::thread([this]() { app_.port(port_).multithreaded().run(); });
    return {};
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
