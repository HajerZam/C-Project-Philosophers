# C-Project-Philosophers
my sevenths project in 42 . . . sometimes I wonder if this is the trenches of programming ( ;´ - `;)

### Philosophers, because no one bothered to ask the waiter for extra forks

## 1. Core Concept (Problem to Solve)

> It’s a classic concurrency problem: **Five philosophers sit around a table**, alternating between thinking and eating. They need **two forks (shared resources)** to eat, but **each fork is shared between neighbors**.
> 

The challenge ahead: **Avoid [deadlock](https://www.sciencedirect.com/topics/computer-science/deadlock#:~:text=Deadlock%20refers%20to%20a%20situation,resource%2C%20leading%20to%20a%20standstill.) and race conditions** while ensuring everyone gets to eat!

---

## 2. Key Requirements

- Create a **multithreaded program** using **mutexes** (and possibly **semaphores**).
- Ensure **no deadlocks**, **no starvation** (every philosopher must get a chance to eat).
- Each philosopher is a thread.
- Shared resources = forks (often represented as mutexes).
- Timing matters: You’ll simulate **thinking, eating, sleeping** using `usleep()` or similar.

---

## **3. Steps to Approach the Project**

**Step-by-step breakdown:**

1. **Parse arguments** (number of philosophers, time to die, eat, sleep, etc.)
2. **Initialize data structures**:
    - Philosopher structs with their own IDs, time tracking.
    - Mutexes for forks.
3. **Create a thread per philosopher.**
4. **Each thread (philosopher) loops through:**
    - Thinking
    - Picking up left fork (`pthread_mutex_lock`)
    - Picking up right fork
    - Eating (track timestamp)
    - Putting down both forks
    - Sleeping
5. **Monitor thread** (separate thread to check if any philosopher died).

---

### **4. Critical Concepts**

- **Mutex** (`pthread_mutex_t`) to protect shared resources.
- **Thread creation** (`pthread_create`) and management.
- **Race conditions** and **how to avoid them**.
- **Deadlocks**: All philosophers pick up one fork and wait forever—unless you’re an immortal you need to keep it in mind?
    - Use odd/even strategies.
    - Limit the number of philosophers who can pick up forks simultaneously.
- **Time handling**: precise timestamps with `gettimeofday` or `clock_gettime`.

---

## 5. New Functions Used

### **Memory & Output Functions**

### `memset`

> Sets a block of memory to a specific value (like zeroing a struct).
> 

```c
memset(ptr, 0, sizeof(struct));

```

**Use in project:** Initialize structs or arrays cleanly to zero.

---

### `printf`

> Prints formatted text to the terminal.
> 

```c
printf("%d has taken a fork\n", id);

```

**Use in project:** Output philosopher actions with timestamps.

---

### `malloc` / `free`

> Allocate and free dynamic memory.
> 

```c
philos = malloc(sizeof(t_philo) * num);
free(philos);

```

**Use in project:** Allocate space for philosophers and forks dynamically.

---

### `write`

> Lower-level function to write to a file descriptor.
> 

```c
write(1, "Hello\n", 6);  // 1 = stdout

```

**Use in project:** Not often directly, but you could use it instead of `printf` if required.

---

### **Timing Functions**

### `usleep`

> Sleeps for a given number of microseconds.
> 

```c
usleep(1000);  // sleeps for 1 ms

```

**Use in project:** Control when a philosopher is eating/sleeping.

---

### `gettimeofday`

> Gets the current time with microsecond precision.
> 

```c
struct timeval tv;
gettimeofday(&tv, NULL);
long timestamp = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

```

**Use in project:** Track philosopher death timing and action logs.

---

### **Threading Functions (The Core of Your Project)**

### `pthread_create`

> Starts a new thread.
> 

```c
pthread_create(&thread_id, NULL, routine_function, (void *)arg);

```

**Use in project:** Create a thread for each philosopher.

---

### `pthread_detach`

> Detaches a thread (cleans up automatically when it finishes).
> 

```c
pthread_detach(thread_id);

```

**Use in project:** Optional; used if you don’t need to `join` a thread later.

---

### `pthread_join`

> Waits for a thread to finish.
> 

```c
pthread_join(thread_id, NULL);

```

**Use in project:** Used in `main` to wait for philosopher threads.

---

### **Mutex (Mutual Exclusion) Functions**

### `pthread_mutex_init` / `pthread_mutex_destroy`

> Initialize and destroy a mutex (a lock).
> 

```c
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);
pthread_mutex_destroy(&mutex);

