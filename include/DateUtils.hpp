#ifndef CHRONOCLI_DATE_UTILS_HPP
#define CHRONOCLI_DATE_UTILS_HPP

#include <string>

namespace chronocli {

struct Date {
    int day;
    int month;
    int year;
};

class DateUtils {
public:
    bool isValidDate(const Date& date) const;

    int getDayOfWeek(const Date& date) const;
    std::string getDayOfWeekName(const Date& date) const;

    int compareDates(const Date& firstDate, const Date& secondDate) const;
    long long getDifferenceInDays(const Date& firstDate, const Date& secondDate) const;

private:
    long long getSerialDayNumber(const Date& date) const;
    bool isLeapYear(int year) const;
    int getDaysInMonth(int month, int year) const;
};

} // namespace chronocli

#endif