#include "postgres.h"
#include <cstdint>
#include <optional>
#include <pqxx/internal/statement_parameters.hxx>

PostgresRepository::PostgresRepository(std::unique_ptr<pqxx::connection> con)
    : con_(std::move(con)) {}

std::optional<int64_t> PostgresRepository::addList(const List& list) {
  pqxx::work tx(*con_);

  try {
    pqxx::result result =
        tx.exec("SELECT add_list($1)", pqxx::params{list.list_name});

    if (result.empty()) {
      return std::nullopt;
    }

    tx.commit();
    return result.one_row()[0].as<int64_t>();

  } catch (const pqxx::sql_error& e) {
    return std::nullopt;
  }
}

std::optional<List> PostgresRepository::getList(int64_t list_id) {
  pqxx::work tx(*con_);
  pqxx::result result =
      tx.exec("SELECT * FROM get_list($1)", pqxx::params{list_id});

  if (result.empty()) {
    return std::nullopt;
  }

  tx.commit();

  return List{result.one_row()[0].as<int64_t>(),
              result.one_row()[1].as<std::string>()};
}

void PostgresRepository::deleteList(int64_t list_id) {
  pqxx::work tx(*con_);
  tx.exec("CALL delete_list($1)", pqxx::params{list_id});
  tx.commit();
}
