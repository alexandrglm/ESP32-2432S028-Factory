#include <WiFi.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft;

const char *ssid = "CMCC-404";
const char *password = "12345678";

WiFiServer server(10000);  // Servidor en puerto 10000
lv_obj_t *btn;             // Declaración global del botón
lv_obj_t *label;           // Declaración global de la etiqueta

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  tft.pushImage(area->x1, area->y1, 
               area->x2 - area->x1 + 1, 
               area->y2 - area->y1 + 1, 
               (uint16_t *)color_p);
  lv_disp_flush_ready(disp);
}

void setup() {
  // Inicialización de pantalla
  tft.begin();
  tft.setRotation(1);
  
  // Inicialización de LVGL
  lv_init();
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  lv_disp_drv_register(&disp_drv);

  // Creación de elementos LVGL
  btn = lv_btn_create(lv_scr_act());
  lv_obj_set_pos(btn, 50, 50);
  lv_obj_set_size(btn, 100, 50);
  label = lv_label_create(btn);
  lv_label_set_text(label, "Panic Button");

  // Configuración WiFi
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started on port 10000");
}

void loop() {
  lv_timer_handler();
  delay(5);

  WiFiClient client = server.available();
  if (client) {
    Serial.println("[Client connected]");
    String readBuff;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        readBuff += c;
        
        if (c == '\r') {
          client.print("Received: " + readBuff);
          Serial.println("Received: " + readBuff);
          readBuff = "";
        }
      }
    }
    
    client.stop();
    Serial.println("[Client disconnected]");
  }
}
