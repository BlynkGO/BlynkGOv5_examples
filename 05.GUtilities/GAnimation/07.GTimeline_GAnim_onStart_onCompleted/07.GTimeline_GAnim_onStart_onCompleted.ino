#include <BlynkGOv5.h>

GTimeline timeline;
  GAnimation anim[3];

GContainer cont_btn(LAYOUT_ROW_M, 10, 10, 10, 10, 30);
  GButton btn_forward("Forward", cont_btn);
  GButton btn_backward("Backward", cont_btn);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();

  for(int i = 0; i < 3; i++) {
    anim[i].init([](void * var, int32_t v) {
      int ii = (GAnimation *) var - &anim[0];
      Serial.printf("%*s[GAnim %d] %d\n", 20 * ii, "", ii, v);
    }, 0, 1024, 300, (i==1)? ANIM_PATH_BOUNCE : ANIM_PATH_LINEAR);
    anim[i].var(&anim[i]);
    anim[i].onStart([](GANIM) {
      int ii = a - &anim[0];
      Serial.printf("%*s[GAnim %d] start!!\n", 20 * ii, "", ii);
    });
    anim[i].onCompleted([](GANIM) {
      int ii = a - &anim[0];
      Serial.printf("%*s[GAnim %d] completed\n", 20 * ii, "", ii);
    });
    timeline.add(200 * i, anim[i]);
  }

  cont_btn.bg_opa(0);
  btn_forward.onClicked(GWIDGET_CB {
    timeline.reverse(false);
    timeline.progress(0);
    Serial.println("-------------------------------------------------");
    Serial.println("  Timeline Forward");
    Serial.println("-------------------------------------------------");
    timeline.start();
  });
  btn_backward.onClicked(GWIDGET_CB {
    timeline.reverse(true);
    timeline.progress(PROGRESS_MAX);
    Serial.println("-------------------------------------------------");
    Serial.println("  Timeline Backward");
    Serial.println("-------------------------------------------------");
    timeline.start();
  });
}

void loop()
{
  BlynkGO.update();
}
