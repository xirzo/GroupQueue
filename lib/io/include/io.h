#ifndef IO_H
#define IO_H

#include <expected>
#include <memory>
#include <string>

#include "command.h"

class IO
{
public:
    virtual ~IO() = default;

    virtual std::expected<void, std::string> tryRegisterCommand(
        std::unique_ptr<Command> command) = 0;

    virtual std::expected<void, std::string> startListening() = 0;
    virtual void stopListening() = 0;
    virtual bool running() const = 0;
};

#endif  // !IO_H
