# 🐞 my_gdb – A Minimal Signal-Based Debugger in C

`my_gdb` is a command-line mini-debugger for C programs, built from scratch using **Linux signals**, **process forking**, and **runtime code instrumentation**. It mimics GDB-like behavior for stepping through `main()` line-by-line using keyboard commands.

> 🎯 Ideal for students and system programmers learning **signal handling**, **process control**, and **basic debugging internals**.

---

## 🔧 Features

- 🧠 **Custom signal handlers** to pause/resume execution
- 🔀 **Fork-based process control** for parent-child interaction
- ⏸️ **Line-by-line stepping** using inserted `pause()` calls
- 🧼 **Preprocessing** and filtering via `gcc -E` and `grep`
- 🎨 **Color-coded terminal UI** using ANSI escape codes
- 🧾 Simple debugger interface: `(r)un`, `(n)ext`, `(q)uit`

---

## 🛠️ How It Works

1. Parses the given `.c` file.
2. Rewrites it:
   - Inserts signal handlers.
   - Adds `pause()` after every line inside `main()`.
3. Compiles and forks:
   - Child runs the modified binary.
   - Parent controls execution via **SIGUSR1**.

---

## 🧪 Example Usage

```bash
$ ./my_gdb test.c

use:./my_gdb <file.c>
press (r)un to start,(q)uit

(my_gdb) : r
Running gdb
gdb:4321, temp:4322
***** MY GDB BASIC *****
(n)ext line, (q)uit

line-nn  : x = 10;
(my_gdb) : n
line-nn  : y = x + 5;
```

---
## 📚 Files
```bash
my_gdb/
├── my_gdb.c         # Source code
├── test.c           # Sample program to debug
├── prog.c           # Instrumented source
├── prog.i           # Preprocessed file
├── temp.i           # Final file with pause() inserted
```
---
## 📚 Concepts Used

- Linux Signals: `SIGUSR1`, `SIGUSR2`, `SIGKILL`, `pause()`
- Process Creation: `fork()`, `execl()`, `kill()`
- File I/O and parsing: `fopen()`, `fgets()`, `fputs()`
- ANSI Escape Sequences for color terminal output
- GCC Preprocessing: `gcc -E`
- Input Flushing: `__fpurge(stdin)`

---

## 💡 Educational Value

- Helps understand how debuggers work behind the scenes.
- Excellent mini-project for **OS**, **System Programming**, or **Debugging Tools** coursework.
- Demonstrates practical use of signals, forks, and inter-process control.

---

## 🧑‍💻 Author

Tej, B.Tech in Electrical Engineering  
Focused on Embedded Systems, Linux Internals & C Systems Programming  
🇮🇳 India

---

## 📝 License

MIT License – feel free to learn and build on this project.
