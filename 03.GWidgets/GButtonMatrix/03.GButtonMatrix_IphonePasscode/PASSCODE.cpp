#include "project.h"

namespace PASSCODE {
  GRect rect_passcode;
    GLabel lb_enterpasscode(rect_passcode);
    GContainer cont_dot_passcode(rect_passcode);
      GCircle dot_passcode[6];
    GButtonMatrix btnm_passcode(rect_passcode);
    GButton btn_exit(rect_passcode);

  String _key_input="";
  String _passcode="123456";
  SoftTimer _timer;
  void (*_fn_onReady)(void) = NULL;
  void (*_fn_onCancel)(void) = NULL;

  void design();
  void reset();
  void show(bool en);
  void passcode(uint32_t passcode);
  void onReady( uint32_t passcode, void (*fn)(void));
  void onCancel(void (*fn)(void));
}


void PASSCODE::design(){
  if(PASSCODE::rect_passcode.isCreated()) return;

  rect_passcode.color(TFT_BLACK);

    static const char * passcode_map[] = 
    {
      "1", "2", "3", "\n",
      "4", "5", "6", "\n",
      "7", "8", "9", "\n",
      " ", "0", " ", "" 
    };

    static buttonmatrix_ctrl_t passcode_ctrl_map[] = 
    {
      4, 4, 4,
      4, 4, 4,
      4, 4, 4,
      BUTTONMATRIX_CTRL_HIDDEN | 4, 4, BUTTONMATRIX_CTRL_HIDDEN | 4
    };

    btnm_passcode.map(passcode_map);
    btnm_passcode.ctrl_map(passcode_ctrl_map);

    btnm_passcode.height(PCT(80));
    btnm_passcode.padding(10);
    btnm_passcode.button_border(1, TFT_WHITE);

    uint32_t btnm_btn_height = (btnm_passcode.height() - 5*btnm_passcode.padding())/ 4;
    uint32_t btnm_width      = 3*btnm_btn_height       + 4*btnm_passcode.padding() + 6*btnm_passcode.button_border();
    
    btnm_passcode.width(btnm_width);

    btnm_passcode.opa(0);
    btnm_passcode.border(0);
    btnm_passcode.font_color(TFT_WHITE);
    btnm_passcode.button_radius(RADIUS_CIRCLE);
    btnm_passcode.button_opa(0);
    btnm_passcode.button_pressed_opa(120);
    btnm_passcode.align(ALIGN_CENTER,0,PCT(7));

    lb_enterpasscode = "Enter Passcode";
    lb_enterpasscode.color(TFT_WHITE);
    lb_enterpasscode.align(btnm_passcode, ALIGN_TOP,0,-PCT(5));
    cont_dot_passcode.layout(LAYOUT_ROW_M,0,0,0,0,5);
    cont_dot_passcode.opa(0);
    for(int i=0; i<6; i++){
      dot_passcode[i].parent(cont_dot_passcode);
      dot_passcode[i].radius(PCT(2));
      dot_passcode[i].opa(0);
      dot_passcode[i].color(TFT_WHITE);
      dot_passcode[i].border(1,TFT_WHITE);
    }
    cont_dot_passcode.align(lb_enterpasscode, ALIGN_BOTTOM);

    btn_exit.round_design();
    btn_exit = SYMBOL_CLOSE;
    btn_exit.width(PCT(8));
    btn_exit.height(btn_exit.width());
    btn_exit.align(ALIGN_TOP_RIGHT,-10,10);
    btn_exit.onClicked(GWIDGET_CB{
      PASSCODE::show(false);
      if( PASSCODE::_fn_onCancel) PASSCODE::_fn_onCancel();
    });

    btnm_passcode.onValueChanged(GWIDGET_CB{
      if(PASSCODE::_key_input.length()>=6 ) return;

      String key = PASSCODE::btnm_passcode.selected_button();
      PASSCODE::_key_input += key;

      for(int i=0; i< 6; i++){
        PASSCODE::dot_passcode[i].opa( (i < PASSCODE::_key_input.length())? 255: 0);
      }

      if(PASSCODE::_key_input.length()==6){
        if(PASSCODE::_key_input == PASSCODE::_passcode) {
          PASSCODE::lb_enterpasscode = "Passcode : OK!!";
          PASSCODE::lb_enterpasscode.color(TFT_PALETTE(TFT_PALETTE_GREEN));
          PASSCODE::_timer.delay(2000,[](){
            PASSCODE::show(false);
            if( PASSCODE::_fn_onReady ) PASSCODE::_fn_onReady();
          });
        }else{
          PASSCODE::lb_enterpasscode = "Passcode : Wrong!!";
          PASSCODE::lb_enterpasscode.color(TFT_PALETTE(TFT_PALETTE_RED));
          PASSCODE::_timer.delay(2000,[](){
            PASSCODE::show(true);     // ภายในจะ reset อัตโนมัติ
          });
        }
      }
    });
  
  PASSCODE::show(false);
}

void PASSCODE::reset(){
  if(!PASSCODE::rect_passcode.isCreated()) PASSCODE::design();
  for(int i=0; i<6; i++){
    PASSCODE::dot_passcode[i].opa(0);
  }
  PASSCODE::_key_input = "";
  PASSCODE::lb_enterpasscode = "Enter Passcode";
  PASSCODE::lb_enterpasscode.color(TFT_WHITE);
}


void PASSCODE::show(bool en){
  if(!rect_passcode.isCreated()) PASSCODE::design();
  PASSCODE::rect_passcode.show(en);
  PASSCODE::reset();
}

void PASSCODE::passcode(uint32_t passcode){
  if(!PASSCODE::rect_passcode.isCreated()) PASSCODE::design();
  PASSCODE::_passcode = StringX::printf("%06d", passcode % 1000000);
}

String PASSCODE::passcode(){
  return _passcode;
}

void PASSCODE::onReady( uint32_t passcode, void (*fn)(void)){
  if(!PASSCODE::rect_passcode.isCreated()) PASSCODE::design();
  PASSCODE::passcode( passcode);
  PASSCODE::_fn_onReady = fn;
  PASSCODE::show(true);
}

void PASSCODE::onCancel(void (*fn)(void)){
  if(!PASSCODE::rect_passcode.isCreated()) PASSCODE::design();
  PASSCODE::_fn_onCancel = fn;
}

