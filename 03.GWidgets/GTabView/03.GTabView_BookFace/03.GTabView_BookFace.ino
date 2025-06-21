/* ออกแบบสำหรับ 320x480 แนวตั้ง */

#include <BlynkGOv5.h>

FONT_DECLARE(google_icon_basic_20);

#define SYMBOL_HOME               "\ue88a"
#define SYMBOL_SETTINGS           "\ue8b8"
#define SYMBOL_MORE_VERT          "\ue5d4"    // จุด 3 จุด แนวตั้ง
#define SYMBOL_MORE_HORIZ         "\ue5d3"    // จุด 3 จุด แนวนอน
#define SYMBOL_DEHAZE             "\ue3c7"    // ขีด 3 ขีด แนวตั้ง
#define SYMBOL_DASHBOARD          "\ue871"
#define SYMBOL_ADMIN_PANEL_SETTINGS "\uef3d"
#define SYMBOL_MANAGE_ACCOUNTS    "\uf02e"
#define SYMBOL_SEARCH             "\ue8b6"
#define SYMBOL_SIGNAL_CELLULAR_ALT "\ue202"
#define SYMBOL_WIFI               "\ue63e"
#define SYMBOL_WIFI_1_BAR         "\ue4ca"
#define SYMBOL_WIFI_2_BAR         "\ue4d9"
#define SYMBOL_BLUETOOTH          "\ue1a7"
#define SYMBOL_HOME_IOT_DEVICE    "\ue283"
#define SYMBOL_BLOOD_PRESSURE     "\ue097"
#define SYMBOL_CARDIOLOGY         "\ue09c"
#define SYMBOL_DIAGNOSIS          "\ue0a8"

