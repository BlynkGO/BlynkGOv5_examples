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


GTabView tabview;
  GContainer tab_bar(tabview);
  GPage tab_home(tabview);
    GLabel label1(tab_home);
    GLabel label2(tab_home);
    GLabel label3(tab_home);
  GPage tab_admin(tabview);
  GPage tab_wifi(tabview);

  GButton tab_btn;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  tabview.size(PCT(100),PCT(100));
  // tabview.tab_bar_position(DIR_BOTTOM);  // วาง tab_bar ไว้ด้านล่าง
  // tabview.tab_bar_position(DIR_LEFT);    // วาง tab_bar ไว้ด้านซ้าย
  // tabview.tab_bar_position(DIR_RIGHT);   // วาง tab_bar ไว้ด้านขวา
  tabview.tab_bar_size(40);                 // ความสูงของ tab_bar หากอยู่ด้านบนหรือล่าง , ความกว้างของ tab_bar หากอยู่ซ้ายหรือขวา
    tab_bar = tabview.tab_bar();
    tab_bar.font(google_icon_basic_20);

    // สร้าง tab
    tab_home  = tabview.add_tab(SYMBOL_HOME);                  // เพิ่ม tab ใหม่ ใช้ icon จาก font แทน ข้อความ 
    tab_admin = tabview.add_tab(SYMBOL_ADMIN_PANEL_SETTINGS);
    tab_wifi  = tabview.add_tab(SYMBOL_WIFI);

    // ปรับแต่งดีไซน์ ให้ปุ่มบน tab ให้ปรับแต่งดีไซน์ที่ GSTATE_ON ของปุ่ม
    for(int i=0; i< tab_bar.child_count(); i++){
      tab_btn = tab_bar.get_child_by_type(i, BUTTON_CLASS);
      tab_btn.bg_opa(0, GSTATE_ON);                                           // ไม่ต้องมีสีพื้นหลัง
      tab_btn.font_color(TFT_PALETTE(TFT_PALETTE_INDIGO), GSTATE_ON);         // สีของ font บนปุ่ม ที่ tab bar
      tab_btn.border_color(TFT_PALETTE(TFT_PALETTE_INDIGO), GSTATE_ON);       // สีของ border บนปุ่มที่ tab bar
      // tab_btn.border_side(BORDER_SIDE_RIGHT, GSTATE_ON);                   // ให้ปุ่มบน tab bar มีขอบเฉพาะด้านขวา
    }

    tab_home.layout(LAYOUT_COL_L,10,10,10,10,0);
      label1 = "Hello World";
      label2 = "บรรทัด 2";
      label3 = "บรรทัด 3";
}

void loop(){
  BlynkGO.update();
}
