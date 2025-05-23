// ################################################################
//
//  BLE LED Server Test - ESP32 (TFT + Touch + BLE control)
//
//  For ESP32-2432s028 (ILI9341 3,4" + XPT2046 )
//  
// ################################################################

// ******************************************
// IMPORTS
// ******************************************

// >>>>> IMPORTANT
//#include <Arduino.h> 
// Uncomment this if you wanna try this using ESP-IDF IDE, also uncomment the final 'C' starter

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
// These are local TFT-TOUCH main initializers. 
// >>>> IMPORTANT:  Avoid using library User_Setup, or copy sreen_init.h -> to User_Setup.h
#include "include/screen_init.h"
#include "include/TFT_ILI9341_component.h"

#ifndef XPT2046_CS
    #include "include/TOUCH_XPT2046_component.h"
#endif

// >>>> IMPORTANT: Configure desired BLE Name and GPIO pins
#include "BLE_data.h"
#include "GPIO_data.h"


// ******************************************
//  GLOBAL VARS. INIT
// ******************************************

BLEServer *pServer = nullptr;
BLECharacteristic *pLedTrigger = nullptr;

bool deviceConnected = false;


// ******************************************
// GPIO -> LED COMPONENT
// ******************************************
void setupGPIO() {
  
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    
    Serial.println("[GPIO/LED] Initial status: OFF");

}

// ******************************************
// MENUS COMPONENT
// ******************************************
class ScreenCallbacks {
public:

    void MenuBroadcast() {

        tft.fillRect(0, 0, tft.width(), 30, TFT_ORANGE);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_BLACK);
        tft.drawString("BROADCASTING ...", tft.width() / 2, 15);

        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_CYAN);
        tft.drawString("- BLE LED Server -", tft.width() / 2, tft.height() / 2 - 20);

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextColor(TFT_ORANGE);
        tft.drawString("Waiting for BLE connection ...", tft.width() / 2, tft.height() / 2 + 20);

    }

    void MenuConnect() {

        tft.fillRect(0, 0, tft.width(), 30, TFT_BLUE);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("CLIENT CONNECTED", tft.width() / 2, 15);

        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_CYAN);
        tft.drawString("- BLE LED Server -", tft.width() / 2, tft.height() / 2 - 20);

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextColor(TFT_GREEN);
        tft.drawString("Client Connected!", tft.width() / 2, tft.height() / 2 + 20);


    }

    void MenuDisconnect() {

        tft.fillRect(0, 0, tft.width(), 30, TFT_RED);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("CLIENT DISCONNECTED", tft.width() / 2, 15);

        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_CYAN);
        tft.drawString("- BLE LED Server -", tft.width() / 2, tft.height() / 2 - 20);

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextColor(TFT_RED);
        tft.drawString("Client Disconnected!", tft.width() / 2, tft.height() / 2 + 20);
    }

    void MenuWriteON(){

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_GREEN);
        tft.drawString("LED status:  ON", tft.width() / 2, tft.height() / 2);
    }

    void MenuWriteOFF() {

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_RED);
        tft.drawString("LED status:  OFF", tft.width() / 2, tft.height() / 2);
    }

    void MenuWriteERROR() {

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_YELLOW);
        tft.drawString("INVALID CMD!", tft.width() / 2, tft.height() / 2 - 20);
        tft.drawString("Use 'ON' / 'OFF'", tft.width() / 2, tft.height() / 2 + 20);
    }

    void MenuConfigSent() {

        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_CYAN);
        tft.drawString("BLE Service Configs", tft.width() / 2, tft.height() / 2 - 20);
        tft.drawString("sent to the client!'", tft.width() / 2, tft.height() / 2 + 20);
            
    }

};

// ===========================================
// BLE COMPONENT
// ===========================================
class MyServerCallbacks: public BLEServerCallbacks {

public:
        
    void onBroadcast(BLEServer* pServer) {

        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

        ScreenCallbacks().MenuBroadcast();
        
        pAdvertising->addServiceUUID(LED_SERVICE_UUID);
        pAdvertising->start();
        
    }
    
    void onConnect(BLEServer* pServer) {
        
        deviceConnected = true;

        ScreenCallbacks().MenuConnect();
        
        Serial.println("[BLE] -> Device Connected!");
        pServer -> getAdvertising() -> stop();
    
    }

    void onDisconnect(BLEServer* pServer) {
        
        deviceConnected = false;
        Serial.println("[BLE] -> Device Disconnected!");

        ScreenCallbacks().MenuDisconnect();

        delay(3000);

        onBroadcast(pServer);
        pServer -> startAdvertising();

    }
};

class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {

        std::string value = pCharacteristic -> getValue().c_str();

        if (value == "ON") {

            digitalWrite(LED_PIN, LOW);
            Serial.println("[LED] -> Turned ON");

            pLedTrigger->setValue("1");
            pLedTrigger->notify();

            ScreenCallbacks().MenuWriteON();



        } else if (value == "OFF") {

            digitalWrite(LED_PIN, HIGH);
            Serial.println("[LED] -> Turned OFF");

            pLedTrigger->setValue("0");
            pLedTrigger->notify();

            ScreenCallbacks().MenuWriteOFF();

        } else {

            Serial.println("\n[ERROR/LED] -> Invalid command!\nUse 'ON' / 'OFF' !");

            ScreenCallbacks().MenuWriteERROR();
        }
    }

    void onRead(BLECharacteristic *pCharacteristic) {

        Serial.println("[BLE] -> Config sent to Client");

        ScreenCallbacks().MenuConfigSent();
    }
};


void setupBLE() {

    MyServerCallbacks* myCallbacks = nullptr;

    BLEDevice::init(bleServerName);
    pServer = BLEDevice::createServer();

    myCallbacks = new MyServerCallbacks();
    pServer -> setCallbacks(myCallbacks);

    BLEService *pLedService = pServer->createService(LED_SERVICE_UUID);

    pLedTrigger = pLedService->createCharacteristic(
        LED_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pLedTrigger->setValue("0");
    pLedTrigger->addDescriptor(new BLE2902());
    pLedTrigger->setCallbacks(new MyCharacteristicCallbacks());

    pLedService->start();

    myCallbacks -> onBroadcast(pServer);
  
}

// ******************************************
// MAIN APP ROUTINES
// ******************************************
void startServer() {
    
    setupBLE();
    
    setupGPIO();

    if ( pServer != nullptr) {

        MyServerCallbacks().onBroadcast(pServer);
    
    }

};

// ******************************************
// and.. ARDUINO inits
// ******************************************
void setup() {

    Serial.begin(115200);

    initTFTandTouch();

    startServer();

}

void loop() {
    delay(1000);
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
