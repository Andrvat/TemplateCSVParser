#include <iostream>

#include "TemplateTuplePrinter.h"
#include "CSVParser.h"
#include "ArgsHandler.h"

int main(int argc, char *argv[]) {
    std::ifstream inputFileStream;
    inputFileStream.open("input.csv");
    std::freopen("log.txt", "w", stderr);

    ArgsHandler handler(argc, argv);
    CSVParser<int, long, double, double, std::string, std::string> parser(inputFileStream,
                                                                          handler.getSkipLinesNumber(),
                                                                          handler.getDelimiter());
    try {
        for (auto &it : parser) {
            std::cout << it << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << "Main: program stopped!" << std::endl;
    }
    return 0;
}
