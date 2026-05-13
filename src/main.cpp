#include "Calendar.hpp"
#include "DateUtils.hpp"
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
    std::cout << "5. Find weekday of any date\n";
    std::cout << "6. Compare two dates\n";
    std::cout << "7. Calculate difference between two dates\n";
    std::cout << "0. Exit\n";
}

chronocli::Date readDateFromUser(const std::string& label) {
    std::cout << "\n" << label << "\n";

    chronocli::Date date;

    date.day = chronocli::readIntegerInRange("Enter day (1-31): ", 1, 31);
    date.month = chronocli::readIntegerInRange("Enter month (1-12): ", 1, 12);
    date.year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    return date;
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

void handleDateValidationAndWeekday(const chronocli::DateUtils& dateUtils) {
    chronocli::Date date = readDateFromUser("Enter date details:");

    if (!dateUtils.isValidDate(date)) {
        std::cout << "Invalid date.\n";
        return;
    }

    std::cout << "The date is valid.\n";
    std::cout << "Weekday: " << dateUtils.getDayOfWeekName(date) << '\n';
}

void handleDateComparison(const chronocli::DateUtils& dateUtils) {
    chronocli::Date firstDate = readDateFromUser("Enter first date:");
    chronocli::Date secondDate = readDateFromUser("Enter second date:");

    int result = dateUtils.compareDates(firstDate, secondDate);

    if (result < 0) {
        std::cout << "First date is earlier than second date.\n";
    } else if (result > 0) {
        std::cout << "First date is later than second date.\n";
    } else {
        std::cout << "Both dates are the same.\n";
    }
}

void handleDateDifference(const chronocli::DateUtils& dateUtils) {
    chronocli::Date firstDate = readDateFromUser("Enter first date:");
    chronocli::Date secondDate = readDateFromUser("Enter second date:");

    long long difference = dateUtils.getDifferenceInDays(firstDate, secondDate);

    std::cout << "Difference: " << difference << " day(s).\n";
}

} // anonymous namespace

int main() {
    chronocli::Calendar calendar;
    chronocli::DateUtils dateUtils;

    bool isRunning = true;

    while (isRunning) {
        displayHeader();
        displayMainMenu();

        int choice = chronocli::readIntegerInRange("\nEnter your choice: ", 0, 7);

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

                case 5:
                    handleDateValidationAndWeekday(dateUtils);
                    chronocli::waitForEnter();
                    break;

                case 6:
                    handleDateComparison(dateUtils);
                    chronocli::waitForEnter();
                    break;

                case 7:
                    handleDateDifference(dateUtils);
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