#include "Calendar.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

int main() {
    int month;
    int year;

    std::cout << "Welcome to ChronoCLI\n";
    std::cout << "Enter month (1-12): ";
    std::cin >> month;

    std::cout << "Enter year: ";
    std::cin >> year;

    if (std::cin.fail()) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        return 1;
    }

    if (year <= 0) {
        std::cout << "Invalid year. Year must be greater than 0.\n";
        return 1;
    }

    try {
        chronocli::Calendar calendar;
        calendar.printMonth(month, year);
    } catch (const std::invalid_argument& error) {
        std::cout << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}