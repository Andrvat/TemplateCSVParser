#include <iostream>
#include <tuple>

#include "TemplateTuplePrinter.h"
#include "CSVParser.h"


int main(int argc, char *argv[]) {
    std::ifstream inputFileStream;
    inputFileStream.open("input.csv");

    CSVParser<int, std::string, double> parser(inputFileStream, 1, ';');

    for (auto &it : parser) {
        std::cout << it << std::endl;
    }
    return 0;
}
