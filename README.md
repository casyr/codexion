*This project has been created as part of the 42 curriculum by yriffard.*

# Codexion

## Description

Codexion is a multithreaded simulation inspired by the classic Dining Philosophers problem. A ring of coders shares USB dongles placed between neighboring seats. Each coder alternates through three phases: compiling, debugging, and refactoring. To compile, a coder must acquire both adjacent dongles at the same time.

The goal of the project is to implement a thread-safe scheduler that allows coders to progress fairly and efficiently while avoiding deadlocks, starvation, and burnout. The simulation must remain correct even under heavy contention and with strict timing constraints.

## Instructions

### Build

From the project root, run:

```bash
make
```

### Run

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

Example:

```bash
./codexion 4 40 2 0 4 2 2 edf
```

### Optional debugging

```bash
valgrind --tool=helgrind -s ./codexion <arguments>
```

## Blocking cases handled

This solution addresses the main concurrency hazards of the simulation:

- Deadlock prevention: coders do not acquire resources in a way that creates circular waiting.
- Starvation prevention: the scheduler is designed to give each coder fair access to the required dongles over time.
- Cooldown handling: resource release and reuse are coordinated so that a coder cannot immediately reclaim the same dongles in a harmful pattern.
- Burnout detection: each coder tracks waiting time and exits the simulation if the wait becomes too long, preventing endless blocking.
- Log serialization: log output is protected so that messages from multiple threads are printed consistently and without interleaving.

## Thread synchronization mechanisms

The implementation uses several synchronization primitives to coordinate access to shared resources:

- `pthread_mutex_t` protects shared state such as the dongles, monitor bookkeeping, and log output. This prevents race conditions when multiple coders read or modify the same variables.
- `pthread_cond_t` is used to coordinate thread startup and wake-up events. The monitor waits until all coder threads are created, then signals them so they begin their work together.
- A lightweight custom event mechanism is used to make thread readiness and phase transitions explicit without relying on busy waiting.

Typical synchronization flow:

- When a coder wants to acquire a dongle, it must lock the corresponding mutex before checking and updating the resource state.
- When the monitor determines that all threads are ready, it signals the waiting coders to start simultaneously.
- When a coder finishes a compile cycle, it releases the dongles and updates the shared state under the same mutex so that no other thread can observe partial or inconsistent information.

This guarantees thread-safe communication between coders and the monitor and prevents races around resource ownership, logging, and simulation state.

## Resources

- Visualizer: https://codexion-visualizer.sacha-dev.me/
- POSIX threads overview: `man pthreads`
- Mutex documentation: `man pthread_mutex_lock`, `man pthread_mutex_init`
- Condition variable documentation: `man pthread_cond_wait`, `man pthread_cond_broadcast`
- Thread introduction playlist: https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2

### AI usage

AI was used to:

- help structure the README and explain the synchronization design,
- review concurrency edge cases such as deadlock, starvation, and burnout handling,
- assist with debugging and documentation clarity.

It was used as a support tool for explanation and code review, not as a replacement for the project's core implementation.