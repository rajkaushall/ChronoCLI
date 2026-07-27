#include "Calendar.hpp"
#include "DateUtils.hpp"
#include "EventManager.hpp"
#include "InputUtils.hpp"
#include "TerminalUI.hpp"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

chronocli::Date getTodayDate() {
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    return {localTime.tm_mday, localTime.tm_mon + 1, localTime.tm_year + 1900};
}

std::filesystem::path getEventFilePath(const char* executablePath) {
    std::filesystem::path basePath = std::filesystem::current_path();

    if (executablePath != nullptr) {
        std::filesystem::path resolvedPath = std::filesystem::absolute(executablePath);

        if (resolvedPath.has_parent_path()) {
            basePath = resolvedPath.parent_path();
        }
    }

    return basePath / "data" / "events.txt";
}

std::vector<int> getEventDaysForMonth(
    const chronocli::EventManager& eventManager,
    int month,
    int year
) {
    std::vector<int> eventDays;
    std::vector<chronocli::Event> events = eventManager.getEventsByMonth(month, year);

    for (const chronocli::Event& event : events) {
        bool alreadyAdded = false;

        for (int day : eventDays) {
            if (day == event.date.day) {
                alreadyAdded = true;
                break;
            }
        }

        if (!alreadyAdded) {
            eventDays.push_back(event.date.day);
        }
    }

    return eventDays;
}

bool saveEvents(const chronocli::EventManager& eventManager, const std::filesystem::path& eventFilePath) {
    return eventManager.saveEventsToFile(eventFilePath.string());
}

void handleSaveEvents(const chronocli::EventManager& eventManager, const std::filesystem::path& eventFilePath) {
    if (saveEvents(eventManager, eventFilePath)) {
        chronocli::printSuccess("Events saved to " + eventFilePath.string() + ".");
    } else {
        chronocli::printError("Could not save events.");
    }
}

chronocli::Date readDateFromUser(const std::string& label) {
    std::cout << "\n" << label << "\n";

    chronocli::Date date;

    date.day = chronocli::readIntegerInRange("Enter day (1-31): ", 1, 31);
    date.month = chronocli::readIntegerInRange("Enter month (1-12): ", 1, 12);
    date.year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    return date;
}

void readMonthAndYear(int& month, int& year) {
    while (true) {
        std::string input = chronocli::readText("Enter month and year (MM YYYY): ");
        std::stringstream stream(input);
        char extraCharacter;

        if (stream >> month >> year && !(stream >> extraCharacter)
            && month >= 1 && month <= 12
            && year >= 1 && year <= 9999) {
            return;
        }

        chronocli::printError("Please enter a valid month and year, for example: 7 2026");
    }
}

void handleMonthlyCalendar(
    const chronocli::Calendar& calendar,
    const chronocli::EventManager& eventManager,
    const chronocli::Date& today
) {
    int month = 0;
    int year = 0;

    readMonthAndYear(month, year);

    std::vector<int> eventDays = getEventDaysForMonth(eventManager, month, year);

    calendar.printMonth(month, year, eventDays, &today);

    if (!eventDays.empty()) {
        chronocli::printInfo("* marks dates with events. Brackets mark today.");
    }
}

void handleYearlyCalendar(const chronocli::Calendar& calendar) {
    int year = chronocli::readIntegerInRange("Enter year (1-9999): ", 1, 9999);

    calendar.printYear(year);
}

void handleToday(
    const chronocli::Calendar& calendar,
    const chronocli::EventManager& eventManager,
    const chronocli::Date& today
) {
    std::vector<int> eventDays = getEventDaysForMonth(eventManager, today.month, today.year);

    calendar.printMonth(today.month, today.year, eventDays, &today);
    chronocli::printEventTable(eventManager.getEventsByDate(today));
}

void handleDateValidationAndWeekday(const chronocli::DateUtils& dateUtils) {
    chronocli::Date date = readDateFromUser("Enter date details:");

    if (!dateUtils.isValidDate(date)) {
        chronocli::printError("Invalid date.");
        return;
    }

    chronocli::printSuccess("The date is valid.");
    std::cout << "Weekday: " << dateUtils.getDayOfWeekName(date) << '\n';
}

void handleAddEvent(chronocli::EventManager& eventManager, const std::filesystem::path& eventFilePath) {
    chronocli::Date date = readDateFromUser("Enter event date:");
    std::string title = chronocli::readNonEmptyText("Enter event title: ");
    std::string description = chronocli::readText("Enter event description: ");

    int eventId = eventManager.addEvent(date, title, description);

    chronocli::printSuccess("Event added with ID: " + std::to_string(eventId));
    handleSaveEvents(eventManager, eventFilePath);
}

void handleUpcomingEvents(const chronocli::EventManager& eventManager, const chronocli::Date& today) {
    chronocli::printEventTable(eventManager.getUpcomingEvents(today));
}

