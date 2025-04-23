/* ปรับแต่งกราฟิกดีไซน์ โดยใช้ GStyle เป็นการกำหนดที่ GStyle แล้วสามารถนำไปใช้ร่วมกับ วิตเจ็ตต่างๆ ได้ โดยลดการเปลืองเม็มไม่ต้องกำหนดแบบแยกกัน */

#include <BlynkGOv5.h>

GButton btn1;
GButton btn2;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  GScreen.layout(LAYOUT_COL_M, 0, 0, 0, 0, 30);
  GScreen.content_align(CONTENT_ALIGN_CENTER);

  static GStyle  style_default;
    style_default.radius(8);
    style_default.opa(OPA_100);
    style_default.color( TFT_PALETTE(TFT_PALETTE_BLUE));
    style_default.grad_color( TFT_PALETTE_DARKEN(LV_PALETTE_BLUE, 2));
    style_default.grad_dir(GRAD_DIR_VER);

    style_default.border_opa(OPA_40);
    style_default.border(2, TFT_PALETTE(TFT_PALETTE_GRAY));

    style_default.shadow(8, TFT_PALETTE(TFT_PALETTE_GRAY));
    
    style_default.shadow_offset(0,8);

    style_default.outline_color(TFT_PALETTE(TFT_PALETTE_BLUE));
    style_default.outline_opa(OPA_COVER);

    style_default.padding(50);

  static GStyle style_pressed;
 
    style_pressed.outline(30);
    style_pressed.outline_opa(OPA_0);

    style_pressed.translate_y(5);
    style_pressed.shadow_offset(0,3);
    style_pressed.color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,2));
    style_pressed.grad_color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE,4));

    static GTransition trans;
    static style_prop_t props[] = {GSTYLE_OUTLINE, GSTYLE_OUTLINE_OPA, 0};
    trans.init(props, ANIM_PATH_LINEAR, 300);

    style_pressed.transition(trans);

  btn1 ="ปุ่มกด1";
    btn1.addStyle(style_default , GSTATE_DEFAULT);    // กำหนด style เมื่อปุ่มขณะปกติ
    btn1.addStyle(style_pressed , GSTATE_PRESSED);    // กำหนด style เมื่อปุ่มขณะถูกกด
  
  btn2 ="ปุ่มกด2";
  btn2.addStyle(style_default , GSTATE_DEFAULT);      // ใช้ GStyle ร่วมกับ btn1 ทำให้ประหยัดเม็มได้
  btn2.addStyle(style_pressed , GSTATE_PRESSED);      // ใช้ GStyle ร่วมกับ btn1 ทำให้ประหยัดเม็มได้
}

void loop(){
  BlynkGO.update();
}

