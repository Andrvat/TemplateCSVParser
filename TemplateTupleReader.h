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
#include <type_traits>

#include "CSVParserException.h"

namespace TupleOperators {

    static const unsigned int NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING = 1;

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

    void skipSymbols(std::istringstream &stream, size_t skipSymbolsNumber) {
        for (int skipSymbolsCounter = 0; skipSymbolsCounter < skipSymbolsNumber; skipSymbolsCounter++) {
            stream.get();
        }
    }

    bool isNextStreamSymbolEqualEscape(std::istringstream &stream, const char escapeSymbol) {
        return stream.peek() == escapeSymbol;
    }

    bool isThereAnyFurtherEscapeSymbols(std::istringstream &stream, const char escapeSymbol) {
        if (!stream.eof()) {
            skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
            if (!stream.eof() && stream.peek() == escapeSymbol) {
                return true;
            } else {
                stream.unget();
            }
        }

        return false;
    }

    template<typename T>
    void readFullValueContainingEscapesSymbol(T &data, std::istringstream &stream, const unsigned int column,
                                              const char escapeSymbol, const char delimiter) {}

    void readFullValueContainingEscapesSymbol(std::string &data, std::istringstream &stream, const unsigned int column,
                                              const char escapeSymbol, const char delimiter) {
        if (!stream.eof()) {
            skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
            while (isNextStreamSymbolEqualEscape(stream, escapeSymbol)) {
                throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, column);
                size_t lastSymbolStringIndex = data.size() - 1;
                data[lastSymbolStringIndex] = delimiter;
                std::string extraData;
                skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
                stream >> extraData;
                throwExceptionIfStreamFailedAfterReadingInputData(stream, column);
                data += extraData;
                if (!isThereAnyFurtherEscapeSymbols(stream, escapeSymbol)) {
                    break;
                }
            }
        }
    }

    template<typename First>
    std::tuple<First>
    readNextTuple(std::istringstream &stream, unsigned int &column, const char escapeSymbol, const char delimiter) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, column);
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, column);
        readFullValueContainingEscapesSymbol(inputData, stream, column, escapeSymbol, delimiter);
        throwExceptionIfExtraDataPresent(stream, column);
        ++column;
        return std::make_tuple(inputData);
    }

    template<typename First, typename Second, typename ...Args>
    std::tuple<First, Second, Args...>
    readNextTuple(std::istringstream &stream, unsigned int &column, const char escapeSymbol, const char delimiter) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, column);
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, column);
        readFullValueContainingEscapesSymbol(inputData, stream, column, escapeSymbol, delimiter);
        ++column;
        return std::tuple_cat(std::make_tuple(inputData),
                              readNextTuple<Second, Args...>(stream, column, escapeSymbol, delimiter));
    }

    template<typename ...Args>
    std::tuple<Args...> getCsvFileCurrentRowsTuple(std::istringstream &stream, const char escapeSymbol, const char delimiter) {
        unsigned int currentColumn = 0;
        return readNextTuple<Args...>(stream, currentColumn, escapeSymbol, delimiter);
    }
}