#include <SQLiteCpp/Database.h>

#include "server.h"
#include "sqlite_repository.h"

constexpr std::size_t kPort = 5000;
constexpr char kDbPath[] = "data.db3";

int main() {
    try {
        const std::vector<User> kUsers = {
            User(618211233, "Alexander", "Zhidkov", "Vladimirovich", true),
            User(6446112154, "Fedor", "Fedorov", "Fedorovich", false)};

        auto db = std::make_unique<SQLite::Database>(
            kDbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        auto sqlite_repo = std::make_shared<SqliteRepository>(std::move(db), kUsers);

        auto init_result = sqlite_repo->init();

        if (!init_result) {
            std::cerr << init_result.error() << std::endl;
            return 1;
        }

        Server server(kPort, sqlite_repo);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
