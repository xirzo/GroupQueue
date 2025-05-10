#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "list.h"
#include <stdint.h>

class Repository {
  public:
    virtual ~Repository() = default;
    virtual int64_t addList(const List &list) = 0;
    virtual List getList(int64_t list_id) = 0;
    virtual void deleteList(int64_t list_id) = 0;
};

#endif // !REPOSITORY_H
