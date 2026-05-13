#include "Calendar.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    chronocli::Calendar calendar;

    int choice;

    std::cout << "==============================\n";
    std::cout << "        Welcome to ChronoCLI   \n";
    std::cout << "==============================\n\n";

    std::cout << "1. Display monthly calendar\n";
    std::cout << "2. Display yearly calendar\n";
    std::cout << "3. Check leap year\n";
    std::cout << "4. Find first weekday of a month\n";
    std::cout << "0. Exit\n\n";

    std::cout << "Enter your choice: ";
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cout << "Invalid input. Please enter numbers only.\n";
        clearInputBuffer();
        return 1;
    }

    try {
        if (choice == 1) {
            int month;
            int year;

            std::cout << "Enter month (1-12): ";
            std::cin >> month;

            std::cout << "Enter year: ";
            std::cin >> year;

            if (std::cin.fail()) {
                std::cout << "Invalid input. Please enter numbers only.\n";
                clearInputBuffer();
                return 1;
            }

            calendar.printMonth(month, year);
        }
        else if (choice == 2) {
            int year;

            std::cout << "Enter year: ";
            std::cin >> year;

            if (std::cin.fail()) {
                std::cout << "Invalid input. Please enter numbers only.\n";
                clearInputBuffer();
                return 1;
            }

            calendar.printYear(year);
        }
        else if (choice == 3) {
            int year;

            std::cout << "Enter year: ";
            std::cin >> year;

            if (std::cin.fail()) {
                std::cout << "Invalid input. Please enter numbers only.\n";
                clearInputBuffer();
                return 1;
            }

            if (calendar.isLeapYear(year)) {
                std::cout << year << " is a leap year.\n";
            } else {
                std::cout << year << " is not a leap year.\n";
            }
        }
        else if (choice == 4) {
            int month;
            int year;

            std::cout << "Enter month (1-12): ";
            std::cin >> month;

            std::cout << "Enter year: ";
            std::cin >> year;

            if (std::cin.fail()) {
                std::cout << "Invalid input. Please enter numbers only.\n";
                clearInputBuffer();
                return 1;
            }

            int firstDay = calendar.getFirstDayOfMonth(month, year);

            std::cout << "The first day of "
                      << calendar.getMonthName(month)
                      << " "
                      << year
                      << " is "
                      << calendar.getWeekdayName(firstDay)
                      << ".\n";
        }
        else if (choice == 0) {
            std::cout << "Thank you for using ChronoCLI.\n";
        }
        else {
            std::cout << "Invalid choice. Please select a valid option.\n";
        }
    }
    catch (const std::invalid_argument& error) {
        std::cout << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}