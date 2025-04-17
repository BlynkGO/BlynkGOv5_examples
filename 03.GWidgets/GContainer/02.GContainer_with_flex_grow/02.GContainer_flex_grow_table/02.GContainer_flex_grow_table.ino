// ตัวอย่าง GContainer  ที่มี LAYOUT_PRETTY กับการขยายขนาดลูกๆด้วย flex_grow(..)
// ให้ขยายขนาดอัตโนมัติ เต็มพื้นที่ด้านกว้าง ที่เหลือ ในบรรทัดนั้นๆ

#include <BlynkGOv5.h>

class GCell : public GContainer {
  public:
    /***********************
     * Constructor
     ***********************/
    GCell(GWidget&parent=GScreen) : GContainer(parent)    {}
    GCell(String topic, GWidget&parent=GScreen)
     : GContainer(parent), _topic(topic)                  {}

    /***********************
     * Child Widgets
     ***********************/
    GLabel lb_topic;
    GRect  rect_value;
      GLabel lb_value;

    /***********************
     * Main Create()
     ***********************/
    void create(){
      if(isCreated()) return;
      GContainer::create();
      GContainer::layout(LAYOUT_COL_L, 0,0,5,5,-15);
      GContainer::border(1, TFT_SILVER);
      GContainer::border_opa(80);
      GContainer::radius(0);
      lb_topic.parent(this);
      lb_topic.font(prasanmit_20);
      lb_topic = _topic;
      rect_value.parent(this);
      rect_value.size(PCT(100), SIZE_CONTENT);
      rect_value.border(0);
      rect_value.bg_opa(0);
      rect_value.clickable(false);
        lb_value.parent(rect_value);
        lb_value = "-";
    }
    /***********************
     * Widget APIs
     ***********************/
    inline void topic(String t)                   { create(); lb_topic = (_topic=t);              }
    inline String topic()                         { create(); return _topic;                      }
    inline void value(float v, int32_t decimal=2) { create(); lb_value= String(_value=v,decimal); }
    inline float value()                          { create(); return _value;                      }

  private:
    String _topic = "topic";
    float  _value = 0.0;
};


GContainer table(LAYOUT_PRETTY,0,0,0,0,0);
  GCell cell[] = {
    {"Total Voltage", table},
    {"Total Current", table},
    {"SOC"          , table},
    {"Cell Min"     , table},
    {"Cell Avg"     , table},
    {"Call Max"     , table},
    {"Gear"         , table},
  };


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  table.show(false);
  table.width(PCT(100));                    // fix ขนาด เต็มกว้างจอ
  table.radius(0);
  
    //----------------------------
    cell[0].width(110);
    cell[1].flex_grow(1);                   // ขยายด้านกว้าง 1 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    cell[2].width(60);

    //----------------------------
    cell[3].flex_new_line(true);            // ขึ้น flex บรรทัดใหม่
    cell[3].flex_grow(1);                   // ขยายด้านกว้าง 1 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    cell[4].flex_grow(1);                   // ขยายด้านกว้าง 1 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    cell[5].flex_grow(1);                   // ขยายด้านกว้าง 1 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    cell[6].flex_grow(1);                   // ขยายด้านกว้าง 1 สัดส่วน จาก ระยะด้านกว้างที่เหลือ ในบรรทัด
    //----------------------------

  table.show(true);
}

void loop(){
  BlynkGO.update();
}
