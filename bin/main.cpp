#include <cstdlib>
#include <iostream>
#include <memory>

#include "json_reader.h"
#include "list_holder.h"

constexpr char kUsersPath[] = "users.json";
constexpr char kDbPath[] = "data.db";
// constexpr std::size_t kPort = 5000;

int main(void) {
    auto json_reader = std::make_unique<JsonReader>(kUsersPath);

    auto list_holder_result = ListHolder::construct(kDbPath, std::move(json_reader));

    if (!list_holder_result) {
        std::cerr << list_holder_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
