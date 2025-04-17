/**************************************************************************************
 * 
 * ตัวอย่างสร้าง วิตเจ็ตส่วนตัว GMyWidget 
 *   โดยสร้าง สืบทอด (derived class) จาก GContainer
 * 
 **************************************************************************************/

#include <BlynkGOv5.h>

class GMyWidget : public GContainer{
public:
  ~GMyWidget() {}                         // ให้มีด้วยเสมอ โดยชื่อตรงกับ วิตเจ็ตส่วนตัว ที่ตั้ง

  /***********************
   * วิตเจ็ตลูกๆ
   ***********************/
  GLabel title;
  GGauge gauge;

  /***********************
   * API ออกแบบ
   ***********************/
  void create(){                          // ให้มีฟังก์ชั่น create() สำหรับออกแบบวิตเจ็ตส่วนตัว
    if(GContainer::isCreated()) return;   // ให้มี่บรรทัดนี้ในฟังก์ชั่น create() ด้วยเสมอ
    GContainer::create();
    GContainer::layout(LAYOUT_COL_M,0,10,20,20,0);
    GContainer::border(1, TFT_BLACK);
    GContainer::color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_BLUE,3));
    title.parent(this);                   // วิตเจ็ตลูกๆให้ กำหนด parent(this)
    gauge.parent(this);                   // วิตเจ็ตลูกๆให้ กำหนด parent(this)
    title = "ไตเติ้ล";
    gauge.size(100,100);
  }
  /***********************
   * API อื่นๆสำหรับ วิตเจ็ตส่วนตัว ตามต้องการ
   ***********************/
  void set_title(String t)                { create(); title = t;          }
  void value(float value)                 { create(); gauge.value(value); }
};

GMyWidget my_widget_temp;
GMyWidget my_widget_humid;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  my_widget_temp.set_title("อุณหภูมิ");
  my_widget_temp.value(89.23);
  my_widget_temp.gauge.color(TFT_RED);
  my_widget_temp.align(ALIGN_TOP_LEFT,10,10);

  my_widget_humid.set_title("ความชื้น");
  my_widget_humid.value(62.64);
  my_widget_humid.color(TFT_BLACK);
  my_widget_humid.title.color(TFT_WHITE);
  my_widget_humid.gauge.color(TFT_BLUE);
  my_widget_humid.gauge.font_color(TFT_WHITE);
  my_widget_humid.align(ALIGN_BOTTOM_RIGHT,-10,-10);

}

void loop(){
  BlynkGO.update();
}

