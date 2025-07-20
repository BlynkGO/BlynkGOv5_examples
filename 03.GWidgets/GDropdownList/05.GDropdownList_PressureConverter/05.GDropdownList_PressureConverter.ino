#include <BlynkGOv5.h>

//--- Container & Widgets ---
GContainer cont_pressure;
  GTextArea ta_pressure(cont_pressure);
  GDropdownList dd_pressure(cont_pressure);

String pressure_units[] = {"Pa", "kPa", "MPa", "bar", "atm", "psi"};
enum {
  PRESSURE_PA,
  PRESSURE_KPA,
  PRESSURE_MPA,
  PRESSURE_BAR,
  PRESSURE_ATM,
  PRESSURE_PSI
};

uint8_t pressure_type = PRESSURE_PA;  // หน่วยเริ่มต้น: Pa

double pressure_atm = 0.0;  // เก็บค่ากลางใน atm

//--- แปลงไป atm (หน่วยกลาง) ---
double toAtm(double value, uint8_t unit) {
  switch(unit) {
    case PRESSURE_PA:   return value / 101325.0;
    case PRESSURE_KPA:  return value / 101.325;
    case PRESSURE_MPA:  return value / 0.101325;
    case PRESSURE_BAR:  return value / 1.01325;
    case PRESSURE_ATM:  return value;
    case PRESSURE_PSI:  return value / 14.6959;
    default: return 0;
  }
}

//--- แปลงจาก atm ไปยังหน่วยใหม่ ---
double fromAtm(double atm_value, uint8_t unit) {
  switch(unit) {
    case PRESSURE_PA:   return atm_value * 101325.0;
    case PRESSURE_KPA:  return atm_value * 101.325;
    case PRESSURE_MPA:  return atm_value * 0.101325;
    case PRESSURE_BAR:  return atm_value * 1.01325;
    case PRESSURE_ATM:  return atm_value;
    case PRESSURE_PSI:  return atm_value * 14.6959;
    default: return 0;
  }
}

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  //--- Layout ---
  cont_pressure.layout(LAYOUT_ROW_M, 0, 0, 0, 0, 0);
  cont_pressure.border(1, TFT_SILVER);
  cont_pressure.radius(5);

  ta_pressure.width(150);
  ta_pressure.bg_opa(0);
  ta_pressure.border(1, TFT_BLACK);
  ta_pressure.border_side(BORDER_SIDE_RIGHT);
  ta_pressure.radius(10);
  ta_pressure.default_keyboard(true, KEYBOARD_MODE_NUMPAD);

  dd_pressure.options(pressure_units, 6);
  dd_pressure.border(0);
  dd_pressure.bg_opa(0);
  dd_pressure.size(60, ta_pressure.height());
  dd_pressure.radius(0);
  dd_pressure.symbol(NULL);

  dd_pressure.selected_color(TFT_PALETTE(TFT_PALETTE_CYAN));
  dd_pressure.font(prasanmit_20, TFT_BLACK);
  dd_pressure.text_align(TEXT_ALIGN_CENTER);
  dd_pressure.list_padding(3, 3, 10, 10);
  dd_pressure.list_radius(5);

  // กำหนดค่าเริ่มต้นใน ta_pressure
  ta_pressure = "0";

  // อ่านค่าเริ่มต้นมาเก็บใน pressure_atm
  pressure_atm = toAtm(ta_pressure.toDouble(), pressure_type);

  dd_pressure.onValueChanged(GWIDGET_CB{
    Serial.println(dd_pressure.selected());
    uint8_t new_pressure_type = dd_pressure.selected_id();
    if (new_pressure_type != pressure_type) {
      // **อัปเดตค่ากลาง pressure_atm ก่อนแปลงหน่วย**
      pressure_atm = toAtm(ta_pressure.toDouble(), pressure_type);

      // แปลงค่าจาก atm เป็นหน่วยใหม่
      double new_value = fromAtm(pressure_atm, new_pressure_type);

      // แสดงผลลัพธ์ใน ta_pressure
      ta_pressure = String(new_value, 5);

      // อัพเดต pressure_type
      pressure_type = new_pressure_type;
    }
  });

  // กำหนด callback เวลาข้อมูลใน ta_pressure เปลี่ยน (เช่นผู้ใช้พิมพ์)
  ta_pressure.onValueChanged(GWIDGET_CB{
    double input_value = ta_pressure.toDouble();
    // แปลงค่าที่ป้อนเข้ามาเป็น atm และเก็บไว้
    pressure_atm = toAtm(input_value, pressure_type);
  });
}

void loop() {
  BlynkGO.update();
}
