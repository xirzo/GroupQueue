#ifndef SERVER_H
#define SERVER_H

#include <crow/app.h>
#include <crow/common.h>

#include <cstddef>
#include <expected>
#include <nlohmann/json.hpp>

#include "repository.h"

class Server
{
public:
    Server(std::size_t port, std::shared_ptr<Repository> repository);

private:
    crow::json::wvalue jsonFromUser(const User& user);
    crow::json::wvalue jsonFromList(const List& list);

private:
    crow::SimpleApp app_;
    std::size_t port_;
    std::shared_ptr<Repository> repository_;
};

#endif  // !SERVER_H
