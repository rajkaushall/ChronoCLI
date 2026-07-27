#ifndef CHRONOCLI_EVENT_MANAGER_HPP
#define CHRONOCLI_EVENT_MANAGER_HPP

#include "Date.hpp"

#include <string>
#include <vector>

namespace chronocli {

struct Event {
    int id;
    Date date;
    std::string title;
    std::string description;
};

class EventManager {
public:
    EventManager();

    int addEvent(const Date& date, const std::string& title, const std::string& description);

    bool updateEvent(int eventId, const Date& date, const std::string& title, const std::string& description);
    bool deleteEvent(int eventId);

    std::vector<Event> getEventsByDate(const Date& date) const;
    std::vector<Event> getEventsByMonth(int month, int year) const;
    std::vector<Event> getUpcomingEvents(const Date& fromDate) const;
    std::vector<Event> searchEvents(const std::string& keyword) const;
    std::vector<Event> getAllEvents() const;

    bool hasEventOnDate(const Date& date) const;
    bool hasEvents() const;

    bool saveEventsToFile(const std::string& filePath) const;
    bool loadEventsFromFile(const std::string& filePath);

private:
    std::vector<Event> events;
    int nextEventId;

    bool isSameDate(const Date& firstDate, const Date& secondDate) const;

    std::string escapeText(const std::string& text) const;
    std::string unescapeText(const std::string& text) const;
    std::vector<std::string> splitEscapedLine(const std::string& line) const;
    bool parseEventLine(const std::string& line, Event& event) const;
};

} // namespace chronocli

#endif
