#include <BlynkGOv5.h>

IMAGE_DECLARE(img_btn_released);
IMAGE_DECLARE(img_btn_pressed);

IMAGE_DECLARE(img_sw_on);
IMAGE_DECLARE(img_sw_off);

GImageButton btn(img_btn_released, img_btn_pressed);
GImageButton sw(BUTTON_SWITCH, img_sw_off, img_sw_on);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  btn.onClicked(GWIDGET_CB{
    Serial.println("[btn] clicked");
  });

  sw.align(btn, ALIGN_BOTTOM, 0, 30);
  sw.onValueChanged(GWIDGET_CB{
    if(sw.isON()){
      Serial.println("[sw] ON");
    }else{
      Serial.println("[sw] OFF");
    }
  });
}

void loop(){
  BlynkGO.update();
}
