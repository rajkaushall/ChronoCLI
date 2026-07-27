# ChronoCLI Features

ChronoCLI is a C++17 terminal calendar and event manager. Its current feature set is intentionally focused and portfolio-friendly.

## Calendar

- Display a monthly calendar using a Sunday-first layout.
- Display all months in a selected year.
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
- View events by exact date.
- View events by month and year.
- View all saved events.
- Delete events by ID.

## Storage

- Save events to `events.txt`.
- Load saved events on launch.
- Preserve the next event ID after loading.
- Escape text containing pipes, slashes, and line breaks.
- Skip malformed or invalid saved rows.

## Engineering

- Modular header/source organization.
- Shared `Date` value type.
- Reusable input utilities.
- CMake build configuration.
- CTest-compatible test target.
- GitHub Actions CI workflow.
