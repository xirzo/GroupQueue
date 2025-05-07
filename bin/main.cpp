#include <SQLiteCpp/Database.h>

#include "server.h"
#include "sqlite_repository.h"
#include <exception>
#include <nlohmann/json.hpp>

constexpr std::size_t kPort = 5000;
constexpr char kDbPath[] = "data.db3";
constexpr char kUsersFile[] = "users.json";

User fromJson(const nlohmann::json &json) {
  return User(json.value("user_id", -1), json.at("telegram_id").get<int64_t>(),
              json.at("first_name").get<std::string>(),
              json.at("surname").get<std::string>(),
              json.at("second_name").get<std::string>(),
              json.at("admin").get<bool>());
}

std::vector<User> loadUsersFromFile(const char *file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open users file");
  }

  nlohmann::json json_data;
  file >> json_data;

  std::vector<User> users;
  for (const auto &user_json : json_data) {
    users.push_back(fromJson(user_json));
  }

  return users;
}

int main() {

  try {
    const std::vector<User> kUsers = loadUsersFromFile(kUsersFile);

    auto db = std::make_unique<SQLite::Database>(
        kDbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    auto sqlite_repo =
        std::make_shared<SqliteRepository>(std::move(db), kUsers);

    auto init_result = sqlite_repo->init();

    if (!init_result) {
      std::cerr << init_result.error() << std::endl;
      return 1;
    }

    Server server(kPort, sqlite_repo);
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
