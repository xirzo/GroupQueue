#include "app.h"

namespace gq {

App::App(std::unique_ptr<FileReader> file_reader)
    : file_reader_(std::move(file_reader)) {}

}  // namespace gq
