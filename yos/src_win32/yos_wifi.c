/* WiFi station 
*/
#include <stdio.h>
#include <string.h>
#include <Windows.h>


int yos_wifi_station_scan(char* ssids, size_t max_size)
{
    Sleep(2000);
    snprintf(ssids, max_size, "SSID1\nSSID2");
    return 0;
}

int yos_wifi_station_connect(const char* ssid, const char* password)
{
    Sleep(2000);
    return strcmp(password, "aa");
}

int yos_wifi_station_get_ip4(char ip[20])
{
    char host_name[256] = {0};
    if (gethostname(host_name, sizeof(host_name)) != 0) {
        { WSADATA data; WSAStartup(MAKEWORD(2, 2), &data); }
        gethostname(host_name, sizeof(host_name));
    }
    
    struct hostent* he = gethostbyname(host_name);
    if ((he != NULL) && (he->h_addr_list != NULL)) {
        struct in_addr* addr = (struct in_addr*)he->h_addr_list[0];
        //InetNtop(AF_INET, addr, ip, 20);
        strcpy(ip, inet_ntoa(*addr));
        return 0;
    }
    return -1;
}