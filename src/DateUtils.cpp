#include "DateUtils.hpp"

#include <array>
#include <stdexcept>

namespace chronocli {

bool DateUtils::isLeapYear(int year) const {
    if (year <= 0) {
        return false;
    }

    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int DateUtils::getDaysInMonth(int month, int year) const {
    if (month < 1 || month > 12 || year <= 0) {
        return 0;
    }

    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }

    return 31;
}

bool DateUtils::isValidDate(const Date& date) const {
    if (date.year <= 0) {
        return false;
    }

    if (date.month < 1 || date.month > 12) {
        return false;
    }

    int maxDays = getDaysInMonth(date.month, date.year);

    return date.day >= 1 && date.day <= maxDays;
}

int DateUtils::getDayOfWeek(const Date& date) const {
    if (!isValidDate(date)) {
        throw std::invalid_argument("Invalid date.");
    }

    /*
        Zeller's Congruence

        For this project:
        0 = Saturday
        1 = Sunday
        2 = Monday
        3 = Tuesday
        4 = Wednesday
        5 = Thursday
        6 = Friday
    */

    int day = date.day;
    int month = date.month;
    int year = date.year;

    if (month == 1 || month == 2) {
        month += 12;
        year--;
    }

    int yearOfCentury = year % 100;
    int zeroBasedCentury = year / 100;

    int weekday = (
        day
        + (13 * (month + 1)) / 5
        + yearOfCentury
        + yearOfCentury / 4
        + zeroBasedCentury / 4
        + 5 * zeroBasedCentury
    ) % 7;

    return weekday;
}

std::string DateUtils::getDayOfWeekName(const Date& date) const {
    static const std::array<std::string, 7> weekdayNames = {
        "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"
    };

    int weekday = getDayOfWeek(date);

    return weekdayNames[weekday];
}

int DateUtils::compareDates(const Date& firstDate, const Date& secondDate) const {
    if (!isValidDate(firstDate) || !isValidDate(secondDate)) {
        throw std::invalid_argument("Cannot compare invalid dates.");
    }

    long long firstSerial = getSerialDayNumber(firstDate);
    long long secondSerial = getSerialDayNumber(secondDate);

    if (firstSerial < secondSerial) {
        return -1;
    }

    if (firstSerial > secondSerial) {
        return 1;
    }

    return 0;
}

long long DateUtils::getDifferenceInDays(const Date& firstDate, const Date& secondDate) const {
    if (!isValidDate(firstDate) || !isValidDate(secondDate)) {
        throw std::invalid_argument("Cannot calculate difference for invalid dates.");
    }

    long long firstSerial = getSerialDayNumber(firstDate);
    long long secondSerial = getSerialDayNumber(secondDate);

    long long difference = secondSerial - firstSerial;

    return difference < 0 ? -difference : difference;
}

long long DateUtils::getSerialDayNumber(const Date& date) const {
    if (!isValidDate(date)) {
        throw std::invalid_argument("Invalid date.");
    }

    long long year = date.year - 1;

    long long totalDays = year * 365
        + year / 4
        - year / 100
        + year / 400;

    for (int month = 1; month < date.month; month++) {
        totalDays += getDaysInMonth(month, date.year);
    }

    totalDays += date.day;

    return totalDays;
}

} // namespace chronocli