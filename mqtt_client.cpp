#include <cassert>
#include "lwip/dns.h"
#include "mqtt_client.h"
#include "pico/cyw43_arch.h"

MqttClient::MqttClient(std::string&& host, int port)
:   _host{host}, _port{port}, _host_address{0}, _dns_lookup_done{false}
{

}

MqttClient::~MqttClient()
{

}

void MqttClient::Publish(const std::string& topic, const std::string& message)
{
    if (_host_address.addr == 0)
    {
        dns_lookup();
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
