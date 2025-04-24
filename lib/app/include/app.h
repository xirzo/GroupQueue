#ifndef APP_H
#define APP_H

#include <expected>
#include <filesystem>
#include <memory>

#include "db.h"
#include "file_reader.h"
#include "io.h"

namespace gq {

class App
{
public:
    App(std::unique_ptr<FileReader> file_reader, std::unique_ptr<IO> io) noexcept;
    ~App();

    std::expected<void, std::string> init(std::filesystem::path db_path) noexcept;

private:
    std::unique_ptr<FileReader> file_reader_;
    std::unique_ptr<IO> io_;
    std::unique_ptr<Database> db_;
};

template <typename... Args>
std::expected<std::unique_ptr<App>, std::string> makeApp(std::filesystem::path db_path,
                                                         Args&&... args) {
    auto app = std::make_unique<App>(std::forward<Args>(args)...);
    std::expected<void, std::string> init_result = app->init(db_path);

    if (!init_result) {
        return std::unexpected(init_result.error());
    }

    return std::move(app);
}

}  // namespace gq

#endif  // !APP_H
