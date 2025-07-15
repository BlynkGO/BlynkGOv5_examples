#include <BlynkGOv5.h>

FONT_DECLARE(BebasNeueBold_num_45);

GContainer cont_time_setting;
  GRoller roller_minute(cont_time_setting);
  GRoller roller_second(cont_time_setting);

GLabel lb_timer;
GButton btn_timer;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  cont_time_setting.layout(LAYOUT_ROW_M,5,5,5,5,5);
  cont_time_setting.bg_opa(0);
    roller_minute.options(0,59, true);
    roller_second.options(0,59, true);

  btn_timer = SYMBOL_PLAY;
  btn_timer.round_design();
  btn_timer.border(1, TFT_SILVER);
  btn_timer.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN,2));
  btn_timer.align(cont_time_setting, ALIGN_BOTTOM, 0, 30);

  lb_timer.font(BebasNeueBold_num_45, TFT_BLACK);
  static SoftTimer timer_counter, timer_delay;
  static int32_t counter;
  enum {STATE_STOP, STATE_RUNNING, STATE_PAUSE};
  static uint8_t state=STATE_STOP;
  static bool long_pressed=false;
  btn_timer.onClicked(GWIDGET_CB{
    if(long_pressed) { long_pressed=false; return;}
    switch(state){
      case STATE_STOP:
      {
        state = STATE_RUNNING;
        counter = roller_minute.toInt() * 60 + roller_second.toInt();
        cont_time_setting.show(false);
        lb_timer.show(true);
        lb_timer = StringX::printf("%02d:%02d", counter/60, counter%60);
        timer_counter.setInterval(1000, counter,[](){
          counter--;
          lb_timer = StringX::printf("%02d:%02d", counter/60, counter%60);
        });
        timer_counter.ready_cb([](){
          timer_delay.delay(1000,[](){
            lb_timer.show(false);
            cont_time_setting.show(true);
            btn_timer.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN,2));
            btn_timer = SYMBOL_PLAY;
            state = STATE_STOP;
          });
        });
        btn_timer = SYMBOL_PAUSE;
        btn_timer.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_YELLOW,2));
        break;
      }
      case STATE_RUNNING:
      { 
        timer_counter.pause();
        btn_timer.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN,2));
        btn_timer = SYMBOL_PLAY;
        state = STATE_PAUSE;
        break;
      }
      case STATE_PAUSE:
      { 
        timer_counter.resume();
        btn_timer = SYMBOL_PAUSE;
        btn_timer.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_YELLOW,2));
        state = STATE_RUNNING;
        break;
      }
    }        
  });
  btn_timer.onLongPressed(GWIDGET_CB{
    long_pressed = true;
    lb_timer.show(false);
    cont_time_setting.show(true);
    btn_timer.font_color(TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN,2));
    btn_timer = SYMBOL_PLAY;
    state= STATE_STOP;
    timer_counter.del();
  });
}

void loop(){
  BlynkGO.update();
}

