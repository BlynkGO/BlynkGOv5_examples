#include <BlynkGOv5.h>
#define MY_RATIO      (80/120.0)

class GNeoButton : public GRect
{ public:
    GNeoButton(GWidget&parent=GScreen) : GRect(parent) {}
    GRect rect[2];
    GCircle cir[2];
    GRect stroke_rect0;
    GCircle stroke_cir1;
    void create(){
      if(GRect::isCreated()) return;
      GRect::create();

      /* step 1 กำหนดขนาด สี่เหลี่ยมนอก กลาง ใน */
      GRect::size(120*MY_RATIO,120*MY_RATIO);
      rect[0].size(88*MY_RATIO,88*MY_RATIO);
      rect[1].size(80*MY_RATIO,80*MY_RATIO);

      /* step 2 กำหนดขนาด วงกลม นอก ใน */
      cir[0].size(68*MY_RATIO,68*MY_RATIO);
      cir[1].size(56*MY_RATIO,56*MY_RATIO);

      /* step 3 กำหนดรัศมี สี่เหลี่ยมนอก กลาง ใน */
      GRect::radius(20);
      rect[0].radius(16);
      rect[1].radius(12);

      /* step 4 กำหนด stroke 1px (ใช้ GRect แทน) ให้ สี่เหลี่ยมกลาง และ stroke 1px ให้ วงกลมใน */
      stroke_rect0.size(rect[0].width()+1, rect[0].height()+1); stroke_rect0.radius(rect[0].radius()+1);
      stroke_cir1.size(cir[1].width()+1, cir[1].height()+1);    stroke_cir1.radius(cir[1].radius()+1);

      /* step 5 กรอบนอก silver gradient */
      GRect::grad_dir(GRAD_DIR_VER);
      GRect::color(TFT_COLOR_HEX(0xE0E0E0), TFT_COLOR_HEX(0xB0B0B0));

      cir[0].grad_dir(GRAD_DIR_VER);
      cir[0].color(TFT_COLOR_HEX(0xE0E0E0), TFT_COLOR_HEX(0xB0B0B0));

      /* step 6 สี่เหลี่ยมกลางและวงกลมใน (เนื้อปุ่ม) */
      rect[1].grad_dir(GRAD_DIR_VER);
      rect[1].color(TFT_COLOR_HEX(0xFFFFFF), TFT_COLOR_HEX(0xC0C0C0));  // ไฮไลท์ → เงินอ่อน

      cir[1].grad_dir(GRAD_DIR_VER);
      cir[1].color(TFT_COLOR_HEX(0xFFFFFF), TFT_COLOR_HEX(0xC0C0C0));

      /* step 7 stroke สี่เหลี่ยมกลาง */
      stroke_rect0.grad_dir(GRAD_DIR_VER);
      stroke_rect0.color(TFT_COLOR_HEX(0xC0C0C0), TFT_COLOR_HEX(0xA0A0A0));

      /* step 8 stroke วงกลม */
      stroke_cir1.grad_dir(GRAD_DIR_VER);
      stroke_cir1.color(TFT_COLOR_HEX(0xD0D0D0), TFT_COLOR_HEX(0x909090));

      /* step 9 สีสี่เหลี่ยมกลาง */
      rect[0].color(TFT_COLOR_HEX(0xDCDCDC));   // เงินกลาง

      /* step 10 เงากดลง (inner shadow) */
      cir[1].shadow(12, TFT_COLOR_HEX(0x9A9A9A)); 
      cir[1].shadow_offset(0, 10);

      /* step 11 เงานอกแบบ neo silver */
      GRect::shadow(12, 3, TFT_COLOR_HEX(0x949494));
      GRect::shadow_opa(180);

      /* step 12 จัด parent ลงใน สี่เหลี่ยมนอก โดย วางตามลำดับ z-order 
        เพื่อนำไปจัด position/align เฉพาะ สี่เหลี่ยมนอก ได้ง่าย ในต่อๆไป 
        และ ปิดไม่ให้ สัมผัสได้ */
      stroke_rect0.parent(this);   stroke_rect0  .clickable(false);
      rect[0]     .parent(this);   rect[0]       .clickable(false);
      rect[1]     .parent(this);   rect[1]       .clickable(false);
      cir[0]      .parent(this);   cir[0]        .clickable(false);
      stroke_cir1 .parent(this);   stroke_cir1   .clickable(false);
      cir[1]      .parent(this);   cir[1]        .clickable(false);

      /* step 13 กำหนด event ให้ สี่เหลี่ยมนอก ขณะกำลังกด และ ขณะปล่อย 
        ให้เปลี่ยนเงา วงกลมในเพื่อให้สมจริงเวลามีการกด */
      GRect::onPressing(GWIDGET_CB{
        GNeoButton *_mybtn = (GNeoButton*) widget->user_data();

        // วงกลมใน “ยุบลง”
        _mybtn->cir[1].shadow(4, TFT_COLOR_HEX(0x7A7A7A));   // เงาเข้มขึ้น
        _mybtn->cir[1].shadow_offset(0, 3);                  // ยุบลงน้อยกว่าเดิม

        // ปรับ gradient ให้ดูจมลง (ทำให้เข้มกว่าเดิม)
        _mybtn->cir[1].color(TFT_COLOR_HEX(0xE0E0E0), TFT_COLOR_HEX(0xA5A5A5));
      }, this);
      
      GRect::onReleased(GWIDGET_CB{
        GNeoButton *_mybtn = (GNeoButton*) widget->user_data();

        // วงกลมใน “นูนกลับ”
        _mybtn->cir[1].shadow(12, TFT_COLOR_HEX(0x9A9A9A));
        _mybtn->cir[1].shadow_offset(0, 10);

        // กลับ gradient เงินแบบปกติ
        _mybtn->cir[1].color(TFT_COLOR_HEX(0xFFFFFF), TFT_COLOR_HEX(0xC0C0C0));
      }, this);
    }
};

