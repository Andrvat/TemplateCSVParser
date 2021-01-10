#include <iostream>

#include "TemplateTuplePrinter.h"
#include "CSVTupleParser.h"
#include "CSVParserArgsHandler.h"

using namespace TupleOperators;

static const std::string MAIN_INDICATOR = "Main:";
static const std::string STOP_PROGRAM_MESSAGE = "program stopped!";

void redirectStandardErrorStreamToFileStream(const char *filename) {
    std::freopen(filename, "w", stderr);
}

void printInfoAboutStoppingProgram() {
    std::cerr << MAIN_INDICATOR << " " << STOP_PROGRAM_MESSAGE << std::endl;
}

int main(int argc, char *argv[]) {
    redirectStandardErrorStreamToFileStream("log.txt");

    CSVParser::CSVParserArgsHandler handler;
    try {
        CSVParser::initHandlerParams(handler, argc, argv);
    } catch (std::exception &e) {
        printInfoAboutStoppingProgram();
        return EXIT_SUCCESS;
    }

    std::ifstream inputFileStream;
    inputFileStream.open(handler.getSourceFileName(), std::ios::binary);
    CSVParser::CSVTupleParser<int, std::string, double, double, std::string> parser(inputFileStream,
                                                                                    handler.getLinesToSkipNumber(),
                                                                                    handler.getCsvDelimiter(),
                                                                                    handler.getCsvEscapeSymbol());
    try {
        for (auto &it : parser) {
            std::cout << it << std::endl;
        }
    } catch (std::exception &e) {
        printInfoAboutStoppingProgram();
    }
    return EXIT_SUCCESS;
}
