# ChronoCLI — C++ Terminal Calendar & Event Manager

<p align="center">
  <b>A modular C++17 command-line calendar and event management application</b>
</p>

<p align="center">
  <a href="https://github.com/rajkaushall/ChronoCLI/actions/workflows/ci.yml">
    <img src="https://github.com/rajkaushall/ChronoCLI/actions/workflows/ci.yml/badge.svg" alt="C++ CI">
  </a>
  <a href="LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="MIT License">
  </a>
  <img src="https://img.shields.io/badge/C++-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/Build-CMake-064F8C.svg" alt="CMake">
</p>

---

## Overview

**ChronoCLI** is a terminal-based calendar and event manager built using **modern C++17**.  
It allows users to view monthly/yearly calendars, perform date calculations, validate dates, manage events, and persist event data locally.

This project focuses on clean code, modular architecture, file handling, unit testing, and CI/CD using GitHub Actions.

---

## Key Highlights

- Modular C++ project structure
- Calendar generation for monthly and yearly views
- Date validation and date utility operations
- Event creation, deletion, viewing, and persistence
- File-based storage system
- Unit testing with CTest
- CMake-based build system
- GitHub Actions CI workflow
- Professional documentation and project organization

---

## Features

### Calendar Module

- Display monthly calendar
- Display yearly calendar
- Detect leap years
- Calculate the first weekday of any month
- Generate clean terminal calendar output

### Date Utilities

- Validate user-entered dates
- Find the weekday of any date
- Compare two dates
- Calculate the difference in days between two dates

### Event Management

- Add events for specific dates
- Delete saved events
- View events by date
- View events by month
- View all stored events
- Auto-load events on launch
- Auto-save events on exit

---

## Tech Stack

| Category | Technology |
|---|---|
| Language | C++17 |
| Build System | CMake |
| Testing | CTest |
| CI/CD | GitHub Actions |
| Storage | File-based persistence |
| Version Control | Git & GitHub |
| Platform | Linux / WSL / macOS |

---

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
```

---

## Getting Started

### Prerequisites

Make sure the following tools are installed:

- `g++` with C++17 support
- `cmake` version 3.14 or higher
- `make`
- `git`

---

## Installation & Build

```bash
git clone https://github.com/rajkaushall/ChronoCLI.git
cd ChronoCLI
```

Create a build directory:

```bash
mkdir build
cd build
```

Generate build files:

```bash
cmake ..
```

Build the project:

```bash
cmake --build .
```

Run the application:

```bash
./chronocli
```

> Note: If your executable name is different, check it using `ls` inside the `build` directory.

---

## Usage

After running the application, ChronoCLI opens an interactive terminal menu:

```text
============================
 ChronoCLI — Main Menu
============================

1. View Monthly Calendar
2. View Yearly Calendar
3. Date Utilities
4. Event Manager
5. Exit

Enter choice:
```

### Example: Monthly Calendar

```text
March 2025

Sun Mon Tue Wed Thu Fri Sat
                         1
  2   3   4   5   6   7   8
  9  10  11  12  13  14  15
 16  17  18  19  20  21  22
 23  24  25  26  27  28  29
 30  31
```

### Example: Add Event

```text
Enter date (DD MM YYYY): 20 05 2025
Enter event description: Project submission deadline

Event saved successfully.
```

---

## Running Tests

From the `build` directory, run:

```bash
ctest --verbose
```

The test suite validates the core logic of:

- Calendar generation
- Date utilities
- Event management

Tests also run automatically through GitHub Actions on every push.

---

## CI/CD

This project uses **GitHub Actions** to automatically build and test the project whenever changes are pushed to the repository.

Workflow file:

```text
.github/workflows/ci.yml
```

This ensures that the project remains stable and build-ready.

---

## Skills Demonstrated

This project demonstrates:

- C++17 programming
- Object-oriented design
- Modular code organization
- Header and source file separation
- File handling and persistence
- Input validation
- Date calculation logic
- Unit testing
- CMake build configuration
- GitHub Actions CI
- Git and GitHub workflow
- Clean technical documentation

---

## Future Improvements

Planned improvements for future versions:

- SQLite database integration
- REST API layer
- User authentication
- Web dashboard
- Event reminders
- Recurring events
- Export events to CSV or JSON
- Docker support
- Deployment-ready architecture

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## Author

**Raj Kaushal**

- GitHub: [rajkaushall](https://github.com/rajkaushall)
- LinkedIn: [rajkaushall](https://linkedin.com/in/rajkaushall)
- LeetCode: [rajkaushall](https://leetcode.com/u/rajkaushall)

---

<p align="center">
  Built with C++17, CMake, and clean software engineering practices.
</p>