"""Run after build.ps1; uses only Python's standard library."""
from pathlib import Path
import os
import subprocess

root = Path(__file__).resolve().parents[1]
exe = root / 'bin' / ('organizer.exe' if os.name == 'nt' else 'organizer')

def check(name, entries, expected):
    result = subprocess.run([str(exe)], input='\n'.join(map(str, entries)) + '\n',
                            text=True, capture_output=True, timeout=5)
    assert result.returncode == 0, (name, result.stderr)
    for text in expected:
        assert text in result.stdout, (name, text, result.stdout)
    print('PASS:', name)

check('exit', [0], ['Goodbye!'])
check('menu validation', ['abc', -1, 4, '1.5', 'x'*200, 0], ['Input too long', 'Enter a whole number'])
check('documented subject ranking', [1, 2, 'English', 1, 10, 7, 1, 'Math', 3, 12, 5, 1.5, 0, 3, 0],
      ['1. Math | Remaining: 7 | Weight: 21 | Hours: 10.5', 'Total estimated workload: 13.5', 'Minimum study days at 3.0 hours/day: 5', 'Math: 3.00 hours'])
check('task deadline and importance order', [1, 1, 'Done', 1, 0, 0, 1, 3,
      'Later', 5, 3, 1, 'Today low', 0, 1, 1, 'Today high', 0, 3, 2, 2, 0],
      ['1. Today high', '2. Today low', '3. Later', 'Today high: 2.00 hours'])
check('completed workload', [1, 1, 'Done', 1, 0, 0, 1, 0, 2, 0], ['No pending work', 'Minimum study days at 2.0 hours/day: 0'])
check('attendance target boundary', [2, 1, 75, 'Math', 20, 15, 1, 0, 0, 1, 1, 0],
      ['Attendance: 75.0%', 'Meets your entered target', 'pattern: 2'])
check('attendance recovery and validation', [2, 1, 75, 'Math', 20, 21, 10, 0, 0, 0, 1, 1, 0],
      ['Enter a whole number from 0 to 20', 'Attend the next 20 consecutive classes', 'pattern: 3'])
check('no classes', [2, 1, 75, 'New', 0, 0, 1, 1, 1, 1, 1, 0], ['Attendance: N/A'])
check('100 percent requirement', [2, 1, 100, 'Math', 20, 19, 1, 1, 1, 1, 1, 0], ['Past absences prevent reaching exactly 100%'])
check('sleep total and invalid inputs', [3, 7, 'nan', 'inf', -1, 25, 'abc', 7, 6, 8, 5, 9, 6, 7, 0],
      ['Total sleep: 48.0', 'Average per day: 6.9', 'Zero-sleep days: 0', '0.1 hours below'])
check('sleep endpoints', [3, 8, 0, 24, 8, 8, 8, 8, 8, 0],
      ['Shortest day: 0.0 | Longest day: 24.0', 'Zero-sleep days: 1'])
check('sleep personal goal equality', [3, 8, 8, 8, 8, 8, 8, 8, 8, 0], ['matches your personal goal'])
check('end of input while entering course', [2], ['Goodbye!'])
print('All 13 checks passed.')
