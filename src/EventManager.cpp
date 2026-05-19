#include "EventManager.hpp"

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

bool EventManager::isSameDate(const Date& firstDate, const Date& secondDate) const {
    return firstDate.day == secondDate.day
        && firstDate.month == secondDate.month
        && firstDate.year == secondDate.year;
}

} // namespace chronocli