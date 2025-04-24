#include "post_list.h"

#include <expected>

PostList::PostList(std::shared_ptr<ListHolder> list_holder) noexcept
    : list_holder_(std::move(list_holder)) {}

const std::string PostList::str() const {
    return "/postlist";
}

std::expected<CommandResponse, std::string> PostList::execute(CommandRequest req) {
    if (req.text.empty()) {
        return std::unexpected("Empty parameter for AddList");
    }

    if (req.text.find(' ') != std::string::npos) {
        return std::unexpected("AddList parameter does not consist of one word");
    }

    auto add_result = list_holder_->tryAddList(List(req.text));

    if (!add_result) {
        return std::unexpected(add_result.error());
    }

    return CommandResponse{.text = "Succesfully added new list"};
}
