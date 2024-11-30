#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include "lwip/ip_addr.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"

class MqttClient
{
public:
    MqttClient(std::string&& host, int port = 1883);
    ~MqttClient();
    void Publish(const std::string& topic, const std::string& message);

private:
    void dns_lookup();
    static void dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
    err_t mqtt_connect();
    static void mqtt_connect_callback(mqtt_client_t *client, void *callback_arg, mqtt_connection_status_t status);
    static void mqtt_publish_callback(void *callback_arg, err_t result);

private:
    std::string _host;
    int _port;
    ip_addr_t _host_address;
    bool _dns_lookup_done;
    mqtt_client_t _mqtt_client;
    bool _mqtt_publish_done;
    std::string _topic;
    std::string _message;
};

#endif
