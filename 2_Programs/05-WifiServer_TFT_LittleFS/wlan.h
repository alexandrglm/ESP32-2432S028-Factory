
#ifndef WLAN_H
#define WLAN_H

#include <WiFi.h>
#include <IPAddress.h>

class WifiConfig {
public:
    // Configuración WiFi básica
    const char* WLAN_SSID = "MyArduinoShitties";
    const char* WLAN_PASS = "Shitties@#@#@#1982";
    // const int WLAN_CHANNEL = 0;
    // const uint8_t WLAN_BSSID[6] = {0x82, 0x2D, 0x1A, 0x19, 0xE4, 0x10};
    const bool WLAN_HIDDEN = true;

    // Configuración IPv4
    bool WLAN_DHCP = true; // false = IP estática
    
    IPAddress local_ip = IPAddress(192, 168, 1, 128);
    IPAddress gateway = IPAddress(192, 168, 1, 1);
    IPAddress subnet = IPAddress(255, 255, 255, 0);
    IPAddress dns1 = IPAddress(1, 1, 1, 1);
    IPAddress dns2 = IPAddress(9, 9, 9, 9);
};

extern WifiConfig wifiConfig;

#endif