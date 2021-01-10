//
// Created by user on 30.11.2020.
//

#pragma once

#include <string>

namespace CSVParser {

    enum class CSVParserConfigsArgumentsAmount {
        NO_SPECIFIED_ALL_FEATURES = 2,
        SPECIFIED_LINES_TO_SKIP_NUMBER = 3,
        SPECIFIED_DELIMITER_AND_LINES_TO_SKIP = 4,
        SPECIFIED_DELIMITER_LINES_TO_SKIP_AND_ESCAPE_SYMBOL = 5,
        INVALID_ARGUMENTS
    };

    static const std::string CSV_HANDLER_INDICATOR = "Handler:";
    static const std::string CSV_VALIDATOR_INDICATOR = "Validator:";
    static const std::string INVALID_ARGUMENTS_AMOUNT_MESSAGE = "In configurations there were invalid arguments than expected!";
    static const std::string INVALID_LINES_TO_SKIP_NUMBER_MESSAGE = "Invalid lines to skip number!";
    static const std::string INVALID_DELIMITER_MESSAGE = "Invalid delimiter!";
    static const std::string INVALID_ESCAPE_SYMBOL_MESSAGE = "Invalid escape symbol!";
    static const std::string INVALID_SOURCE_FILE_NAME_MESSAGE = "Invalid source file name! File can't be opened!";
    static const std::string SET_DEFAULT_NUMBER_MESSAGE = "Set default:";

    static const int ARGS_VALUE_DELIMITER_STRING_LENGTH = 1;

    class CSVParserArgsHandler {
    private:
        size_t argumentsAmount_;
        std::string sourceFileName_;

        int linesToSkipNumber_ = DEFAULT_LINES_TO_SKIP_NUMBER;
        char csvDelimiter_ = DEFAULT_CSV_DELIMITER;
        char csvEscapeSymbol_ = DEFAULT_CSV_ESCAPE_SYMBOL;

        CSVParserConfigsArgumentsAmount parserConfigurationIndicator_ = CSVParserConfigsArgumentsAmount::INVALID_ARGUMENTS;

        [[nodiscard]] bool isParserConfigsIndicatorSpecifiedDelimiterAndLinesToSkipNumber() const {
            return parserConfigurationIndicator_ ==
                   CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_AND_LINES_TO_SKIP;
        }

        [[nodiscard]] bool isParserConfigsIndicatorSpecifiedDelimiterLinesToSkipAndEscapeSymbol() const {
            return parserConfigurationIndicator_ ==
                   CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_LINES_TO_SKIP_AND_ESCAPE_SYMBOL;
        }

        class DataValidator {
        public:
            static bool isArgumentsAmountOneOfConstArgsValues(const int value) {
                return value == static_cast<int>(CSVParserConfigsArgumentsAmount::NO_SPECIFIED_ALL_FEATURES) ||
                       value == static_cast<int>(CSVParserConfigsArgumentsAmount::SPECIFIED_LINES_TO_SKIP_NUMBER) ||
                       value ==
                       static_cast<int>(CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_AND_LINES_TO_SKIP) ||
                       value ==
                       static_cast<int>(CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_LINES_TO_SKIP_AND_ESCAPE_SYMBOL);
            }

            static bool isStringValueDigit(const std::string &stringNumber) {
                if (!std::all_of(stringNumber.begin(), stringNumber.end(), isdigit)) {
                    printInfoAboutSettingDefaultLinesToSkipNumber();
                    return false;
                }
                return true;
            }

            static bool isStringValueContainOnlyOneLetter(const std::string &string) {
                if (string.size() != ARGS_VALUE_DELIMITER_STRING_LENGTH) {
                    return false;
                }
                return true;
            }

            static void printInfoAboutSettingDefaultCsvDelimiter() {
                std::cerr << CSV_HANDLER_INDICATOR << " "
                          << CSV_VALIDATOR_INDICATOR << " "
                          << INVALID_DELIMITER_MESSAGE << " "
                          << SET_DEFAULT_NUMBER_MESSAGE << " " << DEFAULT_CSV_DELIMITER
                          << std::endl;
            }

