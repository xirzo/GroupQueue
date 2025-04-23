#ifndef DB_H
#define DB_H

#include <SQLiteCpp/SQLiteCpp.h>

#include <expected>
#include <filesystem>
#include <memory>

namespace gq {

class Database
{
public:
    std::optional<std::string> init(std::filesystem::path db_path) noexcept;

private:
    std::unique_ptr<SQLite::Database> db_;
};

template <typename... Args>
std::expected<std::unique_ptr<Database>, std::string> makeDatabase(
    std::filesystem::path db_path, Args&&... args) {
    auto db = std::make_unique<Database>(std::forward<Args>(args)...);
    auto init_result = db->init(db_path);

    if (init_result) {
        return std::unexpected(init_result.value());
    }

    return std::move(db);
}

}  // namespace gq

#endif  // !DB_H
