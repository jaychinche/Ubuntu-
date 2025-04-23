#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int fifo(vector<int> pages, int capacity, int &hits) {
    vector<int> frame(capacity, -1);
    int front = 0, faults = 0;
    hits = 0;

    for (int page : pages) {
        if (find(frame.begin(), frame.end(), page) != frame.end()) {
            hits++;
        } else {
            frame[front] = page;
            front = (front + 1) % capacity;
            faults++;
        }
    }
    return faults;
}

int lru(vector<int> pages, int capacity, int &hits) {
    vector<int> frame(capacity, -1);
    vector<int> lastUsed(capacity, -1);
    int faults = 0;
    hits = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool found = false;

        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                hits++;
                found = true;
                lastUsed[j] = i;
                break;
            }
        }

        if (!found) {
            int lru_index = 0;
            for (int j = 1; j < capacity; j++) {
                if (lastUsed[j] < lastUsed[lru_index]) {
                    lru_index = j;
                }
            }
            frame[lru_index] = page;
            lastUsed[lru_index] = i;
            faults++;
        }
    }
    return faults;
}

int optimal(vector<int> pages, int capacity, int &hits) {
    vector<int> frame(capacity, -1);
    int faults = 0;
    hits = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool found = false;

        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                hits++;
                found = true;
                break;
            }
        }

        if (!found) {
            int replace_index = -1, farthest = i + 1;

            for (int j = 0; j < capacity; j++) {
                int k;
                for (k = i + 1; k < pages.size(); k++) {
                    if (frame[j] == pages[k]) break;
                }

                if (k == pages.size()) {
                    replace_index = j;
                    break;
                }

                if (k > farthest) {
                    farthest = k;
                    replace_index = j;
                }
            }

            if (replace_index == -1)
                replace_index = 0;

            frame[replace_index] = page;
            faults++;
        }
    }
    return faults;
}

int main() {
    int n, capacity;
    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter the page sequence: ";
    for (int i = 0; i < n; i++) cin >> pages[i];

    cout << "Enter number of frames: ";
    cin >> capacity;

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. FIFO\n2. LRU\n3. Optimal\n4. Exit\nChoose: ";
        cin >> choice;

        int hits = 0, faults = 0;

        switch (choice) {
            case 1:
                faults = fifo(pages, capacity, hits);
                cout << "FIFO Page Faults: " << faults << ", Page Hits: " << hits << endl;
                break;
            case 2:
                faults = lru(pages, capacity, hits);
                cout << "LRU Page Faults: " << faults << ", Page Hits: " << hits << endl;
                break;
            case 3:
                faults = optimal(pages, capacity, hits);
                cout << "Optimal Page Faults: " << faults << ", Page Hits: " << hits << endl;
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 4);

    return 0;
}
