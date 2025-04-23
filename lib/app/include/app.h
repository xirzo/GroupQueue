#ifndef APP_H
#define APP_H

#include <expected>
#include <memory>
#include <optional>

#include "file_reader.h"

namespace gq {

class App
{
public:
    App(std::unique_ptr<FileReader> file_reader) noexcept;

    std::optional<std::string> init() noexcept;

private:
    std::unique_ptr<FileReader> file_reader_;
};

template <typename... Args>
std::expected<std::unique_ptr<App>, std::string> makeApp(Args&&... args) {
    auto app = std::make_unique<App>(std::forward<Args>(args)...);
    std::optional<std::string> init_result = app->init();

    if (init_result && init_result->empty() == false) {
        return std::unexpected(init_result.value());
    }

    return std::move(app);
}

}  // namespace gq

#endif  // !APP_H
