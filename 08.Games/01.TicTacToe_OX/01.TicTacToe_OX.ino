#include <BlynkGOv5.h>

#define O_COLOR   TFT_PALETTE(TFT_PALETTE_RED)
#define X_COLOR   TFT_PALETTE(TFT_PALETTE_BLUE)

GLabel image_o[5] =  { "O", "O", "O", "O", "O"};
GLabel image_x[5] =  { "X", "X", "X", "X", "X"};

GContainer cont_tic_tac_toe;
GRect rect[3][3];
GLabel label;
GButton reset_button;

int turn = 0;    // ติดตามตาเล่น
int board[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };  // 0 = ว่าง, 1 = O, 2 = X
bool game_over = false;  // ตรวจสอบว่าเกมจบหรือยัง

struct Move {
  int turn;  // ตาที่เล่น
  char player;  // O หรือ X
  int player_index;  // ลำดับของ O หรือ X
};

Move move_sequence[9];  // เก็บข้อมูลลำดับการเล่น (สูงสุด 9 ครั้ง)

void display_winner(int winner) {
  if (winner == 1) {
    label = "O ชนะ!";
    label.color(O_COLOR); 

  } else if (winner == 2) {
    label = "X ชนะ!";
    label.color(X_COLOR);
  }

  int target = (winner == 1) ? 1 : 2;  // กำหนดว่าเราจะตรวจหาหมายเลขไหน (1 = O, 2 = X)

  // ตรวจหาแถวที่ชนะ
  for (int r = 0; r < 3; r++) {
    if (board[r][0] == target && board[r][1] == target && board[r][2] == target) {
      rect[r][0].font_color((target==1)? O_COLOR : X_COLOR);
      rect[r][1].font_color((target==1)? O_COLOR : X_COLOR);
      rect[r][2].font_color((target==1)? O_COLOR : X_COLOR);
      return;
    }
  }

  // ตรวจหาคอลัมน์ที่ชนะ
  for (int c = 0; c < 3; c++) {
    if (board[0][c] == target && board[1][c] == target && board[2][c] == target) {
      rect[0][c].font_color((target==1)? O_COLOR : X_COLOR);
      rect[1][c].font_color((target==1)? O_COLOR : X_COLOR);
      rect[2][c].font_color((target==1)? O_COLOR : X_COLOR);
      return;
    }
  }

  // ตรวจหาแนวทแยงหลัก (ซ้ายบนไปขวาล่าง)
  if (board[0][0] == target && board[1][1] == target && board[2][2] == target) {
    rect[0][0].font_color((target==1)? O_COLOR : X_COLOR);
    rect[1][1].font_color((target==1)? O_COLOR : X_COLOR);
    rect[2][2].font_color((target==1)? O_COLOR : X_COLOR);
    return;
  }

  // ตรวจหาแนวทแยงรอง (ขวาบนไปซ้ายล่าง)
  if (board[0][2] == target && board[1][1] == target && board[2][0] == target) {
    rect[0][2].font_color((target==1)? O_COLOR : X_COLOR);
    rect[1][1].font_color((target==1)? O_COLOR : X_COLOR);
    rect[2][0].font_color((target==1)? O_COLOR : X_COLOR);
    return;
  }
}

void check_winner() {
  // ตรวจสอบแถวที่ชนะ
  for (int r = 0; r < 3; r++) {
    if (board[r][0] == board[r][1] && board[r][1] == board[r][2] && board[r][0] != 0) {
      game_over = true;
      display_winner(board[r][0]);
      return;
    }
  }

  // ตรวจสอบคอลัมน์ที่ชนะ
  for (int c = 0; c < 3; c++) {
    if (board[0][c] == board[1][c] && board[1][c] == board[2][c] && board[0][c] != 0) {
      game_over = true;
      display_winner(board[0][c]);
      return;
    }
  }

  // ตรวจสอบแนวทแยงที่ชนะ
  if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
    game_over = true;
    display_winner(board[0][0]);
    return;
  }
  if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
    game_over = true;
    display_winner(board[0][2]);
    return;
  }

  // ตรวจสอบว่าเสมอหรือไม่ (ถ้าเต็มทุกช่องและไม่มีผู้ชนะ)
  bool draw = true;
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      if (board[r][c] == 0) {
        draw = false; // ยังมีช่องว่างอยู่ แสดงว่ายังไม่เสมอ
      }
    }
  }

  if (draw) {
    game_over = true;
    label = "เสมอ!";
    label.color(TFT_PALETTE(TFT_PALETTE_YELLOW)); // ใช้สีเหลืองหรือสีอื่นที่มองเห็นได้
  }
}


void reset_game() {
  // รีเซ็ตกระดานให้ว่าง
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      board[r][c] = 0;
      rect[r][c].font_color(TFT_BLACK); // รีเซ็ตสีข้อความ
      rect[r][c].clean(); // ลบข้อความหรือภาพที่แสดง
    }
  }

  // รีเซ็ตข้อมูลลำดับการเล่น
  for (int i = 0; i < 9; i++) {
    move_sequence[i] = {0, ' ', 0};
  }

  // รีเซ็ตตัวแปรเกม
  turn = 0;
  game_over = false;
  label = "";
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  cont_tic_tac_toe.GRID_CELL( GRID_COL{75,75,75}, GRID_ROW{75,75,75} );
  cont_tic_tac_toe.bg_opa(0);
  
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      rect[r][c].parent(cont_tic_tac_toe);
      rect[r][c].grid_cell(c, r);

      rect[r][c].onClicked(GWIDGET_CB {
        int ii = (GRect*)widget - &rect[0][0];
        int c = ii % 3;
        int r = ii / 3;

        if (game_over || board[r][c] != 0) return; // ห้ามกดซ้ำหรือกดหลังเกมจบ

        // เก็บข้อมูลการเล่นลงใน move_sequence
        Move m;
        m.turn = turn;  // เก็บหมายเลขตา
        m.player = (turn % 2 == 0) ? 'O' : 'X';  // เลือกว่าเป็น O หรือ X
        m.player_index = (turn / 2) % 5;  // ลำดับ O หรือ X ภายในตัว array (สูงสุด 5 ตัว)

        move_sequence[turn] = m;  // เก็บการเล่นที่ตำแหน่งนี้

        // วาง O หรือ X ที่ช่อง rect[r][c]
        if (turn % 2 == 0) {
          image_o[m.player_index].parent(rect[r][c]);
          board[r][c] = 1;  // O ลงช่องนี้
        } else {
          image_x[m.player_index].parent(rect[r][c]);
          board[r][c] = 2;  // X ลงช่องนี้
        }

        // ตรวจสอบว่าผู้เล่นคนไหนชนะหรือไม่
        check_winner();

        turn++;  // เพิ่มตาการเล่น
      });
    }
  }


  label.align(cont_tic_tac_toe, ALIGN_TOP);

  // สร้างปุ่มรีเซ็ต
  reset_button = "Reset Game";
  reset_button.align(ALIGN_TOP_RIGHT,-20, 20);
  reset_button.onClicked(GWIDGET_CB{
    reset_game();
  });
}

void loop(){
  BlynkGO.update();
}
