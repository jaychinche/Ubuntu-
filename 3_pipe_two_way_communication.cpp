#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>

using namespace std;

int main() {
    int pipefds1[2], pipefds2[2];
    pid_t pid;
    char pipe1writemessage[] = "Hi";
    char pipe2writemessage[] = "Hello";
    char readmessage[100];

    // Create first pipe
    if (pipe(pipefds1) == -1) {
        cerr << "Unable to create pipe 1" << endl;
        return 1;
    }

    // Create second pipe
    if (pipe(pipefds2) == -1) {
        cerr << "Unable to create pipe 2" << endl;
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    }

    if (pid > 0) {
        // ----- Parent Process -----
        close(pipefds1[0]); // Close read end of pipe1
        close(pipefds2[1]); // Close write end of pipe2

        cout << "In Parent: Writing to pipe 1 – Message is '" << pipe1writemessage << "'" << endl;
        write(pipefds1[1], pipe1writemessage, strlen(pipe1writemessage) + 1);

        read(pipefds2[0], readmessage, sizeof(readmessage));
        cout << "In Parent: Reading from pipe 2 – Message is '" << readmessage << "'" << endl;

        close(pipefds1[1]);
        close(pipefds2[0]);
    } else {
        // ----- Child Process -----
        close(pipefds1[1]); // Close write end of pipe1
        close(pipefds2[0]); // Close read end of pipe2

        read(pipefds1[0], readmessage, sizeof(readmessage));
        cout << "In Child: Reading from pipe 1 – Message is '" << readmessage << "'" << endl;

        cout << "In Child: Writing to pipe 2 – Message is '" << pipe2writemessage << "'" << endl;
        write(pipefds2[1], pipe2writemessage, strlen(pipe2writemessage) + 1);

        close(pipefds1[0]);
        close(pipefds2[1]);
    }

    return 0;
}
