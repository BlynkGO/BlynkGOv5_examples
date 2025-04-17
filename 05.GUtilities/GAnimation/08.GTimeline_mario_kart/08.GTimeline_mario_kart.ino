#include <BlynkGOv5.h>

IMAGE_DECLARE(img_mario);

GRect racing;
GRect racing_border1;
GRect racing_border2;

GImage mario(img_mario);

GContainer cont_btn(LAYOUT_ROW_M, 10, 10, 10, 10, 30);
  GButton btn_new("new", cont_btn);
  GButton btn_play_pause("pause/play", cont_btn);

GTimeline timeline;
  GAnimation anim1;
  GAnimation anim2;
  GAnimation anim3;
  GAnimation anim4;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  racing.border(100, TFT_COLOR_HSV(0, 0, 10));
  racing.radius(RADIUS_CIRCLE);
  racing.bg_opa(0);

  racing_border1.bg_opa(0);
  racing_border1.border(5, TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED, 1));
  racing_border1.radius(RADIUS_CIRCLE);
  racing_border2.size(racing.width() - 2 * racing.border() + 10, racing.height() - 2 * racing.border() + 10);
  racing_border2.bg_opa(0);
  racing_border2.border(5, TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED, 1));
  racing_border2.radius(RADIUS_CIRCLE);

  mario.center(220, 50);

  cont_btn.bg_opa(0);

  btn_new.onClicked(GWIDGET_CB {
    auto rand1 = (random(0, 100) > 90) ?  random(2000, 2500) : random(300, 1200);
    auto rand2 = (random(0, 100) > 90) ?  random(800, 1500)  : random(200, 500);
    auto rand3 = (random(0, 100) > 90) ?  random(3000, 4000) : random(600, 1200);
    auto rand4 = (random(0, 100) > 90) ?  random(800, 1500)  : random(300, 300);

    anim1.init(mario, [](GWIDGET, int32_t v) {
      mario.rotation(0);
      mario.center(v, 50);
    }, 220, 600, rand1, (anim_path_type_t) random(4));

    anim2.init(mario, [](GWIDGET, int32_t v) {
      if(! timeline.isProgressing()) return;
      Point point_control[4] = { {600, 50}, {805.37, 70.44}, {816.55, 394.75}, {600, 430}};
      Point p;

      mario.rotation(map(v, 0, 1024, 0, 180));
      mario.center(p.bezier(float(v) / 1024, point_control, 4));
    }, 0, 1024, rand2, (anim_path_type_t) random(4));

    anim3.init(mario, [](GWIDGET, int32_t v) {
      if(! timeline.isProgressing()) return;
      mario.rotation(180);
      mario.center(v, 430);
    }, 600, 220, rand3, (anim_path_type_t) random(4));

    anim4.init(mario, [](GWIDGET, int32_t v) {
      if(! timeline.isProgressing()) return;

      Point point_control[4] = { {220, 430}, {52, 375.37}, {29.26, 54.79}, {220, 50}};
      Point p;

      mario.rotation(map(v, 0, 1024, 180, 360));
      mario.center(p.bezier(float(v) / 1024, point_control, 4));
    }, 0, 1024, rand4, (anim_path_type_t) random(4));

    Serial.printf("[rand] %d %d %d %d\n", rand1, rand2, rand3, rand4);
    timeline.create();
    timeline.add(0, anim1);
    timeline.add(rand1, anim2);
    timeline.add(rand1 + rand2, anim3);
    timeline.add(rand1 + rand2 + rand3, anim4);

    timeline.onCompleted([](GTIMELINE) {
      btn_new.event_send(EVENT_CLICKED);
      timeline.start();
    });
    timeline.progress(0);
  });
  btn_new.event_send(EVENT_CLICKED);

  btn_play_pause.onClicked(GWIDGET_CB {
    (timeline.isProgressing())? timeline.pause() : timeline.start();
  });
}

void loop()
{
  BlynkGO.update();
}