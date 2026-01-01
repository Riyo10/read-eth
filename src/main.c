#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "../include/server.h"
#include "../include/network.h"

void usage() {
    printf("Usage: network-diagnostic [options]\n");
    printf("Options:\n");
    printf("  -p, --port PORT     Port to run server on (default: 8080)\n");
    printf("  -h, --help          Show this help message\n");
    printf("  -v, --version       Show version\n");
}

int main(int argc, char *argv[]) {
    int port = SERVER_PORT;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) {
            if (i + 1 < argc) {
                port = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage();
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("Network Diagnostic Tool v1.0.0\n");
            return 0;
        }
    }

    printf("========================================\n");
    printf("   Network Diagnostic Tool v1.0.0\n");
    printf("========================================\n");
    printf("Starting server on port %d...\n", port);
    printf("Open http://localhost:%d in your browser\n", port);
    printf("Press Ctrl+C to stop\n");
    printf("========================================\n\n");

    // Create server thread
    pthread_t server_thread;
    int port_arg = port;
    pthread_create(&server_thread, NULL, (void *(*)(void *)) server_accept_loop, &port_arg);

    // Wait for server thread
    pthread_join(server_thread, NULL);

    printf("\nServer stopped\n");
    cleanup_network();

    return 0;
}
