#include <BlynkGOv5.h>

FONT_DECLARE(BebasNeueBold_num_45);

#define KEY_DEL       SYMBOL_BACKSPACE

GContainer cont_numpad;
  GTextArea ta_numpad(cont_numpad);
  GButtonMatrix numpad(cont_numpad);
    const char * numpad_map[] = 
      { "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        ".", "0", KEY_DEL, ""
      };
float price;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  cont_numpad.layout(LAYOUT_COL_M,10,10,10,10,10);
  cont_numpad.color(TFT_COLOR_HSV(0,0,10));
  cont_numpad.border(1,TFT_SILVER);
    ta_numpad.width(300);
    ta_numpad.font(BebasNeueBold_num_45);
    ta_numpad.default_keyboard(false);  // ไม่ใช้ default keyboard
    ta_numpad.text_align(TEXT_ALIGN_RIGHT); 
    ta_numpad.onValueChanged(GWIDGET_CB{           // หาก ta_numpad มีการเปลี่ยนแปลงค่า
      price = ta_numpad.toFloat();                  // ดึงค่า ta_numpad ออกมาเป็น float ให้ price
      Serial.printf("[Price] %.2f\n", price);       // แสดง price ออก Serial Monitor
    });
    ta_numpad = 0;                                  // ค่าเริ่มต้นให้ ta_numpad
    numpad.size(ta_numpad.width(), 300);
    numpad.map(numpad_map);
    numpad.onValueChanged(GWIDGET_CB{
      uint32_t  selected_id     = numpad.selected_id();
      String    selected_button = numpad.selected_button();
      Serial.printf("[NUMPAD] ปุ่มที่กด [%d] %s\n", selected_id, selected_button.c_str());
      if(selected_button == KEY_DEL){     // หากเป็น key ลบ
        ta_numpad.backspace();            // ให้ลบ ta_numpad ออกไป 1 ตัวจากท้าย
        if(ta_numpad.length() == 0) ta_numpad = 0;  // หากถูกลบจนหมด ให้ใส่ 0 เข้าไปใน ta_numpad
      }else{
        if(ta_numpad.toFloat() == 0) ta_numpad = selected_button;  // หากค่าเดิมเป็น 0 ให้ ใส่ค่า key เข้าไปแทน
        else                         ta_numpad += selected_button; // ให้เพิ่ม key อื่นๆ ให้เพิ่มเข้าไปที่ ta_numpad 
      }
    });  
}
void loop(){
  BlynkGO.update();
}

