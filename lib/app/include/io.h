#ifndef IO_H
#define IO_H

#include <functional>
#include <string>

namespace gq {

enum class InputType
{
    UNKNOWN,
    GET_LIST_USERS,
    GET_LISTS,
    ADD_LIST,
    DELETE_LIST,
};

class IO
{
public:
    virtual ~IO() = default;

    virtual void setCallback(std::function<std::string(gq::InputType)> callback) = 0;
    virtual bool running() const = 0;
    virtual void startListening() = 0;
    virtual void stopListening() = 0;
};

}  // namespace gq

#endif  // !IO_H
