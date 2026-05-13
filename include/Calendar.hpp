#ifndef CHRONOCLI_CALENDAR_HPP
#define CHRONOCLI_CALENDAR_HPP

#include <string>

namespace chronocli {

class Calendar {
public:
    bool isLeapYear(int year) const;
    int getDaysInMonth(int month, int year) const;
    int getFirstDayOfMonth(int month, int year) const;
    std::string getMonthName(int month) const;
    void printMonth(int month, int year) const;

private:
    bool isValidMonth(int month) const;
};

} // namespace chronocli

#endif