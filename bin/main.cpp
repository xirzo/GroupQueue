#include <cstdlib>
#include <iostream>
#include <memory>

#include "app.h"
#include "json_reader.h"

constexpr char kUsersPath[] = "users.json";
constexpr char kDbPath[] = "data.db";

int main(void) {
    auto json_reader = std::make_unique<JsonReader>(kUsersPath);
    auto app_result = gq::makeApp(kDbPath, std::move(json_reader));

    if (!app_result) {
        std::cerr << app_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<gq::App> app = std::move(app_result.value());

    return EXIT_SUCCESS;
}
