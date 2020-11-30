//
// Created by user on 28.11.2020.
//

#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "TemplateTuplePrinter.h"
#include "CSVParserIterator.h"


template<typename ...Args>
class CSVParser {
private:
    std::ifstream &csvInputStream_;
    char csvDelimiter_ = ';';

    void skipLines(size_t skipLinesNumber) {
        std::string temporaryString;
        while (skipLinesNumber != 0) {
            std::getline(csvInputStream_, temporaryString);
            --skipLinesNumber;
        }
    }

public:
    CSVParser(std::ifstream &inputStream, const size_t skipLinesNumber, const char delimiter) :
            csvInputStream_(inputStream),
            csvDelimiter_(delimiter) {
        skipLines(skipLinesNumber);
    };

    iterator<Args...> begin() {
        return iterator<Args...>(csvInputStream_, false);
    }

    iterator<Args...> end() {
        return iterator<Args...>(csvInputStream_, true);
    }
};

template<typename First>
std::tuple<First> readTuple(std::stringstream &stream) {
    First inputData;
    stream >> inputData;
    return std::make_tuple(inputData);
}

template<typename First, typename Second, typename ...Args>
std::tuple<First, Second, Args...> readTuple(std::stringstream &stream) {
    First inputData;
    stream >> inputData;
    return std::tuple_cat(std::make_tuple(inputData), readTuple<Second, Args...>(stream));
}

template<typename ...Args>
std::ifstream &operator>>(std::ifstream &file, std::tuple<Args...> &tuple) {
    std::string fileLine;
    std::getline(file, fileLine);
    std::stringstream processCurrentLine(fileLine);
    std::string item;
    std::string resultString;
    while (std::getline(processCurrentLine, item, ';')) {
        resultString += item;
        resultString += " ";
    }
    std::stringstream tupleStringStream(resultString);
    tuple = readTuple<Args...>(tupleStringStream);
    return file;
}
