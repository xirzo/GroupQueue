#ifndef USER_H
#define USER_H

#include <string>

namespace gq {

struct User
{
    int64_t user_id;
    int64_t telegram_id;
    std::string first_name;
    std::string surname;
    std::string second_name;
    bool admin;
};

}  // namespace gq

#endif  // !USER_H
