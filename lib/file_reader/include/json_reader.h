#ifndef JSON_READER_H
#define JSON_READER_H

#include <filesystem>

#include "file_reader.h"

class JsonReader : public gq::FileReader
{
public:
    JsonReader(std::filesystem::path users_path);

    std::expected<std::vector<gq::User>, std::string> readUsers() const noexcept override;

private:
    std::filesystem::path users_path_;
};

#endif  // !JSON_READER_H
