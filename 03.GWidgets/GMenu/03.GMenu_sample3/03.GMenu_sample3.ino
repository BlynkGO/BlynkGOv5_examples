#include <BlynkGOv5.h>

GMenu menu;
  GButton back_btn(menu);
  GRect sub_1_page(menu);
    GContainer cont1(sub_1_page);
      GLabel label1(cont1);
  GRect sub_2_page(menu);
    GContainer cont2(sub_2_page);
      GLabel label2(cont2);
  GRect sub_3_page(menu);
    GContainer cont3(sub_3_page);
      GLabel label3(cont3);

  GRect main_page(menu);
    GContainer cont_mp_1(main_page);
      GLabel label_mp_1(cont_mp_1);
    GContainer cont_mp_2(main_page);
      GLabel label_mp_2(cont_mp_2);
    GContainer cont_mp_3(main_page);
      GLabel label_mp_3(cont_mp_3);

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  back_btn = menu.main_header_back_button();
  back_btn = SYMBOL_ARROW_LEFT;

  sub_1_page = menu.page_create(menu, "Page 1");
    cont1 = menu.cont_create(sub_1_page);
      label1 = "Hello, I am hiding here";

  sub_2_page = menu.page_create(menu, "Page 2");
    cont2 = menu.cont_create(sub_2_page);
      label2 = "Hello, I am hiding here";
  
  sub_3_page = menu.page_create(menu, "Page 3");
    cont3 = menu.cont_create(sub_3_page);
      label3 = "Hello, I am hiding here";

  main_page = menu.page_create(menu);
    cont_mp_1 = menu.cont_create(main_page);
      label_mp_1 = "Item 1 (Click me!)";
      menu.load_page_event(cont_mp_1 , sub_1_page);

    cont_mp_2 = menu.cont_create(main_page);
      label_mp_2 = "Item 2 (Click me!)";
      menu.load_page_event(cont_mp_2 , sub_2_page);

    cont_mp_3 = menu.cont_create(main_page);
      label_mp_3 = "Item 3 (Click me!)";
      menu.load_page_event(cont_mp_3 , sub_3_page);

  menu.page(main_page);
}

void loop()
{
  BlynkGO.update();
}