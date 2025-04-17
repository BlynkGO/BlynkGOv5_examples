#include <BlynkGOv5.h>

#define QUESTION_NUM  9
#define CHOICE_NUM    3

struct Question {
  String prompt;
  String choice[CHOICE_NUM];
};

Question question[QUESTION_NUM] = {
  { "1. คุณกำลังเดินไปตามทางเดิน\nแล้วเห็นอะไรอยู่รอบตัว",  
    { 
      "ป่าทึบ มองขึ้นข้างบน\nแทบไม่เห็นท้องฟ้า", 
      "ทุ่งข้าวโพดเหลือง\nอร่ามตัดกับสีขอบฟ้า", 
      "เนินเขาสีเขียว เห็น\nภูเขาอยู่ลืบๆ", 
    }
  },
  { "2. คุณเห็นอะไรตกอยู่ข้างๆ เท้า",  
    { 
      "กระจก", 
      "แหวน", 
      "ขวด", 
    }
  },
  { "3. เก็บมันขึ้นมาไหม",  
    { 
      "เก็บ", 
      "ไม่เก็บ", 
      "-",
    }
  },
  { "4. เดินต่อไปเจอแหล่งน้ำ\nแหล่งน้ำที่ว่าคือ...",  
    { 
      "ทะเลสาบใส", 
      "น้ำตก", 
      "ลำธาร",
    }
  },
  { "5. กุญแจที่จมอยู่ในน้ำซึ่งกำลังจะ\nเก็บขึ้นมานั้นมีลักษณะอย่างไร",  
    { 
      "กุญแจบ้าน", 
      "กุญแจโบราณ", 
      "กุญแจล็อคเกอร์เล็กๆ",
    }
  },
  { "6. ต่อมาเจอะบ้านหลังหนึ่ง\nบ้านหลังนั้นเป็นบ้านแบบไหน",  
    { 
      "แมนชั่นหรูแบบ\nละแวกฮอลลีวู้ด", 
      "กระท่อมพร้อม\nสนามหญ้า", 
      "ปราสาทสวยโทรมๆ",
    }
  },
  { "7. แล้วทำยังไงต่อ",  
    { 
      "มองเข้าไปทางหน้าต่าง", 
      "เข้าไปสำรวจ", 
      "ไม่สน แล้วเดินต่อไป",
    }
  },
  { "8. มีบางอย่างกระโจนใส่ทำให้ตกใจ!\nคิดว่าสิ่งนั้นคือ?",  
    { 
      "หมี", 
      "พ่อมด", 
      "เหยื่อที่ใช้ตกปลา",
    }
  },
  { "9. ด้วยความตกใจ จึงวิ่งไปจนถึง\nกำแพงมีประตู คุณจึงมองลอด\nรูกุญแจ ก็เลยเห็น…",  
    { 
      "สวนเขียวขจีในบริเวณ\nบ้านหลังหนึ่ง", 
      "บ่อน้ำกลางทะเลทราย", 
      "ชายหาดและเกลียวคลื่น",
    }
  },
};

int8_t answer[QUESTION_NUM];

namespace Question_MultiChoice {
  void start();
} 

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  Question_MultiChoice::start();

}

void loop(){
  BlynkGO.update();
}

//---------------------------------------------------------------------------
namespace Question_MultiChoice {
  GPage  app(LAYOUT_COL_L,5,0,0,0,0);
    GContainer cont_prompt(LAYOUT_CENTER, 5,5,0,0, app);
      GLabel lb_prompt(cont_prompt);
    GContainer  cont_choices(LAYOUT_COL_L,10,0,55,0,3, app);
      GCheckbox cb_choice[CHOICE_NUM];
        const char* symbol[] = { "ก.", "ข.", "ค.", "ง.", "จ."};
      int8_t _answer = -1;
      int8_t* _p_ans = NULL;
    enum {BTN_PREV, BTN_NEXT};
    GRect rect_bottom_space(app);
    GButton btn[2];
    GButton btn_ok(app);
  int8_t Q_id=0;

  void start();
  void create();  
  void set_Question(uint8_t q_id);
} // namespace Question_MultiChoice

