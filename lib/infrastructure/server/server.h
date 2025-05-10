#ifndef SERVER_H
#define SERVER_H

#include <crow.h>
#include <memory>

#include "list.h"
#include "repository.h"

class Server {
  public:
    explicit Server(std::shared_ptr<Repository> repository);

    void start(std::size_t port);

  private:
    crow::json::wvalue jsonFromList(const List &list);

  private:
    std::unique_ptr<crow::SimpleApp> app_;
    std::shared_ptr<Repository> r_;
};

#endif // !SERVER_H
