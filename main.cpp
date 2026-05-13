#include <iostream>
#include <iomanip>
using namespace std;

class Calendar
{
private:
    bool isLeapYear(int year)
    {
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    }



    int getDaysInMonth(int month, int year)
    {
        if (month < 1 || month > 12)
            return 0; // Invalid month
        if (month == 4 || month == 6 || month == 9 || month == 11)
            return 30;
        else if (month == 2)
            return isLeapYear(year) ? 29 : 28;
        else
            return 31;
    }



    int getFirstDay(int month, int year)
    {
        int m = month;
        int y = year;

        if (m == 1 || m == 2)
        {
            m += 12; // Jan=13, Feb=14
            y -= 1;
        }

        int q = 1;       // First day of month
        int K = y % 100; // Last 2 digits
        int J = y / 100; // First 2 digits

        int d = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
        return d; // 0=Sat, 1=Sun, 2=Mon ...
    }

    

public:
    void printMonth(int month, int year)
    {
        cout << "======= CALENDAR ==========" << endl;
        cout << setw(4) << "SAT" << setw(4) << "SUN" << setw(4) << "MON"
             << setw(4) << "TUE" << setw(4) << "WED" << setw(4) << "THU"
             << setw(4) << "FRI" << setw(4) << endl;

        int total_days_in_month = getDaysInMonth(month, year);
        int first_day_of_month = getFirstDay(month, year);

        for (int i = 0; i < first_day_of_month; i++)
            cout << "    ";

        for (int day = 1; day <= total_days_in_month; day++)
        {
            cout << setw(4) << day;

            if ((day + first_day_of_month) % 7 == 0)
                cout << endl; 
        }
        cout << endl;
    }
};

int main()
{
    int month, year;
    cout << "Enter month: ";
    cin >> month;
    cout << "Enter year: ";
    cin >> year;

    Calendar cal;
    cal.printMonth(month, year);

    return 0;
}
// This program prints the calendar for a given month and year.