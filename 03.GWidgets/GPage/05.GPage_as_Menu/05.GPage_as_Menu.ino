#include <BlynkGOv5.h>

IMAGE_DECLARE(img_compass);
IMAGE_DECLARE(img_deposit);
IMAGE_DECLARE(img_eco_house);
IMAGE_DECLARE(img_island);
IMAGE_DECLARE(img_laundry);
IMAGE_DECLARE(img_ocean);
IMAGE_DECLARE(img_suite);

GPage page(GRID_CELL_COL {80,80,80}, GRID_CELL_ROW {80,80,80});
  GIcon icons[] = {
    { img_compass   , "เข็มทิศ",   page},
    { img_deposit   , "Deposit", page},
    { img_eco_house , "House",   page},
    { img_island    , "เกาะ",     page},
    { img_laundry   , "ซักผ้า",    page},
    { img_ocean     , "ทะเล",    page},
    { img_suite     , "ห้องสูท",  page},
  };

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  page.size(300,220);
  for(int i=0; i< 7; i++){
    int col = i%3, row = i/3;         // 3 คือ จำนวน column ใน แถว
    icons[i].grid_cell(col, row);
    icons[i].width(70);
    icons[i].font(prasanmit_20,TFT_BLACK);
    icons[i].onClicked(GWIDGET_CB{
      int ii= (GIcon*)widget - &icons[0];
      Serial.print(ii); Serial.print(" "); Serial.println( icons[ii].text());
    });
  }
}

void loop(){
  BlynkGO.update();
}
