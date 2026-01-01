#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include "../include/server.h"
#include "../include/network.h"
#include "../include/json.h"

static int server_socket = -1;
static int running = 0;

void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        running = 0;
    }
}

int start_server(int port) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        return -1;
    }

    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return -1;
    }

    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        perror("listen");
        return -1;
    }

    printf("Server listening on port %d\n", port);
    running = 1;
    return 0;
}

int stop_server() {
    running = 0;
    if (server_socket >= 0) {
        close(server_socket);
        server_socket = -1;
    }
    return 0;
}

void send_response(int client_fd, int status_code, const char *content_type, const char *body) {
    char response[8192];
    const char *status_text = "OK";

    switch (status_code) {
        case 200: status_text = "OK"; break;
        case 404: status_text = "Not Found"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "Unknown"; break;
    }

    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s; charset=utf-8\r\n"
             "Content-Length: %lu\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status_code, status_text, content_type, strlen(body), body);

    send(client_fd, response, strlen(response), 0);
}

void send_file(int client_fd, const char *filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL) {
        send_response(client_fd, 404, "text/plain", "File not found");
        return;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char header[512];
    const char *content_type = "text/plain";

    if (strstr(filepath, ".html")) {
        content_type = "text/html";
    } else if (strstr(filepath, ".css")) {
        content_type = "text/css";
    } else if (strstr(filepath, ".js")) {
        content_type = "application/javascript";
    } else if (strstr(filepath, ".json")) {
        content_type = "application/json";
    } else if (strstr(filepath, ".png")) {
        content_type = "image/png";
    } else if (strstr(filepath, ".jpg") || strstr(filepath, ".jpeg")) {
        content_type = "image/jpeg";
    }

    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "Connection: close\r\n"
             "\r\n",
             content_type, file_size);

    send(client_fd, header, strlen(header), 0);

    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_fd, buffer, bytes_read, 0);
    }

    fclose(fp);
}

void handle_network_info_request(int client_fd) {
    NetworkInfo info;
    memset(&info, 0, sizeof(info));
    strncpy(info.ipv4, "N/A", sizeof(info.ipv4) - 1);
    strncpy(info.ipv6, "N/A", sizeof(info.ipv6) - 1);
    strncpy(info.gateway, "N/A", sizeof(info.gateway) - 1);
    strncpy(info.dns1, "N/A", sizeof(info.dns1) - 1);
    strncpy(info.dns2, "N/A", sizeof(info.dns2) - 1);
    
    // Get actual network information
    get_network_info(&info);

    JSONBuffer *json = json_create_object();
    json_add_string(json, "ipv4", info.ipv4);
    json_add_string(json, "ipv6", info.ipv6);
    json_add_string(json, "gateway", info.gateway);
    json_add_string(json, "dns1", info.dns1);
    json_add_string(json, "dns2", info.dns2);

    const char *response = json_get_string(json);
    send_response(client_fd, 200, "application/json", response);
    json_free(json);
}

void handle_speed_test_request(int client_fd) {
    SpeedTestResult result;
    memset(&result, 0, sizeof(result));
    perform_speed_test(&result);

    JSONBuffer *json = json_create_object();
    json_add_number(json, "download_mbps", result.download_mbps);
    json_add_number(json, "upload_mbps", result.upload_mbps);
    json_add_number(json, "ping_ms", result.ping_ms);
    json_add_integer(json, "test_time", (int)result.test_time);

    const char *response = json_get_string(json);
    send_response(client_fd, 200, "application/json", response);
    json_free(json);
}

void handle_isp_info_request(int client_fd) {
    ISPInfo info;
    memset(&info, 0, sizeof(info));
    strncpy(info.isp_name, "Unknown", sizeof(info.isp_name) - 1);
    strncpy(info.country, "Unknown", sizeof(info.country) - 1);
    strncpy(info.city, "Unknown", sizeof(info.city) - 1);
    strncpy(info.latitude, "N/A", sizeof(info.latitude) - 1);
    strncpy(info.longitude, "N/A", sizeof(info.longitude) - 1);
    strncpy(info.timezone, "Unknown", sizeof(info.timezone) - 1);
    
    // Get ISP information
    get_isp_info(&info);

    JSONBuffer *json = json_create_object();
    json_add_string(json, "isp", info.isp_name);
    json_add_string(json, "country", info.country);
    json_add_string(json, "city", info.city);
    json_add_string(json, "latitude", info.latitude);
    json_add_string(json, "longitude", info.longitude);
    json_add_string(json, "timezone", info.timezone);

    const char *response = json_get_string(json);
    send_response(client_fd, 200, "application/json", response);
    json_free(json);
}

void handle_interface_stats_request(int client_fd) {
    InterfaceStats stats;
    memset(&stats, 0, sizeof(stats));
    strncpy(stats.interface_name, "eth0", sizeof(stats.interface_name) - 1);
    strncpy(stats.bytes_sent, "0", sizeof(stats.bytes_sent) - 1);
    strncpy(stats.bytes_recv, "0", sizeof(stats.bytes_recv) - 1);
    
    // Get interface statistics
    get_interface_stats(&stats);

    JSONBuffer *json = json_create_object();
    json_add_string(json, "interface", stats.interface_name);
    json_add_string(json, "bytes_sent", stats.bytes_sent);
    json_add_string(json, "bytes_received", stats.bytes_recv);

    const char *response = json_get_string(json);
    send_response(client_fd, 200, "application/json", response);
    json_free(json);
}

void* handle_client_connection_thread(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);  // Free the allocated memory
    
    char buffer[MAX_BUFFER_SIZE];
    int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read < 0) {
        perror("recv");
        close(client_fd);
        return NULL;
    }

    buffer[bytes_read] = '\0';

    // Parse request line
    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    printf("[%s] %s %s\n", method, path, protocol);

    // Handle CORS preflight
    if (strcmp(method, "OPTIONS") == 0) {
        char response[512];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Access-Control-Allow-Origin: *\r\n"
                 "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                 "Access-Control-Allow-Headers: Content-Type\r\n"
                 "Content-Length: 0\r\n"
                 "Connection: close\r\n"
                 "\r\n");
        send(client_fd, response, strlen(response), 0);
        close(client_fd);
        return NULL;
    }

    // Route requests
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            send_file(client_fd, "./web/index.html");
        } else if (strcmp(path, "/api/network-info") == 0) {
            handle_network_info_request(client_fd);
        } else if (strcmp(path, "/api/speed-test") == 0) {
            handle_speed_test_request(client_fd);
        } else if (strcmp(path, "/api/isp-info") == 0) {
            handle_isp_info_request(client_fd);
        } else if (strcmp(path, "/api/interface-stats") == 0) {
            handle_interface_stats_request(client_fd);
        } else if (strncmp(path, "/static/", 8) == 0) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "./web%s", path);
            send_file(client_fd, filepath);
        } else {
            send_response(client_fd, 404, "text/plain", "Not Found");
        }
    } else {
        send_response(client_fd, 404, "text/plain", "Method Not Allowed");
    }

    close(client_fd);
    return NULL;
}

void* server_accept_loop(void *arg) {
    int port = *(int *)arg;

    if (start_server(port) < 0) {
        fprintf(stderr, "Failed to start server\n");
        return NULL;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    while (running) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            if (running) {
                perror("accept");
            }
            continue;
        }

        printf("Client connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        // Handle client in a thread
        pthread_t thread;
        int *client_fd_ptr = (int *)malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        pthread_create(&thread, NULL, handle_client_connection_thread, client_fd_ptr);
        pthread_detach(thread);
    }

    stop_server();
    return NULL;
}
