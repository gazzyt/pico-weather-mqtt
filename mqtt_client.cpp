#include "lwip/dns.h"
#include "logger.h"
#include "mqtt_client.h"
#include "pico/cyw43_arch.h"
#include <cassert>

MqttClient::MqttClient(std::string&& host, int port)
:   _host{host},
    _port{port}
{

}

void MqttClient::Publish(const std::string& topic, const std::string& message)
{
    MqttPublishContext publishContext{_host, _port, topic, message};

    dns_lookup(publishContext);

    cyw43_arch_lwip_begin();
    auto connect_err = mqtt_connect(publishContext);
    cyw43_arch_lwip_end();

    if (connect_err == ERR_OK)
    {
        while (!publishContext.mqtt_publish_done)
        {
            // Busy wait
        }
        
        LogInfo("Disconnect MQTT");
        cyw43_arch_lwip_begin();
        mqtt_disconnect(&publishContext.mqtt_client);
        cyw43_arch_lwip_end();
    }
    else
    {
        LogInfo("MQTT connect failed with error code {}", connect_err);
    }
}

void MqttClient::dns_lookup(MqttPublishContext& publishContext)
{
    auto hostname = publishContext.host.c_str();
    LogInfo("Begin DNS lookup for {}", hostname);
    auto err = dns_gethostbyname(hostname, &publishContext.host_address, dns_found_callback, &publishContext);

    if (err == ERR_ARG)
    {
        LogError("Failed to start DNS query");
        return;
    }

    if (err == ERR_OK)
    {
        LogInfo("Address found in local cache");
        return;
    }

    while (!publishContext.dns_lookup_done)
    {
        // Busy wait
    }
    
}

void MqttClient::dns_found_callback([[maybe_unused]] const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    assert(callback_arg != nullptr);
    auto* publishContext = reinterpret_cast<MqttPublishContext*>(callback_arg);

    if (ipaddr == nullptr)
    {
        LogError("DNS query failed");
    }
    else
    {
        LogInfo("DNS query returned address {}", ip4addr_ntoa(ipaddr));
        publishContext->host_address = *ipaddr;
    }
    publishContext->dns_lookup_done = true;
}

err_t MqttClient::mqtt_connect(MqttPublishContext& publishContext) const
{
    mqtt_connect_client_info_t client_info
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

    return mqtt_client_connect(&publishContext.mqtt_client, &publishContext.host_address, _port, mqtt_connect_callback, &publishContext, &client_info);
}

void MqttClient::mqtt_connect_callback([[maybe_unused]] mqtt_client_t *client, void *callback_arg, mqtt_connection_status_t status)
{
    assert(callback_arg != nullptr);
    auto* publishContext = reinterpret_cast<MqttPublishContext*>(callback_arg);

    if (status == MQTT_CONNECT_ACCEPTED)
    {
        LogInfo("MQTT connection successful");
        u8_t qos = 1;
        u8_t retain = 1;
        auto err = mqtt_publish(&publishContext->mqtt_client, publishContext->topic.c_str(), publishContext->message.c_str(), publishContext->message.length(), qos, retain, mqtt_publish_callback, callback_arg);
        if (err != ERR_OK)
        {
            LogError("Publish failed with result {}", err);
        }
    }
    else
    {
        LogInfo("MQTT disconnected status {}", static_cast<int>(status));
        publishContext->mqtt_publish_done = true;
    }
}

void MqttClient::mqtt_publish_callback(void *callback_arg, err_t result)
{
    assert(callback_arg != nullptr);
    auto* publishContext = reinterpret_cast<MqttPublishContext*>(callback_arg);

    if (result == ERR_OK)
    {
        LogInfo("MQTT publish succeeded");
    }
    else
    {
        LogError("MQTT publish failed with status {}", result);
    }

    publishContext->mqtt_publish_done = true;
}
