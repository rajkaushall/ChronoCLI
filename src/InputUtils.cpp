#include "InputUtils.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

namespace chronocli {

std::string trim(const std::string& text) {
    std::size_t start = 0;

    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) {
        start++;
    }

    std::size_t end = text.size();

    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        end--;
    }

    return text.substr(start, end - start);
}

int readInteger(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string input;

        if (!std::getline(std::cin, input)) {
            throw std::runtime_error("Input stream closed.");
        }

        std::stringstream stream(input);

        int value;
        char extraCharacter;

        if (stream >> value && !(stream >> extraCharacter)) {
            return value;
        }

        std::cout << "Invalid input. Please enter a valid number.\n";
    }
}

int readIntegerInRange(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        int value = readInteger(prompt);

        if (value >= minValue && value <= maxValue) {
            return value;
        }

        std::cout << "Please enter a value between "
                  << minValue
                  << " and "
                  << maxValue
                  << ".\n";
    }
}

std::string readText(const std::string& prompt) {
    std::cout << prompt;

    std::string input;

    if (!std::getline(std::cin, input)) {
        throw std::runtime_error("Input stream closed.");
    }

    return input;
}

std::string readNonEmptyText(const std::string& prompt) {
    while (true) {
        std::string input = readText(prompt);

        if (!trim(input).empty()) {
            return input;
        }

        std::cout << "Input cannot be empty. Please try again.\n";
    }
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::string ignoredInput;

    if (!std::getline(std::cin, ignoredInput)) {
        throw std::runtime_error("Input stream closed.");
    }
}

void printLine(char symbol, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << symbol;
    }

    std::cout << '\n';
}

} // namespace chronocli
