#include "Calendar.hpp"
#include "InputUtils.hpp"

#include <iostream>
#include <stdexcept>

namespace {

void displayHeader() {
    chronocli::printLine('=', 45);
    std::cout << "              ChronoCLI\n";
    std::cout << "     C++ Terminal Calendar Manager\n";
    chronocli::printLine('=', 45);
}

void displayMainMenu() {
    std::cout << "\nMain Menu\n";
    chronocli::printLine('-', 45);

    std::cout << "1. Display monthly calendar\n";
    std::cout << "2. Display yearly calendar\n";
    std::cout << "3. Check leap year\n";
    std::cout << "4. Find first weekday of a month\n";
    std::cout << "0. Exit\n";
}

void handleMonthlyCalendar(const chronocli::Calendar& calendar) {
    int month = chronocli::readIntegerInRange("Enter month (1-12): ", 1, 12);
    int year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    calendar.printMonth(month, year);
}

void handleYearlyCalendar(const chronocli::Calendar& calendar) {
    int year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    calendar.printYear(year);
}

void handleLeapYearCheck(const chronocli::Calendar& calendar) {
    int year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    if (calendar.isLeapYear(year)) {
        std::cout << year << " is a leap year.\n";
    } else {
        std::cout << year << " is not a leap year.\n";
    }
}

void handleFirstWeekday(const chronocli::Calendar& calendar) {
    int month = chronocli::readIntegerInRange("Enter month (1-12): ", 1, 12);
    int year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    int firstDay = calendar.getFirstDayOfMonth(month, year);

    std::cout << "The first day of "
              << calendar.getMonthName(month)
              << " "
              << year
              << " is "
              << calendar.getWeekdayName(firstDay)
              << ".\n";
}

} // anonymous namespace

int main() {
    chronocli::Calendar calendar;
    bool isRunning = true;

    while (isRunning) {
        displayHeader();
        displayMainMenu();

        int choice = chronocli::readIntegerInRange("\nEnter your choice: ", 0, 4);

        try {
            switch (choice) {
                case 1:
                    handleMonthlyCalendar(calendar);
                    chronocli::waitForEnter();
                    break;

                case 2:
                    handleYearlyCalendar(calendar);
                    chronocli::waitForEnter();
                    break;

                case 3:
                    handleLeapYearCheck(calendar);
                    chronocli::waitForEnter();
                    break;

                case 4:
                    handleFirstWeekday(calendar);
                    chronocli::waitForEnter();
                    break;

                case 0:
                    std::cout << "Thank you for using ChronoCLI.\n";
                    isRunning = false;
                    break;

                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    chronocli::waitForEnter();
                    break;
            }
        }
        catch (const std::invalid_argument& error) {
            std::cout << "Error: " << error.what() << '\n';
            chronocli::waitForEnter();
        }

        std::cout << '\n';
    }

    return 0;
}