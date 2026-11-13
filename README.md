# Student Life Organizer

A C console application for study planning, course attendance and weekly sleep summaries. Originally a first-semester Programming Fundamentals project at FAST-NUCES Karachi (Fall 2025).

## Project history and credits

Original team: **Abdullah Safeer** and **Daniyal Shafquat**.

The original complete source was unavailable. This edition was reconstructed with Codex assistance in November 2026 from the surviving project report and email excerpts. It is a new implementation of the documented project, not the original submitted code. Individual contributions to the original project have not been assigned here.

## Features

- Rank subjects by remaining chapters multiplied by difficulty.
- Order pending tasks by deadline, using importance to break ties.
- Estimate workload and allocate a daily study budget without exceeding it.
- Calculate course attendance against a user-entered requirement, including the classes needed to recover.
- Summarize Mon-Fri absence streaks separately from attendance totals.
- Summarize seven days of sleep against a personal goal.
- Reject invalid numbers, out-of-range entries and overlong input; exit cleanly at end of input.

## Build and run

Requires a C99 compiler. Open `Student Life Organizer.cbp` in Code::Blocks, select a configured GNU GCC compiler, then Build and Run. Dev-C++ can also compile `main.c` as C with the C99 standard enabled and the math library linked.

From a terminal with GCC available:

```sh
gcc -std=c99 -Wall -Wextra -Wpedantic main.c -o organizer -lm
```

Run `organizer.exe` on Windows or `./organizer` on Linux/macOS. On this computer, `build.ps1` locates an installed GCC and builds `bin/organizer.exe`.

## Example

Math with 12 chapters, 5 completed, difficulty 3 and 1.5 hours per chapter produces 7 remaining chapters, priority weight 21 and 10.5 estimated hours.

## Limitations

Records last for the current session only. There is no GUI, calendar integration or automatic deadline tracking. Daily planning is a simple ordered allocation, not a guarantee that every deadline is feasible. Task hours must exclude chapter work already counted. The weekly attendance pattern assumes one presence indicator per weekday. Sleep summaries describe the supplied data and do not assess health.

## Reconstruction changes

Added robust input validation and a clear exit option. Attendance requirements and sleep goals are entered by the user. Fixed the email excerpt's sleep-range validation error and avoided its overlapping or missing category boundaries. The report contains some unrelated grade-calculation text; this implementation follows the documented three modules instead.

Original reports are retained locally under `reference/` and excluded from Git because they contain student IDs and document metadata.

## Validation

Built with GCC 13.1 using C99 and strict compiler warnings. Run `python tests/check_cli.py` after building to repeat the 13 passing checks across all three modules, boundary values, invalid input and end-of-input handling.
