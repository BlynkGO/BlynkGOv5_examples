#include <BlynkGOv5.h>

IMAGE_DECLARE(img_gauge_frame);     // รูปส่วนกรอบของหน้าปัด gauge
IMAGE_DECLARE(img_gauge_needle);    // รูปส่วนเข็มของ gauge ให้ชี้ตามแนวนอน แกน x+

GImage gauge_frame(img_gauge_frame);
  GScale scale(gauge_frame);
  GImage scale_needle(img_gauge_needle, gauge_frame); 


#define START_VALUE     120

GSlider slider_value;
GLabel  lb_value;

void gauge_set_value(int32_t v){
  scale.image_needle_value(scale_needle, constrain(v, scale.range_min_value(), scale.range_max_value()));
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  gauge_frame.align(ALIGN_CENTER,0,-10);
    scale.size(230,230);
    scale.angle_offset(90);     // องศาเริ่มต้น ของ scale นับตามแกน x+ หมุนตามเข็ม
    scale.angle_range(27*11);   // ระยะ องศาทั้งหมด ของ scale   11 ช่วง ช่วงละ 27 องศา
    scale.range(30, 140);       // ระยะค่าจริงบนหน้าปัด
    scale.mode(SCALE_MODE_ROUND /* scale แบบโค้ง */, 10*11+1 /* จน.ขีดทั้งหมด ให้รวมขีดแรกด้วย */, 10 /* ขีดหลักทุกๆ 10 ขีด */);

    // ตัวข้อความ ที่แสดงบน scale
    scale.label_show(true);                       // ให้แสดงข้อความ ที่เส้นขีดหลักบน scale ด้วย
    static const char * custom_labels[] =  {"30",  "40",  "50",  "60",  "70",  "80",  "90",  "100", "110", "120", "130", "140", NULL};  // 11 ช่วง 
    scale.text_src(custom_labels);                // กำหนดข้อความ ที่ เส้นขีดหลัก
    scale.pad_radial(8);                          // เพิ่ม ช่องไฟอีก 15px ระหว่างข้อความบน scale กับตัว scale ตามโค้งหน้าปัด
    scale.font(prasanmit_30, TFT_BLACK);

    scale.tick_length(10,5);                      // ความยาว ขีดหลัก, ขีดรอง ของ scale
    scale.thickness(0,3,1);                       // ความหนา ของเส้นแกน, ขีดหลัก, ขีดรอง  
    scale.color(TFT_BLACK);                       // สีของ scale  (หรือจะใส่ แยกสีของเส้นแกน, ขีดหลัก, ขีดรอง แบบ 3 parameters ก็ได้)

      static GScaleSection section;
      section = scale.add_section();
      section.range(100,140);
      section.color(TFT_RED);
      section.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_RED,2));

    scale_needle.align(ALIGN_CENTER, 40, 0);    // วางให้ รูปเข็มหน้าปัด มาอยู่กลาง GScale ที่เป็นแม่
    scale_needle.pivot(32,18);                  // กำหนดจุดหมุน ให้ระยะ x และ y เข้าไป ถึงจุดหมุน ของ รูป เข็มหน้าปัด ให้เป็นแนวนอน ชี้ไปทางแกน X ทิศ +
  
    gauge_set_value(START_VALUE); 

  slider_value.range(scale.range_min_value(), scale.range_max_value());
  slider_value.align(ALIGN_BOTTOM,0,-30);
  slider_value.onValueChanged(GWIDGET_CB{
    gauge_set_value(slider_value.toInt());
    lb_value = slider_value.toInt();
  });
  lb_value.align(slider_value, ALIGN_TOP,0,-5);  
  lb_value.color(TFT_WHITE);

  slider_value = START_VALUE;
  lb_value     = START_VALUE;

}

void loop(){
  BlynkGO.update();
}
