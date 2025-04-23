#include "app.h"

#include <optional>

#include "file_reader.h"

namespace gq {

App::App(std::unique_ptr<FileReader> file_reader) noexcept
    : file_reader_(std::move(file_reader)) {}

std::optional<std::string> App::init() noexcept {
    auto users_result = file_reader_->readUsers();

    if (!users_result) {
        return users_result.error();
    }

    // add users to according tables if they are not present

    return std::nullopt;
}

}  // namespace gq
