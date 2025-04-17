#include <BlynkGOv5.h>

IMAGE_DECLARE(img_deadpool);
IMAGE_DECLARE(img_captain_america);
IMAGE_DECLARE(img_spiderman);
IMAGE_DECLARE(img_hulk);
IMAGE_DECLARE(img_thor);

/* GRID_CELL แบบ 3 x 3 
 *   โดย คอลัมภ์กว้าง  90, 90, 90
 *       แถวสูง      80, 80 
 */
GContainer frame(
  GRID_COL {90, 90, 90}, 
  GRID_ROW {40, 80, 80},
  OPA_0
);

GLabel title(frame);
GIcon icon[]={
  {img_deadpool         , "Deadpool"},
  {img_captain_america  , "Captain A"},
  {img_spiderman        , "SpiderMan"},
  {img_hulk             , "Hulk"},
  {img_thor             , "Thor"},
};

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  frame.border(1, TFT_WHITE);
  frame.padding(10,10,30,30,20);      // ช่องไฟของวิตเจ็ต บน ล่าง ซ้าย ขวา ระหว่าง

  title = "จักรวาลมาเวล";
  title.color(TFT_WHITE);
  title.grid_cell(0, 0, 3, 1);       // วางที่ พิกัด grid_cell 0,0 โดย ขยาย span ด้านกว้างไป 3 ช่อง ด้านสูง 1 ช่อง

  for(int i=0; i<5; i++){
    int col = i%3, row = i/3+1;
    icon[i].parent(frame);
    icon[i].grid_cell(col, row);     // icon[i] วางที่พิกัดใน grid_cell ที่ col, row
    icon[i].onClicked(GWIDGET_CB{
      int i = (GIcon*)widget - &icon[0];
      Serial.println(icon[i].text());
    });
  }
}

void loop()
{
  BlynkGO.update();
}
