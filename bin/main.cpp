#include "server.h"
#include "sqlite_repository.h"
#include <SQLiteCpp/Database.h>
#include <charconv>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

const char *getEnvOrDefault(const char *envVar, const char *defaultValue) {
  const char *value = std::getenv(envVar);
  return value ? value : defaultValue;
}

const char *kUsersFile = getEnvOrDefault("USERS_FILE", "/app/users.json");
const char *kDbPath = getEnvOrDefault("DB_PATH", "/app/data.db3");

User fromJson(const nlohmann::json &json) {
  return User(json.value("user_id", -1),
              json.value("telegram_id", static_cast<int64_t>(-1)),
              json.value("first_name", std::string()),
              json.value("surname", std::string()),
              json.value("second_name", std::string()),
              json.value("admin", false));
}

std::vector<User> loadUsersFromFile(const char *file_path) {
  if (!std::filesystem::exists(file_path)) {
    throw std::runtime_error("Users file does not exist");
  }

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
  const char *port_env = std::getenv("PORT");
  std::size_t port = 5000;

  if (!port_env) {
    std::cerr << "Env 'PORT' is not set, using default port: " << port
              << std::endl;
  } else {
    std::string kPortStr(port_env);
    if (kPortStr.empty()) {
      std::cerr << "port environment variable is empty, using default port: "
                << port << std::endl;
    } else {
      auto [ptr, ec] = std::from_chars(kPortStr.data(),
                                       kPortStr.data() + kPortStr.size(), port);
      if (ec != std::errc()) {
        std::cerr
            << "Error converting port string to number, using default port: "
            << port << std::endl;
      }
    }
  }

  try {
    const std::vector<User> kUsers = loadUsersFromFile(kUsersFile);

    auto db = std::make_unique<SQLite::Database>(
        kDbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    auto sqlite_repo =
        std::make_shared<SqliteRepository>(std::move(db), kUsers);

    auto init_result = sqlite_repo->init();

    if (!init_result) {
      std::cerr << "Database initialization failed: " << init_result.error()
                << std::endl;
      return 1;
    }

    Server server(port, sqlite_repo);
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Unhandled exception: " << e.what() << std::endl;
    return 1;
  }
}
