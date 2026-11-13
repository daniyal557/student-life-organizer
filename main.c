/* Student Life Organizer
 * Reconstructed in November 2026 from the Fall 2025 report and email excerpts.
 * Original project: Abdullah Safeer and Daniyal Shafquat.
 * This is a new implementation, not recovered original source.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define MAX_RECORDS 30
#define NAME_SIZE 80

typedef struct {
    char name[NAME_SIZE];
    int total, completed, difficulty, weight;
    double hours;
} Subject;

typedef struct {
    char name[NAME_SIZE];
    int days, importance;
    double hours;
} Task;

/* Read a whole line so invalid input never gets stuck in stdin. */
static void line(const char *prompt, char *buffer, int size)
{
    int ch;
    size_t len;
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(buffer, size, stdin)) {
            puts("\nGoodbye!");
            exit(EXIT_SUCCESS);
        }
        len = strlen(buffer);
        if (len && buffer[len - 1] == '\n') buffer[--len] = '\0';
        else if (!feof(stdin)) {
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            puts("Input too long. Please try again.");
            continue;
        }
        if (len && buffer[len - 1] == '\r') buffer[--len] = '\0';
        for (len = 0; buffer[len] && isspace((unsigned char)buffer[len]); ++len) {}
        if (buffer[len]) return;
        puts("Please enter a value.");
    }
}

static int integer(const char *prompt, int low, int high)
{
    char buf[128], *end;
    long value;
    for (;;) {
        line(prompt, buf, sizeof buf);
        errno = 0;
        value = strtol(buf, &end, 10);
        while (isspace((unsigned char)*end)) ++end;
        if (!errno && end != buf && !*end && value >= low && value <= high)
            return (int)value;
        printf("Enter a whole number from %d to %d.\n", low, high);
    }
}

static double number(const char *prompt, double low, double high)
{
    char buf[128], *end;
    double value;
    for (;;) {
        line(prompt, buf, sizeof buf);
        errno = 0;
        value = strtod(buf, &end);
        while (isspace((unsigned char)*end)) ++end;
        if (!errno && end != buf && !*end && isfinite(value) && value >= low && value <= high)
            return value;
        printf("Enter a number from %.1f to %.1f.\n", low, high);
    }
}

static void studyPlanner(void)
{
    Subject subjects[MAX_RECORDS], tmp;
    Task tasks[MAX_RECORDS], taskTmp;
    int count, taskCount, i, j, remaining;
    double daily, total = 0, budget, allocation;
    puts("\n--- STUDY PLANNER ---");
    count = integer("Number of subjects (1-30): ", 1, MAX_RECORDS);
    for (i = 0; i < count; ++i) {
        printf("\nSubject %d\n", i + 1);
        line("Name: ", subjects[i].name, NAME_SIZE);
        subjects[i].difficulty = integer("Difficulty (1=Low, 2=Medium, 3=High): ", 1, 3);
        subjects[i].total = integer("Total chapters: ", 0, 10000);
        subjects[i].completed = integer("Completed chapters: ", 0, subjects[i].total);
        subjects[i].hours = number("Estimated hours per chapter: ", 0.1, 1000);
        remaining = subjects[i].total - subjects[i].completed;
        subjects[i].weight = remaining * subjects[i].difficulty;
        total += remaining * subjects[i].hours;
    }
    taskCount = integer("\nNumber of pending tasks (0-30): ", 0, MAX_RECORDS);
    for (i = 0; i < taskCount; ++i) {
        printf("\nTask %d\n", i + 1);
        line("Task / quiz / exam name: ", tasks[i].name, NAME_SIZE);
        tasks[i].days = integer("Days until deadline (0=today): ", 0, 3650);
        tasks[i].importance = integer("Importance (1=Low, 2=Medium, 3=High): ", 1, 3);
        tasks[i].hours = number("Additional preparation hours (exclude chapter work above): ", 0, 10000);
        total += tasks[i].hours;
    }
    daily = number("\nAvailable study hours per day: ", 0.1, 24);
    for (i = 0; i < count - 1; ++i)
        for (j = 0; j < count - i - 1; ++j)
            if (subjects[j].weight < subjects[j + 1].weight) {
                tmp = subjects[j]; subjects[j] = subjects[j + 1]; subjects[j + 1] = tmp;
            }
    for (i = 0; i < taskCount - 1; ++i)
        for (j = 0; j < taskCount - i - 1; ++j)
            if (tasks[j].days > tasks[j + 1].days ||
                (tasks[j].days == tasks[j + 1].days && tasks[j].importance < tasks[j + 1].importance)) {
                taskTmp = tasks[j]; tasks[j] = tasks[j + 1]; tasks[j + 1] = taskTmp;
            }
    puts("\nSubject priority (remaining chapters x difficulty):");
    for (i = 0; i < count; ++i)
        printf("%d. %s | Remaining: %d | Weight: %d | Hours: %.1f\n", i + 1,
               subjects[i].name, subjects[i].total - subjects[i].completed,
               subjects[i].weight, (subjects[i].total - subjects[i].completed) * subjects[i].hours);
    puts("\nTask order (nearest deadline first; importance breaks ties):");
    for (i = 0; i < taskCount; ++i) {
        printf("%d. %s | Due in %d day(s) | Hours: %.1f\n", i + 1, tasks[i].name, tasks[i].days, tasks[i].hours);
        if (tasks[i].days == 0) puts("   Due today: check your remaining time.");
        else if (tasks[i].hours > daily * tasks[i].days)
            puts("   Warning: this task alone exceeds your study capacity before its deadline.");
    }
    printf("\nTotal estimated workload: %.1f hours\n", total);
    printf("Minimum study days at %.1f hours/day: %.0f\n", daily, ceil(total / daily));
    puts("\nSuggested next study day (tasks first, then subject priority):");
    budget = daily;
    for (i = 0; i < taskCount && budget > 0.000001; ++i) {
        allocation = tasks[i].hours < budget ? tasks[i].hours : budget;
        if (allocation > 0) printf("  %s: %.2f hours\n", tasks[i].name, allocation);
        budget -= allocation;
    }
    for (i = 0; i < count && budget > 0.000001; ++i) {
        allocation = (subjects[i].total - subjects[i].completed) * subjects[i].hours;
        if (allocation > budget) allocation = budget;
        if (allocation > 0) printf("  %s: %.2f hours\n", subjects[i].name, allocation);
        budget -= allocation;
    }
    if (total == 0) puts("No pending work. All entered chapters and tasks are complete.");
    puts("Estimates are a planning aid; review deadlines and take regular breaks.");
}

