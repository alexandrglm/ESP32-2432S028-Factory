#include "ip.h"
#include <WiFi.h>

void setupIPv4() {
    
    if (!wifiConfig.WLAN_DHCP) {
        WiFi.config(
            wifiConfig.local_ip,
            wifiConfig.gateway,
            wifiConfig.subnet,
            wifiConfig.dns1,
            wifiConfig.dns2
        );
        Serial.println("[SERVER/IP] -> Static IPv4 started!");
    } else {
        Serial.println("[SERVER/IP] -> DHCP IPv4 started!");
    }
}