            static void printInfoAboutSettingDefaultEscapeSymbol() {
                std::cerr << CSV_HANDLER_INDICATOR << " "
                          << CSV_VALIDATOR_INDICATOR << " "
                          << INVALID_ESCAPE_SYMBOL_MESSAGE << " "
                          << SET_DEFAULT_NUMBER_MESSAGE << " " << DEFAULT_CSV_ESCAPE_SYMBOL
                          << std::endl;
            }

            static void printInfoAboutSettingDefaultLinesToSkipNumber() {
                std::cerr << CSV_HANDLER_INDICATOR << " "
                          << CSV_VALIDATOR_INDICATOR << " "
                          << INVALID_LINES_TO_SKIP_NUMBER_MESSAGE << " "
                          << SET_DEFAULT_NUMBER_MESSAGE << " " << DEFAULT_LINES_TO_SKIP_NUMBER
                          << std::endl;
            }

            static void printInfoAboutUnableToOpenSourceFile() {
                std::cerr << CSV_HANDLER_INDICATOR << " "
                          << CSV_VALIDATOR_INDICATOR << " "
                          << INVALID_SOURCE_FILE_NAME_MESSAGE
                          << std::endl;
            }

            static void printInfoAboutInvalidArgument() {
                std::cerr << CSV_HANDLER_INDICATOR << " "
                          << INVALID_ARGUMENTS_AMOUNT_MESSAGE
                          << std::endl;
            }

            static bool isFileWithSourceFileNameCanBeOpened(const std::string &filename) {
                std::ifstream stream(filename);
                if (!stream.is_open()) {
                    return false;
                }
                stream.close();
                return true;
            }
        };

    public:

        void setSourceFileName(char *argumentsValues[]) {
            if (parserConfigurationIndicator_ == CSVParserConfigsArgumentsAmount::INVALID_ARGUMENTS) {
                DataValidator::printInfoAboutUnableToOpenSourceFile();
                throw std::invalid_argument(CSV_HANDLER_INDICATOR);
            }
            size_t possibleSourceFileNameArgsValuesIndex =
                    static_cast<int>(CSVParserConfigsArgumentsAmount::NO_SPECIFIED_ALL_FEATURES) - 1;
            std::string sourceFileNameFromConfigs(argumentsValues[possibleSourceFileNameArgsValuesIndex]);
            if (!DataValidator::isFileWithSourceFileNameCanBeOpened(sourceFileNameFromConfigs)) {
                DataValidator::printInfoAboutUnableToOpenSourceFile();
                throw std::invalid_argument(CSV_HANDLER_INDICATOR);
            } else {
                sourceFileName_ = sourceFileNameFromConfigs;
            }
        }

        void setArgumentsAmount(const int argumentsAmount) {
            if (!DataValidator::isArgumentsAmountOneOfConstArgsValues(argumentsAmount)) {
                DataValidator::printInfoAboutInvalidArgument();
                throw std::invalid_argument(CSV_HANDLER_INDICATOR);
            }
            argumentsAmount_ = argumentsAmount;
            parserConfigurationIndicator_ = (CSVParserConfigsArgumentsAmount) argumentsAmount;
        }

