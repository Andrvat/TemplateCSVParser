#include <iostream>

#include "TemplateTuplePrinter.h"
#include "CSVParser.h"
#include "ArgsHandler.h"

int main(int argc, char *argv[]) {
    std::ifstream inputFileStream;
    inputFileStream.open("input.csv", std::ios::binary);
    std::freopen("log.txt", "w", stderr);

    ArgsHandler handler(argc, argv);
    CSVParser<int, long, double, double, std::string, int> parser(inputFileStream,
                                                                          handler.getSkipLinesNumber(),
                                                                          handler.getDelimiter());
    try {
        for (auto &it : parser) {
            auto test = parser.begin();
            ++test;
            std::cout << it << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << "Main: program stopped!" << std::endl;
    }
    return 0;
}
