#ifndef USER_H
#define USER_H

#include <cstddef>
#include <string>

namespace gq {

struct User
{
    std::size_t user_id;
    std::size_t telegram_id;
    std::string first_name;
    std::string surname;
    std::string second_name;
};

}  // namespace gq

#endif  // !USER_H
