#include <BlynkGOv5.h>

GContainer cont_frame;
  GRect zone[9];
  GMultiScreen multiscreen(cont_frame);
    GLabel label[5];
  
void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_frame.size(PCT(100), PCT(100));
  cont_frame.bg_opa(0);
  // cont_frame.GRID_CELL(  // 8FR x 6FR
  //   GRID_COL{ GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1)}, 
  //   GRID_ROW{ GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1) }
  // );
  cont_frame.GRID_CELL(  // 16FR x 12FR
    GRID_COL{ GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), 
              GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1) }, 
    GRID_ROW{ GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1),
              GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1), GRID_FR(1) }
  );
  cont_frame.padding(5);

  for(int i=0; i< sizeof(zone)/sizeof(zone[0]); i++) {
    zone[i].parent(cont_frame);
    zone[i].radius(8);
    zone[i].color(TFT_PALETTE(TFT_PALETTE_BLUE));
  }

  zone[0].grid_cell( 0, 0, 5, 4);
  zone[1].grid_cell( 5, 0, 7, 3);
  zone[2].grid_cell(12, 0, 4, 9);
  zone[3].grid_cell( 0, 4, 5, 2);
  zone[4].grid_cell( 0, 6, 5, 2);
  zone[5].grid_cell( 0, 8, 7, 4);
  zone[6].grid_cell( 7, 8, 5, 3);
  zone[7].grid_cell( 7,11, 5, 1);
  zone[8].grid_cell(12, 9, 4, 3);

  multiscreen.grid_cell(5,3,7,5, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  multiscreen.addScreen(5);
  multiscreen.border(2, TFT_RED);
  multiscreen.radius(8);
  for(int i=0; i < multiscreen.num(); i++){
    label[i].parent(multiscreen[i]);
    label[i] = "หน้า " + String(i+1);
    label[i].color(TFT_WHITE);
  }
}

void loop() {
  BlynkGO.update();
}