void Question_MultiChoice::create(){
  if(app.isCreated()) return;
  app.color(TFT_WHITE);
  app.size(GScreen);
  app.font(prasanmit_30,TFT_BLACK);
    cont_prompt.size(PCT(100), SIZE_CONTENT);
    cont_prompt.min_height(60);
      lb_prompt = String("[โจทย์] เนื้อหาโจทย์");
      lb_prompt.line_space(0);
      lb_prompt.letter_space(-1);
      lb_prompt.text_align(TEXT_ALIGN_CENTER);
    cont_choices.bg_opa(0);
    cont_choices.size(PCT(100), SIZE_CONTENT);
    cont_choices.align(ALIGN_TOP_LEFT, 0, PCT(30));
    cont_choices.font(prasanmit_25, TFT_BLACK);
    for(int i=0; i < CHOICE_NUM; i++) {
      cb_choice[i].choice_design(symbol[i]);
      cb_choice[i] = String("คำตอบ ตัวเลือก ") +  String(i+1);
      cb_choice[i].parent(cont_choices);
    }
    cont_choices.onClicked(GWIDGET_CB{
      if(_answer!=-1) cont_choices.child(_answer)->checked(false);    // วิตเจ็ตลูก ของเก่า ให้ unchecked
      _answer = cont_choices.event_target_id();                       // id วิตเจ็ตลูก ของใหม่ ที่มีการถูก clicked
      cont_choices.child(_answer)->checked(true);                     // วิตเจ็ตลูก ของใหม่ ให้ checked
      Serial.println(_answer);
      if(_p_ans !=NULL) *_p_ans = _answer;
      btn[BTN_NEXT].inactive( *_p_ans == -1);
    });

    rect_bottom_space.bg_opa(0);
    rect_bottom_space.height(60);

    btn[BTN_PREV] = SYMBOL_ARROW_LEFT; 
    btn[BTN_NEXT] = SYMBOL_ARROW_RIGHT;
    for(int i=0; i<2 ;i++){
      btn[i].padding(0);
      btn[i].size(50,50);
      btn[i].radius(RADIUS_CIRCLE);
      btn[i].font(prasanmit_30);
      btn[i].color(TFT_PALETTE(TFT_PALETTE_GREEN));

      btn[i].ignore_layout(true); 
      btn[i].floating(true);
      btn[i].parent(app);
    }
    btn[BTN_PREV].align(app, ALIGN_BOTTOM_LEFT , 10,-10);
    btn[BTN_NEXT].align(app, ALIGN_BOTTOM_RIGHT,-10,-10);

    btn_ok.hidden(true);
    btn_ok = "OK";
    btn_ok.font(prasanmit_25);
    btn_ok.ignore_layout(true);
    btn_ok.floating(true);
    btn_ok.size(50,50);
    btn_ok.radius(RADIUS_CIRCLE);
    btn_ok.color(TFT_PALETTE(TFT_PALETTE_DEEP_PURPLE));
    btn_ok.align(app, ALIGN_BOTTOM_RIGHT,-10,-10);
    btn_ok.onClicked(GWIDGET_CB{
      //....
    });
}

void Question_MultiChoice::set_Question(uint8_t q_id) {
  create();
  if(q_id>= QUESTION_NUM ) return;

  Q_id = q_id;
  _p_ans = &answer[Q_id];
  _answer = *_p_ans;

  lb_prompt = question[Q_id].prompt;
  for(int i=0; i < CHOICE_NUM; i++) {
    cb_choice[i] = question[Q_id].choice[i];
    cb_choice[i].hidden(question[Q_id].choice[i] == "-");
  }
  for(int i=0;i<CHOICE_NUM; i++) {   cb_choice[i].checked( *_p_ans == i);  }

  app.scroll_to(0,0);

  btn[BTN_PREV].inactive( Q_id == 0);
  btn[BTN_PREV].hidden( Q_id == 0);
  btn[BTN_NEXT].inactive( *_p_ans == -1);
  btn[BTN_NEXT].hidden(Q_id == QUESTION_NUM-1);
  btn_ok.show(Q_id== QUESTION_NUM-1);

  btn[BTN_PREV].onClicked(GWIDGET_CB{
    if( Question_MultiChoice::Q_id == 0 ) return;
    Question_MultiChoice::Q_id--;
    Question_MultiChoice::set_Question(Question_MultiChoice::Q_id);
  });

  btn[BTN_NEXT].onClicked(GWIDGET_CB{
    if( Question_MultiChoice::Q_id < QUESTION_NUM-1)
    Question_MultiChoice::Q_id++;
    Question_MultiChoice::set_Question(Question_MultiChoice::Q_id);
  });
}

void Question_MultiChoice::start(){
  create();
  for(int i=0; i<QUESTION_NUM; i++) answer[i] = -1;  // reset ค่าให้เป็น -1 คือยังไม่มีการเลือกคำตอบใดๆ
  Q_id = 0;
  set_Question(Q_id);
}
