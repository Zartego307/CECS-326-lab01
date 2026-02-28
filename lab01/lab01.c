/*
CECS 326 - Lab 01 (pthread_cancel)

NAME: Diego Ortega-Salazar

QUESTIONS (long-form):

1) Explain what pthread is doing in this program. Be specific.
- "pthread" (POSIX threads) is the library that lets one process run multiple threads of execution at the same time.
- In this program, the main thread calls pthread_create() to spawn a second thread (worker thread).
- That worker thread runs a function that loops forever: sleep 3 seconds, then print a message.
- The main thread waits for the user to press Enter. When Enter is pressed, the main thread calls pthread_cancel()
  to request cancellation of the worker thread. The worker thread is set to "deferred cancellation", which means
  it actually stops at a cancellation point. sleep() is a cancellation point, so the worker will get canceled
  cleanly the next time it hits sleep (or while sleeping).
- After canceling, the main thread calls pthread_join() to wait until the worker is fully gone (so we don’t leave
  a zombie thread running). Then the program waits 5 seconds to prove the worker is not printing anymore, and exits.

2) Explain why the sleeping thread can print its periodic messages while the main thread is waiting for keyboard input.
- Because threads run concurrently. Even if the main thread is blocked waiting for input (like getchar()),
  the OS scheduler can still run the other thread.
- When the worker thread wakes up from sleep, it gets CPU time and prints its message, while the main thread
  is still blocked waiting for the Enter key. Blocking in one thread doesn’t freeze the whole process (unless
  you were using user-level threads without kernel support, but pthreads are kernel-scheduled on Linux).

NOTES:
- Compile on Linux with: gcc -pthread lab01.c -o lab01
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>   // sleep()
#include <stdlib.h>   // EXIT_SUCCESS

// Worker thread function: loops forever, sleeps 3 seconds, prints message.
void* worker_thread(void* arg) {
    (void)arg; // unused

    // Make cancellation "deferred" (default), and make sure it is enabled.
    // Deferred means: thread only cancels at cancellation points (sleep is one).
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while (1) {
        sleep(3);  // cancellation point
        printf("[worker] still alive... (printed every 3 seconds)\n");
        fflush(stdout); // makes sure it prints immediately
    }

    // Never reached, but included for completeness.
    return NULL;
}

int main(void) {
    pthread_t tid;

    // Create the second thread.
    if (pthread_create(&tid, NULL, worker_thread, NULL) != 0) {
        perror("pthread_create failed");
        return 1;
    }

    printf("[main] Worker thread started.\n");
    printf("[main] Press ENTER to cancel the worker thread...\n");
    fflush(stdout);

    // Wait for the user to press Enter.
    // getchar() blocks THIS thread, but the worker keeps running.
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);

    // Cancel the worker thread.
    if (pthread_cancel(tid) != 0) {
        perror("pthread_cancel failed");
        return 1;
    }

    printf("[main] Sent cancel request to worker thread.\n");
    fflush(stdout);

    // Wait for the worker thread to actually terminate.
    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join failed");
        return 1;
    }

    printf("[main] Worker thread terminated.\n");
    printf("[main] Waiting 5 seconds to prove the worker is gone...\n");
    fflush(stdout);

    sleep(5);

    printf("[main] Done. Exiting.\n");
    return EXIT_SUCCESS;
}
