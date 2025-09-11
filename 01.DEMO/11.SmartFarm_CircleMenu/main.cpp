#include <BlynkGOv5.h>

// ภาพสำหรับทำพื้นหลัง
IMAGE_DECLARE(img_smartfarm_bg);

// รูปตรงกลาง ของ circle menu ไว้กด
IMAGE_DECLARE(img_lettuce);

// icon สำหรับปุ่ม menu 6 ปุ่ม
IMAGE_DECLARE(img_app);
IMAGE_DECLARE(img_gps);
IMAGE_DECLARE(img_monitor);
IMAGE_DECLARE(img_sensors);
IMAGE_DECLARE(img_server);
IMAGE_DECLARE(img_setting);

#define CIRCLE_MENU_RADIUS      160 // px  

GImage          background(img_smartfarm_bg);
  GLabel        lb_selected;
  GImage        circle_menu(img_lettuce, background);
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

    for(int i=0; i< 6; i++){
      menu_btns[i].align_center(); // เริ่มต้นให้ อยู่กลางจอ แต่จะถูก รูปตรงกลางบังยังมองไม่เห็น
      // วางปุ่มกดต่างๆ มีจุดศก.ตำแหน่งที่ รัศมี ตามกำหนด ทำมุม i*360/6 โดยมีมุมเริ่มต้น 270 รอบจุดศก.ของภาพพื้นหลัง แบบ ตามเข็มนาฬิกา
      // menu_btns[i].center(Point(CIRCLE_MENU_RADIUS, i*360/6, 270, background.centerpoint(), CLOCKWISE));

      menu_btns[i].onClicked(GWIDGET_CB{
        int menu_id = (GImageButton*)widget -&menu_btns[0];
        Serial.printf("[Menu] %d  selected\n", menu_id);
        lb_selected.show(true); 
        lb_selected = menu_id;
        static SoftTimer timer;
        timer.delay(2000,[](){ lb_selected.show(false); }); // อีก 2 วินาทีให้ซ่อน

        circle_menu.event_send(EVENT_CLICKED);  // ส่ง EVENT ให้มีการเสมือนว่า circle_menu ถูกกด (จำลองแทนเสมือนมีคนกด)

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
    circle_menu.align_center();
    circle_menu.clickable(true);          // ปกติ GImage กดไม่ได้ เปิดให้สามารถกดสัมผัสได้
    circle_menu.onClicked(GWIDGET_CB{
      enum {MENU_STATE_HIDE, MENU_STATE_ANIM, MENU_STATE_SHOW };
      static int8_t menu_state = MENU_STATE_HIDE;
      static GAnimation anim_menu;
      switch(menu_state){
        case MENU_STATE_HIDE:
          menu_state = MENU_STATE_ANIM;

          // สร้าง animation ปุ่มกดทั้ง 6 เคลื่อนที่ด้วย รัศมี v 
          // โดย v มีค่าจาก 0 ไปยัง CIRCLE_MENU_RADIUS ในเวลา 800ms
          // เพื่อ แสดง ปุ่มกดทั้ง 6 ออกมา ให้เคลื่อนที่แบบ ช้าไปเร็ว
          anim_menu.init(circle_menu,[](GWIDGET, int32_t v){
            for(int i=0; i<6; i++){
              menu_btns[i].center(Point(v, i*360/6, 270, background.centerpoint(), CLOCKWISE));
            }
          },0,CIRCLE_MENU_RADIUS, 800, ANIM_PATH_EASE_OUT);
          anim_menu.completed_cb([](GANIMATION){
            menu_state = MENU_STATE_SHOW;
            static SoftTimer timer;
            timer.delay(3000,[](){  // อีก 3 วินาที
              if(menu_state == MENU_STATE_SHOW) {       // ถ้า เมนูยังแสดงอยู่
                circle_menu.event_send(EVENT_CLICKED);  // ส่ง EVENT ให้มีการเสมือนว่า circle_menu ถูกกด (จำลองแทนเสมือนมีคนกด)
              }
            });
          });
          anim_menu.start();
          break;
        case MENU_STATE_ANIM:
          break;
        case MENU_STATE_SHOW:
          menu_state = MENU_STATE_ANIM;

          // สร้าง animation ปุ่มกดทั้ง 6 เคลื่อนที่ด้วย รัศมี v 
          // โดย v มีค่า CIRCLE_MENU_RADIUS ไปยัง 0 ในเวลา 800ms
          // เพื่อ หด ปุ่มกดทั้ง 6 กลับเข้าไป ให้เคลื่อนที่แบบ ช้าไปเร็ว
          anim_menu.init(circle_menu,[](GWIDGET, int32_t v){
            for(int i=0; i<6; i++){
              menu_btns[i].center(Point(v, i*360/6, 270, background.centerpoint(), CLOCKWISE));
            }
          }, CIRCLE_MENU_RADIUS, 0, 800, ANIM_PATH_EASE_OUT);
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
