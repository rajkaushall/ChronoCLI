#include "Calendar.hpp"
#include "DateUtils.hpp"
#include "EventManager.hpp"
#include "InputUtils.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

const std::string EVENT_FILE_PATH = "events.txt";

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
    std::cout << "8. Add event\n";
    std::cout << "9. View events by date\n";
    std::cout << "10. View events by month\n";
    std::cout << "11. View all events\n";
    std::cout << "12. Delete event\n";
    std::cout << "13. Save events now\n";
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

void printEvent(const chronocli::Event& event) {
    chronocli::printLine('-', 45);

    std::cout << "Event ID    : " << event.id << '\n';
    std::cout << "Date        : "
              << event.date.day << "/"
              << event.date.month << "/"
              << event.date.year << '\n';

    std::cout << "Title       : " << event.title << '\n';
    std::cout << "Description : " << event.description << '\n';
}

void printEvents(const std::vector<chronocli::Event>& events) {
    if (events.empty()) {
        std::cout << "No events found.\n";
        return;
    }

    for (const chronocli::Event& event : events) {
        printEvent(event);
    }

    chronocli::printLine('-', 45);
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

void handleAddEvent(chronocli::EventManager& eventManager, const chronocli::DateUtils& dateUtils) {
    chronocli::Date date = readDateFromUser("Enter event date:");

    if (!dateUtils.isValidDate(date)) {
        std::cout << "Invalid date. Event was not added.\n";
        return;
    }

    std::string title = chronocli::readNonEmptyText("Enter event title: ");
    std::string description = chronocli::readText("Enter event description: ");

    int eventId = eventManager.addEvent(date, title, description);

    std::cout << "Event added successfully with ID: " << eventId << '\n';
}

void handleViewEventsByDate(const chronocli::EventManager& eventManager, const chronocli::DateUtils& dateUtils) {
    chronocli::Date date = readDateFromUser("Enter date to search events:");

    if (!dateUtils.isValidDate(date)) {
        std::cout << "Invalid date.\n";
        return;
    }

    std::vector<chronocli::Event> events = eventManager.getEventsByDate(date);

    printEvents(events);
}

void handleViewEventsByMonth(const chronocli::EventManager& eventManager) {
    int month = chronocli::readIntegerInRange("Enter month (1-12): ", 1, 12);
    int year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    std::vector<chronocli::Event> events = eventManager.getEventsByMonth(month, year);

    printEvents(events);
}

void handleViewAllEvents(const chronocli::EventManager& eventManager) {
    std::vector<chronocli::Event> events = eventManager.getAllEvents();

    printEvents(events);
}

void handleDeleteEvent(chronocli::EventManager& eventManager) {
    if (!eventManager.hasEvents()) {
        std::cout << "No events available to delete.\n";
        return;
    }

    int eventId = chronocli::readIntegerInRange("Enter event ID to delete: ", 1, 999999);

    bool deleted = eventManager.deleteEvent(eventId);

    if (deleted) {
        std::cout << "Event deleted successfully.\n";
    } else {
        std::cout << "Event not found.\n";
    }
}

void handleSaveEvents(const chronocli::EventManager& eventManager) {
    bool saved = eventManager.saveEventsToFile(EVENT_FILE_PATH);

    if (saved) {
        std::cout << "Events saved successfully to " << EVENT_FILE_PATH << ".\n";
    } else {
        std::cout << "Error: Could not save events.\n";
    }
}

} // anonymous namespace

int main() {
    chronocli::Calendar calendar;
    chronocli::DateUtils dateUtils;
    chronocli::EventManager eventManager;

    bool loaded = eventManager.loadEventsFromFile(EVENT_FILE_PATH);

    if (loaded) {
        std::cout << "Saved events loaded from " << EVENT_FILE_PATH << ".\n";
    } else {
        std::cout << "No saved event file found. Starting fresh.\n";
    }

    bool isRunning = true;

    while (isRunning) {
        displayHeader();
        displayMainMenu();

        int choice = chronocli::readIntegerInRange("\nEnter your choice: ", 0, 13);

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

                case 8:
                    handleAddEvent(eventManager, dateUtils);
                    chronocli::waitForEnter();
                    break;

                case 9:
                    handleViewEventsByDate(eventManager, dateUtils);
                    chronocli::waitForEnter();
                    break;

                case 10:
                    handleViewEventsByMonth(eventManager);
                    chronocli::waitForEnter();
                    break;

                case 11:
                    handleViewAllEvents(eventManager);
                    chronocli::waitForEnter();
                    break;

                case 12:
                    handleDeleteEvent(eventManager);
                    chronocli::waitForEnter();
                    break;

                case 13:
                    handleSaveEvents(eventManager);
                    chronocli::waitForEnter();
                    break;

                case 0:
                    handleSaveEvents(eventManager);
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