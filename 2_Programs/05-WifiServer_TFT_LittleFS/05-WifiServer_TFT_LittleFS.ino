// ################################################################
//
//  WiFI Server - LittleFS - TFT/Touch
//
//  For ESP32-2432s028 (ILI9341 3,4" + XPT2046 )
//  
// ################################################################

// ******************************************
// IMPORTS
// *****************************************
#include <Arduino.h>

#include <SPI.h>
#include <FS.h>
#include <memory>

#include <WiFi.h>
#include <esp32-hal.h>

#include <LittleFS.h>
#include <SPIFFS.h>
#include <SD.h>

#include <ArduinoJson.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
// These are local TFT-TOUCH main initializers.
// >>>> IMPORTANT:  Avoid using library User_Setup, or copy sreen_init.h -> to User_Setup.h
#include "screen_init.h"
#include "TFT_ILI9341_component.h"

#ifndef XPT2046_CS
    #include "TOUCH_XPT2046_component.h"
#endif

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "wlan.h"
#include "ip.h"


// ################################################################
// GLOBALS
// ################################################################
const int serverPort = 80;
const char* indexPage = "index.html";
AsyncWebServer server(serverPort);

class MenuComponent;
extern TFT_eSPI tft;
extern MenuComponent menu;

WifiConfig wifiConfig;

bool serverActive = false;


// ******************************************
// TFT MENUS
// *****************************************
class MenuComponent {
private:

    TFT_eSPI &tft;

public:

    // My first Constructor in C++
    String menuSSID;
    String menuIP;
    MenuComponent(TFT_eSPI &display): tft(display) {    

        networkData();
    
    }

    void networkData() {

        menuSSID = WiFi.SSID();
        menuIP = WiFi.localIP().toString();
    }


    void startMenu() {

        tft.fillRect(0, 0, tft.width(), 30, TFT_YELLOW);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("SERVER STARTED", tft.width() / 2, 15);

        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_CYAN);
        tft.drawString("- WiFi Server - :  Connecting ...", tft.width() / 2, tft.height() / 2 - 20);

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextColor(TFT_ORANGE);
        tft.drawString("WiFi Server is connecting to AP ...", tft.width() / 2, tft.height() / 2 + 20);

        tft.setTextColor(TFT_ORANGE);
        tft.drawString(menuSSID.c_str(), tft.width() / 2, tft.height() / 2 + 40);
    }


    void serverMenuON() {

        networkData();

        tft.fillRect(0, 0, tft.width(), 30, TFT_GREENYELLOW);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_BLACK);
        tft.drawString("- WiFi Server - :  ONLINE", tft.width() / 2, 15);

        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_BLACK);
        tft.drawString("- WiFi Server is running! :-D", tft.width() / 2, tft.height() / 2 - 20);

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Connected to:", tft.width() / 2, 100);

        tft.setTextColor(TFT_GREENYELLOW);
        tft.drawString("SSID: " + String(menuSSID), tft.width() / 2, 125);
        tft.drawString("IP: " + String(menuIP), tft.width() / 2, 145);
    
    }

    void disconnectAndRetry() {

        tft.fillRect(0, 0, tft.width(), 30, TFT_RED);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("WiFI DISCONNECTED !!!!", tft.width() / 2, 15);

        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_CYAN);
        tft.drawString("Retrying connection  ...", tft.width() / 2, tft.height() / 2 - 20);

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextColor(TFT_ORANGE);
        tft.drawString("Attempt WiFi AP :", tft.width() / 2, tft.height() / 2 + 20);

        tft.setTextColor(TFT_ORANGE);
        tft.drawString(menuSSID.c_str(), tft.width() / 2, tft.height() / 2 + 40);
    }

    void fatalERROR() {

        Serial.println(" -- FATAL ERROR --\nReboot device...");

        tft.fillScreen(TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextSize(2);


        bool blinkState = false;
        uint16_t blinkColor = TFT_RED;
        unsigned long previousMillis = 0;
        const long blinkInterval = 500;

        while(true) {

            unsigned long currentMillis = millis();

            if(currentMillis - previousMillis >= blinkInterval) {

                previousMillis = currentMillis;
                blinkState = !blinkState;

                blinkColor = blinkState ? TFT_BLUE : TFT_RED;
                tft.fillRect(0, 0, tft.width(), 30, blinkColor);

                tft.setTextColor(TFT_WHITE, blinkColor);
                tft.drawString("FATAL ERROR !!!", tft.width() / 2, 15);
            }

            tft.setTextColor(TFT_WHITE);
            tft.drawString("Fatal error ocurred!", tft.width() / 2, tft.height() / 2 - 20);
            
            tft.setTextColor(TFT_YELLOW);
            tft.drawString("Check logs, WiFi, Screen, LittleFS.", tft.width() / 2, tft.height() / 2 + 10);
            
            tft.setTextColor(TFT_WHITE, TFT_RED);
            tft.setTextSize(1);
            tft.drawString("Reset device to continue", tft.width() / 2, tft.height() - 20);

            delay(50);
        }        
    }

    void displayStatsOnTFT() {

      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_GREEN);
      tft.setCursor(0, 180);
      tft.printf("RAM: %u bytes\n", ESP.getFreeHeap());
      tft.printf("CPU: %.1f%%\n", 100.0 - (millis() % 100));
      tft.printf("WiFi: %d dBm", WiFi.RSSI());
    }

};
MenuComponent menu(tft);


