#include <iostream>

#include "TemplateTuplePrinter.h"
#include "CSVTupleParser.h"
#include "CSVParserArgsHandler.h"

using namespace CSVParser;

int main(int argc, char *argv[]) {
    static const std::string MAIN_INDICATOR = "Main:";
    static const std::string STOP_PROGRAM_MESSAGE = "program stopped!";
    std::freopen("log.txt", "w", stderr);

    CSVParser::CSVParserArgsHandler handler;
    try {
        handler.setArgumentsAmount(argc);
        handler.setSourceFileName(argv);
        handler.setLinesToSkipNumber(argv);
        handler.setCsvDelimiter(argv);
        handler.setCsvEscapeSymbol(argv);
    } catch (std::exception &e) {
        std::cerr << MAIN_INDICATOR << " " << STOP_PROGRAM_MESSAGE << std::endl;
        return EXIT_SUCCESS;
    }
    std::ifstream inputFileStream;
    inputFileStream.open(handler.getSourceFileName(), std::ios::binary);
    CSVParser::CSVTupleParser<int, long, double, double, std::string> parser(inputFileStream,
                                                                             handler.getLinesToSkipNumber(),
                                                                             handler.getCsvDelimiter(),
                                                                             handler.getCsvEscapeSymbol());
    try {
        for (auto &it : parser) {
            std::cout << it << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << MAIN_INDICATOR << " " << STOP_PROGRAM_MESSAGE << std::endl;
    }
    return EXIT_SUCCESS;
}
