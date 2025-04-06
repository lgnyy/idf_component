/*
 * WiFi search and connection
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Wi-Fi station mode.
 *
 * This function initializes the Wi-Fi station mode by setting up the default event loop,
 * initializing the network interface, creating the default Wi-Fi station, and configuring
 * the Wi-Fi settings. It also loads the Wi-Fi configuration from non-volatile storage (NVS)
 * and attempts to connect to the access point (AP) using the loaded configuration.
 *
 * @return
 *     - 0 on success
 *     - Non-zero error code on failure
 */
int yos_wifi_station_init(void);

/**
 * @brief Scans for available Wi-Fi access points and stores their SSIDs in the provided buffer.
 *
 * This function starts a Wi-Fi scan, waits for it to complete, and then retrieves the list of
 * available access points. The SSIDs of the access points are concatenated into the provided
 * buffer, separated by newline characters.
 *
 * @param ssids Pointer to a buffer where the SSIDs will be stored. The buffer must be large enough
 *              to hold the concatenated SSIDs and newline characters.
 * @param max_size The maximum size of the buffer pointed to by ssids.
 *
 * @return 0 on success, or an error code on failure.
 */
int yos_wifi_station_scan(char* ssids, size_t max_size);

/**
 * @brief Connects to a Wi-Fi station with the given SSID and password.
 *
 * This function configures the Wi-Fi settings and attempts to connect to the specified
 * Wi-Fi network. If the connection is successful, the configuration is saved.
 *
 * @param ssid The SSID of the Wi-Fi network to connect to.
 * @param password The password of the Wi-Fi network.
 * @return int Returns 0 on successful connection, otherwise returns an error code.
 */
int yos_wifi_station_connect(const char* ssid, const char* password);

/**
 * @brief Retrieves the IPv4 address of the Wi-Fi station.
 *
 * This function obtains the current IPv4 address assigned to the Wi-Fi station
 * and stores it in the provided character array.
 *
 * @param[out] ip A character array with a size of at least 20 bytes to store
 *                the null-terminated IPv4 address string.
 *                Example format: "192.168.1.1".
 *
 * @return int Returns 0 on success, or a negative value on failure.
 */
int yos_wifi_station_get_ip4(char ip[20]);

#ifdef __cplusplus
}
#endif
