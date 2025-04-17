#include <BlynkGOv5.h>

GTextArea ta; 
GKeyboard kbd;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  ta.default_keyboard(false);       // ปิดไม่ใช้ default keyboard  เพื่อจะใช้แป้นพิมพ์จาก GKeyboard
  ta.cursor_show(true);             // ให้แสดง cursor ด้วย เมื่อมีการกดเลือก
  ta.accepted_chars("0123456789:"); // รับการใส่เฉพาะค่า ที่กำหนด
  ta.max_length(5);                 // จำกัดการใส่ค่า ไม่เกิน 5 ตัวอักษร
  ta.align(ALIGN_TOP,0,20);

  kbd.mode(KEYBOARD_MODE_NUMBER);   // ให้แป้นพิมพ์โหมด ตัวเลขทำงาน ขึ้นมา
  kbd.textarea(ta);
  kbd.onValueChanged(GWIDGET_CB{
    if(kbd.selected_button() == LV_SYMBOL_BACKSPACE){
      if(ta.length() ==2 ) {
        ta.backspace();
      } else{
        goto check_colon;
      }
    }else{
      check_colon:
        if( ta[0] >= '0' && ta[0] <= '9' &&
            ta[1] >= '0' && ta[1] <= '9' &&
            ta[2] != ':')
        {
          ta.cursor_pos(2);
          ta.add(':');
        }
    }
  });
}

void loop(){
  BlynkGO.update();
}