// ******************************************
// WLAN - IP componets
// *****************************************
void wifiComponent() {

  menu.startMenu();

  setupIPv4();

  Serial.printf("\n[WIFI] -> Connecting to: '%s'...\n", wifiConfig.WLAN_SSID);

  WiFi.begin(
    wifiConfig.WLAN_SSID,
    wifiConfig.WLAN_PASS,
    // wifiConfig.WLAN_CHANNEL,
    // wifiConfig.WLAN_BSSID,
    wifiConfig.WLAN_HIDDEN);

  // Debounce 1
  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {

    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("\nConnected! :-D");
    Serial.print("[SERVER IP] ->   %s"), Serial.println(WiFi.localIP());

  } else {

    Serial.printf("\n[ERROR] -> Wlan/IP error:  %d\n", WiFi.status());
  }
}


// ******************************************
// LittleFS Component
// *****************************************
void littleFsComponent() {

  if(!LittleFS.begin()){

    Serial.println("Error al montar LittleFS");
    menu.fatalERROR();

  }

  Serial.println("[LittleFS]  -> Initialized! ");
}


// *****************************************
// WEB SERVER Component 
// *****************************************

void serverComponent() {

    // Enabled for these MIMEs
    auto getContentType = [](const String& path) {
        if (path.endsWith(".html")) return "text/html";
        if (path.endsWith(".css")) return "text/css";
        if (path.endsWith(".js")) return "application/javascript";
        if (path.endsWith(".png")) return "image/png";
        if (path.endsWith(".jpg") || path.endsWith(".jpeg")) return "image/jpeg";
        if (path.endsWith(".gif")) return "image/gif";
        if (path.endsWith(".ico")) return "image/x-icon";
        if (path.endsWith(".svg")) return "image/svg+xml";
        if (path.endsWith(".json")) return "application/json";
        if (path.endsWith(".txt")) return "text/plain";
        if (path.endsWith(".pdf")) return "application/pdf";
        return "text/plain";
    };

    // TFT menuON
    menu.serverMenuON();

    // ALL ROUTES
    server.onNotFound([getContentType](AsyncWebServerRequest *request) {
        
        String path = request->url();
        
        Serial.printf("[WEB] GET : %s , 200 OK\n", path.c_str());

        // INDEX
        if (path.endsWith("/")) path += indexPage;  


        if (LittleFS.exists(path)) {
        
            request -> send(LittleFS, path, getContentType(path));
            Serial.printf("[DEBUG/WEB] REQUEST: %s\ n", path.c_str());
        
        } else {

            Serial.printf("\n[ERROR/WEB] REQUEST: %s , 404 \n", path.c_str());
          
            if (LittleFS.exists("/404.html")) {
          
                request -> send(LittleFS, "/404.html", "text/html");
          
            } else {
                request->send(404, "text/plain", "404: File Not Found\n" + path);
            }
        }
    });

    server.begin();
    serverActive = true;

    Serial.println("[SERVER] Started at http://" + WiFi.localIP().toString());
}


