#include "EventManager.hpp"
#include "DateUtils.hpp"
#include "InputUtils.hpp"

#include <algorithm>
#include <charconv>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unordered_set>
#include <vector>

namespace chronocli {

namespace {

bool parseStrictInteger(const std::string& text, int& value) {
    if (text.empty()) {
        return false;
    }

    const char* begin = text.data();
    const char* end = text.data() + text.size();

    auto result = std::from_chars(begin, end, value);

    return result.ec == std::errc() && result.ptr == end;
}

bool comesBefore(const Event& firstEvent, const Event& secondEvent) {
    if (firstEvent.date.year != secondEvent.date.year) {
        return firstEvent.date.year < secondEvent.date.year;
    }

    if (firstEvent.date.month != secondEvent.date.month) {
        return firstEvent.date.month < secondEvent.date.month;
    }

    if (firstEvent.date.day != secondEvent.date.day) {
        return firstEvent.date.day < secondEvent.date.day;
    }

    return firstEvent.id < secondEvent.id;
}

std::string toLower(std::string text) {
    std::transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        }
    );

    return text;
}

bool containsCaseInsensitive(const std::string& text, const std::string& keyword) {
    return toLower(text).find(toLower(keyword)) != std::string::npos;
}

} // namespace

EventManager::EventManager()
    : nextEventId(1) {
}

int EventManager::addEvent(const Date& date, const std::string& title, const std::string& description) {
    DateUtils dateUtils;

    if (!dateUtils.isValidDate(date)) {
        throw std::invalid_argument("Invalid event date.");
    }

    if (trim(title).empty()) {
        throw std::invalid_argument("Event title cannot be empty.");
    }

    Event event;

    event.id = nextEventId;
    event.date = date;
    event.title = trim(title);
    event.description = description;

    events.push_back(event);

    nextEventId++;

    return event.id;
}

bool EventManager::updateEvent(int eventId, const Date& date, const std::string& title, const std::string& description) {
    DateUtils dateUtils;

    if (!dateUtils.isValidDate(date)) {
        throw std::invalid_argument("Invalid event date.");
    }

    if (trim(title).empty()) {
        throw std::invalid_argument("Event title cannot be empty.");
    }

    for (Event& event : events) {
        if (event.id == eventId) {
            event.date = date;
            event.title = trim(title);
            event.description = description;
            return true;
        }
    }

    return false;
}

bool EventManager::deleteEvent(int eventId) {
    for (auto iterator = events.begin(); iterator != events.end(); ++iterator) {
        if (iterator->id == eventId) {
            events.erase(iterator);
            return true;
        }
    }

    return false;
}

std::vector<Event> EventManager::getEventsByDate(const Date& date) const {
    std::vector<Event> matchedEvents;

    for (const Event& event : events) {
        if (isSameDate(event.date, date)) {
            matchedEvents.push_back(event);
        }
    }

    std::sort(matchedEvents.begin(), matchedEvents.end(), comesBefore);

    return matchedEvents;
}

std::vector<Event> EventManager::getEventsByMonth(int month, int year) const {
    std::vector<Event> matchedEvents;

    for (const Event& event : events) {
        if (event.date.month == month && event.date.year == year) {
            matchedEvents.push_back(event);
        }
    }

    std::sort(matchedEvents.begin(), matchedEvents.end(), comesBefore);

    return matchedEvents;
}

std::vector<Event> EventManager::getUpcomingEvents(const Date& fromDate) const {
    DateUtils dateUtils;

    if (!dateUtils.isValidDate(fromDate)) {
        throw std::invalid_argument("Invalid start date.");
    }

    std::vector<Event> upcomingEvents;

    for (const Event& event : events) {
        if (dateUtils.compareDates(event.date, fromDate) >= 0) {
            upcomingEvents.push_back(event);
        }
    }

    std::sort(upcomingEvents.begin(), upcomingEvents.end(), comesBefore);

    return upcomingEvents;
}

std::vector<Event> EventManager::searchEvents(const std::string& keyword) const {
    std::vector<Event> matchedEvents;
    std::string trimmedKeyword = trim(keyword);

    if (trimmedKeyword.empty()) {
        return matchedEvents;
    }

    for (const Event& event : events) {
        if (containsCaseInsensitive(event.title, trimmedKeyword)
            || containsCaseInsensitive(event.description, trimmedKeyword)) {
            matchedEvents.push_back(event);
        }
    }

    std::sort(matchedEvents.begin(), matchedEvents.end(), comesBefore);

    return matchedEvents;
}

std::vector<Event> EventManager::getAllEvents() const {
    std::vector<Event> sortedEvents = events;
    std::sort(sortedEvents.begin(), sortedEvents.end(), comesBefore);

    return sortedEvents;
}

bool EventManager::hasEventOnDate(const Date& date) const {
    for (const Event& event : events) {
        if (isSameDate(event.date, date)) {
            return true;
        }
    }

    return false;
}

bool EventManager::hasEvents() const {
    return !events.empty();
}

