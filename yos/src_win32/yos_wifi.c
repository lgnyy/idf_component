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
