# ChronoCLI Features

ChronoCLI is a C++17 terminal calendar and event manager. Its current feature set is intentionally focused and portfolio-friendly.

## Calendar

- Display a monthly calendar using a Sunday-first layout.
- Display a bordered monthly calendar.
- Display all months in a selected year as a 4-month matrix.
- Mark today and event dates in calendar output.
- Detect leap years.
- Calculate month lengths.
- Find the first weekday of any valid month.

## Date Utilities

- Validate dates, including leap-day rules.
- Find the weekday name for a date.
- Compare two valid dates.
- Calculate the absolute number of days between two valid dates.

## Events

- Add events with a date, title, and description.
- Edit existing events.
- Search saved events.
- View upcoming events.
- Delete events by ID.
- Sort event output chronologically.

## Storage

- Save events automatically after add, edit, and delete operations.
- Load saved events on launch.
- Preserve the next event ID after loading.
- Escape text containing pipes, slashes, and line breaks.
- Skip malformed rows, invalid dates, duplicate IDs, and blank titles.

## Engineering

- Modular header/source organization.
- Shared `Date` value type.
- Reusable input utilities.
- Reusable terminal UI formatting.
- CMake build configuration.
- CTest-compatible test target.
- GitHub Actions CI workflow.
