# pomodoro-timer
A simple pomodoro-based timer (from http://cirillocompany.de/pages/pomodoro-technique/) written in C++11 for linux-based distros.
It is the first hobby project, feel free to use it and modify it.

## Building
To build this project make sure you have ```libnotify 0.7.x``` and ```gtk+-2.0``` libraries installed.
### For Ubuntu-based distros:
```
$ sudo apt-get install libnotify-dev build-essential g++ make
```
That should be enough to successfully build the project. After all the libraries' setup just run:
```
$ make
```
