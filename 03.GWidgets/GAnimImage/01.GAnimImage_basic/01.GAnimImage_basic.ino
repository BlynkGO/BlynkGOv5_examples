#include <BlynkGOv5.h>

IMAGE_DECLARE(animimg001);
IMAGE_DECLARE(animimg002);
IMAGE_DECLARE(animimg003);

const img_t * anim_imgs[3] = {
  & animimg001,
  & animimg002,
  & animimg003,
};

GAnimImage anim_image;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  anim_image.position(10, 10);
  anim_image.start(anim_imgs, 3, 1000);   /* เริ่มรัน animation รูป ด้วย array รูป anim_imgs จำนวน 3 รูป ภายใน 1000 ms*/

}

void loop() {
  BlynkGO.update();
}
