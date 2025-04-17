#include <BlynkGOv5.h>

FONT_DECLARE(Dseg7_BoldItalic_40);
FONT_DECLARE(Dseg7_BoldItalic_160);
FONT_DECLARE(material_icon_alarmtimer_40);

#define SYMBOL_TIMER_PLAY         "\uf4ba"
#define SYMBOL_TIMER_PAUSE        "\uf4bb"
#define SYMBOL_TIMER              "\ue425"
#define SYMBOL_TIMER_OFF          "\ue426"
#define SYMBOL_TIMER_ARROW_UP     "\uf377"
#define SYMBOL_TIMER_ARROW_DOWN   "\uf378"
#define SYMBOL_RESTART_ALT        "\uf053"

class GTeamScore : public GContainer {
  public:
    GTextArea ta_team;
    GLabel lb_score[2];
    GRect rect_touch[2];

    GTeamScore(GWidget&parent = GScreen) : GContainer(parent) {}
    void create(){
      if(!this->isCreated()) {
        GContainer::create();
        GContainer::layout(LAYOUT_COL_M, 0, 0, 0, 0, 80);
        GContainer::bg_opa(0);
          ta_team.parent(this);
          ta_team.width(GScreen.width()*45/100);
          ta_team.color(TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE, 3));
          ta_team.text_align(TEXT_ALIGN_CENTER);
          ta_team.font(prasanmit_40, TFT_WHITE);
          ta_team = "TEAM1";
          for(int i=0;i<2; i++) {
            lb_score[i].font(Dseg7_BoldItalic_160);
            lb_score[i].color((i==0)? TFT_COLOR_HSV(0,0,5) : TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE, 3));
            lb_score[i] = (i==0)? String("00"): String(12);
            if(i==1) lb_score[i].ignore_layout(true);
            lb_score[i].parent(this);
          }
          lb_score[1].align(lb_score[0], ALIGN_IN_RIGHT);

          for(int i=0;i<2; i++){
            rect_touch[i].ignore_layout(true);
            rect_touch[i].parent(this);
            rect_touch[i].bg_opa(0);
            rect_touch[i].color( (i==0)? TFT_RED : TFT_BLUE);
            rect_touch[i].size(lb_score[0].width() + 40, lb_score[0].height());
            rect_touch[i].align(lb_score[0], ALIGN_CENTER, 0,( i==0? -1:1 ) * lb_score[0].height()/2 );
            rect_touch[i].user_data(this);
            rect_touch[i].onClicked([](GWIDGET){
              GTeamScore* _team_score = (GTeamScore*) (widget->user_data());
              int ii= (GRect*) widget - &_team_score->rect_touch[0];
              if(ii==0){
                _team_score->score( _team_score->score()+1);
              }else{
                if(_team_score->score()) _team_score->score( _team_score->score()-1);
              }
            });
          }
      }      
    }
    void color(color_t c)       {create(); ta_team.color(c); lb_score[1].color(c); }
    void score(int32_t s)       {create(); lb_score[1] = s; }
    int32_t score()             {create(); return lb_score[1].toInt(); lb_score[1].align(lb_score[0], ALIGN_IN_RIGHT); }
    void team_name(String team) {create(); ta_team = team; }
};

class GBoxInfo : public GContainer {
  public:
    GLabel lb_key;
    GLabel lb_value;
    GBoxInfo(GWidget&parent = GScreen) : GContainer(parent) {}
    void create(){
      if(!this->isCreated()){
        GContainer::create();
        GContainer::bg_opa(0);
        GContainer::layout(LAYOUT_COL_M,0,0,0,0,0);
          lb_key.parent(this);
          lb_key = "key";
          lb_key.font(prasanmit_40, TFT_WHITE);
          lb_value.parent(this);
          lb_value.font(Dseg7_BoldItalic_40);
          lb_value.color(TFT_PALETTE_DARKEN(TFT_PALETTE_YELLOW, 3));
          lb_value = 1;
      }
    }
    void key(String key)      { create(); lb_key = key;     }
    void value(String value)  { create(); lb_value = value; }
    inline void value(int n)  { value(String(n));           }
    void color(color_t c)     { create(); lb_value.color(c);}
};

