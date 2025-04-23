#include<iostream>
#include <climits>
using namespace std;

void first_fit(int blocks, int blocksSize[], int process, int processSize[]) {
    for(int i=0;i<process;i++) {
        for(int j=0;j<blocks;j++) {
            if(processSize[i]<=blocksSize[j]) {
                blocksSize[j]-=processSize[i];
                cout<<"Process "<<i+1<<" fitted in block "<<j+1<<endl;
                break;
            }
        }
    }
}

void next_fit(int blocks, int blocksSize[], int process, int processSize[]) {
    int lastOccupied=0;
    for(int i=0;i<process;i++) {
        for(int j=lastOccupied;j<blocks;j++) {
            if(processSize[i]<=blocksSize[j]) {
                blocksSize[j]-=processSize[i];
                lastOccupied=j;
                cout<<"Process "<<i+1<<" fitted in block "<<j+1<<endl;
                break;
            }
        }
    }
}

void best_fit(int blocks, int blocksSize[], int process, int processSize[]) {
    for(int i=0;i<process;i++) {
        int diff = INT_MAX;
        int index = -1;
        for(int j=0;j<blocks;j++) {
            if(blocksSize[j] >= processSize[i] && (blocksSize[j] - processSize[i]) < diff) {
                diff = blocksSize[j] - processSize[i];
                index = j;
            }
        }
        if(index != -1) {
            blocksSize[index] -= processSize[i];
            cout<<"Process "<<i+1<<" fitted in block "<<index+1<<endl;
        } else {
            cout<<"Process "<<i+1<<" could not be allocated\n";
        }
    }
}

void worst_fit(int blocks, int blocksSize[], int process, int processSize[]) {
    for(int i=0;i<process;i++) {
        int diff = INT_MIN;
        int index = -1;
        for(int j=0;j<blocks;j++) {
            if(blocksSize[j] >= processSize[i] && (blocksSize[j] - processSize[i]) > diff) {
                diff = blocksSize[j] - processSize[i];
                index = j;
            }
        }
        if(index != -1) {
            blocksSize[index] -= processSize[i];
            cout<<"Process "<<i+1<<" fitted in block "<<index+1<<endl;
        } else {
            cout<<"Process "<<i+1<<" could not be allocated\n";
        }
    }
}

int main() {
    int blocks;
    cout<<"Enter number of blocks : ";
    cin>>blocks;
    int blocksSize[blocks];
    int copyofBlocksSize[blocks];
    cout<<"Enter sizes of blocks : ";
    for(int i=0;i<blocks;i++) {
        cin>>blocksSize[i];
    }

    int process;
    cout<<"Enter number of processes : ";
    cin>>process;
    int processSize[process];
    int copyofProcessSize[process];
    cout<<"Enter sizes of processes : ";
    for(int i=0;i<process;i++) {
        cin>>processSize[i];
    }

    int ch=0;
    while(ch!=5) {
        cout<<"Menu: \n";
        cout<<"1. First Fit\n";
        cout<<"2. Next Fit\n";
        cout<<"3. Best Fit\n";
        cout<<"4. Worst Fit\n";
        cout<<"5. Exit\n\n";
        cout<<"Enter a choice : ";
        cin>>ch;

        switch (ch)
        {
        case 1:
            copy(blocksSize,blocksSize+blocks,copyofBlocksSize);
            copy(processSize,processSize+process,copyofProcessSize);
            first_fit(blocks,copyofBlocksSize,process,copyofProcessSize);
            break;
        case 2:
            copy(blocksSize,blocksSize+blocks,copyofBlocksSize);
            copy(processSize,processSize+process,copyofProcessSize);
            next_fit(blocks,copyofBlocksSize,process,copyofProcessSize);
            break;
        case 3:
            copy(blocksSize,blocksSize+blocks,copyofBlocksSize);
            copy(processSize,processSize+process,copyofProcessSize);
            best_fit(blocks,copyofBlocksSize,process,copyofProcessSize);
            break;
        case 4:
            copy(blocksSize,blocksSize+blocks,copyofBlocksSize);
            copy(processSize,processSize+process,copyofProcessSize);
            worst_fit(blocks,copyofBlocksSize,process,copyofProcessSize);
            break;
        case 5:
            cout<<"Exiting...\n";
            break;
        default:
            cout<<"Invalid choice, please try again.";
            break;
        }
    }
    return 0;
}