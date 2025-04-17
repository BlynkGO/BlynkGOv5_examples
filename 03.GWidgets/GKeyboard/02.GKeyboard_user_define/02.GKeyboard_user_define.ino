/*
 *  สามารถกำหนด keyboard map อิสระ ได้ะอง 4 แบบ
 *  โดยกำหนด ให้ KEYBOARD_MODE_USER_1, KEYBOARD_MODE_USER_2, KEYBOARD_MODE_USER_3, KEYBOARD_MODE_USER_4
 */

#include <BlynkGOv5.h>

GTextArea ta; 
GKeyboard kbd;
  /*Create an AZERTY keyboard map*/
  static const char * kb_map[] = {"A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", SYMBOL_BACKSPACE, "\n",
                                  "Q", "S", "D", "F", "G", "J", "K", "L", "M",  SYMBOL_NEW_LINE, "\n",
                                  "W", "X", "C", "V", "B", "N", ",", ".", ":", "!", "?", "\n",
                                  SYMBOL_CLOSE, " ",  " ", " ", SYMBOL_OK, NULL
                                  };

  /*Set the relative width of the buttons and other controls*/
  static const keyboard_ctrl_t kb_ctrl_map[] =  
                                  { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, KEYBOARD_CTRL_CHECKED | 6,
                                    4, 4, 4, 4, 4, 4, 4, 4, 4, KEYBOARD_CTRL_CHECKED | 6,
                                    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                    KEYBOARD_CTRL_CHECKED | 2, KEYBOARD_CTRL_HIDDEN | 2, 6, KEYBOARD_CTRL_HIDDEN | 2, KEYBOARD_CTRL_CHECKED | 2
                                  };


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  ta.width(PCT(80));
  ta.align(ALIGN_TOP,0,10);
  ta.onFocused  (GWIDGET_CB{ kbd.show(true);   });
  ta.onDefocused(GWIDGET_CB{ kbd.show(false);  });  

  kbd.height(PCT(70));                // ความสูง 70% ของหน้าจอ
  kbd.textarea(ta);                   // ผูก GTextArea ที่ต้องการให้ GKeyboad ทำการแก้ไข

  // สามารถกำหนด keyboard map อิสระ ได้ะอง 4 แบบ
  // โดยกำหนด ให้ KEYBOARD_MODE_USER_1, KEYBOARD_MODE_USER_2, KEYBOARD_MODE_USER_3, KEYBOARD_MODE_USER_4
  kbd.map (KEYBOARD_MODE_USER_1, kb_map, kb_ctrl_map); // กำหนด keyboard map และ control map ให้ KEYBOARD_MODE_USER_1
  kbd.mode(KEYBOARD_MODE_USER_1);     // ให้ keyboard เลือกมาใช้โหมด KEYBOARD_MODE_USER_1



}

void loop(){
  BlynkGO.update();
}

