#include <BlynkGOv5.h>

GTextArea ta_login; 
GTextArea ta_password;
GLabel    lb_login;
GLabel    lb_password;
GKeyboard kbd;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  ta_login.default_keyboard(false);       // ปิดไม่ใช้ default keyboard  เพื่อจะใช้แป้นพิมพ์จาก GKeyboard
  ta_login.cursor_show(true);             // เมื่อ กดเลือก ให้ มี cursor แสดงด้วย
  ta_login.password_mode(false);
  ta_login.width(PCT(40));                // กว้าง 40% ของหน้าจอ
  ta_login.placeholder("login");
  ta_login.align(ALIGN_TOP_LEFT, PCT(7), 40);
  ta_login.onClicked(GWIDGET_CB{ kbd.textarea(ta_login); });

  lb_login = "Login :";
  lb_login.align(ta_login, ALIGN_TOP_LEFT);

  ta_password.default_keyboard(false);    // ปิดไม่ใช้ default keyboard  เพื่อจะใช้แป้นพิมพ์จาก GKeyboard
  ta_password.cursor_show(true);          // เมื่อ กดเลือก ให้ มี cursor แสดงด้วย
  ta_password.password_mode(true);
  ta_password.width(ta_login);
  ta_password.placeholder("password");
  ta_password.align(ALIGN_TOP_RIGHT, -PCT(7),40);
  ta_password.onClicked(GWIDGET_CB{ kbd.textarea(ta_password); });

  lb_password = "Password :";
  lb_password.align(ta_password, ALIGN_TOP_LEFT);

  kbd.height(PCT(55));                    // สูง 55% ของ หน้าจอ
  kbd.textarea(ta_password);              // Focus แป้นพิมพ์ ไปยัง text areas เริ่มต้น ที่จะให้พิมพ์
}

void loop(){
  BlynkGO.update();
}
