#ifndef CHRONOCLI_CALENDAR_HPP
#define CHRONOCLI_CALENDAR_HPP

#include <string>

namespace chronocli {

class Calendar {
public:
    bool isLeapYear(int year) const;
    bool isValidYear(int year) const;
    bool isValidMonth(int month) const;

    int getDaysInMonth(int month, int year) const;
    int getFirstDayOfMonth(int month, int year) const;

    std::string getMonthName(int month) const;
    std::string getWeekdayName(int weekday) const;

    void printMonth(int month, int year) const;
    void printYear(int year) const;
};

} // namespace chronocli

#endif