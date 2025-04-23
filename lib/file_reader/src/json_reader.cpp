#include "json_reader.h"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "admin.h"
#include "user.h"

namespace gq {

inline void from_json(const nlohmann::json& j, gq::User& u) {
    j.at("user_id").get_to(u.user_id);
    j.at("telegram_id").get_to(u.telegram_id);
    j.at("first_name").get_to(u.first_name);
    j.at("surname").get_to(u.surname);
    j.at("second_name").get_to(u.second_name);
}

inline void from_json(const nlohmann::json& j, gq::Admin& a) {
    j.at("user_id").get_to(a.user_id);
}

}  // namespace gq

JsonReader::JsonReader(std::filesystem::path users_path,
                       std::filesystem::path admins_path)
    : users_path_(users_path), admins_path_(admins_path) {}

std::vector<gq::User> JsonReader::readUsers() const {
    if (std::filesystem::exists(users_path_) == false) {
        throw std::invalid_argument("Users file does not exist");
    }

    std::ifstream file(users_path_);

    if (file.is_open() == false) {
        throw std::invalid_argument("Cannot open users file");
    }

    nlohmann::json j = nlohmann::json::parse(file);

    std::vector<gq::User> users;

    for (const auto& el : j) {
        users.emplace_back(el.get<gq::User>());
    }

    if (users.empty()) {
        throw std::invalid_argument("No users in users file");
    }

    return users;
}

std::vector<gq::Admin> JsonReader::readAdmins() const {
    if (std::filesystem::exists(admins_path_) == false) {
        throw std::invalid_argument("Users file does not exist");
    }

    std::ifstream file(admins_path_);

    if (file.is_open() == false) {
        throw std::invalid_argument("Cannot open admins file");
    }

    nlohmann::json j = nlohmann::json::parse(file);

    std::vector<gq::Admin> admins;

    for (const auto& el : j) {
        admins.emplace_back(el.get<gq::Admin>());
    }

    if (admins.empty()) {
        throw std::invalid_argument("No admins in admins file");
    }

    return admins;
}
