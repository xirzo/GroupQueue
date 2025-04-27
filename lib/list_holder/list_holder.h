#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include <expected>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include "list.h"
#include "user.h"

class ListHolder
{
public:
    ListHolder(std::filesystem::path db_path, std::vector<User> inital_users)
        : db_path_(std::move(db_path)), initial_users_(std::move(inital_users)) {}

    std::expected<void, std::string> init() {
        try {
            db_ = std::make_unique<SQLite::Database>(
                db_path_, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        }
        catch (const std::exception& e) {
            return std::unexpected(e.what());
        }

        auto create_result = createTables();

        if (!create_result) {
            return std::unexpected(create_result.error());
        }

        auto load_result = loadUsers();

        if (!load_result) {
            return std::unexpected(load_result.error());
        }

        return {};
    }

    std::expected<void, std::string> tryAddList(const List& list) {
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

    std::expected<List, std::string> tryGetList(const std::string& list_name) {
        SQLite::Statement query(*db_, "SELECT * FROM list WHERE name = ?");

        query.bind(1, list_name);

        try {
            if (query.executeStep()) {
                return List(query.getColumn(0), query.getColumn(1));
            } else {
                return std::unexpected("No list with name " + list_name);
            }
        }
        catch (const std::exception& e) {
            return std::unexpected(e.what());
        }
    }

    std::expected<std::vector<List>, std::string> tryGetAllLists() {
        SQLite::Statement query(*db_, "SELECT * FROM list");

        std::vector<List> lists;

        try {
            while (query.executeStep()) {
                lists.emplace_back(query.getColumn(0), query.getColumn(1));
            }

            if (lists.empty()) {
                return std::unexpected("No lists present");
            }

            return lists;
        }
        catch (const std::exception& e) {
            return std::unexpected(e.what());
        }
    }

private:
    std::expected<void, std::string> createTables() {
        try {
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
                "user_id INTEGER, list_user_order INTEGER)");

            return {};
        }
        catch (const SQLite::Exception& e) {
            return std::unexpected(e.what());
        }
    }

    std::expected<void, std::string> loadUsers() {
        try {
            for (const User& user : initial_users_) {
                auto add_result = tryAddUser(user);

                if (!add_result) {
                    std::cerr << "Could not add user: " << user.first_name << " "
                              << user.surname << " | " << add_result.error()
                              << " | skipping..." << std::endl;
                    continue;
                }
            }

            return {};
        }
        catch (const SQLite::Exception& e) {
            return std::unexpected(e.what());
        }
    }

    std::expected<void, std::string> tryAddUser(const User& user) {
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

private:
    std::filesystem::path db_path_;
    std::vector<User> initial_users_;
    std::unique_ptr<SQLite::Database> db_;
};
