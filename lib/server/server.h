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
        CROW_ROUTE(app_, "/add_list/<string>")
        ([this](std::string list_name) {
            auto add_result = lh_->tryAddList(List(list_name));

            if (!add_result) {
                return crow::response(400, add_result.error());
            }

            return crow::response(200, "Successfully created new list!");
        });

        app_.port(port_).multithreaded().run();
    }

private:
    crow::SimpleApp app_;
    std::size_t port_;
    std::shared_ptr<ListHolder> lh_;
};
