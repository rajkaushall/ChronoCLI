#ifndef CHRONOCLI_EVENT_MANAGER_HPP
#define CHRONOCLI_EVENT_MANAGER_HPP

#include "DateUtils.hpp"

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

    bool deleteEvent(int eventId);

    std::vector<Event> getEventsByDate(const Date& date) const;
    std::vector<Event> getEventsByMonth(int month, int year) const;
    std::vector<Event> getAllEvents() const;

    bool hasEvents() const;

private:
    std::vector<Event> events;
    int nextEventId;

    bool isSameDate(const Date& firstDate, const Date& secondDate) const;
};

} // namespace chronocli

#endif