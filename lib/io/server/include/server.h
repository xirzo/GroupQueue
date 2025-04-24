#ifndef SERVER_H
#define SERVER_H

#include <crow/app.h>

#include <cstddef>
#include <thread>
#include <typeindex>

#include "io.h"

class ServerIO : public IO
{
public:
    ServerIO(std::size_t port) noexcept;
    ~ServerIO() override;

    std::expected<void, std::string> tryRegisterCommand(
        std::unique_ptr<Command> command) override;

    bool running() const override;
    std::expected<void, std::string> startListening() override;
    void stopListening() override;

private:
    std::size_t port_;

    std::unordered_map<std::type_index, std::unique_ptr<Command>> commands_;

    crow::SimpleApp app_;
    std::thread server_thread_;
    std::atomic<bool> running_;
};

#endif  // !SERVER_H
