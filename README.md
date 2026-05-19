# ChronoCLI - C++ Terminal Calendar and Event Manager

ChronoCLI is a modern C++ command-line application for viewing calendars, working with dates, and managing events directly from the terminal.

This project is being built as a portfolio-level C++ project with clean code, modular architecture, testing, documentation, and GitHub workflow.

## Current Status

```text
Phase 7: Unit testing



Update features:

```md
## Current Features

- Display monthly calendar
- Display yearly calendar
- Leap year detection
- Find the first weekday of a month
- Validate any date
- Find weekday of any date
- Compare two dates
- Calculate difference between two dates
- Add events
- View events by date
- View events by month
- View all events
- Delete events
- Save events to file
- Load events from file
- Auto-save events on exit
- Reusable input validation
- Continuous menu loop
- Modular C++ structure
- CMake build support
- Automated unit tests

Update project structure:

```md
## Project Structure

```text

Update project structure:

```md
## Project Structure

```text
ChronoCLI/
│
├── include/
│   ├── Calendar.hpp
│   ├── DateUtils.hpp
│   ├── EventManager.hpp
│   └── InputUtils.hpp
│
├── src/
│   ├── Calendar.cpp
│   ├── DateUtils.cpp
│   ├── EventManager.cpp
│   ├── InputUtils.cpp
│   └── main.cpp
│
├── tests/
│   └── test_runner.cpp
│
├── docs/
│   └── project-plan.md
│
├── CMakeLists.txt
├── README.md
└── .gitignore


## Event Storage

ChronoCLI stores events in a local text file named:

```text
events.txt

## Running Tests

ChronoCLI includes automated tests for calendar logic, date utilities, event management, and file storage.

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest --output-on-failure