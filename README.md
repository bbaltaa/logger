# Simple Logger – Header-Only C/C++ Logger

This is a lightweight, header-only logging utility written in C, designed for real-time applications like robotics or embedded systems. It helps you collect time-stamped data and save it to a `.csv` file, without worrying about file I/O during runtime.

## ✨ What it does

- Logs time-stamped data in the format: `timestamp_us, a, b, c`
- Buffers data in memory first (to reduce I/O)
- Periodically flushes data into a bigger buffer
- Saves all logged data to a `.csv` file when you're done
- Works in both C and C++ projects

## 🚀 How to use it

1. **Include the header**

If you want the full implementation in one file, just do:

```c
#define LOGGER_IMPLEMENTATION
#include "logger.h"
