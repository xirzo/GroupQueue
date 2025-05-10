#include "postgre.h"
#include "server.h"
#include <exception>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>

const std::string getEnvVar(const std::string &key,
                            const std::string &default_value = "") {
    const char *value = std::getenv(key.c_str());
    return value ? std::string(value) : default_value;
}

int main(void) {

    try {
        auto repo = std::make_shared<PostgreRepository>(
            "user=" + getEnvVar("POSTGRES_USER", "postgres") + " " +
            "host=" + getEnvVar("POSTGRES_HOST", "localhost") + " " +
            "port=" + getEnvVar("POSTGRES_PORT", "5432") + " " +
            "password=" + getEnvVar("POSTGRES_PASSWORD", "") + " " +
            "dbname=" + getEnvVar("POSTGRES_DB", "postgres"));

        auto server = std::make_unique<Server>(std::move(repo));

        server->start(6000);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
