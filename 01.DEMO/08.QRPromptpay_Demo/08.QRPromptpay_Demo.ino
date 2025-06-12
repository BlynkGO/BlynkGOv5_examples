#include <BlynkGOv5.h>

GContainer cont_frame;
  GLabel lb_title("QR Code Promptpay", cont_frame);
  GQRCode qrcode(cont_frame);
  GLabel lb_nation_id_or_phone_no("รหัสบัตรประชาชน หรือ เบอร์มือถือ", cont_frame);
  GTextArea ta_nation_id_or_phone_no(cont_frame);
  GLabel lb_price("ยอดเงิน :", cont_frame);
  GTextArea ta_price(cont_frame);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_frame.size(SIZE_CONTENT, SIZE_CONTENT);
  cont_frame.GRID_CELL(
    GRID_COL{10,GRID_CONTENT, GRID_CONTENT,20}, GRID_ROW{ GRID_CONTENT, 10, GRID_CONTENT, GRID_CONTENT,GRID_CONTENT, GRID_CONTENT,GRID_CONTENT}
  );
  cont_frame.padding(5,20,5,5,10,20);
  cont_frame.border(1, TFT_SILVER);
    lb_title.grid_cell(0,0,4,1, GRID_ALIGN_STRETCH, GRID_ALIGN_CENTER);
    lb_title.text_align(TEXT_ALIGN_CENTER);
    lb_title.bg_opa(255);
    lb_title.bg_color(TFT_PALETTE(TFT_PALETTE_BLUE));
    lb_title.color(TFT_WHITE);
    qrcode.size(200,200);
    qrcode.grid_cell(1,2,1,4,GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    qrcode.encodePromptPay("01234567891", 100.12);

    lb_nation_id_or_phone_no.font(prasanmit_15);
    lb_nation_id_or_phone_no.grid_cell(2,2, GRID_ALIGN_START,GRID_ALIGN_CENTER);
    ta_nation_id_or_phone_no.grid_cell(2,3, GRID_ALIGN_START, GRID_ALIGN_CENTER);
    lb_price.font(prasanmit_15);
    lb_price.grid_cell(2,4, GRID_ALIGN_START, GRID_ALIGN_CENTER);
    ta_price.grid_cell(2,5, GRID_ALIGN_START, GRID_ALIGN_CENTER);

    ta_nation_id_or_phone_no.onValueChanged(GWIDGET_CB{
      if(ta_nation_id_or_phone_no.toString() == "") return;
      if(ta_price.toString() == "") return;
      String id = ta_nation_id_or_phone_no.toString();
      float price = ta_price.toFloat();
      qrcode.encodePromptPay(id, price);
    });
    ta_price.onValueChanged(GWIDGET_CB{ ta_nation_id_or_phone_no.event_send(EVENT_VALUE_CHANGED); });
}

void loop(){
  BlynkGO.update();
}