bool EventManager::saveEventsToFile(const std::string& filePath) const {
    namespace fs = std::filesystem;

    fs::path targetPath(filePath);
    fs::path parentPath = targetPath.parent_path();

    std::error_code errorCode;

    if (!parentPath.empty()) {
        fs::create_directories(parentPath, errorCode);

        if (errorCode) {
            return false;
        }
    }

    fs::path temporaryPath = targetPath;
    temporaryPath += ".tmp";

    fs::path backupPath = targetPath;
    backupPath += ".backup";

    std::ofstream outputFile(temporaryPath);

    if (!outputFile.is_open()) {
        return false;
    }

    for (const Event& event : events) {
        outputFile << event.id << '|'
                   << event.date.day << '|'
                   << event.date.month << '|'
                   << event.date.year << '|'
                   << escapeText(event.title) << '|'
                   << escapeText(event.description) << '\n';
    }

    outputFile.flush();

    if (!outputFile.good()) {
        outputFile.close();
        fs::remove(temporaryPath, errorCode);
        return false;
    }

    outputFile.close();

    bool hadExistingFile = fs::exists(targetPath, errorCode);

    if (errorCode) {
        fs::remove(temporaryPath, errorCode);
        return false;
    }

    if (hadExistingFile) {
        fs::remove(backupPath, errorCode);
        fs::copy_file(targetPath, backupPath, fs::copy_options::overwrite_existing, errorCode);

        if (errorCode) {
            fs::remove(temporaryPath, errorCode);
            return false;
        }

        fs::remove(targetPath, errorCode);

        if (errorCode) {
            fs::remove(temporaryPath, errorCode);
            return false;
        }
    }

    fs::rename(temporaryPath, targetPath, errorCode);

    if (errorCode) {
        if (hadExistingFile) {
            std::error_code restoreErrorCode;
            fs::copy_file(backupPath, targetPath, fs::copy_options::overwrite_existing, restoreErrorCode);
        }

        return false;
    }

    return true;
}

bool EventManager::loadEventsFromFile(const std::string& filePath) {
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        return false;
    }

    events.clear();
    nextEventId = 1;

    std::string line;
    int maximumEventId = 0;
    std::unordered_set<int> loadedEventIds;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        Event event;

        if (parseEventLine(line, event) && loadedEventIds.insert(event.id).second) {
            events.push_back(event);

            if (event.id > maximumEventId) {
                maximumEventId = event.id;
            }
        }
    }

    nextEventId = maximumEventId + 1;

    return true;
}

bool EventManager::isSameDate(const Date& firstDate, const Date& secondDate) const {
    return firstDate.day == secondDate.day
        && firstDate.month == secondDate.month
        && firstDate.year == secondDate.year;
}

std::string EventManager::escapeText(const std::string& text) const {
    std::string escapedText;

    for (char character : text) {
        if (character == '\\') {
            escapedText += "\\\\";
        } else if (character == '|') {
            escapedText += "\\|";
        } else if (character == '\n') {
            escapedText += "\\n";
        } else if (character == '\r') {
            escapedText += "\\r";
        } else {
            escapedText += character;
        }
    }

    return escapedText;
}

std::string EventManager::unescapeText(const std::string& text) const {
    std::string unescapedText;

    for (std::size_t index = 0; index < text.size(); index++) {
        if (text[index] == '\\' && index + 1 < text.size()) {
            char nextCharacter = text[index + 1];

            if (nextCharacter == '\\') {
                unescapedText += '\\';
            } else if (nextCharacter == '|') {
                unescapedText += '|';
            } else if (nextCharacter == 'n') {
                unescapedText += '\n';
            } else if (nextCharacter == 'r') {
                unescapedText += '\r';
            } else {
                unescapedText += nextCharacter;
            }

            index++;
        } else {
            unescapedText += text[index];
        }
    }

    return unescapedText;
}

std::vector<std::string> EventManager::splitEscapedLine(const std::string& line) const {
    std::vector<std::string> fields;
    std::string currentField;
    bool isEscaped = false;

    for (char character : line) {
        if (isEscaped) {
            currentField += '\\';
            currentField += character;
            isEscaped = false;
        } else if (character == '\\') {
            isEscaped = true;
        } else if (character == '|') {
            fields.push_back(currentField);
            currentField.clear();
        } else {
            currentField += character;
        }
    }

    if (isEscaped) {
        currentField += '\\';
    }

    fields.push_back(currentField);

    return fields;
}

bool EventManager::parseEventLine(const std::string& line, Event& event) const {
    std::vector<std::string> fields = splitEscapedLine(line);

    if (fields.size() != 6) {
        return false;
    }

    try {
        if (!parseStrictInteger(fields[0], event.id)
            || !parseStrictInteger(fields[1], event.date.day)
            || !parseStrictInteger(fields[2], event.date.month)
            || !parseStrictInteger(fields[3], event.date.year)) {
            return false;
        }

        event.title = unescapeText(fields[4]);
        event.description = unescapeText(fields[5]);

        DateUtils dateUtils;

        if (event.id <= 0 || !dateUtils.isValidDate(event.date) || trim(event.title).empty()) {
            return false;
        }

        event.title = trim(event.title);
    }
    catch (...) {
        return false;
    }

    return true;
}

} // namespace chronocli