        void setLinesToSkipNumber(char *argumentValues[]) {
            std::string possibleLinesToSkipNumberStringContainer;
            size_t possibleLinesToSkipNumberArgsValuesIndex;
            switch (parserConfigurationIndicator_) {
                case CSVParserConfigsArgumentsAmount::SPECIFIED_LINES_TO_SKIP_NUMBER:
                case CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_AND_LINES_TO_SKIP:
                case CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_LINES_TO_SKIP_AND_ESCAPE_SYMBOL:
                    possibleLinesToSkipNumberArgsValuesIndex =
                            static_cast<int>(CSVParserConfigsArgumentsAmount::SPECIFIED_LINES_TO_SKIP_NUMBER) - 1;
                    possibleLinesToSkipNumberStringContainer.append(
                            argumentValues[possibleLinesToSkipNumberArgsValuesIndex]);
                    if (DataValidator::isStringValueDigit(possibleLinesToSkipNumberStringContainer)) {
                        linesToSkipNumber_ = std::stoi(possibleLinesToSkipNumberStringContainer);
                    } else {
                        linesToSkipNumber_ = DEFAULT_LINES_TO_SKIP_NUMBER;
                    }
                    break;
                default:
                    DataValidator::printInfoAboutSettingDefaultLinesToSkipNumber();
                    linesToSkipNumber_ = DEFAULT_LINES_TO_SKIP_NUMBER;


            }
        }

        void setCsvDelimiter(char *argumentValues[]) {
            if (isParserConfigsIndicatorSpecifiedDelimiterAndLinesToSkipNumber() ||
                isParserConfigsIndicatorSpecifiedDelimiterLinesToSkipAndEscapeSymbol()) {
                size_t possibleDelimiterArgsValuesIndex =
                        static_cast<int>(CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_AND_LINES_TO_SKIP) - 1;
                std::string possibleDelimiterStringContainer(argumentValues[possibleDelimiterArgsValuesIndex]);
                if (DataValidator::isStringValueContainOnlyOneLetter(possibleDelimiterStringContainer)) {
                    csvDelimiter_ = possibleDelimiterStringContainer.front();
                } else {
                    DataValidator::printInfoAboutSettingDefaultCsvDelimiter();
                    csvDelimiter_ = DEFAULT_CSV_DELIMITER;
                }
            } else {
                DataValidator::printInfoAboutSettingDefaultCsvDelimiter();
                csvDelimiter_ = DEFAULT_CSV_DELIMITER;
            }
        }

        void setCsvEscapeSymbol(char *argumentValues[]) {
            if (isParserConfigsIndicatorSpecifiedDelimiterLinesToSkipAndEscapeSymbol()) {
                size_t possibleEscapeSymbolArgsValuesIndex =
                        static_cast<int>(CSVParserConfigsArgumentsAmount::SPECIFIED_DELIMITER_LINES_TO_SKIP_AND_ESCAPE_SYMBOL) -
                        1;
                std::string possibleEscapeSymbolStringContainer(argumentValues[possibleEscapeSymbolArgsValuesIndex]);
                if (DataValidator::isStringValueContainOnlyOneLetter(possibleEscapeSymbolStringContainer)) {
                    csvEscapeSymbol_ = possibleEscapeSymbolStringContainer.front();
                } else {
                    DataValidator::printInfoAboutSettingDefaultEscapeSymbol();
                    csvEscapeSymbol_ = DEFAULT_CSV_ESCAPE_SYMBOL;
                }
            } else {
                DataValidator::printInfoAboutSettingDefaultEscapeSymbol();
                csvEscapeSymbol_ = DEFAULT_CSV_ESCAPE_SYMBOL;
            }
        }

        [[nodiscard]] size_t getLinesToSkipNumber() const {
            return linesToSkipNumber_;
        }

        [[nodiscard]] char getCsvDelimiter() const {
            return csvDelimiter_;
        }

        [[nodiscard]] char getCsvEscapeSymbol() const {
            return csvEscapeSymbol_;
        }

        [[nodiscard]] const std::string &getSourceFileName() const {
            return sourceFileName_;
        }
    };

    void initHandlerParams(CSVParserArgsHandler &handler, const int argumentsAmount, char *argumentValues[]) {
        handler.setArgumentsAmount(argumentsAmount);
        handler.setSourceFileName(argumentValues);
        handler.setLinesToSkipNumber(argumentValues);
        handler.setCsvDelimiter(argumentValues);
        handler.setCsvEscapeSymbol(argumentValues);
    }
}