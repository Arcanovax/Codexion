*This project has been created as part of the 42 curriculum by mthetcha.*

# Description

Codexion is a C simulation in which multiple threads (“coders”) share limited resources called dongles. To compile, a coder must obtain two dongles, which requires strict synchronization of the threads using a monitor and mutexes.

The program implements two priority policies (FIFO or EDF)
- FIFO (First In, First Out):  the dongle is granted to the coder whose request arrived first.
- EDF (Earliest Deadline First): Gives priority to those whose burnout is imminent.

These arbitration policies must manage fair access to resources and prevent coder burnout. The simulation ends when a coder burns out or when all have completed the required number of compilations.

# Instructions

The program must be compiled using `make`

## Running
```
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

- **number_of_coders**: The number of coders and also the number of dongles.
- **time_to_burnout** (in milliseconds): If a coder did not start compiling within
time_to_burnout milliseconds since the beginning of their last compile or the
beginning of the simulation, they burn out.
- **time_to_compile** (in milliseconds): The time it takes for a coder to compile.
During that time, they must hold two dongles.
- **time_to_debug** (in milliseconds): The time a coder will spend debugging.
- **time_to_refactor** (in milliseconds): The time a coder will spend refactoring.
After completing the refactoring phase, the coder will immediately attempt to
acquire dongles and start compiling again.
- **number_of_compiles_required**: If all coders have compiled at least this
many times, the simulation stops. Otherwise, it stops when a coder burns
out.
- **dongle_cooldown** (in milliseconds): After being released, a dongle is unavailable until its cooldown has passed.
- **scheduler**: The arbitration policy used by dongles to decide who gets them
when multiple coders request them.

### Example
```
./codexion 3 300 10 10 10 2 10 fifo
```

### Functioning

- Each coder runs in a separate thread.
- Dongles are shared and protected by mutexes.
- Access to resources follows the selected scheduling policy.
- A monitor thread detects burnouts in real time and track the number of compiles of each coders.
- Logs are timestamped and are protected with mutexes.


# Blocking cases handled

- Coders with odd IDs are delayed slightly to ensure a synchronized start.
- A queuing system is also in place to prevent race conditions or excessive priority access by a thread.
- A monitor thread tracks all the coders' statistics and stops the simulation when it detects a burnout, a crash, or if all the coders have completed the required number of compilations.
-`printf` are made safe using mutexes to prevent race conditions.

# Thread synchronization mechanisms

**Synchronized start:**
A `wait_go` function has been implemented to let all coders know when to start.

**Mutex for Dongles (pthread_mutex_t):**
Each dongle is protected by a mutex to ensure that only one coder can use it at a time. Before compiling, a thread locks both of its dongles (left and right).

**Mutex per coder (internal state):**
Every coder structure has its own mutex to protect its shared variables . This prevents race conditions between the coder thread and the monitor thread, which might read or modify this information simultaneously.

**Queue and arbitration (FIFO / EDF):**
Access to the dongles is regulated by a global priority queue. The ` has_priority ` function ensures that a coder can only attempt to use the resources if its turn is valid according to the chosen policy (first-come, first-served or nearest deadline).

**Prevention of Race Conditions:**
Access to shared structures (queue, states, resources) is always protected by appropriate locks


# Resources

- **Online documentations** : General research on how threads work
- **Peer-to-peer learning** : Code reviews and discussions
- **Codexion Visualizer** : Visualize Coders and timings
  https://codexion-visualizer.sacha-dev.me/
- **README Markdown** :
  https://docs.github.com/fr/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