class GNeoButtonDark : public GNeoButton
{ public:
    GNeoButtonDark(GWidget&parent=GScreen) : GNeoButton(parent) {}
    GRect rect[2];
    GCircle cir[2];
    GRect stroke_rect0;
    GCircle stroke_cir1;
    void create(){
      if(GNeoButton::isCreated()) return;
      GNeoButton::create();
      /* step 1 กำหนดขนาด สี่เหลี่ยมนอก กลาง ใน */
      GNeoButton::size(120*MY_RATIO,120*MY_RATIO);
      rect[0].size(88*MY_RATIO,88*MY_RATIO);
      rect[1].size(80*MY_RATIO,80*MY_RATIO);

      /* step 2 กำหนดขนาด วงกลม นอก ใน */
      cir[0].size(68*MY_RATIO,68*MY_RATIO);
      cir[1].size(56*MY_RATIO,56*MY_RATIO);

      /* step 3 กำหนดรัศมี สี่เหลี่ยมนอก กลาง ใน */
      GNeoButton::radius(20);
      rect[0].radius(16);
      rect[1].radius(12);
      

      /* step 4 กำหนด stroke 1px (ใช้ GRect แทน) ให้ สี่เหลี่ยมกลาง และ stroke 1px ให้ วงกลมใน */
      stroke_rect0.size(rect[0].width()+1, rect[0].height()+1); stroke_rect0.radius(rect[0].radius()+1);
      stroke_cir1.size(cir[1].width()+1, cir[1].height()+1);    stroke_cir1.radius(cir[1].radius()+1);

      /* step 5 กำหนด สี gradian ให้ สี่เหลี่ยมนอก และ วงกลมนอก #2A2A2A --> #171717 */
      GNeoButton::grad_dir(GRAD_DIR_VER);
      GNeoButton::color(TFT_COLOR_HEX(0x2A2A2A), TFT_COLOR_HEX(0x171717));

      cir[0].grad_dir(GRAD_DIR_VER);
      cir[0].color(TFT_COLOR_HEX(0x2A2A2A), TFT_COLOR_HEX(0x171717));

      /* step 6 กำหนด สี gradian ให้ สี่เหลี่ยมกลาง และ วงกลมใน  #171717 -->  #2A2A2A */
      rect[1].grad_dir(GRAD_DIR_VER);
      rect[1].color(TFT_COLOR_HEX(0x171717), TFT_COLOR_HEX(0x2A2A2A));

      cir[1].grad_dir(GRAD_DIR_VER);
      cir[1].color(TFT_COLOR_HEX(0x171717), TFT_COLOR_HEX(0x2A2A2A));

      /* step 7 กำหนดสี gradian ให้ stroke สี่เหลี่ยมกลาง #171717 -->  #2A2A2A */
      stroke_rect0.grad_dir(GRAD_DIR_VER);
      stroke_rect0.color(TFT_COLOR_HEX(0x171717), TFT_COLOR_HEX(0x2A2A2A));

      /* step 8 กำหนดสี gradian ให้ stroke สี่เหลี่ยมกลาง #3F3F3F -->  #171717 */
      stroke_cir1.grad_dir(GRAD_DIR_VER);
      stroke_cir1.color(TFT_COLOR_HEX(0x3F3F3F), TFT_COLOR_HEX(0x171717));

      /* step 9 กำหนดสีให้ สี่เหลี่ยมกลาง */
      rect[0].color(TFT_BLACK);

      /* step 10 กำหนดเงา ให้ วงกลมใน */
      cir[1].shadow(10, TFT_BLACK);
      cir[1].shadow_offset(0, 10);

      /* step 11 กำหนดเงาให้ สี่เหลี่ยมนอก ใช้ แทน blur */
      GNeoButton::shadow_opa(0);
      // GNeoButton::shadow(12, 3, TFT_COLOR_HEX(0x171717));
      // GNeoButton::shadow_opa(180);

      /* step 12 จัด parent ลงใน สี่เหลี่ยมนอก โดย วางตามลำดับ z-order 
        เพื่อนำไปจัด position/align เฉพาะ สี่เหลี่ยมนอก ได้ง่าย ในต่อๆไป 
        และ ปิดไม่ให้ สัมผัสได้ */
      stroke_rect0.parent(this);   stroke_rect0  .clickable(false);
      rect[0]     .parent(this);   rect[0]       .clickable(false);
      rect[1]     .parent(this);   rect[1]       .clickable(false);
      cir[0]      .parent(this);   cir[0]        .clickable(false);
      stroke_cir1 .parent(this);   stroke_cir1   .clickable(false);
      cir[1]      .parent(this);   cir[1]        .clickable(false);

      /* step 13 กำหนด event ให้ สี่เหลี่ยมนอก ขณะกำลังกด และ ขณะปล่อย 
        ให้เปลี่ยนเงา วงกลมในเพื่อให้สมจริงเวลามีการกด */
      GNeoButton::onPressing(GWIDGET_CB{
        GNeoButtonDark *_mybtn = (GNeoButtonDark*) widget->user_data();

        // วงกลมในดู “จม” → ลด shadow + ลด offset
        _mybtn->cir[1].shadow(4, TFT_COLOR_HEX(0x000000));
        _mybtn->cir[1].shadow_offset(0, 3);

        // สลับ gradient ให้กลายเป็นแบบ “กดลง”
        _mybtn->cir[1].color(TFT_COLOR_HEX(0x191919),  // บนเข้ม
                            TFT_COLOR_HEX(0x2A2A2A)); // ล่างอ่อน

        _mybtn->rect[1].color(TFT_COLOR_HEX(0x191919),
                              TFT_COLOR_HEX(0x2A2A2A));

      }, this);

      GNeoButton::onReleased(GWIDGET_CB{
        GNeoButtonDark *_mybtn = (GNeoButtonDark*) widget->user_data();

        // วงกลมในกลับมา “นูน”
        _mybtn->cir[1].shadow(10, TFT_COLOR_HEX(0x000000));
        _mybtn->cir[1].shadow_offset(0, 10);

        // คืน gradient ปกติ
        _mybtn->cir[1].color(TFT_COLOR_HEX(0x171717),
                            TFT_COLOR_HEX(0x2A2A2A));

        _mybtn->rect[1].color(TFT_COLOR_HEX(0x171717),
                              TFT_COLOR_HEX(0x2A2A2A));

      }, this);
    }
};

