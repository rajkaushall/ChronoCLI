#include "Calendar.hpp"
#include "DateUtils.hpp"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace chronocli {

namespace {

const std::string cyan = "\033[36m";
const std::string green = "\033[32m";
const std::string yellow = "\033[33m";
const std::string dim = "\033[2m";
const std::string reset = "\033[0m";

std::string padRight(const std::string& text, int width) {
    if (static_cast<int>(text.size()) >= width) {
        return text;
    }

    return text + std::string(static_cast<std::size_t>(width - static_cast<int>(text.size())), ' ');
}

std::string centerText(const std::string& text, int width) {
    if (static_cast<int>(text.size()) >= width) {
        return text;
    }

    int totalPadding = width - static_cast<int>(text.size());
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    return std::string(static_cast<std::size_t>(leftPadding), ' ')
        + text
        + std::string(static_cast<std::size_t>(rightPadding), ' ');
}

std::string padMonthRow(const std::string& row, int width) {
    return padRight(row, width);
}

std::vector<std::string> buildMonthBlock(const Calendar& calendar, int month, int year) {
    const int monthWidth = 24;
    const int dayCellWidth = 3;
    const int weekCount = 6;

    std::vector<std::string> lines;

    lines.push_back(centerText(calendar.getMonthName(month) + " " + std::to_string(year), monthWidth));
    lines.push_back(centerText("Su Mo Tu We Th Fr Sa", monthWidth));

    int firstDay = calendar.getFirstDayOfMonth(month, year);
    int totalDays = calendar.getDaysInMonth(month, year);
    int currentDay = 1;

    for (int week = 0; week < weekCount; week++) {
        std::ostringstream row;

        for (int weekday = 0; weekday < 7; weekday++) {
            int cellIndex = week * 7 + weekday;

            if (cellIndex < firstDay || currentDay > totalDays) {
                row << std::setw(dayCellWidth) << "";
            } else {
                row << std::setw(dayCellWidth) << currentDay;
                currentDay++;
            }
        }

        lines.push_back(padMonthRow(row.str(), monthWidth));
    }

    return lines;
}

void printBorder(int contentWidth) {
    std::cout << cyan << "+"
              << std::string(static_cast<std::size_t>(contentWidth + 2), '-')
              << "+"
              << reset
              << '\n';
}

void printBorderedLine(const std::string& text, int contentWidth, const std::string& colour = "") {
    std::cout << cyan << "| " << reset;

    if (!colour.empty()) {
        std::cout << colour;
    }

    std::cout << padRight(text, contentWidth);

    if (!colour.empty()) {
        std::cout << reset;
    }

    std::cout << cyan << " |" << reset << '\n';
}

} // namespace

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

    DateUtils dateUtils;

    return dateUtils.isLeapYear(year);
}

int Calendar::getDaysInMonth(int month, int year) const {
    if (!isValidMonth(month)) {
        throw std::invalid_argument("Month must be between 1 and 12.");
    }

    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    DateUtils dateUtils;

    return dateUtils.getDaysInMonth(month, year);
}

int Calendar::getFirstDayOfMonth(int month, int year) const {
    if (!isValidMonth(month)) {
        throw std::invalid_argument("Month must be between 1 and 12.");
    }

    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    DateUtils dateUtils;

    return dateUtils.getDayOfWeek({1, month, year});
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
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    if (weekday < 0 || weekday > 6) {
        throw std::invalid_argument("Weekday must be between 0 and 6.");
    }

    return weekdayNames[weekday];
}

void Calendar::printMonth(int month, int year, const std::vector<int>& eventDays, const Date* today) const {
    int totalDays = getDaysInMonth(month, year);
    int firstDay = getFirstDayOfMonth(month, year);
    const int contentWidth = 42;

    std::cout << '\n';
    printBorder(contentWidth);
    printBorderedLine(centerText(getMonthName(month) + " " + std::to_string(year), contentWidth), contentWidth, yellow);
    printBorder(contentWidth);

    std::ostringstream weekdayRow;
    weekdayRow << std::setw(6) << "Sun"
               << std::setw(6) << "Mon"
               << std::setw(6) << "Tue"
               << std::setw(6) << "Wed"
               << std::setw(6) << "Thu"
               << std::setw(6) << "Fri"
               << std::setw(6) << "Sat";
    printBorderedLine(weekdayRow.str(), contentWidth, dim);

    std::ostringstream weekRow;

    for (int i = 0; i < firstDay; i++) {
        weekRow << std::setw(6) << "";
    }

    for (int day = 1; day <= totalDays; day++) {
        std::string label = std::to_string(day);
        bool hasEvent = std::find(eventDays.begin(), eventDays.end(), day) != eventDays.end();
        bool isToday = today != nullptr
            && today->day == day
            && today->month == month
            && today->year == year;

        if (isToday) {
            label = "[" + label + "]";
        }

        if (hasEvent) {
            label += "*";
        }

        weekRow << std::setw(6) << label;

        if ((day + firstDay) % 7 == 0) {
            printBorderedLine(weekRow.str(), contentWidth);
            weekRow.str("");
            weekRow.clear();
        }
    }

    if (!weekRow.str().empty()) {
        printBorderedLine(weekRow.str(), contentWidth);
    }

    printBorder(contentWidth);
    std::cout << '\n';
}

void Calendar::printYear(int year) const {
    if (!isValidYear(year)) {
        throw std::invalid_argument("Year must be greater than 0.");
    }

    const int monthsPerRow = 4;
    const int monthWidth = 24;
    const int linesPerMonth = 8;
    const std::string columnGap = "   ";
    const int contentWidth = (monthsPerRow * monthWidth)
        + ((monthsPerRow - 1) * static_cast<int>(columnGap.size()));

    std::cout << '\n';
    printBorder(contentWidth);
    printBorderedLine(centerText("Calendar Year: " + std::to_string(year), contentWidth), contentWidth, green);
    printBorder(contentWidth);

    for (int rowStartMonth = 1; rowStartMonth <= 12; rowStartMonth += monthsPerRow) {
        std::vector<std::vector<std::string>> monthBlocks;

        for (int offset = 0; offset < monthsPerRow; offset++) {
            monthBlocks.push_back(buildMonthBlock(*this, rowStartMonth + offset, year));
        }

        for (int lineIndex = 0; lineIndex < linesPerMonth; lineIndex++) {
            std::ostringstream row;

            for (int blockIndex = 0; blockIndex < monthsPerRow; blockIndex++) {
                if (blockIndex > 0) {
                    row << columnGap;
                }

                row << monthBlocks[static_cast<std::size_t>(blockIndex)][static_cast<std::size_t>(lineIndex)];
            }

            if (lineIndex == 0) {
                printBorderedLine(row.str(), contentWidth, yellow);
            } else if (lineIndex == 1) {
                printBorderedLine(row.str(), contentWidth, dim);
            } else {
                printBorderedLine(row.str(), contentWidth);
            }
        }

        if (rowStartMonth + monthsPerRow <= 12) {
            printBorderedLine("", contentWidth);
        }
    }

    printBorder(contentWidth);
}

} // namespace chronocli
