/* 
  ตัวอย่าง เพิ่ม รูป ขณะ ON/OFF ให้ GSwitch ด้วยการ draw รูป ระดับล่าง
  จะช่วยให้สามารถประหยัดเม็มโมรี่ได้ กว่าการเพิ่มรูปด้วยวิตเจ็ต GImage
 */

#include <BlynkGOv5.h>

IMAGE_DECLARE(img_sun);
IMAGE_DECLARE(img_moon);

GSwitch sw;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  sw.size(80, 35);
  sw.color(TFT_PALETTE(TFT_PALETTE_RED) , GSTATE_ON);
  sw.color(TFT_PALETTE(TFT_PALETTE_BLUE), GSTATE_OFF);
  sw.border(2, TFT_PALETTE_DARKEN(TFT_PALETTE_RED , 3), GSTATE_ON );
  sw.border(2, TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE, 3), GSTATE_OFF);

  sw.hookDrawTask(true);          /* ให้สามารถ hook ดัก ขณะกราฟิก กำลัง draw ระดับล่าง ได้ด้วย */
  sw.onDrawTask(GWIDGET_CB{      /* เมื่อ ขณะกราฟิก กำลัง draw วิตเจ็ตระดับล่าง */
    /* ทำการเพิ่ม ให้ draw รูป ระดับล่าง เพิ่มเติมเข้าไปด้วย */

    draw_image_dsc_t  image_dsc;
    area_t            image_area;

    draw_image_dsc_init(&image_dsc);
    /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ GPART_INDICATOR และ sw มีสถานะ ON */
    if(DRAW_PART == DRAW_PART_INDICATOR && sw.isON()) {
      image_dsc.src         = &img_sun;
      image_dsc.recolor_opa = 0;
      image_get_area(&image_area, image_dsc.src);
      area_align(DRAW_AREA , &image_area, ALIGN_LEFT_MID, 10, 0);
      draw_image(DRAW_LAYER, &image_dsc , &image_area);
    }else
    /* เมื่อ การ draw ระดับล่าง กำลัง draw ในส่วนของ GPART_MAIN และ sw มีสถานะ OFF */
    if(DRAW_PART == DRAW_PART_MAIN && sw.isOFF()){
      image_dsc.src         = &img_moon;
      image_dsc.recolor_opa = 0;
      image_get_area(&image_area, image_dsc.src);
      area_align(DRAW_AREA , &image_area, ALIGN_RIGHT_MID, -10, 0);
      draw_image(DRAW_LAYER, &image_dsc , &image_area);
    }
  });
}

void loop()
{
  BlynkGO.update();
}
