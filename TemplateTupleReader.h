//
// Created by user on 30.11.2020.
//

#pragma once

#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <fstream>
#include <stdexcept>

#include "CSVParserException.h"

namespace CSVParser {

    template<typename First>
    std::tuple<First> readTuple(std::istringstream &stream, unsigned int &column) {
        if (stream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", column,
                                                CSVParser::ExceptionType::DataUnderflow);
        }
        First inputData;
        stream >> inputData;
        if (stream.fail()) {
            throw CSVParser::CSVParserException("", column, CSVParser::ExceptionType::InvalidData);
        }
        if (!stream.eof()) {
            throw CSVParser::CSVParserException("more data was encountered than expected. This line will be skipped...",
                                                column,
                                                CSVParser::ExceptionType::DataOverflow);
        }
        ++column;
        return std::make_tuple(inputData);
    }

    template<typename First, typename Second, typename ...Args>
    std::tuple<First, Second, Args...> readTuple(std::istringstream &stream, unsigned int &column) {
        if (stream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", column,
                                                CSVParser::ExceptionType::DataUnderflow);
        }
        First inputData;
        stream >> inputData;
        if (stream.fail()) {
            throw CSVParser::CSVParserException("", column, CSVParser::ExceptionType::InvalidData);
        }
        ++column;
        return std::tuple_cat(std::make_tuple(inputData),
                              readTuple<Second, Args...>(stream, column));
    }

    template<typename ...Args>
    auto &operator>>(std::istringstream &stream, std::tuple<Args...> &tuple) {
        unsigned int currentColumn = 0;
        tuple = readTuple<Args...>(stream, currentColumn);
        return stream;
    }
}