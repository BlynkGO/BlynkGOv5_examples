#include <BlynkGOv5.h>

GContainer cont_checkbox(LAYOUT_COL_L,0,0,0,0,10); // layout แบบแนวตั้ง ซ้ายตรงกัน ระยะภายใน 10 px
  GCheckbox checkbox[4];
    String cb_str[4] = { "ทุเรียน", "มังคุด", "กล้วย", "แตงโม\nand a new line" };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  cont_checkbox.bg_opa(0);
  cont_checkbox.font(prasanmit_25);
  for(int i=0; i<4; i++){
    checkbox[i] = cb_str[i];
    checkbox[i].parent(cont_checkbox);
    checkbox[i].onValueChanged(GWIDGET_CB{
      int ii = (GCheckbox*)widget - &checkbox[0];
      String checkbox_text = checkbox[ii].text();
      String check_str     = checkbox[ii].checked()? "Checked" : "Unchecked";
      Serial.println( checkbox_text + " " + check_str);
    });
  }

  checkbox[1].checked(true);
  checkbox[2].disabled(true);
  checkbox[3].checked(true);
  checkbox[3].disabled(true);
}

void loop(){
  BlynkGO.update();
}

