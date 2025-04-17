#include <BlynkGOv5.h>

GTextArea ta; 
GKeyboard kbd;


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  ta.width(PCT(80));
  ta.align(ALIGN_TOP,0,10);

  //-----------------------------------
  // คำสั่ง ตำแหน่ง / ขนาด 
  // และ ผูก GTextArea ที่ต้องการให้ GKeyboad ทำการแก้ไข
  //------------------------
  kbd.height(PCT(70));
  kbd.textarea(ta);

  //-----------------------------------
  // คำสั่งปรับแต่ง design
  //-------------------------
  // kbd.btn_border(1, TFT_BLACK);
  // kbd.ctrl_radius(RADIUS_CIRCLE);
  // kbd.ctrl_color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_BLUE,2));
  
  // หากจะสลับไปใช้ แป้นพิมพ์ มาตรฐาน
  // kbd.mode(KEYBOARD_MODE_EN);      // แป้นพิมพ์อังกฤษ
  // kbd.mode(KEYBOARD_MODE_TH);      // แป้นพิมพ์ไทย
  // kbd.mode(KEYBOARD_MODE_NUMPAD);  // แป้นพิมพ์ตัวเลข

}

void loop(){
  BlynkGO.update();
}

