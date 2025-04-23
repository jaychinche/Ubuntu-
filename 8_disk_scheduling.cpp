#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
using namespace std;

// FCFS
void fcfs(vector<int> requests, int head) {
    int seek = 0;
    cout << "Sequence: " << head;
    for (int r : requests) {
        seek += abs(head - r);
        head = r;
        cout << " -> " << head;
    }
    cout << "\nTotal Seek Time (FCFS): " << seek << endl;
}

// SSTF
void sstf(vector<int> requests, int head) {
    vector<bool> visited(requests.size(), false);
    int seek = 0, count = 0;
    cout << "Sequence: " << head;
    while (count < requests.size()) {
        int min_dist = INT_MAX;
        int index = -1;
        for (int i = 0; i < requests.size(); i++) {
            if (!visited[i] && abs(head - requests[i]) < min_dist) {
                min_dist = abs(head - requests[i]);
                index = i;
            }
        }
        visited[index] = true;
        seek += abs(head - requests[index]);
        head = requests[index];
        cout << " -> " << head;
        count++;
    }
    cout << "\nTotal Seek Time (SSTF): " << seek << endl;
}

// SCAN
void scan(vector<int> requests, int head, int disk_size, string direction) {
    vector<int> left, right;
    int seek = 0;

    if (direction == "left") left.push_back(0);
    else right.push_back(disk_size - 1);

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "Sequence: " << head;

    if (direction == "left") {
        for (int i = left.size() - 1; i >= 0; i--) {
            seek += abs(head - left[i]);
            head = left[i];
            cout << " -> " << head;
        }
        for (int i = 0; i < right.size(); i++) {
            seek += abs(head - right[i]);
            head = right[i];
            cout << " -> " << head;
        }
    } else {
        for (int i = 0; i < right.size(); i++) {
            seek += abs(head - right[i]);
            head = right[i];
            cout << " -> " << head;
        }
        for (int i = left.size() - 1; i >= 0; i--) {
            seek += abs(head - left[i]);
            head = left[i];
            cout << " -> " << head;
        }
    }

    cout << "\nTotal Seek Time (SCAN): " << seek << endl;
}

// C-SCAN
void cscan(vector<int> requests, int head, int disk_size) {
    vector<int> left, right;
    int seek = 0;

    right.push_back(disk_size - 1);
    left.push_back(0);

    for (int r : requests) {
        if (r >= head) right.push_back(r);
        else left.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "Sequence: " << head;

    for (int i = 0; i < right.size(); i++) {
        seek += abs(head - right[i]);
        head = right[i];
        cout << " -> " << head;
    }

    // Move to 0
    seek += (disk_size - 1);
    head = 0;
    cout << " -> " << head;

    for (int i = 0; i < left.size(); i++) {
        seek += abs(head - left[i]);
        head = left[i];
        cout << " -> " << head;
    }

    cout << "\nTotal Seek Time (C-SCAN): " << seek << endl;
}

int main() {
    int n, head, disk_size, choice;
    cout << "Enter number of disk requests: ";
    cin >> n;

    vector<int> requests(n);
    cout << "Enter request queue: ";
    for (int i = 0; i < n; i++) {
        cin >> requests[i];
    }

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> disk_size;

    do {
        cout << "\n--- Disk Scheduling Menu ---\n";
        cout << "1. FCFS\n";
        cout << "2. SSTF\n";
        cout << "3. SCAN\n";
        cout << "4. C-SCAN\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                fcfs(requests, head);
                break;
            case 2:
                sstf(requests, head);
                break;
            case 3: {
                string direction;
                cout << "Enter direction (left/right): ";
                cin >> direction;
                scan(requests, head, disk_size, direction);
                break;
            }
            case 4:
                cscan(requests, head, disk_size);
                break;
            case 5:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}



// Enter number of disk requests: 7
// Enter request queue: 82 170 43 140 24 16 190
// Enter initial head position: 50
// Enter disk size: 200


//FCFS 
// Enter your choice: 1
// Sequence: 50 -> 82 -> 170 -> 43 -> 140 -> 24 -> 16 -> 190
// Total Seek Time (FCFS): 642

//SSTF
// Enter your choice: 2
// Sequence: 50 -> 43 -> 24 -> 16 -> 82 -> 140 -> 170 -> 190
// Total Seek Time (SSTF): 208

//SCAN 
// Enter your choice: 3
// Enter direction (left/right): right
// Sequence: 50 -> 82 -> 140 -> 170 -> 190 -> 199 -> 43 -> 24 -> 16
// Total Seek Time (SCAN): 332


// C-SCAN
// Enter your choice: 4
// Sequence: 50 -> 82 -> 140 -> 170 -> 190 -> 199 -> 0 -> 0 -> 16 -> 24 -> 43
// Total Seek Time (C-SCAN): 391