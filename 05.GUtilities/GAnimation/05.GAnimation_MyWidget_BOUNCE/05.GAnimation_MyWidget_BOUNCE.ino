#include <BlynkGOv5.h>

class MyWidget : public GRect {
  public:
    MyWidget(GWidget& parent= GScreen) : GRect(parent)  { }

    GLabel lb_title;
    GLabel lb_value;

    void create(){
      if(!this->isCreated()) {
        GRect::create();
        GRect::size(200,120);
        GRect::radius(20);
        GRect::border(5, TFT_COLOR_HSV(240, 100, 80));
        GRect::padding_hor(10);
        GRect::padding_ver(10);
        lb_title.parent(this);
        lb_title.position(0,0);
        lb_title.font(prasanmit_20);
        lb_title = "Title";
        lb_value.parent(this);
        lb_value.font(BebasNeueBold_num_45);
        lb_value.align(ALIGN_BOTTOM,0,-10);
        lb_value = String(0.00,2);
      }
    }

    inline void title(String title)     { create(); lb_title = title;           }
    inline void value(float value)      { create(); lb_value = String(value,2); }
};

MyWidget mytemp;
MyWidget myhumid;
GButton  btn_anim;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  mytemp.title("อุณหภูมิ");
  mytemp.value(23.54);
  mytemp.align(ALIGN_CENTER, -mytemp.width()/2-5);

  myhumid.title("ความชื้น");
  myhumid.value(89.45);
  myhumid.align(ALIGN_CENTER, myhumid.width()/2+5);

  btn_anim = "Animation";
  btn_anim.align(ALIGN_BOTTOM,0,-20);
  btn_anim.onClicked(GWIDGET_CB{
    mytemp .update_position();            // อัพเดตตำแหน่งให้เป็น ตำแหน่งแบบสมบูรณ์
    myhumid.update_position();            // อัพเดตตำแหน่งให้เป็น ตำแหน่งแบบสมบูรณ์
    static GAnimation anim;
    anim.init([](void*, int32_t v){
      mytemp .posX(map(v,0,1024,-mytemp.width(),BlynkGO.width()/2 - mytemp.width() - 5));
      myhumid.posX(map(v,0,1024,BlynkGO.width(),BlynkGO.width()/2 + 5));
    }, 0, 1024, 1000, ANIM_PATH_BOUNCE);  // ค่า v เริ่มตั้งแต่ 0 ถึง 1024 ภายในเวลา 1000 ms โดย v มีการเปลี่ยนแปลงค่าแบบวิถีค่าเด้ง (ANIM_PATH_BOUNCE)
    anim.start();
  });
}

void loop() {
  BlynkGO.update();
}
