#include <BlynkGOv5.h>

// ภาพสำหรับทำพื้นหลัง
IMAGE_DECLARE(img_smartfarm_bg);

// รูปตรงกลาง ของ circle menu ไว้กด
IMAGE_DECLARE(img_lettuce_s);

// icon สำหรับปุ่ม menu 6 ปุ่ม
IMAGE_DECLARE(img_app);
IMAGE_DECLARE(img_gps);
IMAGE_DECLARE(img_monitor);
IMAGE_DECLARE(img_sensors);
IMAGE_DECLARE(img_server);
IMAGE_DECLARE(img_setting);

GImage          background(img_smartfarm_bg);
  GLabel        lb_selected;
  GImage        expand_menu(img_lettuce_s, background);
  GImageButton  menu_btns[6] = {
    { img_app     , background},
    { img_gps     , background},
    { img_monitor , background},
    { img_sensors , background},
    { img_server  , background},
    { img_setting , background},
  };
  enum { MENU_APP, MENU_GPS, MENU_MONITOR, MENU_SENSORS, MENU_SERVER, MENU_SETTING};


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  background.position(0,0);
    lb_selected.font(prasanmit_40, TFT_PALETTE(TFT_PALETTE_RED));
    lb_selected.align(ALIGN_TOP, 0, 5);

    expand_menu.align(ALIGN_BOTTOM_LEFT, 30, -30);
      for(int i=5; i>=0 ; i--) {
        menu_btns[i].align(expand_menu, ALIGN_CENTER);
        menu_btns[i].layer_id(6-i);       // ย้ายไป layer ที่ 6-i
        menu_btns[i].onClicked(GWIDGET_CB{
          int menu_id = (GImageButton*)widget -&menu_btns[0];
          Serial.printf("[Menu] %d  selected\n", menu_id);
          lb_selected.show(true); 
          lb_selected = menu_id;
          static SoftTimer timer;
          timer.delay(2000,[](){ lb_selected.show(false); }); // อีก 2 วินาทีให้ซ่อน

          expand_menu.event_send(EVENT_CLICKED);  // ส่ง EVENT ให้มีการเสมือนว่า circle_menu ถูกกด (จำลองแทนเสมือนมีคนกด)

          // menu_id ที่กดได้จะให้ไปทำอะไร
          switch (menu_id) {
            case MENU_APP:
              //...
              break;
            case MENU_GPS:
              break;
            case MENU_MONITOR:
              break;
            case MENU_SENSORS:
              break;
            case MENU_SERVER:
              break;
            case MENU_SETTING:
              break;
          }
        });
      }

    expand_menu.toForeground();           // ย้ายไปบนสุด
    expand_menu.clickable(true);          // ปกติ GImage กดไม่ได้ เปิดให้สามารถกดสัมผัสได้
    expand_menu.onClicked(GWIDGET_CB{
      enum {MENU_STATE_HIDE, MENU_STATE_ANIM, MENU_STATE_SHOW };
      static int8_t menu_state = MENU_STATE_HIDE;
      static GAnimation anim_menu;
      switch(menu_state){
        case MENU_STATE_HIDE:
          menu_state = MENU_STATE_ANIM;

          // สร้าง animation ปุ่มกดทั้ง 6 เคลื่อนที่ออกมา ด้วยระยะห่างที่ละ 100px
          // ในเวลา 800ms ให้เคลื่อนที่แบบ ช้าไปเร็ว
          anim_menu.init(expand_menu,[](GWIDGET, int32_t v){
            for(int i=0; i<6; i++){
              menu_btns[i].center(expand_menu.center() + Point( (v* 100*(i+1))/1024.0,0));
            }
          },0, 1024, 800, ANIM_PATH_EASE_OUT);
          anim_menu.completed_cb([](GANIMATION){
            menu_state = MENU_STATE_SHOW;
            static SoftTimer timer;
            timer.delay(3000,[](){  // อีก 3 วินาที
              if(menu_state == MENU_STATE_SHOW) {       // ถ้า เมนูยังแสดงอยู่
                expand_menu.event_send(EVENT_CLICKED);  // ส่ง EVENT ให้มีการเสมือนว่า circle_menu ถูกกด (จำลองแทนเสมือนมีคนกด)
              }
            });
          });
          anim_menu.start();
          break;
        case MENU_STATE_ANIM:
          break;
        case MENU_STATE_SHOW:
          menu_state = MENU_STATE_ANIM;

          // สร้าง animation ปุ่มกดทั้ง 6 เคลื่อนที่ หดกลับเข้าไป
          // ในเวลา 800ms ให้เคลื่อนที่แบบ ช้าไปเร็ว
          anim_menu.init(expand_menu,[](GWIDGET, int32_t v){
            for(int i=0; i<6; i++){
              menu_btns[i].center(expand_menu.center() + Point( (v*100*(i+1))/1024.0,0));
            }
          }, 1024, 0, 800, ANIM_PATH_EASE_OUT);
          anim_menu.completed_cb([](GANIMATION){
            menu_state = MENU_STATE_HIDE;
          });
          anim_menu.start();
          break;
      }
    });

}

void loop(){
  BlynkGO.update();
}
