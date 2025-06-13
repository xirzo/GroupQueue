#include "postgres.h"
#include "server.h"
#include <exception>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>

/* TODO:
 * read initial data somehow (maybe json?)
 * maybe add a way to add users via frontend?
 * */

/* TODO:
 * authentication + admin system
 * */

const std::string getEnvVar(const std::string &key,
                            const std::string &default_value = "") {
  const char *value = std::getenv(key.c_str());
  return value ? std::string(value) : default_value;
}

int main(void) {
  std::unique_ptr<pqxx::connection> con;
  try {
    con = std::make_unique<pqxx::connection>(
        "user=" + getEnvVar("POSTGRES_USER", "postgres") + " " +
        "host=" + getEnvVar("POSTGRES_HOST", "localhost") + " " +
        "port=" + getEnvVar("POSTGRES_PORT", "5432") + " " +
        "password=" + getEnvVar("POSTGRES_PASSWORD", "") + " " +
        "dbname=" + getEnvVar("POSTGRES_DB", "postgres"));
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }

  auto repo = std::make_shared<PostgresRepository>(std::move(con));
  auto server = std::make_unique<Server>(std::move(repo));

  server->start(6000);
  return 0;
}
