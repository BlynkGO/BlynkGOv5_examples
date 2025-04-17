/*
 * ตัวอย่าง การปรับยกขึ้นเป็น class วิตเจ็ต ส่วนตัวขึ้นมา
 * จากตัวอย่าง 04.GScale_namespace_GAUGE_GRAFANA ที่เคยสร้าง ด้วย namespace
 * 
 * เมื่อเป็น class วิตเจ็ต แล้วจะมีความสามารถเป็นเหมือนวิตเจ็ตได้เลย
 * โดย ออปเจ็ค สามารถจัดวาง align(..) หรือ show(true/false) ได้ เป็นต้น
 * หรือ ทำเป็น array ได้  ในขณะที่ แบบ namespace จะทำเป็น array ไม่ได้
 * 
 */

#include <BlynkGOv5.h>

//-------------------------------------------------------------
// สร้าง derived class (คลาสสืบต่อ) จาก GScale
class GMyGaugeGfana : public GScale {  
  public:
    GMyGaugeGfana(GWidget &parent= GScreen) : GScale(parent)  {}  // ** ต้องมีเพิ่ม ** เวลายกขึ้นเป็น class widget

    uint32_t _range[5] = { 0, 40, 60, 80, 100};
    GScaleSection section[4];
      color_t sect_color[4] = { TFT_BLUE, TFT_GREEN, TFT_YELLOW, TFT_RED};

    GArc arc;
    GLabel lb_value;
    uint8_t _value;

    void create(){
      if(this->isCreated()) return;   // ** ต้องมีเพิ่ม ** เวลายกขึ้นเป็น class widget
      GScale::create();               // ** ต้องมีเพิ่ม ** เวลายกขึ้นเป็น class widget

      GScale::size(200,200);          // คำสั่ง scale.xxx(...)  เดิมใดๆ เปลี่ยนเป็น  GScale::xxx(...) แทน
      GScale::range(0,100);
      GScale::mode(SCALE_MODE_ROUND);
      GScale::tick_opa(0); GScale::label_show(false);

      for(int i=0; i<4; i++) {
        section[i] = GScale::add_section();
        section[i].thickness(5, sect_color[i]);
        section[i].range(_range[i],_range[i+1]);
      }

      arc.parent(this);               // วิตเจ็ตลูกๆ ให้ผูกแม่ด้วย  xxx.parent(this);
      arc.size(PCT(90),PCT(90));
      arc.thickness( 0.1* GScale::width(), TFT_GREEN);
      arc.angle_offset(GScale::angle_offset());
      arc.range(0,GScale::angle_range());
      arc.angle(0,200);

      lb_value.parent(this);          // วิตเจ็ตลูกๆ ให้ผูกแม่ด้วย  xxx.parent(this);
      lb_value.font(BebasNeueBold_num_45, TFT_WHITE);
      lb_value = 90;
    }

    void value(uint8_t value){
      create();
      _value = value;
      lb_value = _value;
      arc.angle(0, map( constrain(value,0,100),0,100,0,270));
      for(int i=1; i<=4; i++){
        if( value <= _range[i]) {
          arc.color(sect_color[i-1]) ;
          break;
        }
      }
    }
    inline uint8_t value()      { create(); return _value;    }
    void operator=(float val)   { value(val);                 }
};
//-------------------------------------------------------------


GMyGaugeGfana gauge;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  gauge = 57.24;

}

void loop(){
  BlynkGO.update();
}

