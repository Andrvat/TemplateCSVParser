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

    void throwExceptionIfEndOfFileMetEarlierThanShouldBe(std::istringstream &stream, const unsigned int column) {
        if (stream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", column,
                                                CSVParser::ExceptionType::DataUnderflow);
        }
    }

    void throwExceptionIfStreamFailedAfterReadingInputData(std::istringstream &stream,
                                                           const unsigned int column) {
        if (stream.fail()) {
            throw CSVParser::CSVParserException("", column, CSVParser::ExceptionType::InvalidData);
        }
    }

    void throwExceptionIfExtraDataPresent(std::istringstream &stream, const unsigned int column) {
        if (!stream.eof()) {
            throw CSVParser::CSVParserException("more data was encountered than expected. This line will be skipped...",
                                                column, CSVParser::ExceptionType::DataOverflow);
        }
    }

    template<typename First>
    std::tuple<First> readNextTuple(std::istringstream &stream, unsigned int &column, const char escapeSymbol) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, column);
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, column);
        throwExceptionIfExtraDataPresent(stream, column);
        ++column;
        return std::make_tuple(inputData);
    }

    template<typename First, typename Second, typename ...Args>
    std::tuple<First, Second, Args...> readNextTuple(std::istringstream &stream, unsigned int &column, const char escapeSymbol) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, column);
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, column);
        ++column;
        return std::tuple_cat(std::make_tuple(inputData),
                              readNextTuple<Second, Args...>(stream, column, escapeSymbol));
    }

    template<typename ...Args>
    std::tuple<Args...> getCsvFileCurrentRowsTuple(std::istringstream &stream, const char escapeSymbol) {
        unsigned int currentColumn = 0;
        return readNextTuple<Args...>(stream, currentColumn, escapeSymbol);
    }
}