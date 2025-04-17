#include <BlynkGOv5.h>

GList list;

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  list.size(180,220);
  list.font(prasanmit_20);
  
  list.add_text( "File");                 // click ไม่ได้
    list.add(SYMBOL_FILE, "New");         // click ได้
    list.add(SYMBOL_DIRECTORY, "Open");
    list.add(SYMBOL_SAVE, "Save");
    list.add( "Save As...");              // click ได้ แบบไม่ใส่ SYMBOL
    list.add(SYMBOL_CLOSE, "Delete");
    list.add(SYMBOL_EDIT, "Edit");
 
  list.add_text( "Connectivity");
    list.add(SYMBOL_BLUETOOTH, "Bluetooth");
    list.add(SYMBOL_GPS, "Navigation");
    list.add(SYMBOL_USB, "USB");
    list.add(SYMBOL_BATTERY_FULL, "Battery");

  list.add_text( "Exit");
    list.add(SYMBOL_OK, "Apply");
    list.add(SYMBOL_CLOSE, "Close");

  list.onValueChanged(GWIDGET_CB{
    if(list.selected() =="") return; 

    Serial.println(list.selected());    
    if(list.selected() == "Save"){
      // ให้ทำอะไร ...
    }
  });

  list.selected("Save");                // เลือกทำเมนูใน list ที่ต้องการ

}

void loop() {
  BlynkGO.update();
}
