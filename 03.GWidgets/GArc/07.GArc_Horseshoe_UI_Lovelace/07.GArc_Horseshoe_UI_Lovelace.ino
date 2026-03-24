#include <BlynkGOv5.h>

FONT_DECLARE(prasanmit_15);

GRect rect;
  GArc arc(rect);
    GRect  info(arc);
      GRect  box_watt(info);
        GLabel lb_watt_v(box_watt);
        GLabel lb_watt_u(box_watt);
      GRect  box_volt(info);
        GLabel lb_volt_v(box_volt);
        GLabel lb_volt_u(box_volt);
      GRect  box_amp(info);
        GLabel lb_amp_v(box_amp);
        GLabel lb_amp_u(box_amp);
  GLabel lb_mppt(rect);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY, 3));
 
  rect.size(SIZE_CONTENT, SIZE_CONTENT);
  rect.padding(10,5,30,30);
  rect.color(TFT_COLOR_HEX(0x1c1c1c));
  rect.radius(10);
    arc.size(120, 120);
    arc.rounded(true);

    arc.arc_color(TFT_BLACK, GPART_MAIN); 
    arc.arc_opa(255, GPART_MAIN);
    arc.arc_width(4, GPART_MAIN);

    arc.arc_color(TFT_PALETTE_DARKEN(TFT_PALETTE_TEAL, 2), GPART_INDICATOR); 
    arc.arc_opa(255, GPART_INDICATOR);
    arc.arc_width(8, GPART_INDICATOR);
    arc.padding(-2, GPART_INDICATOR);

    arc.angle_offset(135);
    arc.angle_range(270);
    arc.range(0,100);              // range ของค่าที่จะใช้กับ arc (หากไม่ใส่ จะเป็นช่วง 0-100 อัตโนมัติ)
    arc = 80;                      // วาด arc แบบ กำหนดด้วยค่า โดยเป็นค่าในช่วง range(..) ที่กำหนด

      info.bg_opa(0);
      info.size(PCT(75), SIZE_CONTENT);
      info.GRID_CELL(GRID_COL{GRID_FR(1), GRID_FR(1)}, GRID_ROW{ 45, 30});
      info.padding(0);
      info.align(ALIGN_CENTER,0,-5);

        box_watt.grid_cell(0,0,2,1);
        box_watt.bg_opa(0);
        box_watt.border(1,TFT_GRAY);
        box_watt.border_side(BORDER_SIDE_BOTTOM);
          lb_watt_v = 1374;
          lb_watt_v.font(prasanmit_35, TFT_WHITE);
          lb_watt_v.align(ALIGN_CENTER,-3);

          lb_watt_u = "W";
          lb_watt_u.font(prasanmit_20, TFT_GRAY);
          lb_watt_u.align(lb_watt_v, ALIGN_RIGHT_TOP,2,8);

        box_volt.grid_cell(0,1);
        box_volt.bg_opa(0);
        box_volt.border(1,TFT_GRAY);
        box_volt.border_side(BORDER_SIDE_RIGHT);
          lb_volt_u = "V";
          lb_volt_u.font(prasanmit_15, TFT_GRAY);
          lb_volt_u.align(ALIGN_RIGHT, -4,0);

          lb_volt_v = 192;
          lb_volt_v.font(prasanmit_20, TFT_WHITE);
          lb_volt_v.align(lb_volt_u, ALIGN_LEFT_TOP, -2, -2);

        box_amp.grid_cell(1,1);
        box_amp.bg_opa(0);
          lb_amp_u = "A";
          lb_amp_u.font(prasanmit_15, TFT_GRAY);
          lb_amp_u.align(ALIGN_RIGHT, -2,0);

          lb_amp_v = "7.14";
          lb_amp_v.font(prasanmit_20, TFT_WHITE);
          lb_amp_v.align(lb_amp_u, ALIGN_LEFT_TOP, -2, -2);

    lb_mppt = "MPPT Garden";
    lb_mppt.font(prasanmit_20, TFT_WHITE);
    lb_mppt.align(arc, ALIGN_BOTTOM,0,-7);
}

void loop(){
  BlynkGO.update();
}

