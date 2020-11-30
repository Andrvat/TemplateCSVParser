//
// Created by user on 30.11.2020.
//

#pragma once

#include <string>

class ArgsHandler {
private:
    size_t argc_;
    size_t skipLinesNumber_ = 0;
    char delimiter_ = ';';
public:
    ArgsHandler(int argc, char*argv[]) {
        argc_ = argc;
        if (argc_ >= 2) {
            char *pEnd;
            skipLinesNumber_ = strtoul(argv[1], &pEnd, 10);
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
