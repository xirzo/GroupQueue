#include <cstdlib>
#include <memory>

#include "app.h"
#include "json_reader.h"

constexpr char kUsersPath[] = "users.json";
constexpr char kAdminsPath[] = "users.json";

int main(void) {
    auto json_reader = std::make_unique<JsonReader>(kUsersPath, kAdminsPath);

    gq::App app(std::move(json_reader));

    return EXIT_SUCCESS;
}
