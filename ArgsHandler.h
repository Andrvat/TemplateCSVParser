//
// Created by user on 30.11.2020.
//

#pragma once

#include <string>

class ArgsHandler {
private:
    size_t argc_;
    int skipLinesNumber_ = 0;
    char delimiter_ = ';';
public:
    ArgsHandler(int argc, char *argv[]) {
        argc_ = argc;
        if (argc_ >= 2) {
            skipLinesNumber_ = std::stoi(argv[1]);
            if (skipLinesNumber_ < 0) {
                std::cerr << "Handler: invalid skip lines number! Set default = 0" << std::endl;
                skipLinesNumber_ = 0;
            }
        }
        if (argc_ >= 3) {
            delimiter_ = argv[2][0];
        }
    }

    [[nodiscard]] size_t getArgc() const {
        return argc_;
    }

    [[nodiscard]] size_t getSkipLinesNumber() const {
        return skipLinesNumber_;
    }

    [[nodiscard]] char getDelimiter() const {
        return delimiter_;
    }
};
