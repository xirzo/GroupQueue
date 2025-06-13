#ifndef POSTGRE_H
#define POSTGRE_H

#include <memory>
#include <pqxx/pqxx>

#include "list.h"
#include "repository.h"

class PostgresRepository : public Repository {
 private:
  std::unique_ptr<pqxx::connection> con_;

 public:
  explicit PostgresRepository(std::unique_ptr<pqxx::connection> con);

  std::optional<int64_t> addList(const List &list) override;
  std::optional<List> getList(int64_t list_id) override;
  void deleteList(int64_t list_id) override;
};

#endif
