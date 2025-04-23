#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
using namespace std;

struct Process {
    int pid, at, bt, priority, wt, tat, ct, rt, remaining_bt;
    int temp_priority;
};

void sortByArrival(vector<Process>& p, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                swap(p[j], p[j + 1]);
            }
        }
    }
}

void fcfs(vector<Process>& p, int n) {
    sortByArrival(p, n);
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < p[i].at)
            t = p[i].at;
        p[i].ct = t + p[i].bt;
        t = p[i].ct;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
}

void sjf(vector<Process>& p, int n) {
    sortByArrival(p, n);
    vector<bool> completed(n, false);
    int t = 0, completedCount = 0;
    while (completedCount < n) {
        int minIndex = -1, minBT = 999;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && p[i].at <= t && p[i].bt < minBT) {
                minBT = p[i].bt;
                minIndex = i;
            }
        }
        if (minIndex == -1) {
            t++;
        } else {
            t += p[minIndex].bt;
            p[minIndex].ct = t;
            p[minIndex].tat = p[minIndex].ct - p[minIndex].at;
            p[minIndex].wt = p[minIndex].tat - p[minIndex].bt;
            completed[minIndex] = true;
            completedCount++;
        }
    }
}

void roundRobin(vector<Process>& p, int n, int quantum) {
    int t = 0, completedCount = 0;
    vector<bool> completed(n, false);
    vector<bool> started(n, false);
    vector<bool> in_q(n, false);
    queue<int> q;
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
    }
    while (completedCount < n) {
        for (int i = 0; i < n; i++) {
            if (!in_q[i] && !completed[i] && p[i].at <= t) {
                q.push(i);
                in_q[i] = true;
            }
        }
        if (q.empty()) {
            t++;
            continue;
        }
        int idx = q.front();
        q.pop();
        if (!started[idx]) {
            p[idx].rt = t - p[idx].at;
            started[idx] = true;
        }
        int exec = min(quantum, p[idx].remaining_bt);
        p[idx].remaining_bt -= exec;
        t += exec;
        for (int i = 0; i < n; i++)
            if (p[i].at <= t && !in_q[i] && !completed[i]) {
                q.push(i);
                in_q[i] = true;
            }
        if (p[idx].remaining_bt > 0) q.push(idx);
        else {
            p[idx].ct = t;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed[idx] = true;
            completedCount++;
        }
    }
}

// Non-Preemptive Priority Scheduling Algorithm
void priorityScheduling(vector<Process>& p, int n) {
    sortByArrival(p, n);
    vector<bool> completed(n, false);
    int t = 0, completedCount = 0;
    while (completedCount < n) {
        int minIndex = -1, minPriority = 999;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && p[i].at <= t && p[i].priority < minPriority) {
                minPriority = p[i].priority;
                minIndex = i;
            }
        }
        if (minIndex == -1) {
            t++;
        } else {
            t += p[minIndex].bt;
            p[minIndex].ct = t;
            p[minIndex].tat = p[minIndex].ct - p[minIndex].at;
            p[minIndex].wt = p[minIndex].tat - p[minIndex].bt;
            completed[minIndex] = true;
            completedCount++;
        }
    }
}


// Preemptive Priority Scheduling Algorithm
void preemptivePriority(vector<Process>& p, int n) {
    sortByArrival(p, n);
    int time = 0, completed = 0;
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
        p[i].temp_priority = p[i].priority;
    }

    const int MIN = -9999;
    while (completed < n) {
        int maxPriority = MIN;
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining_bt > 0 && p[i].temp_priority > maxPriority) {
                maxPriority = p[i].temp_priority;
                idx = i;
            }
        }
        if (idx != -1) {
            p[idx].remaining_bt--;
            time++;
            if (p[idx].remaining_bt == 0) {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                p[idx].temp_priority = MIN;
                completed++;
            }
        } else {
            time++;
        }
    }
}

void displayResults(const vector<Process>& p, int n) {
    float totalWT = 0, totalTAT = 0;
    cout << "\nPID\tAT\tBT\tPriority\tWT\tTAT\tCT\n";
    for (int i = 0; i < n; i++) {
        totalWT += p[i].wt;
        totalTAT += p[i].tat;
        cout << p[i].pid << "\t" << p[i].at << "\t" << p[i].bt << "\t" <<
        p[i].priority << "\t\t" << p[i].wt << "\t" << p[i].tat << "\t" << p[i].ct << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << totalWT / n << endl;
    cout << "Average Turnaround Time: " << totalTAT / n << endl;
}

int main() {
    int n, choice, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);
    cout << "Enter process details (AT BT Priority):\n";
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Process " << p[i].pid << ": ";
        cin >> p[i].at >> p[i].bt >> p[i].priority;
    }

    do {
        cout << "\nChoose Scheduling Algorithm:\n";
        cout << "1. FCFS\n2. SJF\n3. Round Robin\n4. Priority Scheduling (Non-Preemptive)\n5. Preemptive Priority Scheduling\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                fcfs(p, n);
                displayResults(p, n);
                break;
            case 2:
                sjf(p, n);
                displayResults(p, n);
                break;
            case 3:
                cout << "Enter Time Quantum: ";
                cin >> quantum;
                roundRobin(p, n, quantum);
                displayResults(p, n);
                break;
            case 4:
                priorityScheduling(p, n);
                displayResults(p, n);
                break;
            case 5:
                preemptivePriority(p, n);
                displayResults(p, n);
                break;
            case 6:
                cout << "Exiting the program...\n";
                break;
            default:
                cout << "Invalid choice! Please enter a valid option.\n";
        }
    } while (choice != 6);

    return 0;
}





// 5. Preemptive Priority Scheduling

// Enter your choice: 5

// PID     AT      BT      Priority        WT      TAT     CT
// 1       0       5       10              7       12      12
// 2       1       4       20              3       7       8
// 3       2       2       30              0       2       4
// 4       4       1       40              0       1       5

// Average Waiting Time: 2.50
// Average Turnaround Time: 5.50

// Choose Scheduling Algorithm:
// 1. FCFS
// 2. SJF
// 3. Round Robin
// 4. Priority Scheduling (Non-Preemptive)
// 5. Preemptive Priority Scheduling
// 6. Exit
// Enter your choice: 