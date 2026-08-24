*This project has been created as part of the 42 curriculum by haryandr.*

# Codexion

*Master the race for resources before the deadline masters you*

## Description

### Presentation

Codexion is a multithreaded simulation based on the classic dining philosophers problem. One or more **coders** sit around a co-working hub and repeatedly **compile**, **debug**, and **refactor**. To compile, a coder must hold two **USB dongles** at the same time (left and right). There are exactly as many dongles as coders.

### Goal

The goal of this project is to build a solid multithreaded simulation in C, using POSIX threads (`pthread`), mutexes, condition variables, and precise timing.

The main learning objectives are:

- Sharing resources (the USB dongles) between threads
- Synchronizing threads correctly
- Scheduling policies (`fifo` and `edf`)
- Avoiding deadlocks and starvation
- Handling timing and state monitoring properly

The simulation has to run correctly under concurrent access, never deadlock, never starve a coder (when parameters are feasible), and correctly detect burnout.

### Overview

The project is built around four (04) main structures:

👤 **Coders:**
- Each is represented by a POSIX thread.
- They sit in a circle.
- Each coder has a dongle on his left.
- To compile, a coder must acquire two dongles, his own and his right neighbor.
- After compiling, the coder releases the dongles, then debugs and refactors before trying to compile again.
- If only one coder is present, there is only one dongle; the coder cannot compile and will eventually burn out.

🔌 **Dongles:**
- Shared resources protected by their own mutex and condition variable.
- After being released, a dongle becomes hot and stays unavailable for `dongle_cooldown` milliseconds.
- A dedicated dongle thread is responsible for the cooldown period.
- Access is arbitrated by a small priority queue (size <= 2) according to the chosen scheduler (`fifo` or `edf`).

🕵️‍♂️ **Monitor:**
- A global thread that watches every coder to detect **burnout**.  
- If a coder has not started a new compile within `time_to_burnout` milliseconds of their last compile (or the start of the simulation), he burns out and the simulation stops.
- The simulation stops successfully once every coder has compiled at least `number_of_compiles_required` times.

💻 **Simulation:**
- Holds all shared states: arrays of coders and dongles, arguments, start timestamp, running flag, end status, and the logging mutex.
- Provides the global condition variable used to wake threads when the simulation ends.

## Instructions

### Installation

Clone the repository.

```bash
git clone https://github.com/harylax/codexion.git
```

Enter the codexion/coders/ folder before compilation.

```bash
cd codexion/coders/
```

### Compilation

```bash
make
```

This builds the `codexion` binary using `cc` with `-Wall -Wextra -Werror
-pthread`.

Other available rules:

```bash
make all     # builds the binary
make clean   # remove object files
make fclean  # remove object files and the binary
make re      # rebuild from scratch
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max time (ms) a coder can go without starting a compile |
| `time_to_compile` | Time (ms) spent compiling |
| `time_to_debug` | Time (ms) spent debugging |
| `time_to_refactor` | Time (ms) spent refactoring |
| `number_of_compiles_required` | Compiles needed per coder to end the simulation successfully |
| `dongle_cooldown` | Time (ms) a dongle stays unavailable after being released |
| `scheduler` | `fifo` or `edf` — how dongles arbitrate between competing requests |

#### Example:

##### Input:

```bash
./codexion 3 800 200 100 100 2 100 fifo
```
##### Output:

```bash
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
1 2 has taken a dongle
200 1 is debugging
210 1 is refactoring
300 2 has taken a dongle
300 2 is compiling
500 2 is debugging
510 2 is refactoring
600 3 has taken a dongle
600 3 has taken a dongle
600 3 is compiling
800 3 is debugging
800 1 burned out
```


## Resources

- POSIX Threads basics: https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/
- POSIX Threads Programming (LLNL tutorial): https://hpc-tutorials.llnl.gov/posix/
- `man pthreads`, `man pthread_create`, `man pthread_join`, `man gettimeofday`

**AI usage:**
AI was used as a learning aid to quickly understand the concurrency concepts involved in the project (threads, mutexes, condition variables, deadlocks, starvation, Coffman conditions, and scheduling policies).

## Blocking cases handled

- **Deadlock prevention:**
Coders acquire dongles in different orders according to the parity of their id: odd coders take left then right, even coders take right then left. This breaks the circular-wait condition of Coffman’s four conditions.
- **Starvation prevention:**
Each dongle maintains a small priority queue of pending requests and grants access according to the selected scheduler (`fifo` or `edf`). As long as the given parameters are feasible, no coder is starved indefinitely.
- **Dongle cooldown:**
A dedicated dongle thread is responsible for keeping a dongle unavailable for exactly `dongle_cooldown` ms after it has been released.
- **Burnout detection:**
A separate monitor thread checks coder states every millisecond and stops the simulation. The "burned out" message is printed within 10 ms of the actual deadline.
- **Log serialization:**
All log lines are printed while holding a dedicated logging mutex (sim->log_mutex), guaranteeing that two messages never interleave on the same line.

## Thread synchronization mechanisms

- `pthread_mutex_t dongle->mutex`
Protects all per-dongle state: `available`, `hot`, and the priority queue (`t_heap`). Every read or write of these fields is performed under this lock.
- `pthread_cond_t dongle->cond`
Used by coders waiting for a dongle to become available and to be first in the queue, and by the dongle thread waiting for the dongle to become hot. Any state change that may unblock a waiter is followed by `pthread_cond_broadcast`.
- `pthread_mutex_t sim->mutex`
Protects global simulation state: `running`, coder states (`WORKING` / `DONE` / `BURNED_OUT`), `last_compile_start`, `compilations_done`, and the end status.
- `pthread_cond_t sim->cond`
Used to wake all threads when the simulation is stopped (burnout or success).
- `pthread_mutex_t sim->log_mutex`
Protects `printf` calls only, so that log lines never get mixed.

### Race conditions are avoided because:

- Dongle-specific fields are always accessed under the corresponding `dongle->mutex`.
- Global simulation fields and coder states are always accessed under `sim->mutex`.
- The monitor thread only reads coder state under `sim->mutex`, so it never observes a partially updated state.
