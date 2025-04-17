/* 
  ตัวอย่าง เพิ่ม ข้อความ ON/OFF ให้ GSwitch ด้วยการ draw ข้อความ ระดับล่าง
  จะช่วยให้สามารถประหยัดเม็มโมรี่ได้ กว่าการเพิ่มข้อความด้วยวิตเจ็ต GLabel
 */

#include <BlynkGOv5.h>

GSwitch sw;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  sw.size(65, 30);
  sw.color(TFT_GREEN, GSTATE_ON);
  sw.color(TFT_RED  , GSTATE_OFF);

  sw.hookDrawTask(true);          /* hook ดัก ขณะกราฟิก กำลัง draw ระดับล่าง ได้ด้วย */
  sw.onDrawTask(GWIDGET_CB{      /* เมื่อ ขณะกราฟิก กำลัง draw วิตเจ็ตระดับล่าง */
    /* ทำการเพิ่ม ให้ draw ข้อความ ระดับล่าง เพิ่มเติมเข้าไปด้วย */

    draw_label_dsc_t  label_dsc;
    area_t            label_area;

    draw_label_dsc_init(&label_dsc);
    label_dsc.font  = &prasanmit_20;
    label_dsc.color = TFT_COLOR(TFT_WHITE);

    /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ DRAW_PART_INDICATOR และ sw มีสถานะ ON */
    if(DRAW_PART == DRAW_PART_INDICATOR && sw.isON()) {
      label_dsc.text = "ON";      
      text_get_area(&label_area, label_dsc.text, label_dsc.font);
      area_align(DRAW_AREA , &label_area, ALIGN_LEFT_MID, 10, 0);
      draw_label(DRAW_LAYER, &label_dsc , &label_area);
    }else
    /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ GPART_MAIN และ sw มีสถานะ OFF */
    if(DRAW_PART == DRAW_PART_MAIN && sw.isOFF()){
      label_dsc.text = "OFF"; 
      text_get_area(&label_area, label_dsc.text, label_dsc.font);
      area_align(DRAW_AREA , &label_area, ALIGN_RIGHT_MID, -8, 0);
      draw_label(DRAW_LAYER, &label_dsc , &label_area);
    }
  });
}

void loop()
{
  BlynkGO.update();
}
