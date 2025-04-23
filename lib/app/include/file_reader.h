#ifndef FILE_READER_H
#define FILE_READER_H

#include <vector>

#include "admin.h"
#include "user.h"

namespace gq {

class FileReader
{
public:
    virtual ~FileReader() = default;
    virtual std::vector<User> readUsers() const = 0;
    virtual std::vector<Admin> readAdmins() const = 0;
};

}  // namespace gq

#endif  // !FILE_READER_H
