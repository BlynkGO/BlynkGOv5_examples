#include <BlynkGOv5.h>

GContainer cont_ta(LAYOUT_COL_L,15,15,15,15,5);   // ให้วิตเจ็ตลูกๆ วางเรียงแนวตั้งชิดซ้ายตรงกัน มีระยะช่องไฟ บน,ล่าง,ซ้าย,ขวา,ระหว่าง ตามกำหนด
  GTextArea ta_login(cont_ta);                    // วิตเจ็ต GTextArea เป็นลูกของวิตเจ็ต cont_ta
  GTextArea ta_password(cont_ta);


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  ta_login.width(PCT(70));                        // กว้าง 70% ของหน้าจอ
  ta_login.placeholder("LOGIN");
  ta_login.onValueChanged(GWIDGET_CB{
    Serial.println(ta_login.toString());
  });

  ta_password.width(PCT(70));                     // กว้าง 70% ของหน้าจอ
  ta_password.placeholder("PASSWORD");
  ta_password.password_mode(true);
  ta_password.onValueChanged(GWIDGET_CB{
    Serial.println(ta_password.toString());
  });
}

void loop(){
  BlynkGO.update();
}

