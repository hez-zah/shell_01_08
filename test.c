#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum) {
    printf("CTRL+C was pressed. Exiting signal handler...\n");
    // Perform any cleanup or desired actions here.
}

int main() {
    // Set up the signal handler with SA_RESTART flag disabled
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Disable SA_RESTART for SIGINT
    sigaction(SIGINT, &sa, NULL);

    while (1) {
        // Your program's main logic goes here

        // Read data from stdin
        char buffer[256];
        int bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Received data: %s\n", buffer);
        } else if (bytes_read == -1) {
            printf("no data recieved, only signal :(\n");
        }

        // Continue with the rest of your program logic here
        // ...

        // Sleep or do other work to avoid busy-waiting
        // ...

    }

    return 0;
}
