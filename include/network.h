#ifndef NETWORK_H
#define NETWORK_H

#include <time.h>

typedef struct {
    char ipv4[256];
    char ipv6[256];
    char gateway[256];
    char dns1[256];
    char dns2[256];
} NetworkInfo;

typedef struct {
    char isp_name[256];
    char country[256];
    char city[256];
    char latitude[32];
    char longitude[32];
    char timezone[128];
} ISPInfo;

typedef struct {
    double download_mbps;
    double upload_mbps;
    double ping_ms;
    time_t test_time;
    int is_testing;
} SpeedTestResult;

typedef struct {
    char interface_name[64];
    char mac_address[256];
    char bytes_sent[64];
    char bytes_recv[64];
} InterfaceStats;

// Network functions
int get_ipv4_address(char *ipv4);
int get_ipv6_address(char *ipv6);
int get_gateway_address(char *gateway);
int get_dns_servers(char *dns1, char *dns2);
int get_network_info(NetworkInfo *info);

// Speed test functions
int get_current_ping(double *ping_ms);
int perform_speed_test(SpeedTestResult *result);
double measure_download_speed();
double measure_upload_speed();

// ISP Info functions
int get_isp_info(ISPInfo *info);

// Interface stats
int get_interface_stats(InterfaceStats *stats);
int get_wifi_interface_name(char *interface);
int get_wifi_signal_strength(const char *interface, int *strength);

// Utility
void cleanup_network();

#endif // NETWORK_H