static void attendanceMonitor(void)
{
    int count, i, j, held, attended, present, streak, longest, needed;
    double target, percent;
    char name[NAME_SIZE];
    const char *days[] = {"Monday (1=Present, 0=Absent): ", "Tuesday (1/0): ",
                          "Wednesday (1/0): ", "Thursday (1/0): ", "Friday (1/0): "};
    puts("\n--- ATTENDANCE MONITOR ---");
    count = integer("Number of courses (1-30): ", 1, MAX_RECORDS);
    target = number("Your required attendance percentage (1-100): ", 1, 100);
    for (i = 0; i < count; ++i) {
        line("\nCourse name: ", name, NAME_SIZE);
        held = integer("Classes held: ", 0, 100000);
        attended = integer("Classes attended: ", 0, held);
        streak = longest = 0;
        puts("Enter Mon-Fri presence indicators for a separate pattern check.");
        puts("These do not change the class totals entered above.");
        for (j = 0; j < 5; ++j) {
            present = integer(days[j], 0, 1);
            streak = present ? 0 : streak + 1;
            if (streak > longest) longest = streak;
        }
        printf("\n%s\n", name);
        if (held == 0) puts("Attendance: N/A (no classes held).");
        else {
            percent = attended * 100.0 / held;
            printf("Attendance: %.1f%% | Target: %.1f%%\n", percent, target);
            if (percent + 0.000001 >= target) puts("Status: Meets your entered target.");
            else {
                puts("Status: Below your entered target.");
                if (target == 100) puts("Past absences prevent reaching exactly 100% this term.");
                else {
                    needed = (int)ceil((target * held - 100.0 * attended) / (100.0 - target));
                    printf("Attend the next %d consecutive classes to reach the target.\n", needed);
                }
            }
        }
        printf("Longest consecutive absences in the entered Mon-Fri pattern: %d\n", longest);
        if (longest >= 2) puts("Review repeated absences and plan for upcoming classes.");
    }
}

static void sleepAnalyzer(void)
{
    int i, zero = 0;
    double hours, total = 0, shortest = 24, longest = 0, goal;
    puts("\n--- SLEEP ANALYZER ---");
    goal = number("Your personal daily sleep goal in hours: ", 0.1, 24);
    for (i = 0; i < 7; ++i) {
        printf("Day %d\n", i + 1);
        hours = number("Hours slept (0-24): ", 0, 24);
        total += hours;
        if (hours < shortest) shortest = hours;
        if (hours > longest) longest = hours;
        if (hours == 0) ++zero;
    }
    printf("\nTotal sleep: %.1f hours\nAverage per day: %.1f hours\n", total, total / 7);
    printf("Shortest day: %.1f | Longest day: %.1f | Range: %.1f hours\n", shortest, longest, longest - shortest);
    printf("Zero-sleep days: %d\n", zero);
    if (fabs(total / 7 - goal) < 0.05) puts("Your weekly average matches your personal goal.");
    else printf("Your weekly average is %.1f hours %s your personal goal.\n",
                fabs(total / 7 - goal), total / 7 < goal ? "below" : "above");
    puts("This is a record of your entries, not a sleep quality diagnosis.");
}

int main(void)
{
    int choice;
    puts("STUDENT LIFE ORGANIZER");
    puts("Study planning, attendance tracking and weekly sleep summaries.");
    puts("Session only: records are not saved after you exit.");
    do {
        puts("\n1 - Study Planner\n2 - Attendance Monitor\n3 - Sleep Analyzer\n0 - Exit");
        choice = integer("Choice: ", 0, 3);
        switch (choice) {
            case 1: studyPlanner(); break;
            case 2: attendanceMonitor(); break;
            case 3: sleepAnalyzer(); break;
            default: puts("Goodbye!"); break;
        }
    } while (choice != 0);
    return EXIT_SUCCESS;
}
