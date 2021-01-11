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

    class TupleReaderParams {
    private:
        unsigned int column_;
        const char escapeSymbol_;
        const char delimiter_;
    public:
        TupleReaderParams(const unsigned int column, const char escapeSymbol, const char delimiter) :
                column_(column), escapeSymbol_(escapeSymbol), delimiter_(delimiter) {}

        [[nodiscard]] unsigned int getColumn() const {
            return column_;
        }

        [[nodiscard]] char getEscapeSymbol() const {
            return escapeSymbol_;
        }

        [[nodiscard]] char getDelimiter() const {
            return delimiter_;
        }

        void increaseColumn() {
            ++column_;
        }
    };

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
    void readFullValueContainingEscapesSymbol(T &data, std::istringstream &stream, TupleReaderParams &readerParams) {}

    void readFullValueContainingEscapesSymbol(std::string &data, std::istringstream &stream,
                                              TupleReaderParams &readerParams) {
        if (!stream.eof()) {
            skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
            while (isNextStreamSymbolEqualEscape(stream, readerParams.getEscapeSymbol())) {
                throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, readerParams.getColumn());
                size_t lastSymbolStringIndex = data.size() - 1;
                data[lastSymbolStringIndex] = readerParams.getDelimiter();
                std::string extraData;
                skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
                stream >> extraData;
                throwExceptionIfStreamFailedAfterReadingInputData(stream, readerParams.getColumn());
                data += extraData;
                if (!isThereAnyFurtherEscapeSymbols(stream, readerParams.getEscapeSymbol())) {
                    break;
                }
            }
        }
    }

    template<typename First>
    std::tuple<First>
    readNextTuple(std::istringstream &stream, TupleReaderParams &readerParams) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, readerParams.getColumn());
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, readerParams.getColumn());
        readFullValueContainingEscapesSymbol(inputData, stream, readerParams);
        throwExceptionIfExtraDataPresent(stream, readerParams.getColumn());
        readerParams.increaseColumn();
        return std::make_tuple(inputData);
    }

    template<typename First, typename Second, typename ...Args>
    std::tuple<First, Second, Args...>
    readNextTuple(std::istringstream &stream, TupleReaderParams &readerParams) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, readerParams.getColumn());
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, readerParams.getColumn());
        readFullValueContainingEscapesSymbol(inputData, stream, readerParams);
        readerParams.increaseColumn();
        return std::tuple_cat(std::make_tuple(inputData),
                              readNextTuple<Second, Args...>(stream, readerParams));
    }

    template<typename ...Args>
    std::tuple<Args...>
    getCsvFileCurrentRowsTuple(std::istringstream &stream, const char escapeSymbol, const char delimiter) {
        unsigned int currentColumn = 0;
        TupleReaderParams readerParams(currentColumn, escapeSymbol, delimiter);
        return readNextTuple<Args...>(stream, readerParams);
    }
}