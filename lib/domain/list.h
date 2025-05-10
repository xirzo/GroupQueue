#ifndef LISH_H
#define LISH_H

#include <string>

constexpr int64_t kEmptyListId = -1;

struct List {
  int64_t list_id;
  std::string list_name;

  List(int64_t list_id, const std::string &name)
      : list_id(list_id), list_name(name) {
  }
  List(const std::string &name) : list_id(kEmptyListId), list_name(name) {
  }
};

#endif // !LISH_H
