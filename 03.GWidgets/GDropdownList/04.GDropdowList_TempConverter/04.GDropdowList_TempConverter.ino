#include <BlynkGOv5.h>

GContainer cont_textarea;
  GTextArea ta(cont_textarea);
  GDropdownList dd(cont_textarea);
    GList dd_list;
    String dd_units[] = { SYMBOL_DEGREE "C", SYMBOL_DEGREE "F", "K"};
    enum { TEMP_TYPE_CELSIUS, TEMP_TYPE_FAHRENHEIT, TEMP_TYPE_KELVIN };
    uint8_t temp_type = TEMP_TYPE_CELSIUS;

// Celsius ↔ Fahrenheit
double celsiusToFahrenheit(double c) {
  return (c * 9.0 / 5.0) + 32.0;
}

double fahrenheitToCelsius(double f) {
  return (f - 32.0) * 5.0 / 9.0;
}

// Celsius ↔ Kelvin
double celsiusToKelvin(double c) {
  return c + 273.15;
}

double kelvinToCelsius(double k) {
  return k - 273.15;
}

// Fahrenheit ↔ Kelvin
double fahrenheitToKelvin(double f) {
  return (f - 32.0) * 5.0 / 9.0 + 273.15;
}

double kelvinToFahrenheit(double k) {
  return (k - 273.15) * 9.0 / 5.0 + 32.0;
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  //--------------------------------------------
  cont_textarea.layout(LAYOUT_ROW_M,0,0,0,0,0);
  cont_textarea.border(1, TFT_SILVER);
  cont_textarea.radius(5);
    ta.width(150);
    ta.bg_opa(0);
    ta.border(1, TFT_BLACK);
    ta.border_side(BORDER_SIDE_RIGHT);
    ta.radius(10);
    ta.default_keyboard(true, KEYBOARD_MODE_NUMPAD);

    dd.options(dd_units, 3);
    dd.border(0);
    dd.bg_opa(0);
    dd.size(50,ta.height());
    dd.radius(0);
    dd.symbol(NULL);  // ไม่ต้องแสดงลูกศร v ลง

    dd.selected_color(TFT_PALETTE(TFT_PALETTE_AMBER));
    dd.font(prasanmit_20, TFT_BLACK);
    dd.text_align(TEXT_ALIGN_CENTER);
    dd.list_padding(3,3,10,10);
    dd.list_radius(5);

    dd.onValueChanged(GWIDGET_CB{
      Serial.println(dd.selected());
      uint8_t new_temp_type = dd.selected_id();
      if( new_temp_type != temp_type) {
        switch(temp_type){
          case TEMP_TYPE_CELSIUS: 
            switch(new_temp_type) {
              case TEMP_TYPE_FAHRENHEIT:  ta = String(celsiusToFahrenheit(ta.toDouble()),2); break;
              case TEMP_TYPE_KELVIN:      ta = String(celsiusToKelvin(ta.toDouble()),2);     break;
            }
            break;
          case TEMP_TYPE_FAHRENHEIT:
            switch(new_temp_type) {
              case TEMP_TYPE_CELSIUS:     ta = String(fahrenheitToCelsius(ta.toDouble()),2); break;
              case TEMP_TYPE_KELVIN:      ta = String(fahrenheitToKelvin(ta.toDouble()),2);  break;
            }
            break;
          case TEMP_TYPE_KELVIN:
            switch(new_temp_type) {
              case TEMP_TYPE_CELSIUS:     ta = String(kelvinToCelsius(ta.toDouble()),2);     break;
              case TEMP_TYPE_FAHRENHEIT:  ta = String(kelvinToFahrenheit(ta.toDouble()),2);  break;
            }
            break;
        }
        temp_type = new_temp_type;
      }
    });
}

void loop(){
  BlynkGO.update();
}
