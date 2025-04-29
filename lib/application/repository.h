#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <expected>
#include <string>
#include <vector>

#include "list.h"
#include "list_user.h"
#include "user.h"

class Repository
{
public:
    // TODO: move to end
    // TODO: add admin
    // TODO: remove admin

    virtual ~Repository() = default;
    virtual std::expected<std::vector<User>, std::string> tryGetAllUsers() = 0;
    virtual std::expected<User, std::string> tryGetUser(int64_t telegram_id) = 0;
    virtual std::expected<int64_t, std::string> tryAddList(const List& list) = 0;
    // TODO: remove this
    virtual std::expected<void, std::string> tryRemoveList(int64_t list_id) = 0;
    virtual std::expected<List, std::string> tryGetList(const std::string& list_name) = 0;
    virtual std::expected<List, std::string> tryGetList(int64_t list_id) = 0;
    virtual std::expected<std::vector<List>, std::string> tryGetAllLists() = 0;

    virtual std::expected<void, std::string> trySwapUsers(int64_t list_id,
                                                          int64_t first_user_id,
                                                          int64_t second_user_id) = 0;

    virtual std::expected<User, std::string> tryGetUserByTelegramId(
        int64_t telegram_id) = 0;

    virtual std::expected<std::vector<ListUser>, std::string> tryGetListUsers(
        int64_t list_id) = 0;

    virtual std::expected<std::vector<User>, std::string> tryGetUsers(
        int64_t list_id) = 0;
};

#endif  // !REPOSITORY_H