GContainer wire_frame;
  GContainer top_bar(wire_frame);
    GLabel symbol_menu(SYMBOL_DEHAZE, top_bar);
    GLabel lb_title("BookFace", top_bar);
  GPage side_bar(wire_frame);
  GTabView tabview(wire_frame);
    GContainer tab_bar(tabview);
    GPage tab_home(tabview);
      GContainer cont_frame[8];
    GPage tab_admin(tabview);
    GPage tab_bp(tabview);
    GPage tab_wifi(tabview);
    GPage tab_dashboard(tabview);
    GButton tab_btn;
  GRect rect_cover_dark(wire_frame);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.rotation(ROTATION_VERTICAL);

  wire_frame.size(PCT(100), PCT(100));
  wire_frame.bg_opa(0);
  wire_frame.GRID_CELL(GRID_COL{GRID_CONTENT, GRID_FR(1)}, GRID_ROW{GRID_CONTENT, GRID_CONTENT});
  wire_frame.padding(0);
    top_bar.width(PCT(100));
    top_bar.grid_cell(1,0);
    top_bar.color(TFT_WHITE);
    top_bar.radius(0);
    top_bar.layout(LAYOUT_ROW_M,0,0,10,5,10);
      symbol_menu.font(google_icon_basic_20);
      symbol_menu.clickable(true);
      symbol_menu.onClicked(GWIDGET_CB{
        static GAnimation anim;
        if(side_bar.flag_widget1()){
          side_bar.show(true);
          anim.init(side_bar,[](GWIDGET, int32_t v){
            wire_frame.GRID_CELL(GRID_COL{v, GRID_FR(1)}, GRID_ROW{top_bar.flag_widget1()? 0: top_bar.height(), GRID_CONTENT});
            side_bar.translate_x(v - side_bar.width());
          },0, side_bar.width(),200 );
          anim.completed_cb([](GAnimation *a){
            side_bar.flag_widget1(false);
            rect_cover_dark.show(true);
          });
          anim.start();
        }else{
          anim.init(side_bar,[](GWIDGET, int32_t v){
            wire_frame.GRID_CELL(GRID_COL{v, GRID_FR(1)}, GRID_ROW{top_bar.flag_widget1()? 0: top_bar.height(), GRID_CONTENT});
            side_bar.translate_x(v - side_bar.width());
          },side_bar.width(), 0,200 );
          anim.completed_cb([](GAnimation *a){
            side_bar.flag_widget1(true);
            side_bar.show(false);
            rect_cover_dark.show(false);
          });
          anim.start();
        }
      });
      lb_title.font(prasanmit_25, TFT_PALETTE(TFT_PALETTE_INDIGO));
    
    side_bar.size(BlynkGO.width()-40,BlynkGO.height());
    side_bar.radius(0);
    side_bar.grid_cell(0,0,1,2, GRID_ALIGN_START, GRID_ALIGN_START);
    side_bar.show(false);
    side_bar.flag_widget1(true);

    tabview.size(PCT(100),PCT(100));
    tabview.tab_bar_size(40);                 // ความสูง หาก tab_bar_position  เป็น DIR_TOP/BOTTOM; ความกว้าง หาก tab_bar_position เป็น DIR_LEFT/RIGHT
    tabview.grid_cell(1,1);
      tab_bar = tabview.tab_bar();
      tab_bar.font(google_icon_basic_20);     // เปลี่ยน font ที่ tab_bar ให้เป็น ฟอนต์ icon ที่ต้องการ

      // สร้าง tab
      tab_home  = tabview.add_tab(SYMBOL_HOME);                  // เพิ่ม tab ใหม่    
      tab_admin = tabview.add_tab(SYMBOL_ADMIN_PANEL_SETTINGS);
      tab_bp    = tabview.add_tab(SYMBOL_BLOOD_PRESSURE);
      tab_dashboard = tabview.add_tab(SYMBOL_DASHBOARD);
      tab_wifi  = tabview.add_tab(SYMBOL_WIFI);

      // ปรับแต่งดีไซน์ ให้ปุ่มบน tab
      for(int i=0; i< tab_bar.child_count(); i++){
        tab_btn = tab_bar.get_child_by_type(i, BUTTON_CLASS);
        tab_btn.bg_opa(0, GSTATE_ON);                                           // ไม่ต้องมีสีพื้นหลัง
        tab_btn.font_color(TFT_PALETTE(TFT_PALETTE_INDIGO), GSTATE_ON);         // สีของ font บนปุ่ม ที่ tab bar
        tab_btn.border_color(TFT_PALETTE(TFT_PALETTE_INDIGO), GSTATE_ON);       // สีของ border บนปุ่มที่ tab bar
        tab_btn.add_event_cb(EVENT_CB{
          int id =-1;
          for(int i=0; i< tab_bar.child_count(); i++){
            if( ev->original_target == tab_bar.get_child_by_type(i, BUTTON_CLASS)) {
              id = i; break;
            }
          }
          static GAnimation anim;
          if(id==0) {
            if(top_bar.flag_widget1()) {
              anim.init(top_bar, [](GWIDGET, int32_t v){
                wire_frame.GRID_CELL(GRID_COL{GRID_CONTENT, GRID_FR(1)}, GRID_ROW{v, GRID_CONTENT});
              }, 0 , top_bar.height(),200);
              anim.completed_cb([](GAnimation *a){
                top_bar.flag_widget1(false);
              });
              anim.start();
            }
          }else{
            if(!top_bar.flag_widget1()) {
              anim.init(top_bar, [](GWIDGET, int32_t v){
                wire_frame.GRID_CELL(GRID_COL{GRID_CONTENT, GRID_FR(1)}, GRID_ROW{v, GRID_CONTENT});
              }, top_bar.height(), 0 ,200);
              anim.completed_cb([](GAnimation *a){
                top_bar.flag_widget1(true);
              });
              anim.start();
            }
          }
        }, EVENT_CLICKED);
      }

      tab_home.scrollbar(SCROLLBAR_OFF, false, true);
      tab_home.FLEX(FLEX_FLOW_COLUMN, FLEX_ALIGN_START, FLEX_ALIGN_CENTER, FLEX_ALIGN_CENTER);
      tab_home.padding(10);

      for(int i=0; i< 8; i++){
        cont_frame[i].parent(tab_home);
        cont_frame[i].size(PCT(100), SIZE_CONTENT);
        cont_frame[i].min_height(100);
      }
      tab_home.scrollTop();

    rect_cover_dark.color(TFT_BLACK);
    rect_cover_dark.bg_opa(160);
    rect_cover_dark.grid_cell(1,0,1,2);
    rect_cover_dark.show(false);
    rect_cover_dark.onClicked(GWIDGET_CB{
      symbol_menu.event_send(EVENT_CLICKED);
    });
}

void loop(){
  BlynkGO.update();
}

