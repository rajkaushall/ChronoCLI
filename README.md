# ChronoCLI — C++ Terminal Calendar & Event Manager

[![C++ CI](https://github.com/rajkaushall/ChronoCLI/actions/workflows/ci.yml/badge.svg)](https://github.com/rajkaushall/ChronoCLI/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/Build-CMake-064F8C.svg)](https://cmake.org/)

---

## Table of Contents

- [Features](#features)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Running Tests](#running-tests)
- [License](#license)


## Features

### Calendar
- Display monthly and yearly calendar
- Detect leap years
- Find the first weekday of any month

### Date Utilities
- Validate any date input
- Find the weekday for an arbitrary date
- Compare two dates
- Calculate the difference in days between two dates

### Event Management
- Add / delete events tied to specific dates
- View events by date, by month, or all at once
- File-based persistence — auto-saves on exit, auto-loads on launch


## Tech Stack

| Category        | Technology              |
|-----------------|-------------------------|
| Language        | C++ 17                  |
| Build System    | CMake                   |
| Testing         | CTest                   |
| CI/CD           | GitHub Actions          |
| Storage         | File-based persistence  |
| Platform        | Linux / WSL / macOS     |
| Version Control | Git & GitHub            |

## Project Structure

```text
ChronoCLI/
│
├── .github/
│   └── workflows/
│       └── ci.yml          # GitHub Actions CI pipeline
│
├── include/                # Header files (interfaces)
│   ├── Calendar.hpp
│   ├── DateUtils.hpp
│   ├── EventManager.hpp
│   └── InputUtils.hpp
│
├── src/                    # Implementation files
│   ├── Calendar.cpp
│   ├── DateUtils.cpp
│   ├── EventManager.cpp
│   ├── InputUtils.cpp
│   └── main.cpp            # Entry point
│
├── tests/
│   └── test_runner.cpp     # CTest unit tests
│
├── docs/                   # Architecture and planning notes
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


---


```
## Getting Started

### Prerequisites

- `g++` (GCC 9+ or Clang 10+) with C++17 support
- `cmake` (version 3.14 or higher)
- `make`

### Clone & Build

```bash
# Clone the repository
git clone https://github.com/rajkaushall/ChronoCLI.git
cd ChronoCLI

# Create build directory and compile
mkdir build && cd build
cmake ..
make

# Run the application
./ChronoCLI
---

## Usage

ChronoCLI presents an interactive terminal menu on launch:

```
============================

    ChronoCLI — Main Menu

============================

 1. View Monthly Calendar
 2. View Yearly Calendar
 3. Date Utilities
 4. Event Manager
 5. Exit

============================

Enter choice:
```

**View March 2025:**
```
        March 2025
 Sun Mon Tue Wed Thu Fri Sat
                           1
   2   3   4   5   6   7   8
   9  10  11  12  13  14  15
  16  17  18  19  20  21  22
  23  24  25  26  27  28  29
  30  31
```

**Add an Event:**
```
Enter date (DD MM YYYY): 20 05 2025
Enter event description: Project submission deadline
✓ Event saved successfully.
```

---

## Running Tests

```bash
cd build
ctest --verbose
```

All core modules — Calendar, DateUtils, and EventManager — are covered by unit tests that run automatically on every push via GitHub Actions CI.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

*Built by [Raj Kaushal](https://github.com/rajkaushall) · [LinkedIn](https://linkedin.com/in/rajkaushall) · [LeetCode](https://leetcode.com/u/rajkaushall)*