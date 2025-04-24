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
};

#endif  // !USER_H
