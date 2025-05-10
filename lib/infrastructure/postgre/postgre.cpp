#include "postgre.h"
#include <cstdint>
#include <pqxx/internal/statement_parameters.hxx>

PostgreRepository::PostgreRepository(const std::string &str) {
    con_ = std::make_unique<pqxx::connection>(std::move(str));
}

int64_t PostgreRepository::addList(const List &list) {
    pqxx::work tx(*con_);
    pqxx::row row =
        tx.exec("SELECT add_list($1)", pqxx::params{list.list_name}).one_row();
    tx.commit();
    return row[0].as<int64_t>();
}

List PostgreRepository::getList(int64_t list_id) {
    pqxx::work tx(*con_);
    pqxx::row row =
        tx.exec("SELECT * FROM get_list($1)", pqxx::params{list_id}).one_row();
    tx.commit();
    return {row[0].as<int64_t>(), row[1].as<std::string>()};
}

void PostgreRepository::deleteList(int64_t list_id) {
    pqxx::work tx(*con_);
    tx.exec("CALL delete_list($1)", pqxx::params{list_id});
    tx.commit();
}
