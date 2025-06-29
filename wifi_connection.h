#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

class WifiConnection
{
public:
    WifiConnection();
    WifiConnection(WifiConnection&& rhs) = delete;
    WifiConnection(const WifiConnection& rhs) = delete;
    WifiConnection& operator=(const WifiConnection& rhs) = delete;
    WifiConnection& operator=(WifiConnection&& rhs) = delete;
    ~WifiConnection();

private:
    inline static constexpr unsigned int max_connection_retries = 3;
    bool is_connected;
    static const char* status_name(int status);
};

#endif
