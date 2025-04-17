#include <BlynkGOv5.h>

GTileView tileview;
  GTile tile1(tileview);
    GLabel label(tile1);
  GTile tile2(tileview);
    GButton btn(tile2);
  GTile tile3(tileview);
    GList list(tile3);
  GTile tile4(tileview);
    GLabel label4(tile4);

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // tileview.anim_speed(1000);

  tile1.tileview_pos(0, 0, DIR_BOTTOM);
    label = "Scroll Down";

  tile2.tileview_pos( 0, 1, DIR_TOP | DIR_RIGHT);
    btn = "Scroll up or right";

  tile3.tileview_pos( 1, 1, DIR_LEFT | DIR_RIGHT);
    list.size(PCT(100), PCT(100));
    list.add("One");
    list.add("Two");
    list.add("Three");
    list.add("Four");
    list.add("Five");
    list.add("Six");
    list.add("Seven");
    list.add("Eight");
    list.add("Nine");
    list.add("Ten");

  tile4.tileview_pos( 2, 1, DIR_LEFT );
    label4 = "หน้า 2,1";
}

void loop(){
  BlynkGO.update();
}