GContainer cont_score;
  GTeamScore team_score[2];

GBoxInfo box_round;
  int8_t match_round = 0;
GBoxInfo box_timecounter;

GRect rect_touch_protector;
  GRect rect_timecounter(rect_touch_protector);
    GContainer cont_timecounter(rect_timecounter);
      GRoller roller_minute(cont_timecounter);
      GLabel  lb_colon(":", cont_timecounter);
      GRoller roller_second(cont_timecounter);

GRect rect_bottom_touch;      // ไว้สัมผัสเพื่อให้ cont_toolbar แสดงขึ้นมา

GContainer cont_toolbar;      // แถบ toolbar ด้านล่าง
  GContainer cont_timer_control_panel(cont_toolbar);   // กลุ่มรวม ปุ่ม icon (สร้างจาก GLabel) กด
    GLabel lb_timer_control(cont_timer_control_panel);
      enum { STATE_TIMER_STOP, STATE_TIMER_PLAY, STATE_TIMER_PAUSE, STATE_TIMER_FINISHED };
      static int8_t state_timer = STATE_TIMER_STOP;
    GLabel lb_timer_count_up_down(cont_timer_control_panel);
    GLabel lb_reset_scoreboard(cont_timer_control_panel);
  GLabel lb_timelimit(cont_toolbar);  // ตัวบอกเวลา timelimit ที่ตั้งไว้



#define TIME_TOTOAL_SEC(minute, second) (((minute) * 60) + (second))
int32_t round_timeout =  TIME_TOTOAL_SEC(45,0); // second
int32_t round_timeout_action = round_timeout;

String format_time(uint32_t round_timeout) {
  uint32_t minutes = round_timeout / 60;
  uint32_t seconds = round_timeout % 60;

  // ฟอร์แมตเป็น "02d:02d"
  return StringX::printf("%02d:%02d", minutes, seconds);
}

