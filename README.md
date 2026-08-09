<div align="center">

# 🏗️ SPLand Reconstruction Simulation

### An object-oriented C++ engine for simulating post-war settlement reconstruction

![C++](https://img.shields.io/badge/C%2B%2B-11-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Build](https://img.shields.io/badge/build-make-orange?style=for-the-badge&logo=gnu&logoColor=white)
![Memory](https://img.shields.io/badge/memory-valgrind%20clean-brightgreen?style=for-the-badge&logo=checkmarx&logoColor=white)
![License](https://img.shields.io/badge/license-academic-lightgrey?style=for-the-badge)

</div>

---

## 👥 Team

| Name |
|---|
| Renad Abu Shareb |
| Adan Abo Salok |

**Course:** Systems Programming &nbsp;•&nbsp; **Lecturer:** Gil Einziger &nbsp;•&nbsp; **TA in charge:** Nir Sorani

---

## 📑 Table of Contents

- [Overview](#-overview)
- [Project Structure](#-project-structure)
- [Building](#-building)
- [Running](#-running)
- [Core Classes](#-core-classes)
- [Selection Policies](#-selection-policies)
- [Supported Actions](#-supported-actions)
- [Configuration File Format](#-configuration-file-format)
- [Memory Management](#-memory-management)

---

## 🌍 Overview

In the land of **SPLand**, a long war has ravaged cities and villages. This program simulates multiple **reconstruction plans**, each tied to a specific settlement and driven by a chosen **facility selection policy**. Every simulation step advances construction and updates the settlement's:

- 🏫 **Life Quality Score**
- 💰 **Economy Score**
- 🌱 **Sustainability Score**

based on the facilities being built.

The program is fully OOP-based, written in **C++11**, and follows strict memory-management rules — the full **Rule of 5** — with zero leaks, verified via Valgrind.

---

## 📂 Project Structure

```
.
├── include/          # Header files (.h)
├── src/               # Source files (.cpp)
├── bin/               # Build output (executable + object files, empty in repo)
└── makefile           # Build configuration
```

---

## 🔧 Building

Build the project with `make`, which compiles all sources into `bin/` and produces the `bin/Simulation` executable.

```bash
make
```

Compiler flags used (per assignment requirements):

```
-g -Wall -Weffc++ -std=c++11 -Iinclude
```

Clean build artifacts:

```bash
make clean
```

> 💡 **Note:** The project is developed to compile, link, and run correctly on a CS Lab UNIX machine.

---

## ▶️ Running

The program takes the path to a configuration file as its first command-line argument:

```bash
./bin/Simulation <path_to_config_file>
```

**Example:**

```bash
./bin/Simulation config_files/example_config.txt
```

Once started, the simulation prints:

```
The simulation has started
```

and then waits for user commands in an interactive loop.

---

## 🧩 Core Classes

| Class | Description |
|---|---|
| `Simulation` | Manages settlements, plans, facility types, and the actions log; drives the main loop. |
| `Settlement` | Represents a settlement (`Village`, `City`, or `Metropolis`), each with a different construction limit (1, 2, or 3 facilities at a time). |
| `FacilityType` | A general facility definition (name, price, category, and contribution to each score). |
| `Facility` | A facility instance tied to a specific settlement and plan, with a construction status. |
| `Plan` | A reconstruction plan for one settlement; tracks operational/under-construction facilities and accumulated scores. |
| `SelectionPolicy` | Abstract strategy for choosing the next facility to build (`Naive`, `Balanced`, `Economy`, `Sustainability`). |
| `BaseAction` | Abstract base for all user actions, tracking completion status and enabling action logging. |

---

## 🎯 Selection Policies

| Code | Policy | Behavior |
|:---:|---|---|
| `nve` | 🔁 Naive | Selects facilities sequentially from the list. |
| `bal` | ⚖️ Balanced | Chooses the facility that minimizes the gap between the plan's max and min scores. |
| `eco` | 💰 Economy | Prioritizes facilities in the Economy category. |
| `env` | 🌱 Sustainability | Prioritizes facilities in the Environment category. |

---

## ⚙️ Supported Actions

| Command | Syntax | Description |
|---|---|---|
| Simulate Step | `step <n>` | Advances the simulation by `n` time units. |
| Add Plan | `plan <settlement_name> <policy>` | Creates a new reconstruction plan. |
| Add Settlement | `settlement <name> <type>` | Adds a settlement (`0`=Village, `1`=City, `2`=Metropolis). |
| Add Facility | `facility <name> <category> <price> <lifeq> <eco> <env>` | Registers a new facility type. |
| Print Plan Status | `planStatus <plan_id>` | Prints a plan's status, policy, scores, and facilities. |
| Change Plan Policy | `changePolicy <plan_id> <policy>` | Changes a plan's selection policy. |
| Print Actions Log | `log` | Prints the history of all executed actions and their status. |
| Backup Simulation | `backup` | Saves a snapshot of the current simulation state. |
| Restore Simulation | `restore` | Restores the last saved snapshot. |
| Close | `close` | Prints final results for all plans and terminates the simulation. |

---

## 📝 Configuration File Format

The config file defines the initial state before the simulation starts, in the following order:

```
settlement <settlement_name> <settlement_type>
facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
plan <settlement_name> <selection_policy>
```

**Example:**

```
settlement Kfar_SPL 0
settlement Kiryat_SPL 2
facility kindergarten 0 3 3 2 1
facility desalinationPlant 2 4 2 2 3
plan Kiryat_SPL bal
plan Kfar_SPL eco
```

---

## 🧠 Memory Management

All classes managing dynamic resources implement the full **Rule of 5** (copy constructor, copy assignment, move constructor, move assignment, destructor).

The project is verified leak-free with:

```bash
valgrind --leak-check=full --show-reachable=yes bin/Simulation <config_file>
```

Expected output:

```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

<div align="center">

---

Made with 🧱 by **Renad Abu Shareb** & **Adan Abo Salok**

</div>
