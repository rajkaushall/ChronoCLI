#include "Calendar.hpp"
#include "DateUtils.hpp"
#include "EventManager.hpp"

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

int passedTests = 0;
int failedTests = 0;

void check(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << '\n';
        passedTests++;
    } else {
        std::cout << "[FAIL] " << testName << '\n';
        failedTests++;
    }
}

void testCalendarLogic() {
    chronocli::Calendar calendar;

    check(calendar.isLeapYear(2024), "2024 should be leap year");
    check(!calendar.isLeapYear(2025), "2025 should not be leap year");
    check(calendar.getDaysInMonth(2, 2024) == 29, "February 2024 should have 29 days");
    check(calendar.getDaysInMonth(5, 2026) == 31, "May should have 31 days");
    check(calendar.getMonthName(5) == "May", "Month 5 should be May");
}

void testDateUtilsLogic() {
    chronocli::DateUtils dateUtils;

    chronocli::Date validDate{14, 5, 2026};
    chronocli::Date invalidDate{31, 2, 2026};

    check(dateUtils.isValidDate(validDate), "14/5/2026 should be valid");
    check(!dateUtils.isValidDate(invalidDate), "31/2/2026 should be invalid");
    check(dateUtils.getDayOfWeekName(validDate) == "Thursday", "14/5/2026 should be Thursday");

    chronocli::Date firstDate{1, 1, 2024};
    chronocli::Date secondDate{10, 1, 2024};

    check(dateUtils.compareDates(firstDate, secondDate) == -1, "First date should be earlier");
    check(dateUtils.getDifferenceInDays(firstDate, secondDate) == 9, "Difference should be 9 days");
}

void testEventManagerLogic() {
    chronocli::EventManager eventManager;

    chronocli::Date eventDate{14, 5, 2026};

    int eventId = eventManager.addEvent(
        eventDate,
        "Placement preparation",
        "Revise DSA and C++ project"
    );

    check(eventId == 1, "First event ID should be 1");
    check(eventManager.hasEvents(), "Event manager should have events");

    std::vector<chronocli::Event> events = eventManager.getEventsByDate(eventDate);

    check(events.size() == 1, "Should find one event by date");
    check(events[0].title == "Placement preparation", "Event title should match");

    check(eventManager.deleteEvent(eventId), "Delete event should work");
    check(!eventManager.hasEvents(), "Event manager should be empty after delete");
}

void testEventFileStorage() {
    const std::string filePath = "test_events.txt";

    chronocli::EventManager eventManager;
    chronocli::Date eventDate{20, 5, 2026};

    eventManager.addEvent(
        eventDate,
        "System design revision",
        "Read notes and solve examples"
    );

    check(eventManager.saveEventsToFile(filePath), "Events should save to file");

    chronocli::EventManager loadedEventManager;

    check(loadedEventManager.loadEventsFromFile(filePath), "Events should load from file");

    std::vector<chronocli::Event> loadedEvents = loadedEventManager.getAllEvents();

    check(loadedEvents.size() == 1, "Loaded event count should be 1");
    check(loadedEvents[0].title == "System design revision", "Loaded event title should match");

    std::remove(filePath.c_str());
}

int main() {
    std::cout << "Running ChronoCLI tests...\n\n";

    testCalendarLogic();
    testDateUtilsLogic();
    testEventManagerLogic();
    testEventFileStorage();

    std::cout << "\nTest Summary\n";
    std::cout << "------------\n";
    std::cout << "Passed: " << passedTests << '\n';
    std::cout << "Failed: " << failedTests << '\n';

    return failedTests == 0 ? 0 : 1;
}