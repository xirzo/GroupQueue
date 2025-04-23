#ifndef FILE_READER_H
#define FILE_READER_H

#include <expected>
#include <vector>

#include "user.h"

namespace gq {

class FileReader
{
public:
    virtual ~FileReader() = default;
    virtual std::expected<std::vector<User>, std::string> readUsers() const noexcept = 0;
};

}  // namespace gq

#endif  // !FILE_READER_H