void handleSearchEvents(const chronocli::EventManager& eventManager) {
    std::string keyword = chronocli::readNonEmptyText("Enter search keyword: ");

    chronocli::printEventTable(eventManager.searchEvents(keyword));
}

void handleEditEvent(chronocli::EventManager& eventManager, const std::filesystem::path& eventFilePath) {
    if (!eventManager.hasEvents()) {
        chronocli::printInfo("No events available to edit.");
        return;
    }

    chronocli::printEventTable(eventManager.getAllEvents());

    int eventId = chronocli::readIntegerInRange("Enter event ID to edit: ", 1, 999999);
    chronocli::Date date = readDateFromUser("Enter updated event date:");
    std::string title = chronocli::readNonEmptyText("Enter updated event title: ");
    std::string description = chronocli::readText("Enter updated event description: ");

    if (eventManager.updateEvent(eventId, date, title, description)) {
        chronocli::printSuccess("Event updated successfully.");
        handleSaveEvents(eventManager, eventFilePath);
    } else {
        chronocli::printWarning("Event not found.");
    }
}

void handleDeleteEvent(chronocli::EventManager& eventManager, const std::filesystem::path& eventFilePath) {
    if (!eventManager.hasEvents()) {
        chronocli::printInfo("No events available to delete.");
        return;
    }

    chronocli::printEventTable(eventManager.getAllEvents());

    int eventId = chronocli::readIntegerInRange("Enter event ID to delete: ", 1, 999999);
    std::string confirmation = chronocli::readNonEmptyText("Type YES to confirm deletion: ");

    if (confirmation != "YES") {
        chronocli::printWarning("Delete cancelled.");
        return;
    }

    if (eventManager.deleteEvent(eventId)) {
        chronocli::printSuccess("Event deleted successfully.");
        handleSaveEvents(eventManager, eventFilePath);
    } else {
        chronocli::printWarning("Event not found.");
    }
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    chronocli::Calendar calendar;
    chronocli::DateUtils dateUtils;
    chronocli::EventManager eventManager;
    std::filesystem::path eventFilePath = getEventFilePath(argc > 0 ? argv[0] : nullptr);
    std::filesystem::path legacyEventFilePath = std::filesystem::current_path() / "events.txt";

    try {
        if (eventManager.loadEventsFromFile(eventFilePath.string())) {
            chronocli::printSuccess("Saved events loaded from " + eventFilePath.string() + ".");
        } else if (legacyEventFilePath != eventFilePath
            && std::filesystem::exists(legacyEventFilePath)
            && eventManager.loadEventsFromFile(legacyEventFilePath.string())) {
            chronocli::printWarning("Loaded legacy events.txt and migrated storage to " + eventFilePath.string() + ".");
            saveEvents(eventManager, eventFilePath);
        } else {
            chronocli::printInfo("No saved event file found. Starting fresh.");
        }

        bool isRunning = true;

        while (isRunning) {
            chronocli::Date today = getTodayDate();
            std::vector<chronocli::Event> todaysEvents = eventManager.getEventsByDate(today);
            std::vector<chronocli::Event> upcomingEvents = eventManager.getUpcomingEvents(today);

            chronocli::clearScreen();
            chronocli::printHeader(
                today,
                dateUtils.getDayOfWeekName(today),
                static_cast<int>(todaysEvents.size()),
                static_cast<int>(upcomingEvents.size())
            );
            chronocli::printMainMenu();

            int choice = chronocli::readIntegerInRange("\033[32mSelect option > \033[0m", 0, 9);

            try {
                switch (choice) {
                    case 1:
                        handleMonthlyCalendar(calendar, eventManager, today);
                        break;

                    case 2:
                        handleYearlyCalendar(calendar);
                        break;

                    case 3:
                        handleToday(calendar, eventManager, today);
                        break;

                    case 4:
                        handleAddEvent(eventManager, eventFilePath);
                        break;

                    case 5:
                        handleUpcomingEvents(eventManager, today);
                        break;

                    case 6:
                        handleSearchEvents(eventManager);
                        break;

                    case 7:
                        handleEditEvent(eventManager, eventFilePath);
                        break;

                    case 8:
                        handleDeleteEvent(eventManager, eventFilePath);
                        break;

                    case 9:
                        handleDateValidationAndWeekday(dateUtils);
                        break;

                    case 0:
                        saveEvents(eventManager, eventFilePath);
                        chronocli::printSuccess("Thank you for using ChronoCLI.");
                        isRunning = false;
                        break;

                    default:
                        chronocli::printWarning("Invalid choice. Please try again.");
                        break;
                }
            }
            catch (const std::invalid_argument& error) {
                chronocli::printError(error.what());
            }

            if (isRunning) {
                chronocli::waitForEnter();
            }
        }
    }
    catch (const std::runtime_error& error) {
        saveEvents(eventManager, eventFilePath);
        std::cout << "\n" << error.what() << " Exiting ChronoCLI.\n";
    }

    return 0;
}
