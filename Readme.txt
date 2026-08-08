# SPLand Reconstruction Simulation

An object-oriented C++ simulation system for modeling post-war reconstruction plans across settlements. Built as Assignment #1 for the Systems Programming course (Fall 2024), under Lecturer Gil Einziger.

**Authors:** Renad Abu Shareb & Adan Abo Salok

The simulation manages multiple settlements, each with one or more reconstruction **plans**. Every plan follows a configurable **selection policy** that decides which facility (school, hospital, factory, park, etc.) to build next, balancing three core metrics: **Life Quality**, **Economy**, and **Sustainability**.

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Running the Simulation](#running-the-simulation)
- [Configuration File Format](#configuration-file-format)
- [Core Classes](#core-classes)
- [Selection Policies](#selection-policies)
- [Actions](#actions)
- [Memory Management](#memory-management)
- [Testing with Valgrind](#testing-with-valgrind)

## Overview

The program reads an initial configuration (settlements, available facility types, and starter plans), then enters an interactive loop where the user issues text commands ("actions") to:

- Create settlements and plans
- Register new facility types
- Advance the simulation by one or more time steps
- Inspect plan status
- Change a plan's selection policy on the fly
- View the action history log
- Back up and restore simulation state
- Gracefully close the simulation, printing final results and freeing all resources

Each settlement has a **construction limit** (how many facilities it can build concurrently), determined by its type:

| Settlement Type | Construction Limit |
|---|---|
| Village | 1 |
| City | 2 |
| Metropolis | 3 |

## Project Structure

```
.
├── include/          # Header (.h) files
├── src/               # Source (.cpp) files
├── bin/               # Build output (compiled objects + executable) — empty in repo
└── makefile           # Build rules
```

Source files:

```
src/main.cpp
src/Action.cpp
src/Auxiliary.cpp
src/Facility.cpp
src/Plan.cpp
src/SelectionPolicy.cpp
src/Settlement.cpp
src/Simulation.cpp
```

## Building the Project

Build everything with `make`:

```bash
make
```

This compiles each source file with the following flags and links them into `bin/simulation`:

```
g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c <file>.cpp -o bin/<file>.o
g++ -o bin/simulation bin/*.o
```

To clean build artifacts:

```bash
make clean
```

> **Note:** The project must compile cleanly with **no warnings or errors** using `-g -Wall -Weffc++ -std=c++11 -Iinclude`, and must build and run correctly on a CS Lab Unix machine.

## Running the Simulation

The executable takes the path to a configuration file as its only argument:

```bash
./bin/simulation config.txt
```

On startup, the program parses the config file, initializes settlements/facilities/plans, and prints:

```
The simulation has started
```

It then waits for user commands in a loop, executing one action per line until a `close` command is issued.

## Configuration File Format

Each line in the config file describes one entity, in the following order:

**1. Settlements**

```
settlement <settlement_name> <settlement_type>
```

`<settlement_type>` is `0` (Village), `1` (City), or `2` (Metropolis).

```
settlement Kfar_SPL 0     // Village
settlement Kiryat_SPL 2   // Metropolis
```

**2. Facilities**

```
facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
```

`<category>` is `0` (Life Quality), `1` (Economy), or `2` (Environment/Sustainability).

```
facility kindergarten 0 3 3 2 1
facility desalinationPlant 2 4 2 2 3
```

**3. Initial plans**

```
plan <settlement_name> <selection_policy>
```

```
plan Kiryat_SPL bal
plan Kfar_SPL eco
```

Settlements must be parsed before plans, since plan creation depends on existing settlements.

## Core Classes

| Class | Responsibility |
|---|---|
| `Simulation` | Owns settlements, plans, facility catalog, and action history; drives the simulation loop |
| `Settlement` | Represents a settlement (name + type/construction limit) |
| `FacilityType` | General facility blueprint: name, price, category, and score contributions |
| `Facility` | A concrete facility instance tied to a settlement, with a construction status |
| `Plan` | A reconstruction plan for one settlement — tracks operational and under-construction facilities, current scores, and its selection policy |
| `SelectionPolicy` | Abstract strategy for choosing the next facility to build |
| `BaseAction` | Abstract base for all user actions, with `act()`, `toString()`, and status tracking (`COMPLETED` / `ERROR`) |

## Selection Policies

Each plan chooses its next facility using one of four strategies:

| Code | Policy | Behavior |
|---|---|---|
| `nve` | Naïve | Cycles through the facility list in order |
| `bal` | Balanced | Picks the facility that minimizes the gap between the plan's highest and lowest score after construction |
| `eco` | Economy | Prioritizes facilities in the Economy category |
| `env` | Sustainability | Prioritizes facilities in the Environment category |

In case of a tie, the facility with the lower index is chosen. Changing a plan's policy resets `lastSelectedIndex`, except when switching to the Balanced policy, which reinitializes it from current plan scores.

## Actions

All actions are entered as plain text commands during the simulation loop.

| Command | Syntax | Description |
|---|---|---|
| Simulate step(s) | `step <n>` | Advances the simulation by `n` time units |
| Add plan | `plan <settlement_name> <selection_policy>` | Creates a new plan |
| Add settlement | `settlement <settlement_name> <settlement_type>` | Creates a new settlement |
| Add facility | `facility <name> <category> <price> <lifeq> <eco> <env>` | Registers a new facility type |
| Print plan status | `planStatus <plan_id>` | Prints a plan's status, scores, and facilities |
| Change plan policy | `changePolicy <plan_id> <selection_policy>` | Updates a plan's selection policy |
| Print action log | `log` | Prints the full history of executed actions and their outcomes |
| Backup | `backup` | Snapshots the current simulation state (overwrites any previous backup) |
| Restore | `restore` | Restores the last backed-up snapshot |
| Close | `close` | Prints final results for all plans, frees all memory, and ends the program |

Each action reports `COMPLETED` or `ERROR: <message>` and is recorded in the action log (except `log` itself).

## Memory Management

This project makes heavy use of C++'s **Rule of Five** for any class that manages resources (e.g., dynamically allocated pointers): copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor. Classes without owned resources intentionally omit these to avoid unnecessary boilerplate.

All heap-allocated memory is released by the time the `close` action terminates the program — no leaks, no dangling pointers.

## Testing with Valgrind

The project is validated for memory correctness using Valgrind:

```bash
valgrind --leak-check=full --show-reachable=yes bin/simulation config.txt
```

Expected output:

```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Team

- Renad Abu Shareb
- Adan Abo Salok

---

*Developed as part of the Systems Programming course, Fall 2024. Built and tested on a CS Lab Unix environment.*