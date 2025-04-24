#ifndef SERVER_H
#define SERVER_H

#include <crow.h>

#include <atomic>
#include <thread>

#include "io.h"

class ServerIO : public gq::IO
{
public:
    ServerIO(std::size_t port);
    ~ServerIO() override;

    void setCallback(std::function<std::string(gq::InputType)> callback) override;
    bool running() const override;
    void startListening() override;
    void stopListening() override;

private:
    std::size_t port_;
    crow::SimpleApp app_;
    std::function<std::string(gq::InputType)> callback_;
    std::thread server_thread_;
    std::atomic<bool> running_;
};

#endif  // !SERVER_H
