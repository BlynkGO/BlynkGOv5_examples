#include <BlynkGOv5.h>

GSpinner spinner;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  spinner.size(100,100);
  spinner.anim_params(10000,200);

  // spinner.color(TFT_RED);            // สีของ spinner
  // spinner.bg_color(TFT_DARKGRAY);    // สีพื้นด้านล่างของ spinner
  // spinner.thickness(15);             // ความหนาของ spinner
  // spinner.bg_thickness(17);          // ความหนา ด้านล่างของ spinner

}

void loop(){
  BlynkGO.update();
}
