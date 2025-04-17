#include <BlynkGOv5.h>
#include "project.h"

GButton btn_admin;

GRect rect_admin;
  GLabel lb_admin(rect_admin);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  btn_admin  = "หน้า Admin";
  btn_admin.onClicked(GWIDGET_CB{
    PASSCODE::onReady(123456, [](){
      rect_admin.show(true);
    });
    PASSCODE::onCancel([](){
      Serial.println("[Admin] cancel to login");
    });
  });

  rect_admin.show(false);
  rect_admin.color(0xbbb);
  rect_admin.onClicked(GWIDGET_CB{ rect_admin.show(false); });
    lb_admin = "หน้า Admin";

}

void loop(){
  BlynkGO.update();
}

