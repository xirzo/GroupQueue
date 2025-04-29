#ifndef SQLITE_REPOSITORY
#define SQLITE_REPOSITORY

#include <SQLiteCpp/SQLiteCpp.h>

#include <expected>
#include <memory>
#include <vector>

#include "list.h"
#include "list_user.h"
#include "repository.h"
#include "user.h"

class SqliteRepository : public Repository
{
public:
    SqliteRepository(std::unique_ptr<SQLite::Database> db,
                     std::vector<User> inital_users);

    std::expected<void, std::string> init();

    std::expected<std::vector<User>, std::string> tryGetAllUsers() override;
    std::expected<User, std::string> tryGetUser(int64_t telegram_id) override;
    std::expected<int64_t, std::string> tryAddList(const List& list) override;
    std::expected<void, std::string> tryRemoveList(int64_t list_id) override;
    std::expected<List, std::string> tryGetList(const std::string& list_name) override;
    std::expected<List, std::string> tryGetList(int64_t list_id) override;
    std::expected<std::vector<List>, std::string> tryGetAllLists() override;
    std::expected<void, std::string> trySwapUsers(int64_t list_id, int64_t first_user_id,
                                                  int64_t second_user_id) override;

    std::expected<ListUser, std::string> tryGetListUser(int64_t list_id, int64_t user_id);

    std::expected<User, std::string> tryGetUserByTelegramId(int64_t telegram_id) override;

    std::expected<std::vector<ListUser>, std::string> tryGetListUsers(
        int64_t list_id) override;

    std::expected<std::vector<User>, std::string> tryGetUsers(int64_t list_id) override;

private:
    std::expected<void, std::string> addUsersToList(const List& list);
    std::expected<int64_t, std::string> tryAddUser(const User& user);

    std::unique_ptr<SQLite::Database> db_;
    std::vector<User> initial_users_;
};

#endif  // !SQLITE_REPOSITORY
