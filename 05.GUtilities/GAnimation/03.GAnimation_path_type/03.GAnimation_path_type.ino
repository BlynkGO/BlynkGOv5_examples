/* For 800 x 480 px */

#include <BlynkGOv5.h>

IMAGE_DECLARE(img_racing);
IMAGE_DECLARE(img_mario);
IMAGE_DECLARE(img_luigi);
IMAGE_DECLARE(img_peach);
IMAGE_DECLARE(img_toad);
IMAGE_DECLARE(img_turtle);
IMAGE_DECLARE(img_yoshi);

GImage racing(img_racing);
GImage mario(img_mario);
GImage luigi(img_luigi);
GImage peach(img_peach);
GImage toad(img_toad);
GImage turtle(img_turtle);
GImage yoshi(img_yoshi);

GImage* pp[6] = { &mario, &luigi, &peach, &toad, &turtle, &yoshi};
GAnimation a[6];
anim_path_type_t path_type[6] = {
  ANIM_PATH_LINEAR,         // ปกติ ค่าเพิ่มแบบ liner, 
  ANIM_PATH_EASE_IN,        // ค่า จากค่อยๆช้า->เร็ว,      เช่น ลูกบอล กลิ้งลงจากทางชัน, ลูกบอลกระเด้ง ที่สม่ำเสมอไปเรื่อยๆ
  ANIM_PATH_EASE_OUT,       // ค่า จากเร็ว->ช้าลง, 
  ANIM_PATH_EASE_IN_OUT,    // ค่า จากค่อยๆช้า->เร็ว->ช้า,  เช่น ลูกตุ้ม Pendulum
  ANIM_PATH_OVERSHOOT,      
  ANIM_PATH_BOUNCE, 
};

GLabel lb_path[6] = {
  "การเคลื่อนที่แบบ เชิงเส้น",
  "การเคลื่อนที่แบบ EASE IN",
  "การเคลื่อนที่แบบ EASE OUT",
  "การเคลื่อนที่แบบ EASE IN OUT",
  "การเคลื่อนที่แบบ OVERSHOOT",
  "การเคลื่อนที่แบบ เด้ง",
};


GButton btn;
void setup() {
  Serial.begin(9600);
  BlynkGO.begin();

  racing.align(ALIGN_CENTER);
  for(int i=0; i<6; i++){
    lb_path[i].center(400, 40+20+80*i);
    pp[i]->center(35,40+80*i);
    a[i].init(pp[i],ANIM_X,3,690, 3000, path_type[i]);
  }
  a[0].completed_cb([](GANIMATION){  
    static SoftTimer timer;
    timer.delay(2000,[](){
      btn.show(true);
    });
  });

  btn ="Start";
  btn.font(prasanmit_20);
  btn.align(ALIGN_BOTTOM_RIGHT,-150,-5);
  btn.onClicked(GWIDGET_CB{
    static uint8_t state=1;
    switch(state){
      case 0:
        for(int i=0;i<6; i++){
          pp[i]->center(35,40+80*i);
        }
        state = 1;
        break;
      case 1:
        btn.show(false);
        for(int i=0;i<6; i++){
          a[i].start();
        }
        state = 0;
        break;
    }
  });

}

void loop(){
  BlynkGO.update();
}

