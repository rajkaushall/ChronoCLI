#include "Calendar.hpp"
#include "DateUtils.hpp"
#include "EventManager.hpp"
#include "InputUtils.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
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

    std::ostringstream yearlyOutput;
    std::streambuf* originalOutput = std::cout.rdbuf(yearlyOutput.rdbuf());
    calendar.printYear(2025);
    std::cout.rdbuf(originalOutput);

    std::string printedYear = yearlyOutput.str();
    std::size_t januaryPosition = printedYear.find("January 2025");
    std::size_t februaryPosition = printedYear.find("February 2025");
    std::size_t marchPosition = printedYear.find("March 2025");
    std::size_t aprilPosition = printedYear.find("April 2025");

    check(
        januaryPosition != std::string::npos
            && februaryPosition != std::string::npos
            && marchPosition != std::string::npos
            && aprilPosition != std::string::npos
            && januaryPosition < februaryPosition
            && februaryPosition < marchPosition
            && marchPosition < aprilPosition,
        "Year view should print January through April in order"
    );

    check(
        aprilPosition != std::string::npos && aprilPosition > marchPosition,
        "Year view should include April in the first matrix row"
    );

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

void testEventValidationAndSorting() {
    chronocli::EventManager eventManager;

    checkThrowsInvalidArgument(
        [&eventManager]() { eventManager.addEvent({31, 2, 2026}, "Invalid", "Bad date"); },
        "Event manager should reject invalid dates"
    );

    checkThrowsInvalidArgument(
        [&eventManager]() { eventManager.addEvent({1, 3, 2026}, "   ", "Blank title"); },
        "Event manager should reject whitespace-only titles"
    );

    int laterId = eventManager.addEvent({25, 12, 2026}, "Later", "Second chronologically");
    int earlierId = eventManager.addEvent({10, 1, 2026}, "Earlier", "First chronologically");

    std::vector<chronocli::Event> events = eventManager.getAllEvents();

    check(events.size() == 2, "Sorted event test should have two events");
    check(events[0].id == earlierId, "Events should sort by date before ID");
    check(events[1].id == laterId, "Later event should appear after earlier event");
}

void testEventUpdateSearchAndUpcoming() {
    chronocli::EventManager eventManager;

    int firstId = eventManager.addEvent({10, 1, 2026}, "Interview prep", "Revise C++");
    eventManager.addEvent({15, 1, 2026}, "College deadline", "Submit report");
    eventManager.addEvent({1, 1, 2025}, "Past event", "Already done");

    check(eventManager.updateEvent(firstId, {11, 1, 2026}, "Placement interview", "Revise DSA"), "Existing event should update");
    check(!eventManager.updateEvent(999, {11, 1, 2026}, "Missing", "No event"), "Missing event update should fail");

    std::vector<chronocli::Event> searchResults = eventManager.searchEvents("interview");
    check(searchResults.size() == 1, "Search should match updated title");
    check(searchResults[0].id == firstId, "Search should return the updated event");

    std::vector<chronocli::Event> upcomingEvents = eventManager.getUpcomingEvents({10, 1, 2026});
    check(upcomingEvents.size() == 2, "Upcoming events should exclude past events");
    check(upcomingEvents[0].id == firstId, "Upcoming events should be sorted chronologically");
}

void testEventFileStorage() {
    const std::string filePath = "test_data/test_events.txt";

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
    std::filesystem::remove_all("test_data");
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
    outputFile << "5abc|20|5|2026|Bad ID text|Should be skipped\n";
    outputFile << "6|20x|5|2026|Bad day text|Should be skipped\n";
    outputFile << "7|20|5|2026|   |Blank title should be skipped\n";
    outputFile << "5|20|5|2026|Valid event|Should load\n";
    outputFile << "5|21|5|2026|Duplicate ID|Should be skipped\n";
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

void testInputHelpers() {
    check(chronocli::trim("  ChronoCLI  ") == "ChronoCLI", "Trim should remove surrounding spaces");
    check(chronocli::trim(" \t\r\n ") == "", "Trim should treat whitespace-only input as empty");

    std::istringstream emptyInput;
    std::streambuf* originalInput = std::cin.rdbuf(emptyInput.rdbuf());

    try {
        chronocli::readInteger("Number: ");
        check(false, "readInteger should throw on EOF");
    }
    catch (const std::runtime_error&) {
        check(true, "readInteger should throw on EOF");
    }
    catch (...) {
        check(false, "readInteger should throw runtime_error on EOF");
    }

    std::cin.rdbuf(originalInput);
}

int main() {
    std::cout << "Running ChronoCLI tests...\n\n";

    testCalendarLogic();
    testDateUtilsLogic();
    testEventManagerLogic();
    testEventValidationAndSorting();
    testEventUpdateSearchAndUpcoming();
    testEventFileStorage();
    testEscapedEventFileStorage();
    testInvalidEventFileRows();
    testInputHelpers();

    std::cout << "\nTest Summary\n";
    std::cout << "------------\n";
    std::cout << "Passed: " << passedTests << '\n';
    std::cout << "Failed: " << failedTests << '\n';

    return failedTests == 0 ? 0 : 1;
}
