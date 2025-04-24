#ifndef SERVER_H
#define SERVER_H

#include <crow.h>

#include <atomic>
#include <initializer_list>
#include <memory>
#include <thread>
#include <typeindex>

#include "io.h"

// class ServerIO : public gq::IO
// {
// public:
//     ServerIO(std::size_t port);
//     ServerIO(std::size_t port,
//              std::initializer_list<std::pair<gq::Command*, std::string>> list);
//     ~ServerIO() override;
//
//     std::expected<void, std::string> tryAddCommand(std::unique_ptr<gq::Command>
//     command,
//                                                    const std::string& endpoint);
//
//     bool running() const override;
//     std::expected<void, std::string> startListening() override;
//     void stopListening() override;
//
// private:
//     std::size_t port_;
//     std::unordered_map<std::type_index, std::unique_ptr<gq::Command>> commands_;
//     std::unordered_map<std::type_index, std::string> endpoints_;
//
//     crow::SimpleApp app_;
//     std::thread server_thread_;
//     std::atomic<bool> running_;
// };

#endif  // !SERVER_H