// *****************************************
// SERVER STATS html
// *****************************************

class ServerStats {

private:
  
    AsyncWebServer* server;
  
    unsigned long lastUpdate = 0;
    const float updateInterval = 1000;
    bool sdAvailable = false;

    struct AllTheMetrics {
  
        float cpuUsage = 0;
        float temperature = 0;
        uint32_t freeHeap = 0;
        uint32_t minHeap = 0;
        uint32_t maxAllocHeap = 0;
        uint32_t psramTotal = 0;
        uint32_t psramFree = 0;
        int32_t wifiRssi = 0;
        String wifiIP;
        String wifiSSID;
        size_t littlefsTotal = 0;
        size_t littlefsUsed = 0;
        size_t spiffsTotal = 0;
        size_t spiffsUsed = 0;
  
    } metrics;

public:

    ServerStats(AsyncWebServer* srv) : server(srv) {}

    void begin() {

        LittleFS.begin();
        SPIFFS.begin();

        sdAvailable = SD.begin();


        // API CALLS
        server -> on("/api/stats", HTTP_GET, [this](AsyncWebServerRequest *request){

            request -> send(200, "application/json", getMetricsJSON());
        
        });

        server -> on("/api/files", HTTP_GET, [this](AsyncWebServerRequest *request) {
            
            if (!request->hasParam("fs")) {
            
                request -> send(400, "application/json", "{\"error\":\"Missing fs parameter\"}");
                return;
            }

            String fsType = request->getParam("fs")->value();
           
            File root;
            
            if (fsType == "littlefs") root = LittleFS.open("/");
            
            else if (fsType == "spiffs") root = SPIFFS.open("/");
            
            else if (fsType == "sd") {
            
                if (!sdAvailable) {
            
                    request -> send(200, "application/json", "[]");
                    return;
            
                }
                root = SD.open("/");
            } else {

                request -> send(400, "application/json", "{\"error\":\"Invalid fs\"}");
                return;
            }

            DynamicJsonDocument doc(1024);
            JsonArray arr = doc.to<JsonArray>();
            File file = root.openNextFile();
            
            
            while (file) {
            
                JsonObject obj = arr.createNestedObject();
            
                obj["name"] = String(file.name());
                obj["size"] = file.size();
            
                file = root.openNextFile();
            }
            
            String out;
            
            serializeJson(doc, out);
            request->send(200, "application/json", out);
        
        });

        
        // API DELETE FILE/FOLDER
        server -> on("/api/files", HTTP_DELETE, [this](AsyncWebServerRequest *request) {
            
            if (!request -> hasParam("fs") || !request -> hasParam("name")) {
            
                request -> send(400, "application/json", "{\"error\":\"Missing parameters\"}");
                return;
            }
            
            String fsType = request->getParam("fs")->value();
            
            String name = request->getParam("name")->value();

            bool success = false;
            
            if (fsType == "littlefs") success = LittleFS.remove(name);
            
            else if (fsType == "spiffs") success = SPIFFS.remove(name);
            
            else if (fsType == "sd" && sdAvailable) success = SD.remove(name);

            
            // DELETE RESPONSES  
            if (success) request -> send(200, "application/json", "{\"status\":\"OK\"}");
            else request -> send(500, "application/json", "{\"error\":\"Delete failed\"}");
        });


        // API FILE DOWNLOAD
        server -> on("/api/files/download", HTTP_GET, [this](AsyncWebServerRequest *request) {
            if (!request -> hasParam("fs") || !request -> hasParam("name")) {
                
                request -> send(400, "application/json", "{\"error\":\"Missing parameters\"}");
                return;
            
            }
            
            String fsType = request->getParam("fs")->value();
            String name = request->getParam("name")->value();
            
            fs::FS* fs = nullptr;

            if (fsType == "littlefs") fs = &LittleFS;
            else if (fsType == "spiffs") fs = &SPIFFS;
            else if (fsType == "sd" && sdAvailable) fs = &SD;

            
            if (!fs || !fs->exists(name)) {
            
                request -> send(404, "application/json", "{\"error\":\"File not found\"}");
                return;
            }

            request -> send(*fs, name, "application/octet-stream", true);
        });


        // API uploads
        server -> on("/api/files/upload", HTTP_POST, 
            [](AsyncWebServerRequest *request){},
            [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
                if (!request->hasParam("fs", true)) return;

                String fsType = request->getParam("fs", true)->value();
                
                fs::FS* fs = nullptr;
                
                if (fsType == "littlefs") fs = &LittleFS;
                
                else if (fsType == "spiffs") fs = &SPIFFS;
                else if (fsType == "sd") fs = &SD;

                if (!fs) return;

                static File uploadFile;
                
                if (index == 0) {
                
                    if (uploadFile) uploadFile.close();
                
                    uploadFile = fs->open("/" + filename, FILE_WRITE);
                
                }
                
                
                if (uploadFile) uploadFile.write(data, len);
                
                if (final && uploadFile) uploadFile.close();
            }
        );
    }

