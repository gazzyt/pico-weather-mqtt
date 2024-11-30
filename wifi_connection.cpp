#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "secrets.h"
#include "wifi_connection.h"

WifiConnection::WifiConnection()
{
    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }

    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }
}

WifiConnection::~WifiConnection()
{
    printf("Disconnecting from Wi-Fi...\n");
    cyw43_arch_disable_sta_mode();
    cyw43_arch_deinit();
}
