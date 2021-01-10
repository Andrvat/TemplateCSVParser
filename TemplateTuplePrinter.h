//
// Created by user on 28.11.2020.
//

#pragma once

#include <cstdio>
#include <tuple>
#include <iostream>

namespace CSVParser {

    static const char BORDER_LEFT_SYMBOL = '[';
    static const char BORDER_RIGHT_SYMBOL = ']';
    static const char TOKENS_DELIMITER = ' ';

    template<typename Ch, typename Tr, class Tuple, size_t N>
    class TuplePrinter {
    public:
        /*
         * Member-function is static, because need to call it without
         * class object
         *
         * Recursive printing std::tuple with N elements
         */
        static void print(std::basic_ostream<Ch, Tr> &out, const Tuple &t) {
            TuplePrinter<Ch, Tr, Tuple, N - 1>::print(out, t);
            out << TOKENS_DELIMITER << std::get<N - 1>(t);
        }
    };

/*
 * Class-stopping print template std::tuple recursion
 */
    template<typename Ch, typename Tr, class Tuple>
    class TuplePrinter<Ch, Tr, Tuple, 1> {
    public:
        /* Member-function is static, because need to call it without
         * class object */
        static void print(std::basic_ostream<Ch, Tr> &out, const Tuple &t) {
            out << std::get<0>(t);
        }
    };

    template<typename Ch, typename Tr, typename... Args>
    auto &operator<<(std::basic_ostream<Ch, Tr> &out, const std::tuple<Args...> &tuple) {
        out << BORDER_LEFT_SYMBOL;
        TuplePrinter<Ch, Tr, decltype(tuple), sizeof...(Args)>::print(out, tuple);
        out << BORDER_RIGHT_SYMBOL;

        return out;
    }

}