    void update() {

        if(millis() - lastUpdate >= updateInterval) {
            
            updateCPU();
            updateMemory();
            updateTemperature();
            updateNetwork();
            updateFilesystems();
            
            lastUpdate = millis();
        }
    }

    String getMetricsJSON() {
        
        DynamicJsonDocument doc(1024);
        
        doc["memory"]["free"] = metrics.freeHeap;
        doc["memory"]["min"] = metrics.minHeap;
        doc["memory"]["max_alloc"] = metrics.maxAllocHeap;
        doc["memory"]["psram_total"] = metrics.psramTotal;
        doc["memory"]["psram_free"] = metrics.psramFree;
        doc["cpu"]["usage"] = metrics.cpuUsage;
        doc["cpu"]["temperature"] = metrics.temperature;
        doc["network"]["ssid"] = metrics.wifiSSID;
        doc["network"]["ip"] = metrics.wifiIP;
        doc["network"]["rssi"] = metrics.wifiRssi;
        doc["storage"]["littlefs_total"] = metrics.littlefsTotal;
        doc["storage"]["littlefs_used"] = metrics.littlefsUsed;
        doc["storage"]["spiffs_total"] = metrics.spiffsTotal;
        doc["storage"]["spiffs_used"] = metrics.spiffsUsed;
        doc["storage"]["sd_available"] = sdAvailable;
        
        String output;
        
        serializeJson(doc, output);
        return output;
    }

private:

    void updateCPU() {
        
        static unsigned long lastIdle = 0;
        
        unsigned long now = millis();
        
        metrics.cpuUsage = 100.0 - ((now - lastIdle) / (updateInterval / 100.0));
        
        lastIdle = now;
    }

    void updateTemperature() {
        
        metrics.temperature = -1;  // PENDING!!
    
    }

    void updateMemory() {

        metrics.freeHeap = ESP.getFreeHeap();
        metrics.minHeap = ESP.getMinFreeHeap();
        metrics.maxAllocHeap = ESP.getMaxAllocHeap();
      

        // PSRAM
        if (psramFound()) {
      
            metrics.psramTotal = ESP.getPsramSize();
            metrics.psramFree = ESP.getFreePsram();
        }
    
    }

