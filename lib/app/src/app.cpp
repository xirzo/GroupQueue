#include "app.h"

#include <expected>
#include <filesystem>
#include <iostream>

#include "db.h"
#include "file_reader.h"

namespace gq {

App::App(std::unique_ptr<FileReader> file_reader) noexcept
    : file_reader_(std::move(file_reader)) {}

std::expected<void, std::string> App::init(std::filesystem::path db_path) noexcept {
    auto db_result = makeDatabase(db_path);

    if (!db_result) {
        return std::unexpected(db_result.error());
    }

    db_ = std::move(db_result.value());

    auto users_result = file_reader_->readUsers();

    if (!users_result) {
        return std::unexpected(users_result.error());
    }

    for (const User& user : users_result.value()) {
        auto add_user_result = db_->tryAddUser(user);

        if (!add_user_result) {
            // TODO: Remove this
            std::cout << user.first_name << " " << user.surname
                      << " was skipped during init" << std::endl;
            continue;
        }
    }

    return {};
}

}  // namespace gq
