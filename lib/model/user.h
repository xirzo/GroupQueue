#ifndef USER_H
#define USER_H

#include <string>

constexpr int64_t kEmptyUserId = -1;

struct User
{
    int64_t user_id;
    int64_t telegram_id;
    std::string first_name;
    std::string surname;
    std::string second_name;
    bool admin;

    User(int64_t telegram_id, std::string first_name, std::string surname,
         std::string second_name, bool admin)
        : user_id(kEmptyUserId),
          telegram_id(telegram_id),
          first_name(std::move(first_name)),
          surname(std::move(surname)),
          second_name(std::move(second_name)),
          admin(admin) {}
};

#endif  // !USER_H
