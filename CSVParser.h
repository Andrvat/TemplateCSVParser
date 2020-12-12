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

template<typename ...Args>
class CSVParser {
private:
    std::ifstream &csvInputStream_;
    char csvDelimiter_ = ';';
    unsigned int csvSkipLinesNumber_ = 0;

    void skipLines() {
        std::string temporaryString;
        unsigned int currentLine = csvSkipLinesNumber_;
        while (currentLine != 0) {
            std::getline(csvInputStream_, temporaryString);
            --currentLine;
        }
    }

public:
    CSVParser(std::ifstream &inputStream, const size_t skipLinesNumber, const char delimiter) :
            csvInputStream_(inputStream),
            csvDelimiter_(delimiter),
            csvSkipLinesNumber_(skipLinesNumber) {
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

        void readNextTuple() {
            try {
                itInputStream_.clear();
                itInputStream_.seekg(itPos_);
                itInputStream_ >> itCurrentLine_;
                itPos_ = itInputStream_.tellg();
                std::istringstream stringStream(itCurrentLine_);
                setStringStreamLocateParams(stringStream);
                stringStream >> *itTuples_;
                ++itCurrentRow_;
            } catch (CSVParserException &exception) {
                switch (exception.getErrorType()) {
                    case ExceptionType::InvalidData:
                        std::cerr << "CSVParser Iterator: in row " << itCurrentRow_ << ", in column "
                                  << exception.getErrorColumn()
                                  << " an inappropriate type was encountered!" << std::endl;
                        throw std::runtime_error("");
                    case ExceptionType::DataOverflow:
                        // less data was encountered than expected
                        std::cerr << "CSVParser Iterator: in row " << itCurrentRow_
                                  << " " << exception.what() << std::endl;
                        throw std::runtime_error("");
                    case ExceptionType::DataUnderflow:
                        // more data was encountered than expected
                        std::cerr << "CSVParser Iterator: in row " << itCurrentRow_
                                  << " " << exception.what() << std::endl;
                        ++itCurrentRow_;
                        ++(*this);
                        break;
                }
            }
        }

    public:
        friend class CSVParser<Args...>;

        iterator(std::ifstream &itInputStream, const bool itEndOfFile, const unsigned int row, const char delimiter) :
                itInputStream_(itInputStream),
                itEndOfFile_(itEndOfFile),
                itCurrentRow_(row),
                itDelimiter_(delimiter) {

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
        csvInputStream_.clear();
        csvInputStream_.seekg(0);
        return iterator(csvInputStream_, false, csvSkipLinesNumber_ + 1, csvDelimiter_);
    }

    iterator end() {
        return iterator(csvInputStream_, true, UINT_MAX, csvDelimiter_);
    }
};


