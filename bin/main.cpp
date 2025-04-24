#include <nlohmann/json.hpp>

#include "list_holder.h"
#include "server.h"

constexpr std::size_t kPort = 5000;
constexpr char kDbPath[] = "data.db3";

int main() {
    const std::vector<User> kUsers = {
        User(618211233, "Alexander", "Zhidkov", "Vladimirovich", true)};

    auto lh = std::make_shared<ListHolder>(kDbPath, kUsers);

    auto init_result = lh->init();

    if (!init_result) {
        std::cerr << init_result.error() << std::endl;
        return 1;
    }

    Server server(kPort, lh);

    return 0;
}
