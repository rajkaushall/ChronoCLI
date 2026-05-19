# ChronoCLI - C++ Terminal Calendar and Event Manager

![C++ CI](https://github.com/rajkaushall/CalendarProject/actions/workflows/ci.yml/badge.svg)

ChronoCLI is a modular C++ command-line application for viewing calendars, working with dates, and managing events directly from the terminal.

This project started as a simple console calendar application and was upgraded into a portfolio-level C++ project with clean architecture, persistent file storage, unit testing, CMake build support, and GitHub Actions CI.

## Project Highlights

- Built using modern modular C++
- Clean separation of header and source files
- Calendar and date utility algorithms
- Event management system
- File-based persistent storage
- Automated unit tests using CTest
- GitHub Actions CI workflow
- Professional documentation

## Features

### Calendar Features

- Display monthly calendar
- Display yearly calendar
- Check leap year
- Find first weekday of a month

### Date Utility Features

- Validate any date
- Find weekday of any date
- Compare two dates
- Calculate difference between two dates

### Event Management Features

- Add events
- View events by date
- View events by month
- View all events
- Delete events
- Save events to file
- Load events from file
- Auto-save events on exit

### Engineering Features

- Modular C++ architecture
- CMake build system
- Unit tests
- CTest integration
- GitHub Actions CI
- Documentation
- MIT License

## Tech Stack

- C++
- CMake
- CTest
- Git
- GitHub
- GitHub Actions
- Linux / WSL

## Project Structure

```text
ChronoCLI/
│
├── .github/
│   └── workflows/
│       └── ci.yml
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
│   ├── architecture.md
│   ├── features.md
│   ├── learning-notes.md
│   └── project-plan.md
│
├── screenshots/
├── CMakeLists.txt
├── README.md
├── LICENSE
└── .gitignore