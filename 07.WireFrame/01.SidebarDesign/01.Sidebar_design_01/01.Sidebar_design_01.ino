#include <BlynkGOv5.h>

IMAGE_DECLARE(img_temp);
IMAGE_DECLARE(img_humid);
IMAGE_DECLARE(img_apple);

#define SCREEN_NUM    3
const img_t* btn_image[SCREEN_NUM]    = { &img_temp, &img_humid, &img_apple};
const char*  btn_text[SCREEN_NUM]     = { "อุณหภูมิ", "ความชื้น", "แอปเปิ้ล"};


GRect screen[SCREEN_NUM];
  GLabel label[SCREEN_NUM];

namespace SIDEBAR {
  GPage page_sidebar;
    GButton btn_home(page_sidebar);
    GButton btn[3];
  bool _sidebar_maximize = false;

  void design();
  void maximize(bool enable, uint32_t timeout=3000);
  inline bool maximize()        { return _sidebar_maximize; }
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  SIDEBAR::design();
}

void loop(){
  BlynkGO.update();
}

void SIDEBAR::design(){
  page_sidebar.layout(LAYOUT_COL_L, 10, 10, 10, 10, 5);
  page_sidebar.radius(0);
  page_sidebar.height(BlynkGO.height());
  page_sidebar.scrollbar(SCROLLBAR_AUTO, false, true);
  page_sidebar.color(TFT_COLOR_HSV(0,0,10));
  page_sidebar.align(ALIGN_LEFT);

  btn_home = SYMBOL_LIST;
  btn_home.round_design();
  btn_home.width(70);
  btn_home.onClicked(GWIDGET_CB{
    SIDEBAR::maximize(!SIDEBAR::maximize());
  });

  for(int i=0; i< SCREEN_NUM; i++){
    btn[i] = btn_image[i];
    btn[i] = btn_text[i];
    btn[i].icon_width(40);
    btn[i].label_width(80);
    btn[i].parent(page_sidebar);
    btn[i].onClicked(GWIDGET_CB{
      int ii = (GButton*)widget - &btn[0];
      for(int i=0; i< SCREEN_NUM; i++){
        screen[i].show(ii==i);
      }
      Serial.println("ปุ่ม " + btn[ii].text() + " ถูกกด");
      if(SIDEBAR::maximize()) SIDEBAR::maximize(true, 1000);   /* ขยายเวลาในการย่อ sidebar ออกไปอีก 1000 ms*/
    });
  }
  SIDEBAR::maximize(false);

  for(int i=0; i<SCREEN_NUM; i++ ){
    screen[i].color(random(0xFFFF));
    screen[i].width(BlynkGO.width()-page_sidebar.width());
    screen[i].align(ALIGN_RIGHT);
    screen[i].toBackground();
    screen[i].show(i==0);
      label[i].parent(screen[i]);
      label[i] = "หน้า " + btn[i].text();
  }
}

void SIDEBAR::maximize(bool enable, uint32_t timeout){
  SIDEBAR::_sidebar_maximize = enable;
  static SoftTimer timer;

  page_sidebar.toForeground();      /* ให้ sidebar ย้ายมาอยู่บนสุด */
  for(int i=0; i< 3; i++){
    if(enable) {
      btn[i].label_show(true);
      timer.delay(timeout,[](){     /* ตั้งเวลาอีก xxxx ms ถัดไป */
        SIDEBAR::maximize(false);   /* ทำการย่อ sidebar */
      });
    }else{
      btn[i].label_show(false);
      timer.del();                  /* ที่เคยตั้งเวลาเพื่อย่อ sidebar ไว้ให้ยกเลิก */
    }
  }
}
