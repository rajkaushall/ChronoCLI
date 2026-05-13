#include "Calendar.hpp"

#include <array>
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace chronocli {

bool Calendar::isValidYear(int year) const {
    return year > 0;
}

bool Calendar::isValidMonth(int month) const {
    return month >= 1 && month <= 12;
}

bool Calendar::isLeapYear(int year) const {
    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Calendar::getDaysInMonth(int month, int year) const {
    if (!isValidMonth(month)) {
        throw std::invalid_argument("Month must be between 1 and 12.");
    }

    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }

    return 31;
}

int Calendar::getFirstDayOfMonth(int month, int year) const {
    if (!isValidMonth(month)) {
        throw std::invalid_argument("Month must be between 1 and 12.");
    }

    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    /*
        Zeller's Congruence

        This formula calculates the weekday of a given date.

        For this project:
        0 = Saturday
        1 = Sunday
        2 = Monday
        3 = Tuesday
        4 = Wednesday
        5 = Thursday
        6 = Friday
    */

    int adjustedMonth = month;
    int adjustedYear = year;

    if (adjustedMonth == 1 || adjustedMonth == 2) {
        adjustedMonth += 12;
        adjustedYear--;
    }

    int day = 1;
    int yearOfCentury = adjustedYear % 100;
    int zeroBasedCentury = adjustedYear / 100;

    int weekday = (
        day
        + (13 * (adjustedMonth + 1)) / 5
        + yearOfCentury
        + yearOfCentury / 4
        + zeroBasedCentury / 4
        + 5 * zeroBasedCentury
    ) % 7;

    return weekday;
}

std::string Calendar::getMonthName(int month) const {
    if (!isValidMonth(month)) {
        throw std::invalid_argument("Month must be between 1 and 12.");
    }

    static const std::array<std::string, 12> monthNames = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    return monthNames[month - 1];
}

std::string Calendar::getWeekdayName(int weekday) const {
    static const std::array<std::string, 7> weekdayNames = {
        "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"
    };

    if (weekday < 0 || weekday > 6) {
        throw std::invalid_argument("Weekday must be between 0 and 6.");
    }

    return weekdayNames[weekday];
}

void Calendar::printMonth(int month, int year) const {
    int totalDays = getDaysInMonth(month, year);
    int firstDay = getFirstDayOfMonth(month, year);

    std::cout << "\n========== " << getMonthName(month) << " " << year << " ==========\n\n";

    std::cout << std::setw(4) << "Sat"
              << std::setw(4) << "Sun"
              << std::setw(4) << "Mon"
              << std::setw(4) << "Tue"
              << std::setw(4) << "Wed"
              << std::setw(4) << "Thu"
              << std::setw(4) << "Fri"
              << '\n';

    for (int i = 0; i < firstDay; i++) {
        std::cout << std::setw(4) << "";
    }

    for (int day = 1; day <= totalDays; day++) {
        std::cout << std::setw(4) << day;

        if ((day + firstDay) % 7 == 0) {
            std::cout << '\n';
        }
    }

    std::cout << "\n\n";
}

void Calendar::printYear(int year) const {
    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    std::cout << "\n==============================\n";
    std::cout << "Calendar Year: " << year << '\n';
    std::cout << "==============================\n";

    for (int month = 1; month <= 12; month++) {
        printMonth(month, year);
    }
}

} // namespace chronocli