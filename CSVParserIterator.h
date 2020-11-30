//
// Created by user on 30.11.2020.
//

#pragma once

#include <iterator>
#include <tuple>
#include <string>

#include "CSVParser.h"
#include "TemplateTuplePrinter.h"

template<typename ...Args>
class CSVParser;

template<typename ...Args>
class iterator : public std::iterator<std::input_iterator_tag, std::tuple<Args...>> {
private:
    std::ifstream &itInputStream_;
    bool itEndOfFile_ = false;
    std::tuple<Args...> *itTuples_;

public:

    friend class CSVParser<Args...>;

    iterator(std::ifstream &itInputStream, const bool itEndOfFile) :
            itInputStream_(itInputStream),
            itEndOfFile_(itEndOfFile) {

        if (itEndOfFile_) {
            return;
        }

        itTuples_ = new std::tuple<Args...>;

        if (itInputStream_.eof()) {
            itEndOfFile_ = true;
            return;
        }
        itInputStream_ >> *itTuples_;
    }

    iterator &operator++() {
        if (itInputStream_.eof()) {
            itEndOfFile_ = true;
        }
        itInputStream_ >> *itTuples_;
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

    explicit operator std::tuple<Args...>() {
        return itTuples_;
    }

    typename iterator::reference operator*() const {
        return *itTuples_;
    }

};