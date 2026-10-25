#include <stdio.h>

#define MAX_PROCESSES 100
#define MAX_TIME 1000000

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

static void print_results(const Process p[], int n)
{
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    printf("\nPID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].waiting_time,
               p[i].turnaround_time,
               p[i].response_time);

        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
        total_response += p[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Response Time: %.2f\n", total_response / n);
}

void fcfs(const Process original[], int n)
{
    Process p[MAX_PROCESSES];
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        p[i] = original[i];
    }

    /* Sort by arrival time, then PID for equal arrivals. */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time ||
                (p[j].arrival_time == p[j + 1].arrival_time &&
                 p[j].pid > p[j + 1].pid)) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    printf("\n=== First Come First Served (FCFS) ===\n");
    printf("Gantt Chart:\n");

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            printf("| IDLE (%d-%d) ", current_time, p[i].arrival_time);
            current_time = p[i].arrival_time;
        }

        int start_time = current_time;
        current_time += p[i].burst_time;

        p[i].waiting_time = start_time - p[i].arrival_time;
        p[i].turnaround_time = current_time - p[i].arrival_time;
        p[i].response_time = p[i].waiting_time;

        printf("| P%d (%d-%d) ", p[i].pid, start_time, current_time);
    }

    printf("|\n");
    print_results(p, n);
}

void sjf(const Process original[], int n)
{
    Process p[MAX_PROCESSES];
    int completed[MAX_PROCESSES] = {0};
    int current_time = 0;
    int completed_count = 0;

    for (int i = 0; i < n; i++) {
        p[i] = original[i];
    }

    printf("\n=== Shortest Job First (SJF) ===\n");
    printf("Gantt Chart:\n");

    while (completed_count < n) {
        int selected = -1;

        /* Choose the shortest job among processes already available. */
        for (int i = 0; i < n; i++) {
            if (completed[i] || p[i].arrival_time > current_time) {
                continue;
            }

            if (selected == -1 ||
                p[i].burst_time < p[selected].burst_time ||
                (p[i].burst_time == p[selected].burst_time &&
                 p[i].arrival_time < p[selected].arrival_time) ||
                (p[i].burst_time == p[selected].burst_time &&
                 p[i].arrival_time == p[selected].arrival_time &&
                 p[i].pid < p[selected].pid)) {
                selected = i;
            }
        }

        if (selected == -1) {
            int next_arrival = MAX_TIME + 1;

            for (int i = 0; i < n; i++) {
                if (!completed[i] &&
                    p[i].arrival_time < next_arrival) {
                    next_arrival = p[i].arrival_time;
                }
            }

            printf("| IDLE (%d-%d) ", current_time, next_arrival);
            current_time = next_arrival;
            continue;
        }

        int start_time = current_time;
        current_time += p[selected].burst_time;

        p[selected].waiting_time =
            start_time - p[selected].arrival_time;
        p[selected].turnaround_time =
            current_time - p[selected].arrival_time;
        p[selected].response_time = p[selected].waiting_time;

        printf("| P%d (%d-%d) ",
               p[selected].pid, start_time, current_time);

        completed[selected] = 1;
        completed_count++;
    }

    printf("|\n");

    /* Display the table in PID order for easy comparison with FCFS. */
    print_results(p, n);
}

int main(void)
{
    Process processes[MAX_PROCESSES] = {0};
    int n;

    printf("Enter the number of processes: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Enter a process count from 1 to %d.\n",
                MAX_PROCESSES);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        printf("Enter the arrival time and burst time for process %d: ",
               processes[i].pid);

        if (scanf("%d %d",
                  &processes[i].arrival_time,
                  &processes[i].burst_time) != 2 ||
            processes[i].arrival_time < 0 ||
            processes[i].arrival_time > MAX_TIME ||
            processes[i].burst_time < 1 ||
            processes[i].burst_time > MAX_TIME) {
            fprintf(stderr,
                    "Arrival must be 0-%d and burst must be 1-%d.\n",
                    MAX_TIME, MAX_TIME);
            return 1;
        }
    }

    fcfs(processes, n);
    sjf(processes, n);

    return 0;
}
