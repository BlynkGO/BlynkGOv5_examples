#include <BlynkGOv5.h>

IMAGE_DECLARE(img_background_800x480);
IMAGE_DECLARE(img_apple);
IMAGE_DECLARE(img_banana);
IMAGE_DECLARE(img_coconut);
IMAGE_DECLARE(img_grape);
IMAGE_DECLARE(img_lemon);
IMAGE_DECLARE(img_mango);
IMAGE_DECLARE(img_water_melon);

#define RADIUS_A       200
#define RADIUS_B       80

GImage bg;
GImageButton btn[] = { {img_apple}, {img_banana}, {img_coconut}, {img_grape}, {img_lemon}, {img_mango}, {img_water_melon} };
  String btn_str[] = { "แอปเปิ้ล", "กล้วย", "มะพร้าว", "องุ่น", "มะนาว", "มะม่วง", "แตงโม"};
GLabel label;

GContainer cont_btn_x;
  GButton btn_x[2];

int btn_num;
int cur_id = 0;
GAnimation anim_btn;
bool is_animation = false;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  bg  = img_background_800x480;

  btn_num = sizeof(btn) / sizeof(btn[0]);
  for(int i=0; i< btn_num; i++){
    float theta = PI/2 + 2 * PI * i / btn_num; // มุมของปุ่ม i (เรเดียน)  เริ่มต้นที่ 90 องศา (pi/2)
    // คำนวณตำแหน่ง (x, y) บนวงรี
    int x = BlynkGO.width()/2  + RADIUS_A * cos(theta);
    int y = BlynkGO.height()/2 + RADIUS_B * sin(theta);
    btn[i].center(x,y);
    btn[i].onClicked(GWIDGET_CB{
      int ii = (GImageButton*) widget - &btn[0];
      label = btn_str[ii];
    });
  }

  cont_btn_x.bg_opa(0);
  cont_btn_x.layout(LAYOUT_ROW_M, 5, 5,5,5, 100);
  for(int i=0; i< 2; i++){
    btn_x[i] = (i==0)? SYMBOL_ARROW_LEFT : SYMBOL_ARROW_RIGHT;
    btn_x[i].parent(cont_btn_x);
  }
  cont_btn_x.align(ALIGN_BOTTOM, 0,-40);

  btn_x[0].onClicked(GWIDGET_CB {
    if(is_animation) return;
    anim_btn.init(btn[0], [](GWIDGET, int32_t v) {
      // คำนวณมุมเริ่มต้นจากสถานะปัจจุบัน
      float theta_start = PI / 2 + 2 * PI * cur_id / btn_num;
      float offset_angle = map_f(v, 0, 1024, 0, 2 * PI / btn_num);
      for (int i = 0; i < btn_num; i++) {
        float theta = theta_start + offset_angle + 2 * PI * i / btn_num;
        int x = BlynkGO.width() / 2 + RADIUS_A * cos(theta);
        int y = BlynkGO.height() / 2 + RADIUS_B * sin(theta);
        btn[i].center(x, y);
      }
    }, 0, 1024, 500);
    anim_btn.ready_cb([](GAnimation *a) { 
      is_animation = false;
      cur_id = (cur_id + 1) % btn_num;
      label = btn_str[(btn_num-cur_id) % btn_num];
    });
    anim_btn.start();
    is_animation = true;
  });

  btn_x[1].onClicked(GWIDGET_CB {
    if(is_animation) return;
    anim_btn.init(btn[0], [](GWIDGET, int32_t v) {
      // คำนวณมุมเริ่มต้นจากสถานะปัจจุบัน
      float theta_start = PI / 2 + 2 * PI * cur_id / btn_num;
      float offset_angle = map_f(v, 0, 1024, 0, -2 * PI / btn_num); // หมุนย้อนกลับ
      for (int i = 0; i < btn_num; i++) {
        float theta = theta_start + offset_angle + 2 * PI * i / btn_num;
        int x = BlynkGO.width() / 2 + RADIUS_A * cos(theta);
        int y = BlynkGO.height() / 2 + RADIUS_B * sin(theta);
        btn[i].center(x, y);
      }
    }, 0, 1024, 500);
    anim_btn.ready_cb([](GAnimation *a) { 
      is_animation = false;
      cur_id = (btn_num + cur_id - 1) % btn_num;
      label = btn_str[(btn_num-cur_id) % btn_num];
    });
    anim_btn.start();
    is_animation = true;
  });
  
  label = btn_str[(btn_num-cur_id) % btn_num];
}

void loop(){
  BlynkGO.update();
}
