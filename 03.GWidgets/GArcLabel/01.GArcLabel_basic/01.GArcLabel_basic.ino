#include <BlynkGOv5.h>

GArcLabel label_outer;
GArcLabel label_inner;

GSlider slider_outer; GLabel  lb_angle_outer;
GSlider slider_inner; GLabel  lb_angle_inner;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  label_outer.size(300,300);
  label_outer = " Hello #00ffb3 BlynkGO# #fffc44 on# #5956ff Curve# !";
  label_outer.color("#fa1b1b");
  label_outer.recolor(true);
  label_outer.align(ALIGN_LEFT,50);
  // label_outer.text_outer();

  // text_outer_top_left()    , text_outer_top()    , text_outer_top_right(), 
  // text_outer_left()        ,                       text_outer_right(), 
  // text_outer_bottom_left() , text_outer_bottom() , text_outer_bottom_right()
  label_outer.text_outer_top_right();  

  label_inner.size(250,250);
  label_inner = " Hello #00ffb3 BlynkGO# #fffc44 on# #5956ff Curve# !";
  label_inner.color("#fa1b1b");
  label_inner.recolor(true);
  label_inner.align(label_outer, ALIGN_CENTER);
  // label_outer.text_inner();

  // text_inner_top_left()    , text_inner_top()    , text_inner_top_right(), 
  // text_inner_left()        ,                       text_inner_right(), 
  // text_inner_bottom_left() , text_inner_bottom() , text_inner_bottom_right()
  label_inner.text_inner_bottom_left();

  //------------------------------------------
  slider_outer.range(0,360);
  slider_outer = label_outer.angle_offset();
  slider_outer.onValueChanged(GWIDGET_CB{
    label_outer.angle_offset(slider_outer.toInt());
    lb_angle_outer = slider_outer.toInt();
  });
  slider_outer.align(ALIGN_RIGHT, -80, -20);
  lb_angle_outer.align(slider_outer, ALIGN_RIGHT, 20);
  lb_angle_outer = slider_outer.toInt();

  //------------------------------------------
  slider_inner.range(0,360);
  slider_inner = label_inner.angle_offset();
  slider_inner.onValueChanged(GWIDGET_CB{
    label_inner.angle_offset(slider_inner.toInt());
    lb_angle_inner = slider_inner.toInt();
  });
  slider_inner.align(ALIGN_RIGHT, -80, 20);
  lb_angle_inner.align(slider_inner, ALIGN_RIGHT, 20);
  lb_angle_inner = slider_inner.toInt();
  //------------------------------------------

}

void loop(){
  BlynkGO.update();
}