#define DARK_THEME      0

#if DARK_THEME
  GRect cont_btn_dark_frame;
    GContainer cont_btn_dark(cont_btn_dark_frame);
      GNeoButtonDark btn_dark[4];
#else
  GRect cont_btn_frame;
    GContainer cont_btn(cont_btn_frame);
      GNeoButton btn[4];
#endif

GLabel label;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  label.align(ALIGN_TOP,0, 30);

#if DARK_THEME
  BlynkGO.fillScreen(TFT_COLOR_HEX(0x222222));

  cont_btn_dark_frame.size(SIZE_CONTENT, SIZE_CONTENT);
  cont_btn_dark_frame.padding(20);
  cont_btn_dark_frame.color(TFT_COLOR_HEX(0x282828),TFT_COLOR_HEX(0x1b1b1b));
  cont_btn_dark_frame.radius(50);
    cont_btn_dark.color(TFT_BLACK);
    cont_btn_dark.layout(LAYOUT_ROW_M, 10,10,10,10,8);
    cont_btn_dark.radius(30);
    for(int i=0; i<4; i++){
      btn_dark[i].parent(cont_btn_dark);
      btn_dark[i].onClicked(GWIDGET_CB{
        int ii = (GNeoButtonDark*)widget - &btn_dark[0];
        label = ii;
      });
    }
#else
  BlynkGO.fillScreen(TFT_SILVER);
  label.color(TFT_BLACK);
  cont_btn_frame.size(SIZE_CONTENT, SIZE_CONTENT);
  cont_btn_frame.padding(10);
  cont_btn_frame.radius(40);  
  cont_btn_frame.color( 
    TFT_COLOR_HEX(0xE0E0E0),   // บน: เงินกลางอ่อน
    TFT_COLOR_HEX(0xC8C8C8)    // ล่าง: เงินกลาง
  );
  cont_btn_frame.shadow(12,3, TFT_COLOR_HEX(0x949494));
  cont_btn_frame.shadow_opa(180);
    cont_btn.bg_opa(0);
    cont_btn.layout(LAYOUT_ROW_M, 10,10,10,10,8);
    for(int i=0; i<4; i++){
      btn[i].parent(cont_btn);
      btn[i].onClicked(GWIDGET_CB{
        int ii = (GNeoButton*)widget - &btn[0];
        label = ii;
      });
    }
#endif
}

void loop(){
  BlynkGO.update();
}
