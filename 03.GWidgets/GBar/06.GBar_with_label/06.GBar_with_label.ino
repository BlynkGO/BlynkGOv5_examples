/* 
  ตัวอย่าง เพิ่ม ข้อความ แสดงค่าของ GBar ด้วยการ draw ข้อความ ระดับล่าง
  จะช่วยให้สามารถประหยัดเม็มโมรี่ได้ กว่าการเพิ่มข้อความด้วยวิตเจ็ต GLabel
 */


#include <BlynkGOv5.h>

GBar bar;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_COLOR_HEX(0xe0e0e0));

  bar.size(200, 20);
  bar.onDrawMainEnd(GWIDGET_CB{     // เมื่อการ draw กราฟิกระดับล่าง มีการ draw ส่วนหลักของ วิตเจ็ต GBar เสร็จ
    char buf[8]; snprintf(buf, sizeof(buf), "%d", bar.toInt());
    
    draw_label_dsc_t  label_dsc;
    area_t            label_area;

    draw_label_dsc_init(&label_dsc);
    label_dsc.font  = &prasanmit_20;
    label_dsc.text  = buf;

    auto indic_area   = bar.indic_area();   // พื้นที่ indic ของ bar
    auto indic_width  = indic_area.x2 - indic_area.x1 + 1;

    text_get_area(&label_area, label_dsc.text, label_dsc.font);
    auto label_width  = label_area.x2 - label_area.x1 + 1;

    if(indic_width > label_width + 20) {
      area_align( &indic_area , &label_area, ALIGN_RIGHT_MID    , -10, 0);
    }else{
      area_align( &indic_area , &label_area, ALIGN_OUT_RIGHT_MID,  10, 0);
    }
    draw_label(DRAW_LAYER, &label_dsc , &label_area);
  });

  static GAnimation anim;
  anim.init(bar, [](GWIDGET, int32_t value) {
    bar.value(value, true);
  }, 0, 100, 4000, true, true);
  anim.create();

}

void loop()
{
  BlynkGO.update();
}

