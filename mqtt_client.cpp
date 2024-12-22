#include <cassert>
#include "lwip/dns.h"
#include "mqtt_client.h"
#include "pico/cyw43_arch.h"

MqttClient::MqttClient(std::string&& host, int port)
:   _host{host},
    _port{port}
{

}

MqttClient::~MqttClient()
{

}

void MqttClient::Publish(const std::string& topic, const std::string& message)
{
    MqttPublishContext publishContext{_host, _port, topic, message};

    dns_lookup(publishContext);

    auto connect_err = mqtt_connect(publishContext);

    if (connect_err == ERR_OK)
    {
        while (!publishContext.mqtt_publish_done)
        {
            // Busy wait
        }
        
        printf("Disconnect MQTT\n");
        mqtt_disconnect(&publishContext.mqtt_client);
    }
    else
    {
        printf("MQTT connect failed with error code %d\n", connect_err);
    }
}

void MqttClient::dns_lookup(MqttPublishContext& publishContext)
{
    auto hostname = publishContext.host.c_str();
    printf("Begin DNS lookup for %s\n", hostname);
    auto err = dns_gethostbyname(hostname, &publishContext.host_address, dns_found_callback, &publishContext);

    if (err == ERR_ARG)
    {
        printf("Failed to start DNS query\n");
        return;
    }

    if (err == ERR_OK)
    {
        printf("Address found in local cache\n");
        return;
    }

    while (!publishContext.dns_lookup_done)
    {
        // Busy wait
    }
    
}

void MqttClient::dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    assert(callback_arg != nullptr);
    MqttPublishContext* publishContext = reinterpret_cast<MqttPublishContext*>(callback_arg);

    if (ipaddr == nullptr)
    {
        printf("DNS query failed\n");
    }
    else
    {
        printf("DNS query returned address %s\n", ip4addr_ntoa(ipaddr));
        publishContext->host_address = *ipaddr;
    }
    publishContext->dns_lookup_done = true;
}

err_t MqttClient::mqtt_connect(MqttPublishContext& publishContext)
{
    mqtt_connect_client_info_t ci
    {
        .client_id = "picow",
        .client_user = nullptr,
        .client_pass = nullptr,
        .keep_alive = 0,
        .will_topic = nullptr,
        .will_msg = nullptr,
        .will_qos = 0,
        .will_retain = 0
    };

    return mqtt_client_connect(&publishContext.mqtt_client, &publishContext.host_address, _port, mqtt_connect_callback, &publishContext, &ci);
}

void MqttClient::mqtt_connect_callback(mqtt_client_t *client, void *callback_arg, mqtt_connection_status_t status)
{
    assert(callback_arg != nullptr);
    MqttPublishContext* publishContext = reinterpret_cast<MqttPublishContext*>(callback_arg);

    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("MQTT connection successful\n");
        u8_t qos = 1;
        u8_t retain = 1;
        auto err = mqtt_publish(&publishContext->mqtt_client, publishContext->topic.c_str(), publishContext->message.c_str(), publishContext->message.length(), qos, retain, mqtt_publish_callback, callback_arg);
        if (err != ERR_OK)
        {
            printf("Publish failed with result %d\n", err);
        }
    }
    else
    {
        printf("MQTT disconnected status %d\n", status);
        publishContext->mqtt_publish_done = true;
    }
}

void MqttClient::mqtt_publish_callback(void *callback_arg, err_t result)
{
    assert(callback_arg != nullptr);
    MqttPublishContext* publishContext = reinterpret_cast<MqttPublishContext*>(callback_arg);

    if (result == ERR_OK)
    {
        printf("MQTT publish succeeded\n");
    }
    else
    {
        printf("MQTT publish failed with status %d\n", result);
    }

    publishContext->mqtt_publish_done = true;
}
