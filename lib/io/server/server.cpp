#include "server.h"

#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_response.h>

// ServerIO::ServerIO(std::size_t port) : port_(port) {}
//
// ServerIO::ServerIO(std::size_t port,
//                    std::initializer_list<std::pair<gq::Command*, std::string>> list)
//     : port_(port) {
//     for (const auto& [cmd, endpoint] : list) {
//         auto add_result = tryAddCommand(std::unique_ptr<gq::Command>(cmd), endpoint);
//
//         if (!add_result) {
//             std::cout << add_result.error() << std::endl;
//         }
//     }
// }
//
// ServerIO::~ServerIO() {
//     stopListening();
//
//     if (server_thread_.joinable()) {
//         server_thread_.join();
//     }
// }
//
// std::expected<void, std::string> ServerIO::tryAddCommand(
//     std::unique_ptr<gq::Command> command, const std::string& endpoint) {
//     if (commands_.contains(typeid(command))) {
//         return std::unexpected("Trying to add command that is already added");
//     }
//
//     commands_[typeid(command)] = std::move(command);
//     endpoints_[typeid(command)] = endpoint;
//
//     return {};
// }
//
// bool ServerIO::running() const {
//     return running_.load();
// }
//
// // TODO: make so that server gets all commands in constructor and all of these
// // CROW_ROUTE would be single method and it comes from main
//
// std::expected<void, std::string> ServerIO::startListening() {
//     if (running_.load() || commands_.empty()) {
//         return std::unexpected("Cannot start listening");
//     }
//
//     running_.store(true);
//
//     for (const auto& [_, cmd] : commands_) {
//         const std::string& endpoint = endpoints_.at(typeid(cmd));
//
//         app_.route_dynamic(endpoint)([&cmd](const crow::request&, crow::response& res)
//         {
//             res = crow::response{cmd->execute()};
//         });
//     }
//
//     server_thread_ = std::thread([this]() { app_.port(port_).multithreaded().run(); });
//
//     // CROW_ROUTE(app_, "/get_list_users").methods(crow::HTTPMethod::GET)([this]() {
//     //     auto cmd = std::make_unique<GetListUsersCommand>();
//     //     return crow::response{callback_(std::move(cmd))};
//     // });
//
//     // CROW_ROUTE(app_, "/get_lists").methods(crow::HTTPMethod::GET)([this]() {
//     //     auto cmd = std::make_unique<GetListsCommand>();
//     //     return crow::response{callback_(std::move(cmd))};
//     // });
//
//     // CROW_ROUTE(app_, "/add_list")
//     //     .methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
//     //         std::string list_name;
//     //         if (!req.body.empty()) {
//     //             auto json = crow::json::load(req.body);
//     //
//     //             if (json && json.has("list_name")) {
//     //                 list_name = json["list_name"].s();
//     //             }
//     //         }
//     //
//     //         auto cmd =
//     //             std::make_unique<gq::AddList>(std::move(db_), std::move(list_name));
//     //
//     //         return crow::response{callback_(std::move(cmd))};
//     //     });
//
//     // CROW_ROUTE(app_, "/delete_list")
//     //     .methods(crow::HTTPMethod::DELETE)([this](const crow::request& req) {
//     //         std::string list_name;
//     //         if (!req.body.empty()) {
//     //             auto json = crow::json::load(req.body);
//     //             if (json && json.has("list_name")) {
//     //                 list_name = json["list_name"].s();
//     //             }
//     //         }
//     //         auto cmd = std::make_unique<DeleteListCommand>(std::move(list_name));
//     //         return crow::response{callback_(std::move(cmd))};
//     //     });
//     // server_thread_ = std::thread([this]() { app_.port(port_).multithreaded().run();
//     });
//
//     return {};
// }
//
// void ServerIO::stopListening() {
//     if (!running_.load()) {
//         return;
//     }
//
//     running_.store(false);
//
//     app_.stop();
//
//     if (server_thread_.joinable()) {
//         server_thread_.join();
//     }
// }
