
#include <BlynkGOv5.h>

// ตัวแบต
GBar bar_batt;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();

  // ดีไซน์ ตัวแบต
  bar_batt.size(140, 200);
  bar_batt.border(4, TFT_WHITE);
  bar_batt.bg_color(TFT_BLACK);
  bar_batt.bg_opa(255);
  bar_batt.color(TFT_COLOR_HEX(0x67ff49));
  bar_batt.pad(10);
  bar_batt.radius(30);
  bar_batt.radius(25, GPART_INDICATOR);
    // ดีไซน์เพิ่มเส้นเงา เพื่อความสวยงาม
    static GRect rt_reflect(bar_batt);
    rt_reflect.size(12, 50);
    rt_reflect.align(ALIGN_BOTTOM_LEFT, 10, -20);
    rt_reflect.bg_opa(100);
    rt_reflect.color(TFT_WHITE);
    rt_reflect.radius(RADIUS_CIRCLE);

    static GLine ln_reflect(bar_batt);
    Point p1 = Point(bar_batt.width() - 30, 20);
    ln_reflect.setPoints(p1, p1 + Point(0, 30));
    ln_reflect.thick(4, TFT_WHITE);
    ln_reflect.rounded(true);

    // ดีไซน์เพิ่มส่วนขั้้วแบต
    static GRect rt_batt_terminal;
    rt_batt_terminal.size(63, 30);
    rt_batt_terminal.radius(10);
    rt_batt_terminal.color(TFT_WHITE);
    rt_batt_terminal.toBackground();
    rt_batt_terminal.align(bar_batt, ALIGN_TOP, 0, 14);
    static GLine ln_helper(rt_batt_terminal);
    ln_helper.setPoints(Point(0, 15), 100);
    ln_helper.thick(6, TFT_BLACK);

  static GAnimation anim;
  anim.init(bar_batt, [](GWIDGET, int32_t v) {
    bar_batt = v;
  }, 0, 100, 1000, true, true);
  anim.create();
}

void loop()
{
  BlynkGO.update();
}
