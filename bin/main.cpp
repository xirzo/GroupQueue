#include <cstdlib>
#include <iostream>
#include <memory>

#include "app.h"
#include "json_reader.h"
#include "server.h"

constexpr char kUsersPath[] = "users.json";
constexpr char kDbPath[] = "data.db";
constexpr std::size_t kPort = 5000;

int main(void) {
    auto callback = [](gq::InputType type) -> std::string {
        switch (type) {
            case gq::InputType::GET_LIST_USERS:
                return "User list: Alice, Bob, Charlie";
            case gq::InputType::GET_LISTS:
                return "Lists: Shopping, Work, Personal";
            case gq::InputType::ADD_LIST:
                return "A new list was added!";
            case gq::InputType::DELETE_LIST:
                return "A list was deleted!";
            default:
                return "Unknown command";
        }
    };

    auto json_reader = std::make_unique<JsonReader>(kUsersPath);
    auto server_io = std::make_unique<ServerIO>(kPort, callback);
    auto app_result = gq::makeApp(kDbPath, std::move(json_reader), std::move(server_io));

    if (!app_result) {
        std::cerr << app_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<gq::App> app = std::move(app_result.value());

    return EXIT_SUCCESS;
}
