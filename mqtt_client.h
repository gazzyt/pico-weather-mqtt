#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include "lwip/ip_addr.h"

class MqttClient
{
public:
    MqttClient(std::string&& host, int port = 1883);
    ~MqttClient();
    void Publish(const std::string& topic, const std::string& message);

private:
    void dns_lookup();
    static void dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

private:
    std::string _host;
    int _port;
    ip_addr_t _host_address;
    bool _dns_lookup_done;
};

#endif
