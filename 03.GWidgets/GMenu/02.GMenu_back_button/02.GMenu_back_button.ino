#include <BlynkGOv5.h>

GMenu menu;
  GRect sub_page(menu);
    GContainer cont(sub_page);
      GLabel label(cont);
  GRect main_page(menu);
    GContainer cont1(main_page);
      GLabel label1(cont1);
    GContainer cont2(main_page);
      GLabel label2(cont2);
    GContainer cont3(main_page);
      GLabel label3(cont3);
GMessageBox msgbox;

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  menu.mode_root_back_button(MENU_ROOT_BACK_BUTTON_ENABLED);
  menu.onClicked(GWIDGET_CB{
    obj_t * obj = (obj_t*) lv_event_get_target(menu.cur_event());
    GMenu& menu = *(GMenu*)widget;
    if(menu.back_button_is_root(obj)) {
      msgbox.title("Hello");
      msgbox.text("Root back btn click.");
      msgbox.close_button();
    }
  });

  sub_page = menu.page_create();
    cont = menu.cont_create(sub_page);
      label = "Hello, I am hiding here.";
  
  main_page = menu.page_create();
    cont1 = menu.cont_create(main_page);
      label1 = "Item 1";
    cont2 = menu.cont_create(main_page);
      label2 = "Item 2";
    cont3 = menu.cont_create(main_page);
      label3 = "Item 3 (Click me!)";

  menu.load_page_event(cont3, sub_page);    
  menu.page(main_page);

}

void loop()
{
  BlynkGO.update();
}
