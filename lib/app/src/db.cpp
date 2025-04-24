#include "db.h"

#include "list.h"

namespace gq {

std::expected<void, std::string> Database::init(std::filesystem::path db_path) noexcept {
    try {
        db_ = std::make_unique<SQLite::Database>(
            db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        db_->exec(
            "CREATE TABLE IF NOT EXISTS user(user_id INTEGER PRIMARY KEY, "
            "telegram_id "
            "INTEGER UNIQUE, first_name TEXT, surname TEXT, second_name "
            "TEXT, admin INTEGER)");

        db_->exec(
            "CREATE TABLE IF NOT EXISTS list(list_id INTEGER PRIMARY KEY, "
            "name TEXT UNIQUE)");

        db_->exec(
            "CREATE TABLE IF NOT EXISTS list_user(list_user_id INTEGER PRIMARY KEY, "
            "user_id INTEGER, list_user_order INTEGER");

        return {};
    }
    catch (const SQLite::Exception& e) {
        return std::unexpected(e.what());
    }
}

std::expected<void, std::string> Database::tryAddUser(const User& user) noexcept {
    if (user.user_id != kEmptyUserId) {
        return std::unexpected("Trying to add user, whos user_id != kEmptyUserId");
    }

    SQLite::Statement query(*db_,
                            "INSERT INTO user (telegram_id, first_name, surname, "
                            "second_name, admin) VALUES (?, ?, ?, ?, ?)");

    query.bind(1, user.telegram_id);
    query.bind(2, user.first_name);
    query.bind(3, user.surname);
    query.bind(4, user.second_name);
    query.bind(5, user.admin ? 1 : 0);

    try {
        query.exec();
    }
    catch (const std::exception& e) {
        return std::unexpected(e.what());
    }

    return {};
}

std::expected<void, std::string> Database::tryAddList(const List& list) noexcept {
    SQLite::Statement query(*db_, "INSERT INTO list (name) VALUES (?)");

    query.bind(1, list.name);

    try {
        query.exec();
    }
    catch (const std::exception& e) {
        return std::unexpected(e.what());
    }

    return {};
}
}  // namespace gq
