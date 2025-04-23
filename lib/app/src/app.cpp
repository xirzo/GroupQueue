#include "app.h"

#include "file_reader.h"

namespace gq {

App::App(std::unique_ptr<FileReader> file_reader) noexcept
    : file_reader_(std::move(file_reader)) {}

std::optional<std::string> App::init() noexcept {
    return "";
}

}  // namespace gq
