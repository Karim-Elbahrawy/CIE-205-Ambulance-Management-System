# 🚑 Ambulance Management System

## 📋 Project Description

This project simulates a centralized ambulance dispatching and allocation system that manages ambulance services across multiple hospitals. The goal is to automate the ambulance assignment process in a way that ensures optimal use of available ambulance cars, improves response time, and prioritizes emergency cases.

Designed for the **Data Structures and Algorithms** course at **Zewail City - University of Science and Technology**, this system models the dynamic nature of real-world ambulance operations using efficient data structures, algorithms, and scheduling strategies.

## 💡 Objectives

- Simulate emergency and non-emergency patient requests.
- Dispatch the nearest available car (SCars for scheduled transport, NCars for emergency pickup).
- Handle patient request cancellations in real-time.
- Calculate various statistics for performance evaluation (e.g., average wait time, car utilization, etc.).
- Output simulation results in a structured format.

## 📥 Input File Description

The input consists of:

1. Number of hospitals.
2. Speeds of SCars and NCars.
3. A 2D matrix with distances between hospitals.
4. Number of SCars and NCars at each hospital.
5. A list of patient requests:
    - **NP**: Normal patient
    - **SP**: Scheduled patient
    - **EP**: Emergency patient (includes severity)
6. A number of cancellations, followed by lines with patient ID and cancellation time.

> ⚠️ Request lines are sorted in ascending order by request time.  
> ❌ Ignore cancellation if the patient has already been picked up.

### Sample Input
```
4                  --> 4 hospitals
110 75             --> SCars speed = 110, NCars speed = 75

0   154 587 634    --> Distance matrix (4x4)
154 0   533 214
587 533 0   945
634 214 945 0

3 11               --> 3 SCars, 11 NCars in hospital 1
2 5                --> 2 SCars, 5 NCars in hospital 2
6 15               --> 6 SCars, 15 NCars in hospital 3
1 2                --> 1 SCar, 2 NCars in hospital 4

150                --> Number of requests
NP 3 1 2 159        --> NP: time=3, PID=1, HID=3, DST=159m
SP 3 2 1 588        --> SP: time=3, PID=2, HID=1, DST=588m
EP 12 3 4 433 5     --> EP: time=12, PID=3, HID=4, DST=433m, SVR=5
...

7                  --> Number of cancellations
15 1               --> PID=1 canceled at time=15
...
```

## 📤 Output File Description

Each output line corresponds to a patient who successfully arrived at a hospital:

Format:
```
FT  PID  QT  WT
```
Where:
- `FT`: Finish time
- `PID`: Patient ID
- `QT`: Queue time
- `WT`: Wait time

### Sample Output
```
FT   PID  QT  WT
245   12   3   77
352   20   5   30
352    1   1   56
...
```

### Statistics Section
```
patients: 744 [NP: 550, SP: 150, EP: 44]
Hospitals = 23
cars: 112 [SCar: 35, NCar: 77]
Avg wait time = 45
Avg busy time = 152
Avg utilization = 77%
```


## 🧠 Concepts Covered

- Priority queues & scheduling
- Graph traversal and shortest path algorithms
- Simulation modeling
- Dynamic event processing
- File I/O operations
- Data structure design & memory management

## 📎 Note

> This project is part of the course **CIE205: Data Structures and Algorithms – Fall 2024**.  

