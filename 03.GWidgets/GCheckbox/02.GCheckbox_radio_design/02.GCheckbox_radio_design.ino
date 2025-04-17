#include <BlynkGOv5.h>

GPage page_checkbox_A(LAYOUT_COL_L,5,5,10,10,10);
  GCheckbox checkbox_A[5];

GContainer cont_checkbox_B(LAYOUT_COL_L,5,5,10,10,3);
  GCheckbox checkbox_B[4];

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  page_checkbox_A.size(PCT(40), PCT(80));  // 40% x 80% ของหน้าจอ  
  page_checkbox_A.border(1, TFT_COLOR_HEX(0xbbb));
  page_checkbox_A.radius(10);
  page_checkbox_A.bg_opa(0);
  page_checkbox_A.align(ALIGN_CENTER, -page_checkbox_A.width()/2 -10);

  for(int i=0; i<5; i++){
    checkbox_A[i].parent(page_checkbox_A);
    checkbox_A[i].radio_design();
    checkbox_A[i].color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_RED,1));
    checkbox_A[i] = StringX::printf("A %d", i+1);
  }

  static uint32_t active_id_1 = 0;
  page_checkbox_A.child(active_id_1)->checked(true);        // วิตเจ็ตลูก id = 0 ให้ checked
  page_checkbox_A.onClicked([active_id_1](GWIDGET){
    page_checkbox_A.child(active_id_1)->checked(false);     // วิตเจ็ตลูก ของเก่า ให้ unchecked
    active_id_1 = page_checkbox_A.event_target_id();        // id วิตเจ็ตลูก ของใหม่ ที่มีการถูก clicked
    page_checkbox_A.child(active_id_1)->checked(true);      // วิตเจ็ตลูก ของใหม่ ให้ checked
  });

  //--------------------------------

  cont_checkbox_B.size( page_checkbox_A );
  cont_checkbox_B.border(1, TFT_COLOR_HEX(0xbbb));
  cont_checkbox_B.radius(10);
  cont_checkbox_B.bg_opa(0);
  cont_checkbox_B.align(page_checkbox_A, ALIGN_RIGHT,20);
  for(int i=0; i<4; i++){
    checkbox_B[i].parent(cont_checkbox_B);
    checkbox_B[i].radio_design();
    checkbox_B[i] = StringX::printf("B %d", i+1);
  }

  static uint32_t active_id_2 = 2;
  cont_checkbox_B.child(active_id_2)->checked(true);        // วิตเจ็ตลูก id = 2 ให้ checked
  cont_checkbox_B.onClicked([active_id_2](GWIDGET){
    cont_checkbox_B.child(active_id_2)->checked(false);     // วิตเจ็ตลูก ของเก่า ให้ unchecked
    active_id_2 = cont_checkbox_B.event_target_id();        // id วิตเจ็ตลูก ของใหม่ ที่มีการถูก clicked
    cont_checkbox_B.child(active_id_2)->checked(true);      // วิตเจ็ตลูก ของใหม่ ให้ checked
  });
}

void loop(){
  BlynkGO.update();
}

