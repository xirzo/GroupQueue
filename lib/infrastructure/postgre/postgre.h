#ifndef POSTGRE_H
#define POSTGRE_H

#include "list.h"
#include "repository.h"
#include <memory>
#include <pqxx/pqxx>

class PostgreRepository : public Repository {
  private:
    std::unique_ptr<pqxx::connection> con_;

  public:
    explicit PostgreRepository(char const options[]);

    int64_t addList(const List &list) override;
    List getList(int64_t list_id) override;
    void deleteList(int64_t list_id) override;
};

#endif
