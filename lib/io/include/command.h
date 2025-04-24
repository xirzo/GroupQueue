#ifndef COMMAND_H
#define COMMAND_H

#include <expected>
#include <string>
#include <typeinfo>

struct CommandRequest
{
    std::string text;
};

struct CommandResponse
{
    std::string text;
};

class Command
{
public:
    virtual ~Command() = default;

    virtual const std::string str() const = 0;
    virtual std::expected<CommandResponse, std::string> execute(CommandRequest req) = 0;

    virtual std::string typeId() const {
        return typeid(*this).name();
    }
};

#endif  // !COMMAND_H
