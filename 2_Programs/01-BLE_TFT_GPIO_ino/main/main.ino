// ################################################################
//
//  BLE Server Test -> ARDUINO mode.
//
//  Directly for ESP32-2432s028 (ILI9341 3,4" + XPT2046 )
//  
// ################################################################

// ############################################
// IMPORTS
// ############################################

// TFT
#include <SPI.h>
/*
 *
 * TFT_eSPI.h directo de librerias,
 * PERO,
 * el User_setup.h debe estar:
 * 
 *    -> En la propia folder TFT_eSPI de las librerías
 *            o
 *    -> Al Mismo nivel que este script
 */
#include <TFT_eSPI.h>
                      
// BLE
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// GPIO (realmente no es necesario)
#include <driver/gpio.h> 

// IMPORT DE VARIABLES LOCALES
/*
 *  BLE_data    ->  Export de la config para BLE
 *  GPIO_data.h ->  Modular, para implementar futuras acciones en GPIO
 *
 */
#include "BLE_data.h"
#include "GPIO_data.h"


// ############################################
// Initializations
// ############################################

// TFT
TFT_eSPI tft = TFT_eSPI();

// FUNCIONES
/*
 * Establecemos DOS funciones/puntero: 
 *
 * pServer -> Para todas las acciones BLE
 * pLedTrigger -> Para todos los eventos GPIO del LED elegido
 */
BLEServer *pServer = NULL;
BLECharacteristic *pLedTrigger = NULL;

// VARIABLES NECESARIAS A DECLARAR
bool deviceConnected = false;


// ############################################
// CLASSes
// ############################################
// BLE SERVER connect / disconnect
class MyServerCallbacks: public BLEServerCallbacks {

  void onConnect(BLEServer* pServer) {
    
    deviceConnected = true;
    
    Serial.println("[BLE] -> Device Connected!");
    
    tft.fillRect(0, 0, tft.width(), 30, TFT_BLUE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("CLIENT CONNECTED", tft.width() / 2, 15);
    
    

    pServer -> getAdvertising() -> stop();
    Serial.println("[BLE] -> Stopping availability broadcast");
  };

  
  void onDisconnect(BLEServer* pServer) {
 
    deviceConnected = false;
 
    Serial.println("[BLE] -> Device Disconnected!");

    tft.fillRect(0, 0, tft.width(), 30, TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("CLIENT DISCONNECTED", tft.width() / 2, 15);


    pServer -> startAdvertising();
    Serial.println("[BLE] -> BROADCAST started ...");
  }
};

// Events onRead / onWrite
class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) {


    std::string value = pCharacteristic -> getValue().c_str(); // La conversion a std:string es opbligatoria


    // Validacion de entrada
    if (value.length() > 0) {

      Serial.print("[BLE] -> Command RECEIVED:  ");
      
      // Un for con contador clásico, pero en C
      for ( int index = 0; index < value.length(); index++) {
      
        Serial.print(value[index]);
      
      }
      
      Serial.println();


      /*
       *
       * Lógicas rápidas:
       * 
       * ON - TRUE - 1    -> Va a LOW
       * OFF - FALSE - 0  -> Va a HIGH
       * 
       * Por el motivo que sea, esta placa va a su puta bola con las señales H/L de casi todo
       * Entonces:
       * 
       * if ON -> pedimos ON (low) -> Consola / BLE / TFT actualizan
       * if OFF -> pedimos OFF (high) -> Consola / BLE / TFT actualizan
       * 
       * Enviamos los comandos como STRING/UTF-8/etc
       * 
       */
      if ( value == "ON" ) {

        digitalWrite(LED_PIN, LOW);

        // Consola
        Serial.println("[LED] -> Turned ON!"); // Todavia no sé interpolar strings en C

        // BLE  
        pLedTrigger -> setValue("1");
        pLedTrigger -> notify();

        // TFT
        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_GREEN);
        tft.drawString("LED status:  ON", tft.width() / 2, tft.height() / 2);
      
      
      } else if (value == "OFF") {

        digitalWrite(LED_PIN, HIGH);

        // Consola
        Serial.println("[LED] -> Turned OFF!");
        
        // BLE
        pLedTrigger -> setValue("0");
        pLedTrigger -> notify();

        // TFT
        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("LED status:  OFF", tft.width() / 2, tft.height() / 2);
      
      
      // Validación final, cualquier otro caso
      } else {
        
        Serial.println("[LED] -> Invalid command.\n Use 'ON' / 'OFF'");
        
        tft.fillRect(0, 30, tft.width(), tft.height() - 30, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_YELLOW);
        tft.drawString("INVALID CMD!", tft.width() / 2, tft.height() / 2 - 20);
        tft.drawString("Use 'ON' / 'OFF'", tft.width() / 2, tft.height() / 2 + 20);
      }
    }
  }

  void onRead(BLECharacteristic *pCharacteristic) {
    Serial.println("[BLE] Config. sent to Client");
  }
};



