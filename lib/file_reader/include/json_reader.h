#ifndef JSON_READER_H
#define JSON_READER_H

#include <filesystem>

#include "file_reader.h"

class JsonReader : public gq::FileReader
{
public:
    JsonReader(std::filesystem::path users_path, std::filesystem::path admins_path);

    std::vector<gq::User> readUsers() const override;
    std::vector<gq::Admin> readAdmins() const override;

private:
    std::filesystem::path users_path_;
    std::filesystem::path admins_path_;
};

#endif  // !JSON_READER_H
