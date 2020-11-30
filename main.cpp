#include <iostream>

#include "TemplateTuplePrinter.h"
#include "CSVParser.h"
#include "ArgsHandler.h"

int main(int argc, char *argv[]) {
    std::ifstream inputFileStream;
    inputFileStream.open("input.csv");

    ArgsHandler handler(argc, argv);
    CSVParser<int, long, double, double, std::string, std::string> parser(inputFileStream,
                                                                          handler.getSkipLinesNumber(),
                                                                          handler.getDelimiter());

    for (auto &it : parser) {
        std::cout << it << std::endl;
    }
    return 0;
}
