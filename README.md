# Process Scheduling Simulator

A Linux C program implementing First Come First Served (FCFS)
and non-preemptive Shortest Job First (SJF).

## Compile

gcc -std=c11 -Wall -Wextra -Wpedantic scheduler.c -o scheduler

## Run

./scheduler

Enter the number of processes, then each process's arrival time
and burst time as two space-separated integers.

Example input:

4
0 8
1 4
2 9
3 5

## Algorithms

FCFS executes processes in arrival-time order.
Equal arrival times are resolved by smaller PID.

SJF selects the shortest burst among processes that have arrived.
Equal bursts are resolved by earlier arrival, then smaller PID.
A running process completes before another process is selected.

Both algorithms display CPU idle intervals when no process is ready.

## Metrics

Turnaround time = completion time - arrival time.
Waiting time = turnaround time - burst time.
Response time = first execution time - arrival time.

Waiting and response times are equal for these non-preemptive algorithms.

The program displays a Gantt chart, per-process metrics, and
average waiting, turnaround, and response times.

## Sample Results

FCFS: P1 (0-8), P2 (8-12), P3 (12-21), P4 (21-26).
Average waiting: 8.75.
Average turnaround: 15.25.
Average response: 8.75.

SJF: P1 (0-8), P2 (8-12), P4 (12-17), P3 (17-26).
Average waiting: 7.75.
Average turnaround: 14.25.
Average response: 7.75.

The assignment's supplied SJF Gantt order is inconsistent with
arrival times. Only P1 is available at time 0, so it must execute first.

## Input Limits

- Number of processes: 1 to 100.
- Arrival time: 0 to 1000000.
- Burst time: 1 to 1000000.
- Invalid input causes an error message and a nonzero exit status.

## Tests Performed

- Assignment sample: verified execution order and all metrics.
- Additional workload with different burst times: verified both algorithms.
- Initial idle time and a gap between processes: verified idle intervals.
- Equal arrival and burst times: verified deterministic tie handling.
- A short job arriving during execution: verified non-preemptive behavior.
- Zero burst time: verified rejection and exit status 1.