```

**Use in project:** One mutex per fork; plus maybe extra for print/death check.

---

### `pthread_mutex_lock` / `pthread_mutex_unlock`

> Lock and unlock a mutex.
> 

```c
pthread_mutex_lock(&fork);
pthread_mutex_unlock(&fork);

```

**Use in project:** Prevent two philosophers from grabbing the same fork.

---

## TL;DR SUMMARY:

| Function | Use Case in Project |
| --- | --- |
| `memset` | Zero out structs/arrays |
| `printf` | Log what philosophers are doing |
| `malloc`/`free` | Allocate/free philosophers, forks |
| `write` | Low-level output (rare) |
| `usleep` | Sleep for eat/sleep actions |
| `gettimeofday` | Track timing (for death check) |
| `pthread_create` | Start philosopher threads |
| `pthread_detach` | Auto-clean threads (optional) |
| `pthread_join` | Wait for philosopher threads |
| `pthread_mutex_*` | Manage access to forks/shared data |

---

## **STEP-BY-STEP PLAN: PHILOSOPHERS PROJECT**

---

### **🟣 STEP 1: Read and Understand the Rules**

**Goal**: Understand what the program needs to do and what constraints you're under.

- Read the project subject and README of other git repos carefully.
- Know the **arguments** the program takes:
    
    ```bash
    ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
    
    ```
    
- Understand **what causes a philosopher to die** (hasn't eaten in `time_to_die` ms).
- Read about **deadlocks and race conditions**.

---

### **🟣 STEP 2: Set Up the Basic Project Structure**

**Goal**: Get the project compiling and structured.

- Create your `Makefile`.
- Set up `.c` and `.h` files:
    - `main.c` — handles arguments and starts the sim
    - `init.c` — initializes structs and threads
    - `philo.c` — philosopher behavior
    - `utils.c` — time, printing, cleanup, etc.
- Define structs in `philo.h`:
    - `t_philo` (one per philosopher)
    - `t_data` (global simulation data)

---

### **🟣 STEP 3: Parse and Validate Arguments**

**Goal**: Safely read and store CLI arguments.

- Check if inputs are numbers and within range.
- Convert strings to integers (`ft_atoi` or your own).
- Store in `t_data` struct.

---

### **🟣 STEP 4: Initialize Data and Mutexes**

![image.png](attachment:23642d82-9d98-48d7-abd1-4775547d8f3e:image.png)

**Goal**: Set up forks, philosophers, and mutexes.

- Create `t_philo` array with one philosopher per thread.
- Create one **mutex per fork** (i.e., per philosopher).
- Assign:
    - Left fork = own index
    - Right fork = (index + 1) % N
- Also initialize:
    - A mutex for printing
    - A mutex for accessing the death flag or eating checks

---

### **🟣 STEP 5: Create Threads**

**Goal**: Launch a thread for each philosopher.

- `pthread_create` → start `philo_routine(void *arg)` for each.
- Inside the routine:
    - Think
    - Take forks (`pthread_mutex_lock`)
    - Eat (`usleep`)
    - Release forks
    - Sleep
    - Repeat
- Use `pthread_join` in `main.c` to wait for all threads.

---

### **🟣 STEP 6: Add Timing and Death Check**

**Goal**: Track time and detect when a philosopher dies.

- Store `last_meal_time` in each philosopher.
- Create a **monitor thread**:
    - Loops checking time since each philosopher ate
    - If time exceeds `time_to_die`, set `dead = true` and exit
- Use `gettimeofday()` or `clock_gettime()` to track timestamps (in milliseconds).
- Protect shared data with mutexes.

---

### **🟣 STEP 7: Handle Edge Cases and Eating Count**

**Goal**: Handle the optional argument and clean exit.

- Optional 5th argument: stop simulation once every philosopher eats `N` times.
- Track each philosopher’s meal count.
- Monitor thread also checks if all have eaten enough.
- Handle:
    - Only 1 philosopher (special case — no eating possible, just dies).
    - Even/odd number of philosophers to avoid deadlocks.
    - Sleep accurately (use custom `smart_sleep()` for busy-wait).

---

### **🟣 STEP 8: Protect Output and Clean Up**

**Goal**: Make output readable and free memory.

- Use a mutex around `printf` calls to avoid scrambled text.
- Clean up all:
    - Threads (`pthread_join`)
    - Mutexes (`pthread_mutex_destroy`)
    - Allocated memory

---

### **FINAL STEP: Test Like Crazy**

- Test with:
    - 1 philosopher (should die)
    - 5 philosophers
    - Different time intervals
    - Optional argument
- Use valgrind to check for leaks:
    
    ```bash
    valgrind ./philo 5 800 200 200
    
    ```
    

---
