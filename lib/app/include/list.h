#ifndef LISH_H
#define LISH_H

#include <string>

namespace gq {

struct List
{
    int64_t list_id;
    std::string name;

    List(const std::string& name) : list_id(-1), name(name) {}
};

}  // namespace gq

#endif  // !LISH_H
