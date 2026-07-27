#include "TerminalUI.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace chronocli {

namespace {

const int leftColumnWidth = 32;
const int rightColumnWidth = 33;
const int totalInnerWidth = leftColumnWidth + 1 + rightColumnWidth;

std::string formatDate(const Date& date) {
    std::ostringstream stream;

    stream << std::setfill('0')
           << std::setw(2) << date.day << "/"
           << std::setw(2) << date.month << "/"
           << date.year
           << std::setfill(' ');

    return stream.str();
}

std::string truncateText(const std::string& text, std::size_t maximumLength) {
    if (text.size() <= maximumLength) {
        return text;
    }

    if (maximumLength <= 3) {
        return text.substr(0, maximumLength);
    }

    return text.substr(0, maximumLength - 3) + "...";
}

std::string getColourCode(TextColour colour) {
    switch (colour) {
        case TextColour::Cyan:
            return "\033[36m";
        case TextColour::Green:
            return "\033[32m";
        case TextColour::Yellow:
            return "\033[33m";
        case TextColour::Red:
            return "\033[31m";
        case TextColour::Dim:
            return "\033[2m";
        case TextColour::Default:
            return "";
    }

    return "";
}

std::string colourText(const std::string& text, TextColour colour, bool bold = false) {
    std::string prefix;

    if (bold) {
        prefix += "\033[1m";
    }

    prefix += getColourCode(colour);

    if (prefix.empty()) {
        return text;
    }

    return prefix + text + "\033[0m";
}

int visibleTextLength(const std::string& text) {
    int length = 0;

    for (std::size_t index = 0; index < text.size(); index++) {
        if (text[index] == '\033' && index + 1 < text.size() && text[index + 1] == '[') {
            index += 2;

            while (index < text.size()) {
                char character = text[index];

                if (character >= '@' && character <= '~') {
                    break;
                }

                index++;
            }
        } else {
            length++;
        }
    }

    return length;
}

std::string padRight(const std::string& text, int width) {
    int visibleLength = visibleTextLength(text);

    if (visibleLength >= width) {
        return text;
    }

    return text + std::string(static_cast<std::size_t>(width - visibleLength), ' ');
}

std::string centerText(const std::string& text, int width) {
    int visibleLength = visibleTextLength(text);

    if (visibleLength >= width) {
        return text;
    }

    int totalPadding = width - visibleLength;
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    return std::string(static_cast<std::size_t>(leftPadding), ' ')
        + text
        + std::string(static_cast<std::size_t>(rightPadding), ' ');
}

void printFullBorder() {
    setTextColour(TextColour::Cyan);
    std::cout << "+"
              << std::string(static_cast<std::size_t>(totalInnerWidth), '-')
              << "+"
              << '\n';
    resetTextColour();
}

void printColumnBorder() {
    setTextColour(TextColour::Cyan);
    std::cout << "+"
              << std::string(static_cast<std::size_t>(leftColumnWidth), '-')
              << "+"
              << std::string(static_cast<std::size_t>(rightColumnWidth), '-')
              << "+"
              << '\n';
    resetTextColour();
}

void printFullRow(const std::string& text = "") {
    setTextColour(TextColour::Cyan);
    std::cout << "|";
    resetTextColour();
    std::cout << padRight(text, totalInnerWidth);
    setTextColour(TextColour::Cyan);
    std::cout << "|" << '\n';
    resetTextColour();
}

void printTwoColumnRow(const std::string& leftText = "", const std::string& rightText = "") {
    setTextColour(TextColour::Cyan);
    std::cout << "|";
    resetTextColour();
    std::cout << padRight(leftText, leftColumnWidth);
    setTextColour(TextColour::Cyan);
    std::cout << "|";
    resetTextColour();
    std::cout << padRight(rightText, rightColumnWidth);
    setTextColour(TextColour::Cyan);
    std::cout << "|" << '\n';
    resetTextColour();
}

void printStatusMessage(TextColour colour, const std::string& label, const std::string& message) {
    setTextColour(colour);
    std::cout << label;
    resetTextColour();
    std::cout << " " << message << '\n';
}

} // namespace

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void setTextColour(TextColour colour) {
    switch (colour) {
        case TextColour::Cyan:
            std::cout << "\033[36m";
            break;
        case TextColour::Green:
            std::cout << "\033[32m";
            break;
        case TextColour::Yellow:
            std::cout << "\033[33m";
            break;
        case TextColour::Red:
            std::cout << "\033[31m";
            break;
        case TextColour::Dim:
            std::cout << "\033[2m";
            break;
        case TextColour::Default:
            resetTextColour();
            break;
    }
}

void resetTextColour() {
    std::cout << "\033[0m";
}

void printHeader(const Date& today, const std::string& weekdayName, int eventsToday, int upcomingEvents) {
    printFullBorder();
    printFullRow(centerText(colourText("CHRONOCLI", TextColour::Cyan, true), totalInnerWidth));
    printFullRow(centerText("Your terminal calendar manager", totalInnerWidth));
    printFullBorder();

    std::string statusText = "Today: " + weekdayName + ", " + formatDate(today)
        + "   Events today: " + std::to_string(eventsToday)
        + "   Upcoming: " + std::to_string(upcomingEvents);

    printFullRow(centerText(statusText, totalInnerWidth));
    printFullBorder();
}

void printMainMenu() {
    printFullRow();
    printFullRow(centerText(colourText("M A I N   M E N U", TextColour::Green, true), totalInnerWidth));
    printFullRow();
    printColumnBorder();
    printTwoColumnRow(colourText("CALENDAR", TextColour::Cyan, true), colourText("EVENTS", TextColour::Cyan, true));
    printTwoColumnRow("  1. View month", "  4. Add event");
    printTwoColumnRow("  2. View year", "  5. Upcoming events");
    printTwoColumnRow("  3. Go to today", "  6. Search events");
    printTwoColumnRow("", "  7. Edit event");
    printTwoColumnRow("", "  8. Delete event");
    printColumnBorder();
    printTwoColumnRow(colourText("SYSTEM", TextColour::Cyan, true), "");
    printTwoColumnRow("  9. Find weekday", "  0. Exit");
    printColumnBorder();
}

void printSuccess(const std::string& message) {
    printStatusMessage(TextColour::Green, "[OK]", message);
}

void printError(const std::string& message) {
    printStatusMessage(TextColour::Red, "[ERROR]", message);
}

void printWarning(const std::string& message) {
    printStatusMessage(TextColour::Yellow, "[WARN]", message);
}

void printInfo(const std::string& message) {
    printStatusMessage(TextColour::Dim, "[INFO]", message);
}

void printEventTable(const std::vector<Event>& events) {
    if (events.empty()) {
        printInfo("No events found.");
        return;
    }

    std::cout << "+------+------------+----------------------+------------------------------+\n";
    std::cout << "| ID   | Date       | Title                | Description                  |\n";
    std::cout << "+------+------------+----------------------+------------------------------+\n";

    for (const Event& event : events) {
        std::cout << "| " << std::left << std::setw(4) << event.id
                  << " | " << std::setw(10) << formatDate(event.date)
                  << " | " << std::setw(20) << truncateText(event.title, 20)
                  << " | " << std::setw(28) << truncateText(event.description, 28)
                  << " |\n";
    }

    std::cout << std::right;
    std::cout << "+------+------------+----------------------+------------------------------+\n";
}

} // namespace chronocli
