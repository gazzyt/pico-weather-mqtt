#include "logger.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "secrets.h"
#include "watchdog.h"
#include "wifi_connection.h"

WifiConnection::WifiConnection()
:   is_connected{false}
{
    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        LOG_ERROR("Wi-Fi init failed\n");
    }

    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    watchdog_update();

    for (unsigned int connection_attempt = 1; connection_attempt <= max_connection_retries && !is_connected; ++connection_attempt)
    {
        LOG_INFO("Connecting to Wi-Fi attempt %u\n", connection_attempt);
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, Watchdog::watchdog_time_ms)) {
            LOG_ERROR("failed to connect.\n");
        }
        else
        {
            is_connected = true;
            LOG_INFO("Connected.\n");
            // Read the ip address in a human readable way
            uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
            LOG_INFO("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
        }

        watchdog_update();
    }
}

WifiConnection::~WifiConnection()
{
    LOG_INFO("Disconnecting from Wi-Fi...\n");
    cyw43_arch_disable_sta_mode();
    cyw43_arch_deinit();
#ifndef NDEBUG
    auto link_status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
    LOG_INFO("Wi-Fi status is %s (%d)\n", status_name(link_status), link_status);
#endif
    LOG_INFO("Wi-Fi disconnected\n");
}

const char* WifiConnection::status_name(int status)
{
    switch (status) {
    case CYW43_LINK_DOWN:
        return "link down";
    case CYW43_LINK_JOIN:
        return "joining";
    case CYW43_LINK_NOIP:
        return "no ip";
    case CYW43_LINK_UP:
        return "link up";
    case CYW43_LINK_FAIL:
        return "link fail";
    case CYW43_LINK_NONET:
        return "network fail";
    case CYW43_LINK_BADAUTH:
        return "bad auth";
    }
    return "unknown";
}