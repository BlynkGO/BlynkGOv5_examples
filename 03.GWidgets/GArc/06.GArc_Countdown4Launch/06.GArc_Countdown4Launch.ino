#include <BlynkGOv5.h>

GArc   arc_countdown;
  GLabel lb_countdown(arc_countdown);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  arc_countdown.size(200,200);
  arc_countdown.thickness(20);
  arc_countdown.bg_enable(true); 
  arc_countdown.angle_offset(270);
  arc_countdown.angle(0,360);
  arc_countdown.range(0,5000);
  arc_countdown = 5000;
  arc_countdown.color(TFT_PALETTE(TFT_PALETTE_RED));
  arc_countdown.bg_color(TFT_COLOR_MIX(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED,3), TFT_WHITE, 150));
  arc_countdown.clickable(true);
  arc_countdown.onClicked(GWIDGET_CB{
    static GAnimation anim_countdown;
    static int c;
    c=10;
    anim_countdown.init([](GWIDGET, int32_t v){
      int _c = (int) (v/1000.0) + 1;
      if(_c != c) {
        lb_countdown = (c = _c);            // แสดงค่า วินาที ที่่เกจจ์
        lb_countdown.transform_scale(255);  // ไม่ต้องขยาย scale
        lb_countdown.opa(255);              // ความโปร่งใสเป็น 255
      }else{
        int32_t v1000 =  constrain( 1000 - esp32_mod(v,1000), 0, 1000); // เปลี่ยน v ให้อยู่ระหว่าง 0-1000
        if(v1000 >=500) {                   // หากค่า 500 - 1000 ms
          lb_countdown.transform_scale( map(v1000,500,1000,255,360)); // ให้ขยายขนาด จาก 255 -> 360
          lb_countdown.opa( map(v1000,500,1000,255,40));    // ปรับค่าโปร่งใส ให้ค่อยๆ จาก 255 -> 40
        }
      }
      arc_countdown = v;
    }, 5000, 0, 5000);  // v เริ่มจากค่า 5000->0 ใน 5000ms
    anim_countdown.completed_cb([](GANIMATION){ // เมื่อ animation ทำงานเสร็จแล้ว
      lb_countdown = (c = 0);
      lb_countdown.transform_scale(255);
      lb_countdown.opa(255);
      arc_countdown = 0;
      // หากจะให้ทำอะไรต่อ หลัง countdown ครบ 5 วินาทีแล้วให้วางตรงนี้
      
    });
    anim_countdown.start();
  });
    lb_countdown.font(BebasNeueBold_num_45, TFT_WHITE);
    lb_countdown = 5;
}

void loop(){
  BlynkGO.update();
}
