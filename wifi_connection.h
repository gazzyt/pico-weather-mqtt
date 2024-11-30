#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

class WifiConnection
{
public:
    WifiConnection();
    WifiConnection(const WifiConnection& rhs) = delete;
    WifiConnection(WifiConnection&& rhs) = delete;
    ~WifiConnection();
};

#endif
