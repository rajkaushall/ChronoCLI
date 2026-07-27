#include "EventManager.hpp"
#include "DateUtils.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace chronocli {

EventManager::EventManager()
    : nextEventId(1) {
}

int EventManager::addEvent(const Date& date, const std::string& title, const std::string& description) {
    Event event;

    event.id = nextEventId;
    event.date = date;
    event.title = title;
    event.description = description;

    events.push_back(event);

    nextEventId++;

    return event.id;
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

    return matchedEvents;
}

std::vector<Event> EventManager::getEventsByMonth(int month, int year) const {
    std::vector<Event> matchedEvents;

    for (const Event& event : events) {
        if (event.date.month == month && event.date.year == year) {
            matchedEvents.push_back(event);
        }
    }

    return matchedEvents;
}

std::vector<Event> EventManager::getAllEvents() const {
    return events;
}

bool EventManager::hasEvents() const {
    return !events.empty();
}

bool EventManager::saveEventsToFile(const std::string& filePath) const {
    std::ofstream outputFile(filePath);

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

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        Event event;

        if (parseEventLine(line, event)) {
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
        event.id = std::stoi(fields[0]);
        event.date.day = std::stoi(fields[1]);
        event.date.month = std::stoi(fields[2]);
        event.date.year = std::stoi(fields[3]);
        event.title = unescapeText(fields[4]);
        event.description = unescapeText(fields[5]);

        DateUtils dateUtils;

        if (event.id <= 0 || !dateUtils.isValidDate(event.date)) {
            return false;
        }
    }
    catch (...) {
        return false;
    }

    return true;
}

} // namespace chronocli
