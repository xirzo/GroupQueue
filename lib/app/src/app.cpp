#include "app.h"

#include <filesystem>
#include <optional>

#include "db.h"
#include "file_reader.h"

namespace gq {

App::App(std::unique_ptr<FileReader> file_reader) noexcept
    : file_reader_(std::move(file_reader)) {}

std::optional<std::string> App::init(std::filesystem::path db_path) noexcept {
    auto db_result = makeDatabase(db_path);

    if (!db_result) {
        return db_result.error();
    }

    auto users_result = file_reader_->readUsers();

    if (!users_result) {
        return users_result.error();
    }

    // add users to according tables if they are not present
    // db_...

    return std::nullopt;
}

}  // namespace gq
