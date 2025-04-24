#include <cstdlib>
#include <iostream>
#include <memory>

#include "json_reader.h"
#include "list_holder.h"
#include "post_list.h"
#include "server.h"

constexpr char kUsersPath[] = "users.json";
constexpr char kDbPath[] = "data.db";
constexpr std::size_t kPort = 5000;

int main(void) {
    auto json_reader = std::make_unique<JsonReader>(kUsersPath);

    auto list_holder_result = ListHolder::construct(kDbPath, std::move(json_reader));

    if (!list_holder_result) {
        std::cerr << list_holder_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    auto server = std::make_unique<ServerIO>(kPort);

    auto register_result = server->tryRegisterCommand(
        std::make_unique<PostList>(std::move(list_holder_result.value())));

    if (!register_result) {
        std::cerr << register_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    auto start_result = server->startListening();

    if (!start_result) {
        std::cerr << start_result.error() << std::endl;
        server->stopListening();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