    void updateNetwork() {

        metrics.wifiRssi = WiFi.RSSI();
        metrics.wifiIP = WiFi.localIP().toString();
        metrics.wifiSSID = WiFi.SSID();
    
    }

    void updateFilesystems() {
        
        metrics.littlefsTotal = LittleFS.totalBytes();
        metrics.littlefsUsed = LittleFS.usedBytes();
        metrics.spiffsTotal = SPIFFS.totalBytes();
        metrics.spiffsUsed = SPIFFS.usedBytes();
    
    }
};
ServerStats serverStats(&server); // Instanciación después de la clase


// *****************************************
// CHECKS -> Recover from FATAL ERRORS (or not)
// *****************************************
void healthChecks() {

    static unsigned long lastCheck = 0;
    const unsigned long retryInterval = 10000;

    if (WiFi.status() != WL_CONNECTED && millis() - lastCheck >= retryInterval) {
        
        lastCheck = millis();
        
        menu.disconnectAndRetry(); 
        Serial.println("\n[ERROR/WIFI] -> Connection lost!!!\nCleaning services...");

        // My first Array in C++
        enum CheckStage {
            WIFI_CLEANUP,
            LITTLEFS_CLEANUP,
            SERVER_CLEANUP,
            RECOVER_EVAL,
            RESTART_APP
        };
        
        static CheckStage next = WIFI_CLEANUP;
        bool recoverable = true;

        // My first SWITCH - CASE -> BREAK/CONTINUE in C++
        switch(next) {
        
            case WIFI_CLEANUP:
        
                Serial.println("[CHECK 1/4] Cleaning WiFi...");        
                if (!WiFi.disconnect(true)) {

                    Serial.println("[FATAL ERROR] ->  WiFi.disconnect() not responding!");
                    recoverable = false;
                }
                next = LITTLEFS_CLEANUP;
                break;

            case LITTLEFS_CLEANUP:
                Serial.println("[CHECK 2/4] Unmounting LittleFS...");
                
                LittleFS.end();
                delay(300);
                
                if (LittleFS.exists("/index.html")) {
                    Serial.println("[WARNING]  ->  LittleFS still accessible!");
                    recoverable = false;
                }
                
                next = SERVER_CLEANUP;
                break;

            case SERVER_CLEANUP: {
                Serial.println("[CHECK 3/4] Stopping server...");

                server.end();
                delay(500);
                
                WiFiClient testClient;
                if (testClient.connect("127.0.0.1", serverPort)) {
                    Serial.println("[WARNING] ->  Server port still open!");
                    testClient.stop();
                    recoverable = false;
                }
                next = RECOVER_EVAL;
                break;
            }

            case RECOVER_EVAL:
                Serial.println("[CHECK 4/4] Final verification...");
                
                if (!recoverable) {
                    Serial.println("[FATAL] Critical errors detected!");
                    menu.fatalERROR();
                }

            case RESTART_APP:

                Serial.println("[STATUS] System recoverable, restarting...");
                delay(1000);
                
                programRoutines();
                next = WIFI_CLEANUP;
                break;
        }
        delay(300);
    }  
}

void programRoutines() {

  wifiComponent();
  littleFsComponent();
  serverStats.begin();
  serverComponent();
}



// ******************************************
// For ARDUINO IDE
// *****************************************
void setup() {

  Serial.begin(115200);

  screenInit();

  programRoutines();

}

void loop() {

  healthChecks();
  serverStats.update();
  delay(10);

}

// ******************************************
// For purely C + ESP-IDF (uncomment!)
// ******************************************
// #ifdef ARDUINO_ARCH_ESP32
    
//     extern "C" void app_main(void) {

//         setup();

//         while (true) {
//             loop();
//         }
//     }
// #endif
