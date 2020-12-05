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

template<typename First>
std::tuple<First> readTuple(std::istringstream &stream, unsigned int &column) {
    if (stream.eof()) {
        throw std::length_error("less data was encountered than expected.");
    }
    First inputData;
    stream >> inputData;
    if (stream.fail()) {
        throw std::runtime_error(std::to_string(column));
    }
    if (!stream.eof()) {
        throw std::out_of_range("more data was encountered than expected. This line will be skipped...");
    }
    ++column;
    return std::make_tuple(inputData);
}

template<typename First, typename Second, typename ...Args>
std::tuple<First, Second, Args...> readTuple(std::istringstream &stream, unsigned int &column) {
    if (stream.eof()) {
        throw std::length_error("less data was encountered than expected.");
    }
    First inputData;
    stream >> inputData;
    if (stream.fail()) {
        throw std::runtime_error(std::to_string(column));
    }
    ++column;
    return std::tuple_cat(std::make_tuple(inputData),
                          readTuple<Second, Args...>(stream, column));
}

template<typename ...Args>
auto &operator>>(std::istringstream &stream, std::tuple<Args...> &tuple) {
    unsigned int currentColumn = 1;
    tuple = readTuple<Args...>(stream, currentColumn);
    return stream;
}