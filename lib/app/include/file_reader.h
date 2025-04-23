#ifndef FILE_READER_H
#define FILE_READER_H

#include <expected>
#include <vector>

#include "admin.h"
#include "user.h"

namespace gq {

class FileReader
{
public:
    virtual ~FileReader() = default;
    virtual std::expected<std::vector<User>, std::string> readUsers() const noexcept = 0;
    virtual std::expected<std::vector<Admin>, std::string> readAdmins()
        const noexcept = 0;
};

}  // namespace gq

#endif  // !FILE_READER_H
