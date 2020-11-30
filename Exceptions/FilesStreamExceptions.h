#pragma once

#include <exception>
#include <string>

namespace TemplateCSVParser {

    class FileStreamExceptions : std::exception {
    private:
        std::string errorMessage_;
    public:
        explicit FileStreamExceptions(const std::string &errorMessage) {
            errorMessage_ = errorMessage;
        };

        [[nodiscard]] const char *what() const noexcept override {
            return errorMessage_.c_str();
        }
    };

}
