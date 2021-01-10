//
// Created by user on 28.11.2020.
//

#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <algorithm>
#include <stdexcept>

#include "TemplateTuplePrinter.h"
#include "TemplateTupleReader.h"
#include "CSVParserException.h"

namespace CSVParser {

    static const char DEFAULT_CSV_DELIMITER = ';';
    static const char DEFAULT_CSV_ESCAPE_SYMBOL = '\"';
    static const int DEFAULT_LINES_TO_SKIP_NUMBER = 0;

    template<typename ...Args>
    class CSVTupleParser {
    private:
        std::ifstream &csvInputStream_;
        char csvDelimiter_ = CSVParser::DEFAULT_CSV_DELIMITER;
        char csvEscapeSymbol_ = CSVParser::DEFAULT_CSV_ESCAPE_SYMBOL;
        unsigned int csvLinesToSkipNumber_ = CSVParser::DEFAULT_LINES_TO_SKIP_NUMBER;

        void skipLines() {
            std::string temporaryString;
            unsigned int currentLine = csvLinesToSkipNumber_;
            while (currentLine != 0) {
                std::getline(csvInputStream_, temporaryString);
                --currentLine;
            }
        }

    public:
        CSVTupleParser(std::ifstream &inputStream, const size_t skipLinesNumber,
                       const char delimiter, const char escapeSymbol) :
                csvInputStream_(inputStream),
                csvDelimiter_(delimiter),
                csvLinesToSkipNumber_(skipLinesNumber),
                csvEscapeSymbol_(escapeSymbol) {
            skipLines();
        };

        class iterator : public std::iterator<std::input_iterator_tag, std::tuple<Args...>> {
        private:
            std::ifstream &itInputStream_;
            bool itEndOfFile_ = false;
            std::tuple<Args...> *itTuples_ = nullptr;
            unsigned int itCurrentRow_;
            std::string itCurrentLine_;
            char itDelimiter_;
            char itEscapeSymbol_;
            std::streampos itPos_;

            void setStringStreamLocateParams(std::istringstream &stream) {
                std::locale loc(std::locale::classic(), new CSVParserCType(itDelimiter_));
                stream.imbue(loc);
            }

            class CSVParserCType : public std::ctype<char> {
            private:
                mask performedTable[table_size]{};

            public:
                explicit CSVParserCType(char delimiter, size_t refs = 0)
                        : std::ctype<char>(&performedTable[0], false, refs) {
                    std::copy_n(classic_table(), table_size, performedTable);
                    performedTable[' '] = lower;
                    performedTable[delimiter] = space;
                }
            };

            void throwRuntimeExceptionToStopProgram() {
                throw std::runtime_error("");
            }

            void printAboutInvalidDataAndThrowRuntimeException(const CSVParserException &exception) {
                std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_
                          << ", in column " << exception.getErrorColumn()
                          << " an inappropriate type was encountered!" << std::endl;
                throwRuntimeExceptionToStopProgram();
            }

            void printAboutDataOverflowAndThrowRuntimeException(const CSVParserException &exception) {
                std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_
                          << " " << exception.what() << std::endl;
                throwRuntimeExceptionToStopProgram();
            }

            void readNextFileRowToCurrentLineAndSaveStreamPosition() {
                itInputStream_.clear();
                itInputStream_.seekg(itPos_);
                itInputStream_ >> itCurrentLine_;
                itPos_ = itInputStream_.tellg();
            }

            void readNextTuple() {
                try {
                    readNextFileRowToCurrentLineAndSaveStreamPosition();
                    std::istringstream stringStream(itCurrentLine_);
                    setStringStreamLocateParams(stringStream);
                    *itTuples_ = getCsvFileCurrentRowsTuple<Args...>(stringStream, itEscapeSymbol_);
                    ++itCurrentRow_;
                } catch (CSVParserException &exception) {
                    switch (exception.getErrorType()) {
                        case ExceptionType::InvalidData:
                            printAboutInvalidDataAndThrowRuntimeException(exception);
                        case ExceptionType::DataOverflow:
                            printAboutDataOverflowAndThrowRuntimeException(exception);
                        case ExceptionType::DataUnderflow:
                            std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_
                                      << " " << exception.what() << std::endl;
                            ++itCurrentRow_;
                            ++(*this);
                            break;
                    }
                }
            }

        public:
            friend class CSVTupleParser<Args...>;

            iterator(std::ifstream &itInputStream, const bool itEndOfFile,
                     const unsigned int row, const char delimiter,
                     const char escapeSymbol,
                     const std::streampos streamPosition) :
                    itInputStream_(itInputStream),
                    itEndOfFile_(itEndOfFile),
                    itCurrentRow_(row),
                    itDelimiter_(delimiter),
                    itEscapeSymbol_(escapeSymbol),
                    itPos_(streamPosition) {

                itTuples_ = new std::tuple<Args...>;
                if (itEndOfFile_ || itPos_ == EOF) {
                    itEndOfFile_ = true;
                    return;
                }
                readNextTuple();
            }

            ~iterator() {
                delete itTuples_;
            }

            iterator &operator++() {
                if (itEndOfFile_ || itPos_ == EOF) {
                    itEndOfFile_ = true;
                    return *this;
                }
                readNextTuple();
                return *this;
            }

            bool operator==(const iterator &other) const {
                if (other.itEndOfFile_ && this->itEndOfFile_) {
                    return true;
                }
                return (*itTuples_ == *other.itTuples_);
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }

            typename iterator::reference operator*() const {
                return *itTuples_;
            }
        };

        iterator begin() {
            return iterator(csvInputStream_, false, csvLinesToSkipNumber_ + 1, csvDelimiter_, csvEscapeSymbol_,
                            csvInputStream_.tellg());
        }

        iterator end() {
            return iterator(csvInputStream_, true, UINT_MAX, csvDelimiter_, csvEscapeSymbol_,
                            csvInputStream_.tellg());
        }
    };


}