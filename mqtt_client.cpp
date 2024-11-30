#include <cassert>
#include "lwip/dns.h"
#include "mqtt_client.h"
#include "pico/cyw43_arch.h"

MqttClient::MqttClient(std::string&& host, int port)
:   _host{host},
    _port{port},
    _host_address{0},
    _dns_lookup_done{false},
    _mqtt_client{0},
    _mqtt_publish_done{false},
    _topic{},
    _message{}
{

}

MqttClient::~MqttClient()
{

}

void MqttClient::Publish(const std::string& topic, const std::string& message)
{
    _topic = topic;
    _message = message;

    if (_host_address.addr == 0)
    {
        dns_lookup();
    }

    auto connect_err = mqtt_connect();

    if (connect_err == ERR_OK)
    {
        while (!_mqtt_publish_done)
        {
            cyw43_arch_poll();
            cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
        }
        
        printf("Disconnect MQTT\n");
        mqtt_disconnect(&_mqtt_client);
    }
    else
    {
        printf("MQTT connect failed with error code %d\n", connect_err);
    }
}

void MqttClient::dns_lookup()
{
    _dns_lookup_done = false;
    auto hostname = _host.c_str();
    printf("Begin DNS lookup for %s\n", hostname);
    auto err = dns_gethostbyname(hostname, &_host_address, dns_found_callback, this);

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

    while (!_dns_lookup_done)
    {
        cyw43_arch_poll();
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
    }
    
}

void MqttClient::dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    assert(callback_arg != nullptr);
    MqttClient* mqttClient = reinterpret_cast<MqttClient*>(callback_arg);

    if (ipaddr == nullptr)
    {
        printf("DNS query failed\n");
    }
    else
    {
        printf("DNS query returned address %s\n", ip4addr_ntoa(ipaddr));
        mqttClient->_host_address = *ipaddr;
    }
    mqttClient->_dns_lookup_done = true;
}

err_t MqttClient::mqtt_connect()
{
    _mqtt_publish_done = false;
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

    return mqtt_client_connect(&_mqtt_client, &_host_address, _port, mqtt_connect_callback, this, &ci);
}

void MqttClient::mqtt_connect_callback(mqtt_client_t *client, void *callback_arg, mqtt_connection_status_t status)
{
    assert(callback_arg != nullptr);
    MqttClient* mqttClient = reinterpret_cast<MqttClient*>(callback_arg);

    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("MQTT connection successful\n");
        u8_t qos = 1;
        u8_t retain = 0;
        auto err = mqtt_publish(&mqttClient->_mqtt_client, mqttClient->_topic.c_str(), mqttClient->_message.c_str(), mqttClient->_message.length(), qos, retain, mqtt_publish_callback, callback_arg);
        if (err != ERR_OK)
        {
            printf("Publish failed with result %d\n", err);
        }
    }
    else
    {
        printf("MQTT disconnected status %d\n", status);
        mqttClient->_mqtt_publish_done = true;
    }
}

void MqttClient::mqtt_publish_callback(void *callback_arg, err_t result)
{
    assert(callback_arg != nullptr);
    MqttClient* mqttClient = reinterpret_cast<MqttClient*>(callback_arg);

    if (result == ERR_OK)
    {
        printf("MQTT publish succeeded\n");
    }
    else
    {
        printf("MQTT publish failed with status %d\n", result);
    }

    mqttClient->_mqtt_publish_done = true;
}
