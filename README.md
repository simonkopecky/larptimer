# Larptimer

Larptimer is a program that displays a timer/clock in the terminal, written fully in C. It was made in order to improve my programming skills and prepare myself for uni programming exams. 
<img width="1920" height="1080" alt="Larptimer Screenshot" src="https://github.com/user-attachments/assets/8d57ec6c-22d3-43ab-b9a1-6d188a130f66" />

## Installation

For the installation, use the provided Makefile:
```
git clone https://github.com/simonkopecky/larptimer.git
cd larptimer
make
```

or just run:
```
gcc main.c -o larptimer
```

To install fonts, copy them from /figlet-fonts into one of these directories:
```
/usr/share/figlet/
/usr/share/figlet/fonts/
~/.local/share/figlet/fonts/
~/.local/share/figlet/
```
or add a directory path into figlet-helper.h before compilation.

## Usage
Clock showing current time:
```bash
larptimer # AKA no DURATION argument is provided
```

Timer with the specified DURATION (format: `D:H:M:S`):
```
larptimer DURATION
```
To end the timer before the time has run out, press any key.

To see all available fonts:
```
larptimer --list
```

Display the clock/timer with a specific FONT:
```
larptimer -f FONT
```

Show help message:
```
larptimer --help
```

### Examples
Setting a timer for 5 minutes and 30 seconds:
```
larptimer 5:30
```

1 hour, 4 minutes and 10 seconds:
```bash
larptimer 1:04:10 # 1:4:10 would also work
```

Displaying a clock with font avatar:
```
larptimer -f avatar
```

You can combine larptimer with other programs like so:
```
larptimer 5:00 && play alarm.mp3 # This will play the alarm sound after the timer has ended. 
```
## Contributions
If you find a bug or want to add a feature, make a pull request and I'll look into it asap.
