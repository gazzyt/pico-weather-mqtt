#ifndef MQTT_PUBLISH_CONTEXT_H
#define MQTT_PUBLISH_CONTEXT_H

#include "lwip/ip_addr.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include <string>

class MqttPublishContext
{
public:
    MqttPublishContext(const std::string& mqtt_host, int mqtt_port, const std::string& mqtt_topic, const std::string& mqtt_message);
    MqttPublishContext(const MqttPublishContext& rhs) = delete;
    MqttPublishContext(MqttPublishContext&& rhs) = delete;
    MqttPublishContext& operator=(const MqttPublishContext& rhs) = delete;
    MqttPublishContext& operator=(MqttPublishContext&& rhs) = delete;
    ~MqttPublishContext() = default;

public:
    const std::string& host;
    int port;
    ip_addr_t host_address;
    volatile bool dns_lookup_done;
    mqtt_client_t mqtt_client;
    volatile bool mqtt_publish_done;
    std::string topic;
    std::string message;
};

#endif
