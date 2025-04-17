#include <BlynkGOv5.h>

GTextArea     ta;
GButtonMatrix btnm;
  const char* btnm_map[] = {"1", "2", "3", "\n",
                            "4", "5", "6", "\n",
                            "7", "8", "9", "\n",
                            SYMBOL_BACKSPACE, "0", SYMBOL_NEW_LINE, "" 
                            };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  ta.default_keyboard(false);       // ปิดไม่ใช้ default keyboard  เพื่อจะใช้แป้นพิมพ์จาก GButtonMatrix ที่สร้างเอง
  ta.align(ALIGN_TOP,0,10);
  ta.onReady(GWIDGET_CB{
    Serial.println(ta.text());
  });

  btnm.size(200,150);
  btnm.map(btnm_map);
  btnm.align(ALIGN_BOTTOM,0,-10);
  btnm.onValueChanged(GWIDGET_CB{
    String text = btnm.selected_button();
    if(text == SYMBOL_BACKSPACE){
      ta.backspace();
    }else if(text == SYMBOL_NEW_LINE) {
      ta.event_send(EVENT_READY);
    }else{
      ta.add(text);
    }
  });
}

void loop(){
  BlynkGO.update();
}

