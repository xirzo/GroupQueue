#pragma once

#include <crow/app.h>
#include <crow/common.h>
#include <list_holder.h>

#include <cstddef>
#include <expected>
#include <nlohmann/json.hpp>

class Server
{
public:
    Server(std::size_t port, std::shared_ptr<ListHolder> lh)
        : port_(port), lh_(std::move(lh)) {
        CROW_ROUTE(app_, "/get_all_users").methods(crow::HTTPMethod::Get)([this]() {
            auto get_result = lh_->tryGetAllUsers();

            if (!get_result) {
                return crow::response(400, get_result.error());
            }
            crow::json::wvalue j;

            for (std::size_t i = 0; i < get_result.value().size(); ++i) {
                j[i] = jsonFromUser(get_result.value().at(i));
            }

            return crow::response(200, j);
        });

        CROW_ROUTE(app_, "/get_user/<int>")
            .methods(crow::HTTPMethod::Get)([this](int64_t user_id) {
                auto get_result = lh_->tryGetUser(user_id);

                if (!get_result) {
                    return crow::response(400, get_result.error());
                }
                crow::json::wvalue j = jsonFromUser(get_result.value());

                return crow::response(200, j);
            });

        CROW_ROUTE(app_, "/get_user_by_telegram_id/<int>")
            .methods(crow::HTTPMethod::Get)([this](int64_t telegram_id) {
                auto get_result = lh_->tryGetUserByTelegramId(telegram_id);

                if (!get_result) {
                    return crow::response(400, get_result.error());
                }
                crow::json::wvalue j = jsonFromUser(get_result.value());

                return crow::response(200, j);
            });

        CROW_ROUTE(app_, "/add_list/<string>")
            .methods(crow::HTTPMethod::Post)([this](std::string list_name) {
                auto add_result = lh_->tryAddList(List(list_name));

                if (!add_result) {
                    return crow::response(400, add_result.error());
                }

                return crow::response(200, "Successfully created new list!");
            });

        CROW_ROUTE(app_, "/get_list/<string>")
            .methods(crow::HTTPMethod::Get)([this](std::string list_name) {
                auto get_result = lh_->tryGetList(list_name);

                if (!get_result) {
                    return crow::response(400, get_result.error());
                }

                crow::json::wvalue j = jsonFromList(get_result.value());

                return crow::response(200, j);
            });

        CROW_ROUTE(app_, "/get_all_lists").methods(crow::HTTPMethod::Get)([this]() {
            auto get_result = lh_->tryGetAllLists();

            if (!get_result) {
                return crow::response(400, get_result.error());
            }

            crow::json::wvalue j;

            for (std::size_t i = 0; i < get_result.value().size(); ++i) {
                j[i] = jsonFromList(get_result.value().at(i));
            }

            return crow::response(200, j);
        });

        app_.port(port_).multithreaded().run();
    }

private:
    crow::json::wvalue jsonFromUser(const User& user) {
        crow::json::wvalue j;
        j["user_id"] = user.user_id;
        j["telegram_id"] = user.telegram_id;
        j["first_name"] = user.first_name;
        j["surname"] = user.surname;
        j["second_name"] = user.second_name;
        j["admin"] = user.admin;
        return j;
    }

    crow::json::wvalue jsonFromList(const List& list) {
        crow::json::wvalue j;
        j["list_id"] = list.list_id;
        j["name"] = list.name;
        return j;
    }

private:
    crow::SimpleApp app_;
    std::size_t port_;
    std::shared_ptr<ListHolder> lh_;
};
