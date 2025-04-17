#include <BlynkGOv5.h>

GRect rect;
  GLabel label(rect);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_COLOR_HEX(0xdcdcdc));

  rect.size(150,66);
  rect.color(TFT_COLOR_HEX(0xdcdcdc));
  rect.radius(10);

  /* แสงเงาของ rect กำหนดตามปกติ ในส่วนด้านเอียงล่าง */
  rect.shadow(16, 2, TFT_COLOR_HEX(0x282828));
  rect.shadow_opa(100);
  rect.shadow_offset(5,5);
  rect.font(prasanmit_20);

  /* ทำการ hook การ draw กราฟิกระดับเพิ่มเติม โดยไม่เปลืองเม็มเพิ่มเป็นวิตเจ็ตใหม่ */
  rect.hookDrawTask(true);
  rect.onDrawTask(GWIDGET_CB{
    if(DRAW_PART == DRAW_PART_MAIN) {       /* เมื่อการ draw กราฟิกระดับล่างของ rect กำลัง draw ในส่วน หลัก */
      draw_rect_dsc_t rect_dsc;
      draw_rect_dsc_init(&rect_dsc);
      rect_dsc.bg_opa           = 0;
      rect_dsc.radius           = rect.radius();
      rect_dsc.shadow_color     = TFT_COLOR(TFT_WHITE);
      rect_dsc.shadow_width     = rect.shadow_width();
      rect_dsc.shadow_spread    = rect.shadow_spread();
      rect_dsc.shadow_opa       = rect.shadow_opa();
      rect_dsc.shadow_offset_x  = -rect.shadow_offset_x();
      rect_dsc.shadow_offset_y  = -rect.shadow_offset_y();
      draw_rect(DRAW_LAYER, &rect_dsc, DRAW_AREA);      /* draw กราฟิกระดับล่าง เพิ่มเติมในส่วนแสงเงา ด้านเอียงบน */
    }
  });
    label = "กราฟิกแบบลอยตัว";
}

void loop()
{
  BlynkGO.update();
}
