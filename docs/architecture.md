# ChronoCLI Architecture

ChronoCLI is designed as a modular C++ command-line application.

## Architecture Overview

The project separates responsibilities into different modules:

```text
main.cpp
  |
  |-- Calendar
  |-- DateUtils
  |-- EventManager
  |-- InputUtils