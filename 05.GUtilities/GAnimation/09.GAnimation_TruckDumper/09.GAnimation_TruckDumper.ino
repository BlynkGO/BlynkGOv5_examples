#include <BlynkGOv5.h>

IMAGE_DECLARE(img_truck_dump_body);
IMAGE_DECLARE(img_truck_dump_bed);

GRect  truck;
  GLine  truck_hydrolic_bar(truck);
  GImage truck_body(img_truck_dump_body, truck);
  GImage truck_bed(img_truck_dump_bed,truck);

GContainer controller;
  GSlider slider(controller);
  GLabel lb_degree(controller);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  truck.size(350,310);
  truck.bg_opa(0);
  truck.align(ALIGN_CENTER,0,-40);
  truck.onClicked(GWIDGET_CB{
    static GAnimation anim;
    anim.init(truck, [](GWIDGET, int32_t v){
      int l = BlynkGO.width() + truck.width();
      int x = (v <= l/2)? -v : l-v;
      truck.translate_x(x);
    }, 0, BlynkGO.width() + truck.width(), 1000);
    anim.start();
  });
    truck_hydrolic_bar.setPoints(Point(201,240), Point(201,235));
    truck_hydrolic_bar.thick(5, TFT_WHITE);
    truck_body.align(ALIGN_BOTTOM_LEFT);
    truck_bed.align(truck_body, ALIGN_IN_TOP_RIGHT);

  controller.bg_opa(0);
  controller.layout(LAYOUT_COL_M, 10, 10, 20, 20, 10);
  controller.align(ALIGN_BOTTOM,0,-20);
    lb_degree = 0;
    lb_degree.color(TFT_WHITE);
    slider.range(0,60);
    slider.onValueChanged(GWIDGET_CB{
      int truck_deg = slider.toInt();
      lb_degree = truck_deg;
      truck_bed.pivot(230,90);        // จุดหมุน
      truck_bed.rotation(truck_deg);  // หมุนไปกี่องศา
      // วาดเส้น GLine เริ่มจาก จุด 201,240 และอีกจุด เป็นจุด 201,240 แต่หมุนไปเป็นองศาที่ต้องการรอบจุด 300,245 แบบหมุนตามเข็ม
      truck_hydrolic_bar.setPoints(Point(201,240), Point(201,240).rotate(truck_deg, Point(300,245), CLOCKWISE));
    }); 
}

void loop(){
  BlynkGO.update();
}

