#ifndef LISH_H
#define LISH_H

#include <string>

constexpr int64_t kEmptyListId = -1;

struct List
{
    int64_t list_id;
    std::string name;

    List(const std::string& name) : list_id(kEmptyListId), name(name) {}
};

#endif  // !LISH_H
