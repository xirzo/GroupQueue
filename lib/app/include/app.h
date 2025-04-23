#ifndef APP_H
#define APP_H

#include <memory>

#include "file_reader.h"

namespace gq {

class App
{
public:
    App(std::unique_ptr<FileReader> file_reader);

private:
    std::unique_ptr<FileReader> file_reader_;
};

}  // namespace gq

#endif  // !APP_H
