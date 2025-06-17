  

<h1 align="center">🍝 C-Project-Philosophers</h1>
<p align="center"><i>My seventh project in 42. . . sometimes I wonder if this is the programming trenches ( ;´ - `;)</i></p>
<p align="center">
    <img src="https://img.shields.io/badge/Language-C-blue.svg" />
    <img src="https://img.shields.io/github/languages/top/HajerZam/C-Project-Philosophers?style=flat-square" />
    <img src="https://img.shields.io/github/repo-size/HajerZam/C-Project-Philosophers?style=flat-square" />
    <img src="https://img.shields.io/github/last-commit/HajerZam/C-Project-Philosophers?style=flat-square" />
    <img src="https://img.shields.io/github/issues/HajerZam/C-Project-Philosophers?style=flat-square" />
    <img src="https://img.shields.io/badge/42-Common%20Core-critical" />
    <img src="https://img.shields.io/badge/score-100-brightgreen?style=flat-square">
</p>

---

## 🧠 What’s the Problem?

the infamous **Dining Philosophers Problem** — a classic in concurrent programming:

> Five philosophers sit around a table. Each needs **two forks** to eat. But forks are **shared between neighbors**. They alternate between **thinking**, **eating**, and **sleeping**.

Your task is to simulate this scenario using **multithreading**, avoiding:

- 🔁 **Deadlocks** (e.g., everyone grabs one fork and waits forever)
- ⚔️ **Race conditions** (two threads accessing shared data unsafely)
- 😵 **Starvation** (a philosopher never gets to eat)

---

## 🧾 Project Rules

| Requirement | Description |
|------------|-------------|
| Threads | One thread per philosopher |
| Shared Resources | Forks = Mutexes |
| Timing | Precise delays for eating, sleeping, dying |
| Input | CLI args:  
`./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_required]` |
| Output | Timestamped logs of actions |
| Death | Detect when a philosopher hasn't eaten in time |

---

## 🔨 Key Concepts

| Concept | Purpose |
|--------|---------|
| `pthread_create` | Create threads (one per philosopher) |
| `pthread_mutex_t` | Lock/unlock forks (shared resources) |
| `gettimeofday()` | Track precise time for actions |
| `usleep()` | Sleep during eating and sleeping |
| `pthread_join` / `detach` | Manage thread lifecycle |
| `malloc` / `free` | Dynamic memory for threads, forks, etc. |

---

## 📁 Project Structure

```c
philosophers/
├── main.c
├── init.c           // Data + thread setup
├── philo.c          // Philosopher behavior
├── monitor.c        // Death checker
├── utils.c          // Time & print helpers
├── Makefile
├── philo.h          // Structs & headers
````

---

## 🧬 Core Structs

### `t_philo` (per philosopher)

```c
typedef struct s_philo {
    int             id;
    int             meals_eaten;
    long long       last_meal;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_t       thread;
    struct s_data   *data;
} t_philo;
```

### `t_data` (global config)

```c
typedef struct s_data {
    int             num_philos;
    long long       time_to_die;
    long long       time_to_eat;
    long long       time_to_sleep;
    int             must_eat;
    int             dead;
    long long       start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    t_philo         *philos;
} t_data;
```

---

## 🧩 Execution Flow

Each philosopher thread loops:

1. **Think**
2. **Lock left fork**
3. **Lock right fork**
4. **Eat**
5. **Unlock both forks**
6. **Sleep**
7. Repeat...

Meanwhile, a **monitor thread** checks:

* Did anyone exceed `time_to_die`?
* Did all philosophers eat `must_eat` times?

---

## ⚙️ Function Cheat Sheet

| Function                | Use                         |
| ----------------------- | --------------------------- |
| `pthread_mutex_init`    | Initialize forks (mutexes)  |
| `pthread_create`        | Spawn philosopher threads   |
| `pthread_mutex_lock`    | Lock a fork                 |
| `gettimeofday()`        | Get current timestamp in ms |
| `usleep()`              | Sleep precisely             |
| `pthread_join`          | Wait for threads to finish  |
| `pthread_mutex_destroy` | Clean up at end             |

---

## 📋 Step-by-Step Plan

### 1. **Parse & Validate Input**

* Ensure all arguments are positive integers.
* Handle optional `must_eat` argument.

### 2. **Initialize Structs**

* Create `num_philos` mutexes for forks.
* Allocate `t_philo` array and link left/right forks.

### 3. **Create Threads**

* Launch `philo_routine(void *arg)` for each philosopher.

### 4. **Philosopher Routine**

* While no one is dead:

  * Lock left + right forks
  * Print action → "is eating"
  * `usleep(time_to_eat)`
  * Unlock forks
  * Print → "is sleeping"
  * `usleep(time_to_sleep)`
  * Print → "is thinking"

### 5. **Monitor Thread**

* Constantly check:

  * `current_time - last_meal >= time_to_die`
  * If all `meals_eaten >= must_eat`

### 6. **Print Output Safely**

* Use `print_mutex` so logs don’t overlap.
* Format:

```bash
[timestamp] [philo_id] is eating
```

### 7. **Clean Up**

* Destroy all mutexes
* Free malloc’d memory

---

## 🧪 Testing Checklist

* [x] 1 philosopher (should die alone 🪦)
* [x] 5 philosophers, default case
* [x] Even/odd numbers
* [x] Long sleeps to test starvation
* [x] Meals count limit
* [x] Valgrind = 0 leaks ✅

---

## 🚧 Common Pitfalls

| ❌ Problem          | ✅ Solution                                  |
| ------------------ | ------------------------------------------- |
| Deadlocks          | Use odd/even fork order strategy            |
| Race conditions    | Protect shared values with mutexes          |
| Imprecise sleeping | Use `gettimeofday` + custom `smart_sleep()` |
| Unlocked mutexes   | Always unlock after eating, even on error   |

---

## 🔍 Resources

* [Deadlock Explained (ScienceDirect)](https://www.sciencedirect.com/topics/computer-science/deadlock)
* [GFG - Mutex vs Semaphore](https://www.geeksforgeeks.org/mutex-vs-semaphore/)
* [YouTube - Dining Philosophers in C](https://www.youtube.com/watch?v=RGQe8waGJ4w)

---

## 🤯 Final Thoughts

This project will:

* Warp your brain with mutexes.
* Force you to **think in threads**.
* Test your patience with timing.
* Teach you how to **build robust concurrency logic** from scratch.

> Sometimes I wonder if the true philosopher is me — sitting at my desk, starving, debugging mutex deadlocks at 3am.

---

<p align="center"><i>May all your philosophers eat, and none die in vain.</i> 🥄</p>

---
