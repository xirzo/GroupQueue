#include <cstdlib>
#include <iostream>

#include "list_holder.h"

// constexpr char kUsersPath[] = "users.json";
constexpr char kDbPath[] = "data.db";
// constexpr std::size_t kPort = 5000;

int main(void) {
    auto list_holder_result = ListHolder::construct(kDbPath);

    if (!list_holder_result) {
        std::cerr << list_holder_result.error() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
