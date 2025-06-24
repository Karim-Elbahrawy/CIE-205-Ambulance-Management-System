# AmbulanceManagementSystem

Made by @Karim-Elbahrawy and team for Zewail City CIE205  
Data Structures and Algorithms course — Fall 2024.

## Style Guide

This project is designed to simulate ambulance service operations in a centralized hospital system. It should be developed in C++ and run on Windows using standard compilers (Visual Studio 2022 preferred). The style here follows a clean and organized structure influenced by Google's C++ Style Guide and the C++ Core Guidelines.

---

### General

- Use `Ctrl+K, Ctrl+D` in Visual Studio to auto-format code before committing.
- Comment important logic, especially scheduling, car assignment, and queueing logic.

---

### C++ Version

- Stick to standard C++17 or higher.
- Avoid platform-specific extensions unless needed (e.g., file I/O on Windows).

---

### Header Files

- Each `.cpp` file should have a corresponding `.h` header file.
- Use `#pragma once` for include guards.
- Avoid transitive inclusions — include all necessary headers directly.

---

### Naming

1. Use descriptive names — avoid vague abbreviations.
2. File names: `snake_case`, e.g., `dispatcher_system.h`
3. Variables: `camelCase`
4. Types (classes, structs, enums): `PascalCase`
5. Constants and enum values: `SCREAMING_SNAKE_CASE`
6. Private class members: `snake_case_` (trailing underscore)

---

### Scoping

- Limit scope to the smallest region possible.
- Declare variables close to their use.
- Prefer range-based `for` loops.
- No `goto`, `setjmp`, or `longjmp`.
- Avoid global variables completely.

---

### Comments

- Use `//` for inline comments.
- Explain **why**, not just **what**.
- Use `// TODO:` to mark incomplete parts.

---

### Macros

- Avoid macros unless absolutely necessary.
- If used, name them using `SCREAMING_SNAKE_CASE`.

---

### Structs vs Classes

- Use `struct` only for passive data containers with no logic.
- Use `class` when encapsulation, invariants, or behavior is involved.

---

### Enums

- Always use `enum class` for type safety and scoping.

---

### Pointers and References

- Use references when a value is always expected.
- Use pointers only to represent nullable objects.
- Use `nullptr` instead of `NULL`.

---

### Functions

- Keep them short and single-purpose.
- Prefer few arguments.
- Pass small objects by value, large ones by const-reference.

---

## Project Overview

A centralized system that dispatches ambulance cars (SCars and NCars) across multiple hospitals in response to three types of requests:
- **NP**: Normal Patients
- **SP**: Scheduled Patients
- **EP**: Emergency Patients (with severity level)

The system processes:
- Distance matrices
- Speeds of different car types
- Real-time cancellations
- Queued and prioritized requests
- Car availability and scheduling

---

### Input Format

The input begins with the number of hospitals, followed by:
- Speeds of SCars and NCars
- A square distance matrix (NxN)
- Available SCars and NCars in each hospital
- Patient requests (sorted by time)
- Cancellations (time + patient ID)

---

### Output Format

Output includes:
- For each patient who arrives: `FT PID QT WT`
- Summary statistics at the end:
  - Total patients by type
  - Hospital and car counts
  - Avg wait time
  - % of EPs not served by home hospital
  - Avg busy time and utilization

---

### Example Output

```
FT   PID  QT  WT
245   12   3   77
352   20   5   30
...

patients: 744 [NP: 550, SP: 150, EP: 44]
Hospitals = 23
cars: 112 [SCar: 35, NCar: 77]
Avg wait time = 45
Avg busy time = 152
Avg utilization = 77%
```

---

=

This project is intended to test your knowledge of data structures, simulation, algorithm design, and real-time scheduling — critical tools in building intelligent systems.
