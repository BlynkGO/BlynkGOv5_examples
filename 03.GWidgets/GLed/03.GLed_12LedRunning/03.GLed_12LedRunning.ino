#include <BlynkGOv5.h>

GContainer cont_led(LAYOUT_ROW_M,8,8,8,8,8);
  const uint8_t led_num = 12;
  GLed led[led_num];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ออกแบบกราฟิก led ทั้ง 12 
  cont_led.bg_opa(0);
  cont_led.radius(RADIUS_CIRCLE);
  cont_led.border(2, TFT_SILVER);
  for(int i=0; i< led_num; i++){
    led[i].size(15,15);
    led[i].color( TFT_COLOR_HSV( map(i,0, led_num-1,0,240),100,100) );
    led[i].parent(cont_led);
  }

  // ตั้งเวลาทำงาน ให้ led วิ่ง
  static SoftTimer timer;
  timer.setInterval(100, [](){
    static int8_t i=~0, step=1;  static bool state=LOW;
    step = (i==0)? 1 : (i== led_num-1)? -1 : step ;
    led[i=(state == LOW )? i + step : i].state(state=!state);
  });
}

void loop() {
  BlynkGO.update();
}

