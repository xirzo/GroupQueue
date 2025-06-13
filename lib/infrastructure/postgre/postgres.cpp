#include "postgres.h"
#include <cstdint>
#include <optional>
#include <pqxx/internal/statement_parameters.hxx>

PostgresRepository::PostgresRepository(std::unique_ptr<pqxx::connection> con)
    : con_(std::move(con)) {}

int64_t PostgresRepository::addList(const List &list) {
  pqxx::work tx(*con_);
  pqxx::row row =
      tx.exec("SELECT add_list($1)", pqxx::params{list.list_name}).one_row();
  tx.commit();
  return row[0].as<int64_t>();
}

std::optional<List> PostgresRepository::getList(int64_t list_id) {
  pqxx::work tx(*con_);
  pqxx::result tx_result =
      tx.exec("SELECT * FROM get_list($1)", pqxx::params{list_id});

  if (tx_result.empty()) {
    return std::nullopt;
  }

  tx.commit();

  return List{tx_result.one_row()[0].as<int64_t>(),
              tx_result.one_row()[1].as<std::string>()};
}

void PostgresRepository::deleteList(int64_t list_id) {
  pqxx::work tx(*con_);
  tx.exec("CALL delete_list($1)", pqxx::params{list_id});
  tx.commit();
}
