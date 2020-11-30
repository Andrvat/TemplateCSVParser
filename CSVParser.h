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

#include "TemplateTuplePrinter.h"
#include "TemplateTupleReader.h"

template<typename ...Args>
class CSVParser {
private:
    std::ifstream &csvInputStream_;
    char csvDelimiter_;

    void skipLines(size_t skipLinesNumber) {
        std::string temporaryString;
        while (skipLinesNumber != 0) {
            std::getline(csvInputStream_, temporaryString);
            --skipLinesNumber;
        }
    }

    void setCSVLocateParams() {
        std::locale x(std::locale::classic(), new CSVParserCType(csvDelimiter_));
        csvInputStream_.imbue(x);
    }

    class CSVParserCType : public std::ctype<char> {
        mask performedTable[table_size]{};
    public:
        CSVParserCType(char delimiter, size_t refs = 0)
                : std::ctype<char>(&performedTable[0], false, refs) {
            std::copy_n(classic_table(), table_size, performedTable);
            performedTable[' '] = lower;
            performedTable[delimiter] = space;
        }
    };

public:
    CSVParser(std::ifstream &inputStream, const size_t skipLinesNumber, const char delimiter) :
            csvInputStream_(inputStream),
            csvDelimiter_(delimiter) {
        setCSVLocateParams();
        skipLines(skipLinesNumber);
    };

    template<typename ...IterArgs>
    class iterator : public std::iterator<std::input_iterator_tag, std::tuple<IterArgs...>> {
    private:
        std::ifstream &itInputStream_;
        bool itEndOfFile_ = false;
        std::tuple<Args...> *itTuples_;

    public:

        friend class CSVParser<IterArgs...>;

        iterator(std::ifstream &itInputStream, const bool itEndOfFile) :
                itInputStream_(itInputStream),
                itEndOfFile_(itEndOfFile) {

            if (itEndOfFile_) {
                return;
            }

            itTuples_ = new std::tuple<IterArgs...>;

            if (itInputStream_.eof()) {
                itEndOfFile_ = true;
            } else {
                itInputStream_ >> *itTuples_;
            }
        }

        iterator &operator++() {
            if (itInputStream_.eof()) {
                itEndOfFile_ = true;
            } else {
                itInputStream_ >> *itTuples_;
            }
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

    iterator<Args...> begin() {
        return iterator<Args...>(csvInputStream_, false);
    }

    iterator<Args...> end() {
        return iterator<Args...>(csvInputStream_, true);
    }
};


