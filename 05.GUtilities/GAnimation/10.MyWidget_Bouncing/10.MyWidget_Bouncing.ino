#include <BlynkGOv5.h>

class MyWidget : public GRect {
  public:
    MyWidget(GWidget&parent=GScreen) : GRect(parent)  {}
    GLabel  label;
    GGaugeRainbow gauge;
    void create(){
      if(this->isCreated()) return;
      GRect::create();
      GRect::size(BlynkGO.width()-20, (BlynkGO.height()-30)/2);
      GRect::color(TFT_BLACK);
      GRect::border(1, TFT_WHITE);
      GRect::radius(20);
        label.parent(this);
        label = "TEMP";
        label.color(TFT_SILVER);
        label.position(20, 10);
        gauge.parent(this);
        gauge.size(170,170);
        gauge.align(ALIGN_CENTER,0,20);
    }
    inline void title(String t)       { create(); label = t;          }
    inline void value(float v)        { create(); gauge = v;          }
    inline void decimal(uint8_t d)    { create(); gauge.decimal(d);   }
    inline void operator =(float v)   { this->value(v);               }
};

MyWidget temp, humid;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  temp.title("TEMP");   temp.align  (ALIGN_TOP    ,0, 10);
  humid.title("HUMID"); humid.align (ALIGN_BOTTOM ,0,-10);
  temp = 59.23;
  humid = 80.53;

  static GRect touch_area;
  touch_area.bg_opa(0);
  touch_area.onClicked(GWIDGET_CB{
    temp  = random(1000, 5000)/100.0;
    humid = random(6000, 9500)/100.0;
    static GAnimation anim; // ตัวสร้าง Animation
    anim.init([](void*, int32_t v){
      temp.align  (ALIGN_TOP    , 0, map(v,0,4096, -temp.height() ,  10));
      humid.align (ALIGN_BOTTOM , 0, map(v,0,4096,  humid.height(), -10));
    }, 0, 4096, 800 /*ms*/, ANIM_PATH_BOUNCE);  // ให้ค่า ตั้งแต่ 0 ถึง 1024 ใน 800ms และค่าเป็นแบบเด้งได้
    anim.start();
  });
}

void loop(){
  BlynkGO.update();
}
