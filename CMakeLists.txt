cmake_minimum_required(VERSION 3.17)
project(ImprovedTemplateCSVParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(ImprovedTemplateCSVParser main.cpp TemplateTuplePrinter.h CSVParser.h TemplateTupleReader.h ArgsHandler.h)