SoftTimer timer_round_action;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_score.bg_opa(0);
  cont_score.layout(LAYOUT_ROW_M, 5,5,5,5, 30);
  cont_score.align(ALIGN_TOP,0,30);
  for(int i=0; i<2 ;i++){
    team_score[i].parent(cont_score);
    team_score[i].color( (i==0)? TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE, 3) : TFT_PALETTE_DARKEN(TFT_PALETTE_RED, 3) );
    team_score[i].team_name( String("TEAM ")+ String(i+1));
    team_score[i].lb_score[0].translate_x( (i==0? -1:1)* 30 );
    team_score[i].score(random(5,20));
  }

  box_round.key("Round");
  box_round.value(StringX::printf("%02d", match_round = 0));
  box_round.align(ALIGN_CENTER, 0, -40);
  box_round.color(TFT_PALETTE_DARKEN(TFT_PALETTE_YELLOW, 3));
  box_round.onClicked([](GWIDGET){
    Point tp = widget->touch_point();
    if(tp.y <= box_round.height()/2){
      box_round.value(StringX::printf("%02d", ++match_round));
    }else{
      if(match_round){
        box_round.value(StringX::printf("%02d", --match_round));
      }
    }
  });

  
  box_timecounter.key("Time");
  box_timecounter.value(StringX::printf("%02d:%02d", minute(), second() ));
  box_timecounter.align(box_round, ALIGN_BOTTOM, 0, 40);
  box_timecounter.color(TFT_PALETTE(TFT_PALETTE_GREEN));
  box_timecounter.onClicked([](GWIDGET){
    lb_timer_control.event_send(EVENT_CLICKED);
  });
  box_timecounter.onPressed([](GWIDGET){
    lb_timer_control.event_send(EVENT_PRESSED);
  });
  box_timecounter.onLongPressed([](GWIDGET){
    lb_timer_control.event_send(EVENT_LONG_PRESSED);
  });

  rect_touch_protector.show(false);
  rect_touch_protector.bg_opa(0);
  rect_touch_protector.onClicked([](GWIDGET){ rect_touch_protector.show(false); });
    rect_timecounter.size(SIZE_CONTENT, SIZE_CONTENT);
    rect_timecounter.padding(30, 30, 50, 50, 0);
    rect_timecounter.radius(20);
    rect_timecounter.border(3, TFT_SILVER);
    rect_timecounter.event_parent(true);      // หากเกิด event อะไรมาที่นี่ ให้ส่งต่อไปยัง parent ด้วย
      cont_timecounter.bg_opa(0);
      cont_timecounter.layout(LAYOUT_ROW_M,5,5,5,5,5);
        static auto roller_onValueChange =  [](GWIDGET){
                                              round_timeout = TIME_TOTOAL_SEC(roller_minute.toInt(), roller_second.toInt());
                                              lb_timelimit = format_time(round_timeout);
                                              if(state_timer == STATE_TIMER_STOP){
                                                round_timeout_action = (lb_timer_count_up_down == SYMBOL_TIMER_ARROW_UP)? 0 : round_timeout;
                                                box_timecounter.value( format_time(round_timeout_action));
                                              }
                                            };
        roller_minute.options(0,59, true);    // สร้าง options จาก 0 ถึง 59 โดย true คือ ให้มี 0 นำหน้าด้วย
        roller_minute.width(70);
        roller_minute.border(3, TFT_RED, GPART_SELECTED);
        roller_minute.font(prasanmit_40, prasanmit_20);
        roller_minute.onValueChanged(roller_onValueChange);
        roller_minute.selected( round_timeout / 60); 

        lb_colon = ":";

        roller_second.options(0,59, true);    // สร้าง options จาก 0 ถึง 59 โดย true คือ ให้มี 0 นำหน้าด้วย
        roller_second.width(70);
        roller_second.border(3, TFT_RED, GPART_SELECTED);
        roller_second.font(prasanmit_40, prasanmit_20);
        roller_second.onValueChanged(roller_onValueChange);
        roller_second.selected( round_timeout % 60); 

    rect_bottom_touch.bg_opa(0);
    rect_bottom_touch.size(PCT(100), 60);
    rect_bottom_touch.align(ALIGN_BOTTOM);
    rect_bottom_touch.onClicked([](GWIDGET){
      cont_toolbar.show(!cont_toolbar.show());
      if(cont_toolbar.show()) cont_toolbar.toForeground();
    });

    cont_toolbar.bg_opa(0);
    cont_toolbar.layout(LAYOUT_ROW_M, 0,0,0,10,10);
    cont_toolbar.align(ALIGN_BOTTOM_LEFT, 10,-10);
    
      cont_timer_control_panel.bg_opa(20);
      cont_timer_control_panel.radius(RADIUS_CIRCLE);
      cont_timer_control_panel.layout(LAYOUT_ROW_M, 10,10,15,15,20);
      // cont_timer_control_panel.align(ALIGN_BOTTOM_LEFT, 10,-10);
      cont_timer_control_panel.font(material_icon_alarmtimer_40);
        lb_timer_control = SYMBOL_TIMER_OFF;
        lb_timer_control.color(TFT_RED);
        lb_timer_control.clickable(true);
        static bool long_pressed = false;
        lb_timer_control.onClicked([](GWIDGET){
          if(long_pressed) { long_pressed = false; return; }

          switch( state_timer) {
            case STATE_TIMER_STOP:
              lb_timer_control = SYMBOL_TIMER;  // กดจาก stop -> play แต่รูป จะเผื่อไปที่ pause
              lb_timer_control.color(TFT_GREEN);
              box_timecounter.color(TFT_PALETTE(TFT_PALETTE_GREEN));
              state_timer = STATE_TIMER_PLAY;
              timer_round_action.setInterval(1000,[](){
                if(state_timer == STATE_TIMER_PLAY) {
                  round_timeout_action  += ((lb_timer_count_up_down == SYMBOL_TIMER_ARROW_DOWN)? -1 : 1 );
                  box_timecounter.value( format_time(round_timeout_action));
                  if(lb_timer_count_up_down == SYMBOL_TIMER_ARROW_DOWN) {
                    if( round_timeout_action <= 0 ) { 
                      state_timer = STATE_TIMER_FINISHED;
                      timer_round_action.del();
                      // สิ้นสุดการแข่งขัน หากไปทำอะไรต่อให้วางตรงนี้
                    }
                  }else{
                    if( round_timeout_action >= round_timeout){ 
                      state_timer = STATE_TIMER_FINISHED;
                      timer_round_action.del();
                      // สิ้นสุดการแข่งขัน หากไปทำอะไรต่อให้วางตรงนี้
                    }
                  }
                }
              });
              break;
            case STATE_TIMER_PLAY:
              lb_timer_control = SYMBOL_TIMER_PAUSE;  // กดจาก play -> pause แต่รูป จะเผื่อไปที่ timer ปกติ
              lb_timer_control.color(TFT_PALETTE_LIGHTEN(TFT_PALETTE_YELLOW,1));
              box_timecounter.color(lb_timer_control.color());
              state_timer = STATE_TIMER_PAUSE;
              timer_round_action.pause();
              
              break;
            case STATE_TIMER_PAUSE:
              lb_timer_control = SYMBOL_TIMER;  // กดจาก pause -> play แต่รูป จะเผื่อไปที่ pause
              lb_timer_control.color(TFT_GREEN);
              box_timecounter.color(TFT_PALETTE(TFT_PALETTE_GREEN));
              state_timer = STATE_TIMER_PLAY;
              timer_round_action.resume();
              break;
            case STATE_TIMER_FINISHED:
              break;
          }
        });
        lb_timer_control.onPressed([](GWIDGET){  long_pressed = false; });
        lb_timer_control.onLongPressed([](GWIDGET){ 
          if(!long_pressed){
            long_pressed = true;
            lb_timer_control = SYMBOL_TIMER_OFF;  // กดจาก stop -> play แต่รูป จะเผื่อไปที่ pause
            lb_timer_control.color(TFT_RED);
            box_timecounter.color(lb_timer_control.color());
            state_timer = STATE_TIMER_STOP;
            timer_round_action.del();
            lb_timer_count_up_down = SYMBOL_TIMER_ARROW_DOWN;
            box_timecounter.value( format_time(round_timeout_action = (lb_timer_count_up_down == SYMBOL_TIMER_ARROW_UP)? 0 : round_timeout));
          }
        });

        lb_timer_count_up_down = SYMBOL_TIMER_ARROW_DOWN;
        lb_timer_count_up_down.color(TFT_WHITE);
        lb_timer_count_up_down.clickable(true);
        lb_timer_count_up_down.onClicked([](GWIDGET){
          lb_timer_count_up_down = (lb_timer_count_up_down == SYMBOL_TIMER_ARROW_UP)? SYMBOL_TIMER_ARROW_DOWN : SYMBOL_TIMER_ARROW_UP;
          box_timecounter.value( format_time(round_timeout_action =  round_timeout - round_timeout_action));
        });

        lb_reset_scoreboard = SYMBOL_RESTART_ALT;
        lb_reset_scoreboard.clickable(true);
        lb_reset_scoreboard.color(TFT_WHITE);
        lb_reset_scoreboard.color(TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY,3), GSTATE_PRESSED);
        lb_reset_scoreboard.onClicked([](GWIDGET){
          // reset scoreboard เป็นค่าเริ่มต้นใหม่
          for(int i=0; i < 2; i++){
            team_score[i].score(0);
            team_score[i].team_name(StringX::printf("TEAM %d", i+1));
          }
          box_round.value(StringX::printf("%02d",0));

          if(lb_timer_count_up_down == SYMBOL_TIMER_ARROW_DOWN) {
            box_timecounter.value( format_time(round_timeout_action = round_timeout));
          }else{
            box_timecounter.value( format_time(round_timeout_action = 0));
          }
          lb_timer_control.event_send(EVENT_LONG_PRESSED);
        });
      
      lb_timelimit = format_time(round_timeout);
      lb_timelimit.font(prasanmit_25, TFT_SILVER);
      lb_timelimit.clickable(true);
      lb_timelimit.onClicked([](GWIDGET){
        rect_touch_protector.show(true);
        rect_touch_protector.toForeground();    
      });
}

void loop(){
  BlynkGO.update();
}
