# ChronoCLI Architecture

ChronoCLI is a modular C++17 command-line application. The interactive menu is kept separate from the calendar, date, event, and input modules so the core behavior can be tested without driving the full terminal workflow.

```text
main.cpp
|-- Calendar
|-- DateUtils
|-- EventManager
|-- InputUtils
`-- TerminalUI
```

## Modules

- `main.cpp` coordinates the menu, prompts, and user-facing workflow.
- `TerminalUI` owns the bordered terminal dashboard, menu, messages, and event table formatting.
- `Calendar` owns bordered month/year calendar display and calendar-specific validation.
- `DateUtils` owns date validation, weekday lookup, date comparison, and day-difference calculations.
- `EventManager` owns event storage in memory, event validation, sorting, searching, and file persistence.
- `InputUtils` owns reusable EOF-safe integer/text input helpers for the CLI.
- `Date` is a shared value type used by date and event modules.

## Data Flow

1. The user selects an action from the menu.
2. `main.cpp` reads validated input through `InputUtils`.
3. The selected module performs the requested operation.
4. Results are printed to the terminal.
5. Event data is saved automatically after add, edit, and delete operations.

## Storage Format

Events are stored as escaped pipe-separated rows:

```text
id|day|month|year|title|description
```

Malformed rows, invalid IDs, duplicate IDs, blank titles, and invalid dates are ignored during load.
