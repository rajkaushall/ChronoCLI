#include "Calendar.hpp"
#include "DateUtils.hpp"
#include "EventManager.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
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

template <typename Function>
void checkThrowsInvalidArgument(Function function, const std::string& testName) {
    try {
        function();
        check(false, testName);
    }
    catch (const std::invalid_argument&) {
        check(true, testName);
    }
    catch (...) {
        check(false, testName);
    }
}

void testCalendarLogic() {
    chronocli::Calendar calendar;

    check(calendar.isLeapYear(2024), "2024 should be leap year");
    check(calendar.isLeapYear(2000), "2000 should be leap year");
    check(!calendar.isLeapYear(2025), "2025 should not be leap year");
    check(!calendar.isLeapYear(1900), "1900 should not be leap year");
    check(calendar.getDaysInMonth(2, 2024) == 29, "February 2024 should have 29 days");
    check(calendar.getDaysInMonth(2, 2025) == 28, "February 2025 should have 28 days");
    check(calendar.getDaysInMonth(5, 2026) == 31, "May should have 31 days");
    check(calendar.getMonthName(5) == "May", "Month 5 should be May");
    check(calendar.getWeekdayName(0) == "Sunday", "Weekday 0 should be Sunday");
    check(calendar.getWeekdayName(6) == "Saturday", "Weekday 6 should be Saturday");
    check(calendar.getFirstDayOfMonth(3, 2025) == 6, "March 2025 should start on Saturday");
    check(calendar.getFirstDayOfMonth(6, 2026) == 1, "June 2026 should start on Monday");

    checkThrowsInvalidArgument(
        [&calendar]() { calendar.getDaysInMonth(13, 2026); },
        "Invalid month should throw"
    );
}

void testDateUtilsLogic() {
    chronocli::DateUtils dateUtils;

    chronocli::Date validDate{14, 5, 2026};
    chronocli::Date invalidDate{31, 2, 2026};
    chronocli::Date invalidLeapDate{29, 2, 2025};

    check(dateUtils.isValidDate(validDate), "14/5/2026 should be valid");
    check(!dateUtils.isValidDate(invalidDate), "31/2/2026 should be invalid");
    check(!dateUtils.isValidDate(invalidLeapDate), "29/2/2025 should be invalid");
    check(dateUtils.getDayOfWeekName(validDate) == "Thursday", "14/5/2026 should be Thursday");
    check(dateUtils.getDayOfWeekName({1, 1, 2000}) == "Saturday", "1/1/2000 should be Saturday");
    check(dateUtils.getDayOfWeekName({29, 2, 2024}) == "Thursday", "29/2/2024 should be Thursday");

    chronocli::Date firstDate{1, 1, 2024};
    chronocli::Date secondDate{10, 1, 2024};

    check(dateUtils.compareDates(firstDate, secondDate) == -1, "First date should be earlier");
    check(dateUtils.compareDates(secondDate, firstDate) == 1, "Second date should be later");
    check(dateUtils.compareDates(firstDate, firstDate) == 0, "Same dates should compare equally");
    check(dateUtils.getDifferenceInDays(firstDate, secondDate) == 9, "Difference should be 9 days");
    check(dateUtils.getDifferenceInDays({31, 12, 2023}, {1, 1, 2024}) == 1, "Cross-year difference should be 1 day");

    checkThrowsInvalidArgument(
        [&dateUtils]() { dateUtils.compareDates({31, 2, 2026}, {1, 3, 2026}); },
        "Invalid date comparison should throw"
    );
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
    check(eventManager.getEventsByMonth(5, 2026).size() == 1, "Should find one event by month");
    check(eventManager.getEventsByMonth(6, 2026).empty(), "Should not find events in another month");
    check(!eventManager.deleteEvent(999), "Deleting missing event should fail");

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
    check(loadedEventManager.addEvent({21, 5, 2026}, "Next event", "Check restored ID") == 2, "Next ID should be restored after load");

    std::remove(filePath.c_str());
}

void testEscapedEventFileStorage() {
    const std::string filePath = "test_escaped_events.txt";

    chronocli::EventManager eventManager;

    eventManager.addEvent(
        {20, 5, 2026},
        "Pipe | Slash \\ Title",
        "Line 1\nLine 2\rPipe | ok"
    );

    check(eventManager.saveEventsToFile(filePath), "Escaped event should save to file");

    chronocli::EventManager loadedEventManager;

    check(loadedEventManager.loadEventsFromFile(filePath), "Escaped event should load from file");

    std::vector<chronocli::Event> loadedEvents = loadedEventManager.getAllEvents();

    check(loadedEvents.size() == 1, "Escaped loaded event count should be 1");

    if (!loadedEvents.empty()) {
        check(loadedEvents[0].title == "Pipe | Slash \\ Title", "Escaped title should round-trip");
        check(loadedEvents[0].description == "Line 1\nLine 2\rPipe | ok", "Escaped description should round-trip");
    }

    std::remove(filePath.c_str());
}

void testInvalidEventFileRows() {
    const std::string filePath = "test_invalid_events.txt";

    std::ofstream outputFile(filePath);
    outputFile << "not a valid event line\n";
    outputFile << "0|1|1|2026|Bad ID|Should be skipped\n";
    outputFile << "2|31|2|2026|Bad date|Should be skipped\n";
    outputFile << "5|20|5|2026|Valid event|Should load\n";
    outputFile.close();

    chronocli::EventManager eventManager;

    check(eventManager.loadEventsFromFile(filePath), "File with invalid rows should still open");

    std::vector<chronocli::Event> loadedEvents = eventManager.getAllEvents();

    check(loadedEvents.size() == 1, "Only valid event rows should load");

    if (!loadedEvents.empty()) {
        check(loadedEvents[0].id == 5, "Loaded valid event should keep its ID");
        check(loadedEvents[0].title == "Valid event", "Loaded valid event title should match");
    }

    check(eventManager.addEvent({21, 5, 2026}, "Next event", "After invalid rows") == 6, "Next ID should use max loaded valid ID");

    std::remove(filePath.c_str());
}

int main() {
    std::cout << "Running ChronoCLI tests...\n\n";

    testCalendarLogic();
    testDateUtilsLogic();
    testEventManagerLogic();
    testEventFileStorage();
    testEscapedEventFileStorage();
    testInvalidEventFileRows();

    std::cout << "\nTest Summary\n";
    std::cout << "------------\n";
    std::cout << "Passed: " << passedTests << '\n';
    std::cout << "Failed: " << failedTests << '\n';

    return failedTests == 0 ? 0 : 1;
}
