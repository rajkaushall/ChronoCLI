#include "InputUtils.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace chronocli {

int readInteger(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string input;
        std::getline(std::cin, input);

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
    std::getline(std::cin, input);

    return input;
}

std::string readNonEmptyText(const std::string& prompt) {
    while (true) {
        std::string input = readText(prompt);

        if (!input.empty()) {
            return input;
        }

        std::cout << "Input cannot be empty. Please try again.\n";
    }
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::string ignoredInput;
    std::getline(std::cin, ignoredInput);
}

void printLine(char symbol, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << symbol;
    }

    std::cout << '\n';
}

} // namespace chronocli