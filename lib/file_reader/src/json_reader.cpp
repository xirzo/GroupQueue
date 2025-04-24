#include "json_reader.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "user.h"

namespace gq {

inline void from_json(const nlohmann::json& j, gq::User& u) {
    u.user_id = kEmptyUserId;
    j.at("telegram_id").get_to(u.telegram_id);
    j.at("first_name").get_to(u.first_name);
    j.at("surname").get_to(u.surname);
    j.at("second_name").get_to(u.second_name);
    j.at("admin").get_to(u.admin);
}

}  // namespace gq

JsonReader::JsonReader(std::filesystem::path users_path) : users_path_(users_path) {}

std::expected<std::vector<gq::User>, std::string> JsonReader::readUsers() const noexcept {
    if (std::filesystem::exists(users_path_) == false) {
        return std::unexpected("Users file does not exist");
    }

    std::ifstream file(users_path_);

    if (file.is_open() == false) {
        return std::unexpected("Cannot open users file");
    }

    nlohmann::json j;

    try {
        j = nlohmann::json::parse(file);
    }
    catch (const std::exception& e) {
        return std::unexpected(std::string("Users JSON parse error: ") + e.what());
    }

    std::vector<gq::User> users;

    for (const auto& el : j) {
        users.emplace_back(el.get<gq::User>());
    }

    if (users.empty()) {
        return std::unexpected("No users in users file");
    }

    return users;
}
