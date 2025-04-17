#include <BlynkGOv5.h>

//----------------------------------------------------
GLabel  lb_counter;     // วิตเจ็ตข้อความ
GContainer cont_btns;   // วิตเจ็ตสำหรับรวมกลุ่ม วิตเจ็ตปุ่มทั้งหลายเข้าด้วยกัน
  GButton btn_15_min      ("15"         , cont_btns);
  GButton btn_30_min      ("30"         , cont_btns);
  GButton btn_stop        (SYMBOL_STOP  , cont_btns);
  GButton btn_start_pause (SYMBOL_PLAY  , cont_btns);
//----------------------------------------------------
GTimer  timer;

enum { SELECT_15, SELECT_30 };
uint8_t select_type = SELECT_15;
int32_t counter=15;

enum { STATE_STOP, STATE_START, STATE_PAUSE };
uint8_t state = STATE_STOP;
//----------------------------------------------------

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  //-----------------------------------
  // ส่วน ออกแบบ 
  //-------------------------
  lb_counter = StringX::printf(" %d ", counter);
  lb_counter.font(prasanmit_40,TFT_WHITE);
  lb_counter.align(ALIGN_TOP,0,60);

  // จัด LAYOUT และช่องไฟ
  // โดย ช่องไฟ บนล่างซ้ายขวา 20 px; ช่องภายใน ระหว่างปุ่มห่างกัน 6% ของด้านกว้าง
  cont_btns.layout(LAYOUT_ROW_M, 20, 20, 20, 20, PCT_W(6));  
  cont_btns.bg_opa(0);                          // ให้พื้นหลังโปร่งใส
  cont_btns.align_center(0, PCT(10));           // วางไว้กลางจอ เลื่อนลง 30% ของด้านสูง

    static GStyle style_btn;
      style_btn.size(PCT_W(16), PCT_W(16));
      style_btn.color(TFT_BLUE);
      style_btn.border(5, TFT_SKYBLUE);
      style_btn.radius(10);
      style_btn.shadow(20, TFT_BLACK);
      style_btn.shadow_offset(6,6);

    btn_15_min      .addStyle(style_btn);
    btn_30_min      .addStyle(style_btn);
    btn_stop        .addStyle(style_btn);
    btn_start_pause .addStyle(style_btn);


  //-----------------------------------
  // ส่วน ควบคุมการทำงาน
  //-------------------------
  btn_15_min.onClicked(GWIDGET_CB{
    lb_counter = StringX::printf(" %d ", counter=15);
    select_type = SELECT_15;
  });
  
  btn_30_min.onClicked(GWIDGET_CB{
    lb_counter = StringX::printf(" %d ", counter=30);
    select_type = SELECT_30;
  });
  
  btn_stop.onClicked(GWIDGET_CB{
    state = STATE_STOP;
    btn_start_pause = SYMBOL_PLAY;
    timer.del();
    switch(select_type) { 
      case SELECT_15: lb_counter = StringX::printf(" %d ", counter=15); break;
      case SELECT_30: lb_counter = StringX::printf(" %d ", counter=30); break;
    }
  });
  
  btn_start_pause.onClicked (GWIDGET_CB{
    switch(state){
      case STATE_STOP: 
      case STATE_PAUSE:
        state = STATE_START; 
        btn_start_pause = SYMBOL_PAUSE;
        timer.setInterval(1000,[](){ 
          lb_counter = StringX::printf(" %d ", --counter);
          if(counter <= 0) {
            // ให้ อีก 3 วินาทีถัดไป ปุ่ม btn_stop ถูกสั่งเหมือนถูก clicked โดยผู้ใช้
            timer.delay(3000,[](){ 
              btn_stop.event_send(EVENT_CLICKED);
            });
          }
        });
        break;
      case STATE_START:
        state = STATE_PAUSE; 
        btn_start_pause = SYMBOL_PLAY;
        timer.del();
        break;
    }
  }); 
}

void loop() {
  BlynkGO.update();
}

