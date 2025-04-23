#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    int pipefd[2];
    char buffer[100];

    pipe(pipefd); // create pipe
    pid_t pid = fork(); // create child process

    if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close reading end
        const char* msg = "Hello from Child!";
        write(pipefd[1], msg, strlen(msg));
        close(pipefd[1]); // Done writing
    } else {
        // Parent process
        close(pipefd[1]); // Close writing end
        read(pipefd[0], buffer, sizeof(buffer));
        cout << "Parent received: " << buffer << endl;
        close(pipefd[0]); // Done reading
    }

    return 0;
}
    