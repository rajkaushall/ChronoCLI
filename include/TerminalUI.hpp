#ifndef CHRONOCLI_TERMINAL_UI_HPP
#define CHRONOCLI_TERMINAL_UI_HPP

#include "Date.hpp"
#include "EventManager.hpp"

#include <string>
#include <vector>

namespace chronocli {

enum class TextColour {
    Default,
    Cyan,
    Green,
    Yellow,
    Red,
    Dim
};

void clearScreen();
void setTextColour(TextColour colour);
void resetTextColour();

void printHeader(const Date& today, const std::string& weekdayName, int eventsToday, int upcomingEvents);
void printMainMenu();
void printSuccess(const std::string& message);
void printError(const std::string& message);
void printWarning(const std::string& message);
void printInfo(const std::string& message);
void printEventTable(const std::vector<Event>& events);

} // namespace chronocli

#endif
