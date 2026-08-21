*This project has been created as part of the 42 curriculum by haryandr.*

# Codexion

*Master the race for resources befor the deadline masters you*

## Description

### Presentation

Codexion is a multithreaded simulation based on the classic dining philosophers problem. One or more **coders** sit around a co-working hub and take turns **compiling**, **debugging**, and **refactoring**. To compile, a coder needs two **USB dongles** (one on the left, one on the right), and there are exactly as many dongles as coders.

### Goal

The goal of this project is to build a solid multithreaded simulation in C, using POSIX threads (`pthread`), mutexes, condition variables, and precise timing.

The point of the project is to learn concurrent programming, mainly:

- Sharing resources (the USB dongles) between threads
- Synchronizing threads correctly
- Scheduling policies (`fifo` and `edf`)
- Avoiding deadlocks and starvation
- Handling timing and state monitoring properly

The simulation has to run correctly under concurrent access, never deadlock, never starve a coder, and correctly detect burnout.

### Overview

The project is built around three (03) main structures:

👤 **Coders:**
- Each is represented by a POSIX thread that shares the same setup.
- The same routine is compiling with two dongles, debugging and refactoring in loop. 
- They sit around a circular working table.
- Each must use two dongles to compile, the one on their left, and the other on their right.
- A coder shares the dongle of their right neighbor.

🔌 **Dongles:**
- Each dongle and other shared resources such as states are protected by a global mutex and global condition variable.
- Simultaneous access to critical ressources leads to data race.
- Each dongle also has a thread with the same setup.
- The thread waits for the dongle to be released after a compile, then makes it cool down.
- The access to a dongle follows a priority queue, either `fifo` (first arrival) or `edf` (closest to deadline).

🕵️‍♂️ **Monitor:**
- A global thread that watches every coder to detect **burnout**.  
- If a coder does not start compiling again within `time_to_burnout` milliseconds of their last compile, he burns out and the simulation stops.
- The simulation stops successfully once every coder has compiled at least `number_of_compiles_required` times.

## Instructions

### Installation

Clone the repository.

```bash
git clone https://github.com/harylax/codexion.git
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
- `man pthreads`, `man pthread_create`, `man pthread_join`

**AI usage:** to quickly get a grasp of the concepts involved in the project.

## Blocking cases handled

- **Deadlock prevention:** coders pick up their dongles in a different order depending on parity of their id. Odd coders take left-then-right and even coders take right-then-left. This breaks the circular wait condition from Coffman's four conditions.
- **Starvation prevention:** each dongle keeps a small priority queue of requests and grants access according to the chosen `scheduler` (`fifo` or `edf`). So a coder can never be starved indefinitely as long as the parameters given make sense.
- **Dongle cooldown:** a dedicated dongle thread makes sure a dongle stays unavailable for `dongle_cooldown` ms after being released, before it can be taken again.
- **Burnout detection:** a separate monitor thread checks coder states regularly and stops the simulation within 10 ms of the actual burnout deadline.
- **Log serialization:** all log lines are printed while holding a dedicated logging mutex, so two messages can never interleave on the same line.

## Thread synchronization mechanisms

- `pthread_mutex_t sim->mutex` protects all shared simulation state: coder states, dongle availability/heat, and the per-dongle request queues. Every read or write of this state happens under the lock.
- `pthread_cond_t sim->cond` is used to avoid busy-waiting: coders wait on it while a dongle they need is unavailable or not yet their turn, and dongle threads wait on it while cooling down. Any state change that could unblock another thread (a dongle becoming available, a coder finishing a compile, the simulation stopping) is followed by `pthread_cond_broadcast`.
- A separate `pthread_mutex_t sim->log_mutex` protects `printf` calls only, so that logging never has to be serialized through the same lock as the simulation logic (avoiding unnecessary contention).
- Race conditions are avoided because every access to shared fields (`coder->state`, `dongle->available`, `dongle->hot`, the request heap) is always performed under `sim->mutex`; the monitor thread reads coder states the same way, so it can never observe a half-updated state.
