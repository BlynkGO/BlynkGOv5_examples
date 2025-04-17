#include <BlynkGOv5.h>

GQRCode qrcode;

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  qrcode.encodeWiFi("MyWiFi","11112222");
  
}

void loop() {
  BlynkGO.update();
}
