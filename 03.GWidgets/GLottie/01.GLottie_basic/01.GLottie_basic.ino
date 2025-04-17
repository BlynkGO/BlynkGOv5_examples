#include <BlynkGOv5.h>

// extern const uint8_t lottie_approve[];
// extern const size_t  lottie_approve_size;

extern const uint8_t lottie_sandclock[];
extern const size_t lottie_sandclock_size;

GLottie lottie;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // lottie.src_data(lottie_approve, lottie_approve_size);
  lottie.src_data(lottie_sandclock, lottie_sandclock_size);

  static uint8_t buf[64 * 64 * 4];
  lottie.buffer(64, 64, buf);
}

void loop() {
  BlynkGO.update();
}
