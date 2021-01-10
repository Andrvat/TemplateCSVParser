cmake_minimum_required(VERSION 3.17)
project(ImprovedTemplateCSVParser)

set(CMAKE_CXX_STANDARD 17)

add_executable(ImprovedTemplateCSVParser main.cpp TemplateTuplePrinter.h CSVTupleParser.h TemplateTupleReader.h CSVParserArgsHandler.h CSVParserException.h)