#include <BlynkGOv5.h>

GRoller roller;
  String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                        "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                        "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };
void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_PALETTE(TFT_PALETTE_BLUE_GRAY));
  
  roller.options(th_month, 12);             // กำหนด options ให้ roller_month ด้วย String Array รายชื่อเดือน
  roller.radius(0);
  roller.border(0);
  roller.bg_color(TFT_BLACK);
  roller.font_color(TFT_WHITE);
  roller.visible_row_count(4);

  DRAW_BUF_DEFINE_STATIC(mask, 130, 240, COLOR_FORMAT_MASK);
  DRAW_BUF_INIT_STATIC(mask);

  GCanvas canvas;
  GLayer layer;

  canvas.draw_buf(mask);
  canvas.fill_bg(TFT_WHITE, 0);
  canvas.init_layer(layer);
    draw_rect_dsc_t rect_dsc;
    draw_rect_dsc_init(&rect_dsc);

    rect_dsc.bg_grad.dir            = GRAD_DIR_VER;
    rect_dsc.bg_grad.stops[0].opa   = 255;
    rect_dsc.bg_grad.stops[1].opa   = 255;
    rect_dsc.bg_grad.stops[0].color = TFT_COLOR(TFT_BLACK);
    rect_dsc.bg_grad.stops[1].color = TFT_COLOR(TFT_WHITE);

    area_t a = {0, 0, canvas.width() - 1, canvas.height() / 2 - 10};
    layer.drawRect(rect_dsc, a);

    a.y1 = canvas.height() / 2 + 10;
    a.y2 = canvas.height() - 1;
    rect_dsc.bg_grad.stops[0].color = TFT_COLOR(TFT_WHITE);
    rect_dsc.bg_grad.stops[1].color = TFT_COLOR(TFT_BLACK);
    layer.drawRect(rect_dsc, a);

  canvas.finish_layer(layer);
  canvas.del();

  roller.mask(&mask);

}

void loop(){
  BlynkGO.update();
}

