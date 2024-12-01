#include "mqtt_publish_context.h"

MqttPublishContext::MqttPublishContext(const std::string& mqtt_host, int mqtt_port, const std::string& mqtt_topic, const std::string& mqtt_message)
:   host{mqtt_host},
    port{mqtt_port},
    host_address{0},
    dns_lookup_done{false},
    mqtt_client{0},
    mqtt_publish_done{false},
    topic{mqtt_topic},
    message{mqtt_message}
{
}
