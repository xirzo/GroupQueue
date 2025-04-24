#ifndef LIST_HOLDER_H
#define LIST_HOLDER_H

#include <SQLiteCpp/SQLiteCpp.h>

#include <expected>
#include <filesystem>
#include <memory>

#include "file_reader.h"
#include "list.h"
#include "user.h"

class ListHolder
{
public:
    template <typename... Args>
    static std::expected<std::shared_ptr<ListHolder>, std::string> construct(
        Args&&... args) {
        auto db =
            std::shared_ptr<ListHolder>(new ListHolder(std::forward<Args>(args)...));

        auto init_result = db->init();

        if (!init_result) {
            return std::unexpected(init_result.error());
        }

        return std::move(db);
    }

    std::expected<void, std::string> tryAddUser(const User& user) noexcept;
    std::expected<void, std::string> tryAddList(const List& list) noexcept;

private:
    ListHolder(const std::filesystem::path& db_path,
               std::unique_ptr<FileReader> file_reader) noexcept;
    std::expected<void, std::string> init() noexcept;

private:
    std::filesystem::path db_path_;
    std::unique_ptr<SQLite::Database> db_;
    std::unique_ptr<FileReader> file_reader_;
};

#endif  // !LIST_HOLDER_H
