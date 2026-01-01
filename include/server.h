#ifndef SERVER_H
#define SERVER_H

#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 4096
#define MAX_CONNECTIONS 100

typedef struct {
    int socket_fd;
    char buffer[MAX_BUFFER_SIZE];
    int request_complete;
} ClientConnection;

// Server functions
int start_server(int port);
int stop_server();
void handle_client_connection(int client_fd);
void send_response(int client_fd, int status_code, const char *content_type, const char *body);
void send_file(int client_fd, const char *filepath);

// Request handlers
void handle_network_info_request(int client_fd);
void handle_speed_test_request(int client_fd);
void handle_isp_info_request(int client_fd);
void handle_static_file_request(int client_fd, const char *filepath);
void handle_interface_stats_request(int client_fd);

// Thread function
void* handle_client_connection_thread(void* arg);

// Server thread
void* server_accept_loop(void *arg);

#endif // SERVER_H
