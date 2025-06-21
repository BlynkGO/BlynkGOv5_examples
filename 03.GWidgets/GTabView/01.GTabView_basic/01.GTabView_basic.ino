#include <BlynkGOv5.h>

GTabView tabview;
  GContainer tab_bar(tabview);
  GPage tab_home(tabview);
    GLabel label1(tab_home);
    GLabel label2(tab_home);
    GLabel label3(tab_home);
  GPage tab_admin(tabview);
  GPage tab_wifi(tabview);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  tabview.size(PCT(100),PCT(100));
    tab_bar = tabview.tab_bar();
    tab_bar.font(prasanmit_20);
    tab_bar.height(40);
    // สร้าง tab
    tab_home  = tabview.add_tab("Home");                  // เพิ่ม tab ใหม่    
    tab_admin = tabview.add_tab("แอดมิน");
    tab_wifi  = tabview.add_tab("WiFi");

    tab_home.layout(LAYOUT_COL_L,10,10,10,10,0);
      label1 = "Hello World";
      label2 = "บรรทัด 2";
      label3 = "บรรทัด 3";
}

void loop(){
  BlynkGO.update();
}

