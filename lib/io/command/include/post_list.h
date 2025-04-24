#ifndef POST_LIST_H
#define POST_LIST_H

#include <memory>

#include "command.h"
#include "list_holder.h"

class PostList : public Command
{
public:
    explicit PostList(std::shared_ptr<ListHolder> list_holder) noexcept;

    const std::string str() const override;
    std::expected<CommandResponse, std::string> execute(CommandRequest req) override;

private:
    std::shared_ptr<ListHolder> list_holder_;
};

#endif  // !POST_LIST_H