// ############################################
// Inits para Arduino
// ############################################
void setup() {

  // Serial ON
  Serial.begin(115200);
  Serial.println("Welcome to BLE LED SERVER-v0.1 app\n\n :-)\n");

  // TFT inits
  Serial.println("TFT: Initializing TFT...");
  
  tft.begin();
  tft.setRotation(0); 
  tft.invertDisplay(1); // Por el mismo problema de los HIGH/LOW,con esta placa, en pantalla, SIEMPRE invertimos de manera gflobal

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);

  // Text inits
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_CYAN);
  tft.drawString("- BLE LED Server -", tft.width() / 2, tft.height() / 2 - 20);
  tft.setTextColor(TFT_ORANGE);
  tft.drawString("Waiting for BLE connection ...", tft.width() / 2, tft.height() / 2 + 20); // Dibuja el texto centrado

  Serial.println("Main program is initialised!\n\nWaiting for BLE connections ...");


  // GPIO inits
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("[GPIO/LED] Initial status:  OFF");



  // BLE -> Server init
  Serial.println("[BLE] -> Server starting");

  BLEDevice::init(bleServerName);
  
  

  // BLE -> Server starting
  Serial.println("[BLE/Server] -> Creating new server");

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  Serial.print("[BLE] Server started\n");
  Serial.println(bleServerName);



  // BLE/Servicios -> 1. Led
  Serial.println("[BLE/Services] -> Creating LED Services");

  BLEService *pLedService = pServer -> createService(LED_SERVICE_UUID);
  
  Serial.print("[BLE/Service] -> LED Service created : ");
  Serial.println(LED_SERVICE_UUID);

          // Toda esta parte debería ir en una Class propia, o por .h aparte
  Serial.println("[BLE/Service] -> Creating Service properties (READ/WRITE/NOTIFY)");
  
  pLedTrigger = pLedService->createCharacteristic(
    LED_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
    );
    
  Serial.print("[BLE/Service] -> LED Service properties done! : ");
  Serial.println(LED_CHARACTERISTIC_UUID);


  pLedTrigger -> setValue("0");
  pLedTrigger -> addDescriptor(new BLE2902());
  pLedTrigger -> setCallbacks(new MyCharacteristicCallbacks());
  
  pLedService -> start();
  
  Serial.println("\n[BLE/Service] -> Service already working!\n");



  // BROADCASTING init
 
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  pAdvertising -> addServiceUUID(LED_SERVICE_UUID);
  pAdvertising->start();
  
  Serial.println("[BLE/Broadcast] -> Started! (Check in BT devices)");



  // TFT -> BLE BROADCASTING init
  tft.fillRect(0, 0, tft.width(), 30, TFT_ORANGE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK);
  
  tft.drawString("BROADCASTING ...", tft.width() / 2, 15);
}

void loop() {
  
  delay(10);
}
