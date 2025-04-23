#include "db.h"

namespace gq {

std::optional<std::string> Database::init(std::filesystem::path db_path) noexcept {
    try {
        db_ = std::make_unique<SQLite::Database>(
            db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        return std::nullopt;
    }
    catch (const SQLite::Exception& e) {
        return e.what();
    }
}
}  // namespace gq
