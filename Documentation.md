# RTOS Documentation

## 1. System Architecture

This is a Preemptive Priority-Based RTOS with Rate Monotonic Scheduling (RMS) support.

- **Scheduler**: Bitmap-based O(1) priority lookup.
- **Priorities**: 4 Levels (0 lowest, 3 highest). *Note: Configurable.*
- **Task Control**: TCBs, Linked List Queues.
- **IPC**: Message Queues, Semaphores, Mutexes (with Priority Inheritance).

## 2. Timing Budget & WCET (Worst-Case Execution Time)

The following WCET values are **assumed** based on code complexity analysis (Big-O Notation) and baseline measurements (Context Switch ~8µs @ 4MHz).

| Primitive | Complexity | Assumed WCET (µs) | Description |
| :--- | :---: | :---: | :--- |
| **Context Switch** | O(1) | **10 µs** | Hardware save/restore (R4-R11) + Stack updates. Measured ~8µs. |
| **Scheduler (Get Task)** | O(1) | **15 µs** | Bitmap CLZ instruction (1 cycle) + Linked List Dequeue. No searching required. |
| **Scheduler (Add Task)** | O(1) | **10 µs** | Linked List Enqueue + Bitmap Set Bit. |
| **Semaphore Up (Signal)** | O(1) | **25 µs** | Checks waiting queue. If empty: ++. If task waiting: Dequeue + Schedule + Possible Preempt (Switch). |
| **Semaphore Down (Wait)** | O(1) | **25 µs** | Checks count. If >0: --. If 0: Enqueue + Context Switch. |
| **Msg Queue Send** | O(1) | **40 µs** | Semaphore Down (Lock) + Copy Pointer + Semaphore Up (Lock) + Semaphore Up (Signal). |
| **Msg Queue Receive** | O(1) | **40 µs** | Semaphore Down (Wait) + Semaphore Down (Lock) + Copy Pointer + Semaphore Up (Lock). |
| **Mutex Lock** | O(N) | **30 µs** | Includes Priority Inheritance logic (traversing blocked list) to prevent Priority Inversion. |
| **Mutex Unlock** | O(1) | **25 µs** | Restore priority logic + Scheduler update. |
| **RTOS Wait (Period)** | O(1) | **30 µs** | `rtos_task_wait`. Calculated next release + Context Switch. |
| **RTOS Yield** | O(1) | **35 µs** | `rtos_task_yield`. Re-enqueue current + Schedule + Context Switch. |
| **RTOS Delay** | O(1) | **40 µs** | `rtos_task_delay`. Time calc + Dequeue Prio + Sleep Queue + Switch. |
| **Interrupt Latency** | N/A | **< 50 µs** | Max time interrupts are disabled by OS critical sections. |

## 3. Deployment & Timing Analysis

### 3.1 Timing Table (Current Application Config)

Based on `main.c`:

| Task Name | Function | Period (ms) | Execution Budget (ms) | Priority (RMS) |
| :--- | :--- | :---: | :---: | :---: |
| **T1_5ms** | `task_periodic_5ms` | **5 ms** | 1 ms | High (3) |
| **T2_20ms** | `task_periodic_20ms` | **20 ms** | 2 ms | Medium (2) |
| **Logger** | `task_logger` | **100 ms** | 20 ms | Low (1) |
| **Idle** | `os_idle_thread` | **N/A** | N/A | Lowest (0) |

*Note: Priorities are assigned automatically by the RMS Scheduler (shorter period = higher priority).*

### 3.2 Latency Data

Measurements taken on STM32U5 @ 4MHz (Simulated/Calculated):

- **Context Switch Time**: **~8 µs**
  - *Measurement Method*: `DWT->CYCCNT` difference between `pend_sv` entry and exit minus overhead.
- **Max Interrupt Latency**: **~45 µs**
  - *Caused by*: Longest critical section (`__disable_irq`) in `scheduler_release_tasks` or `msg_queue_send`.

### 3.3 Response Time vs. Load (Theoretical)

For a Rate Monotonic Scheduler, the theoretical utilization bound is **~69%** (ln(2)).

**Response Time Behavior:**

1. **Low Load (<50%)**: Response time is constant and effectively equals `Execution Time + Context Switch`.
2. **Medium Load (50-69%)**: Low priority tasks (Logger) usually have higher jitter but still meet deadlines.
3. **Overload (>69%)**: According to RMS theory, the lowest priority task (`Logger`) will start missing deadlines first as the CPU saturates.

**Visual Representation:**

```text
Response Time
  ^
  |                  / (Exponential intent for low prio)
  |                 /
  |         _______/
  |________/
  +--------------------> CPU Load
  0%      60%   100%
```

## 4. Scheduling Policy

- **Algorithm**: Fixed-Priority Preemptive.
- **RMS Support**: `rms_scheduler.c` assigns priorities based on period (Shorter Period = Higher Priority).
- **Budgeting**: Tasks have `execution_time` budgets monitored by `should_switch()`.

## 5. Design Optimizations

The system minimizes overhead and latency by:

1. Using **O(1)** algorithms for all ready-queue operations (Bitmap Scheduler).
2. Using **DMA** for UART to avoid blocking CPU for long I/O operations (e.g., logging).
3. Keeping Critical Sections (`__disable_irq`) extremely short (limited to linked-list pointer manipulations).
