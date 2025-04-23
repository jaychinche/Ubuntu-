#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

int data = 0;
int rc = 0; // Reader count

// Binary semaphores
sem_t db;     // Controls access to the database (writer or first reader)
sem_t mutex;  // Protects rc (reader count)

// Semaphore wrappers
void down(sem_t* s) {
    sem_wait(s);
}

void up(sem_t* s) {
    sem_post(s);
}

// Reader function (synchronized)
void* reader(void*) {
    down(&mutex);
    rc++;
    if (rc == 1)
        down(&db); // First reader locks DB
    up(&mutex);

    cout << "[Reader] Reading data: " << data << endl;
    sleep(1);

    down(&mutex);
    rc--;
    if (rc == 0)
        up(&db); // Last reader unlocks DB
    up(&mutex);

    return NULL;
}

// Writer function (synchronized)
void* writer(void*) {
    down(&db);
    data++;
    cout << "[Writer] Writing data: " << data << endl;
    sleep(1);
    up(&db);
    return NULL;
}

int main() {
    pthread_t r1, r2, w1, w2;
    int choice;

    // Initialize semaphores as binary semaphores
    sem_init(&db, 0, 1);    // Binary semaphore for DB
    sem_init(&mutex, 0, 1); // Binary semaphore for rc

    cout << "1. Without Synchronization (for reference)\n2. With Synchronization (binary semaphores)\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        // UNSYNCHRONIZED (just for demonstration)
        pthread_create(&r1, NULL, [](void*) -> void* {
            cout << "[Reader] Reading data (no sync): " << data << endl;
            sleep(1);
            return NULL;
        }, NULL);

        pthread_create(&w1, NULL, [](void*) -> void* {
            data++;
            cout << "[Writer] Writing data (no sync): " << data << endl;
            sleep(1);
            return NULL;
        }, NULL);

        pthread_create(&r2, NULL, [](void*) -> void* {
            cout << "[Reader] Reading data (no sync): " << data << endl;
            sleep(1);
            return NULL;
        }, NULL);

        pthread_create(&w2, NULL, [](void*) -> void* {
            data++;
            cout << "[Writer] Writing data (no sync): " << data << endl;
            sleep(1);
            return NULL;
        }, NULL);
    } else {
        // SYNCHRONIZED using binary semaphores
        pthread_create(&r1, NULL, reader, NULL);
        pthread_create(&w1, NULL, writer, NULL);
        pthread_create(&r2, NULL, reader, NULL);
        pthread_create(&w2, NULL, writer, NULL);
    }

    // Wait for threads to finish
    pthread_join(r1, NULL);
    pthread_join(w1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w2, NULL);

    // Cleanup
    sem_destroy(&db);
    sem_destroy(&mutex);

    return 0;
}
