#include <BlynkGOv5.h>

IMAGE_DECLARE(img_roller_mask);

String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

GContainer cont(LAYOUT_ROW_M,0,0,0,0,20);
  GRoller roller(cont);
  GRoller roller_masked(cont);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont.bg_opa(0);

  roller.options( th_month, 12);
  roller.visible_rows(4);
  roller.onValueChanged(GWIDGET_CB{
    Serial.print("Selected month: ");
    Serial.println(roller.toString() );
  });
  // roller.font(prasanmit_20);
  // roller.color(TFT_RED);
  // roller.bg_color(TFT_SILVER);

  roller_masked.options( th_month, 12);
  roller_masked.visible_rows(3);
  roller_masked.onValueChanged(GWIDGET_CB{
    Serial.print("Selected month: ");
    Serial.println(roller_masked.toString() );
  });
  roller_masked.mask(img_roller_mask);
  roller_masked.font(prasanmit_40, prasanmit_25);

}

void loop(){
  BlynkGO.update();
}
