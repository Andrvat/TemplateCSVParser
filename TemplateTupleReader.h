//
// Created by user on 30.11.2020.
//

#pragma once

#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <fstream>

template<typename First>
std::tuple<First> readTuple(std::ifstream &file) {
    First inputData;
    file >> inputData;
    return std::make_tuple(inputData);
}

template<typename First, typename Second, typename ...Args>
std::tuple<First, Second, Args...> readTuple(std::ifstream &file) {
    First inputData;
    file >> inputData;
    return std::tuple_cat(std::make_tuple(inputData), readTuple<Second, Args...>(file));
}

template<typename ...Args>
auto &operator>>(std::ifstream &file, std::tuple<Args...> &tuple) {
    tuple = readTuple<Args...>(file);
    return file;
}