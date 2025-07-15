#include <BlynkGOv5.h>

FONT_DECLARE(prasanmit_18);
FONT_DECLARE(google_icon_picture_35);

#define SYMBOL_SETTINGS           "\ue8b8"
#define SYMBOL_MENU               "\ue5d2"
#define SYMBOL_BLUETOOTH          "\ue1a7"
#define SYMBOL_SETTINGS_PHONE     "\ue8c5"
#define SYMBOL_TRAVEL             "\ue6ca"
#define SYMBOL_PRINT              "\ue8ad"
#define SYMBOL_VPN_LOCK           "\ue62f"
#define SYMBOL_CALL               "\ue0b0"
#define SYMBOL_WIFI               "\ue63e"
#define SYMBOL_SEARCH             "\ue8b6"

GContainer cont_frame;
  GRect rect_body(cont_frame);
    GLabel lb_title_body(rect_body);
    GChart chart_body(rect_body);
      chart_series_t * series_body;
    GScale scale_y_chart_body(rect_body);

class GMiniMonitor : public GRect
{
  public:
    GMiniMonitor(GWidget&parent=GScreen) : GRect(parent)   {}  // บังคับใส่
    
    // GRect rect_mini_monitor(cont_frame);
    GLabel lb_title;
    GLabel lb_value;
    GChart mini_chart;
    chart_series_t * series;
    
    void create(){
      if(this->isCreated()) return;   // บังคับให้มี
      GRect::create();                // บังคับ
      GRect::grid_cell(1,0);
      GRect::radius(10);
      GRect::GRID_CELL(GRID_COL{GRID_FR(1), GRID_FR(2)}, GRID_ROW{GRID_CONTENT, GRID_FR(1)});
      GRect::min_height(100);
      GRect::padding(0,10,10,10,0);
      GRect::color(TFT_COLOR_HSV(0,0,20));
        lb_title.parent(this);        // สำหรับลุกๆ ต้องมี
        lb_title = "อุณหภูมิ";
        lb_title.font(prasanmit_18, TFT_PALETTE_DARKEN(TFT_PALETTE_GRAY, 1));
        lb_title.grid_cell(0,0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

        lb_value.parent(this);        // สำหรับลุกๆ ต้องมี
        lb_value = "95.2";
        lb_value.font(prasanmit_40, TFT_WHITE);
        lb_value.grid_cell(1,0, GRID_ALIGN_END, GRID_ALIGN_START);

        mini_chart.parent(this);      // สำหรับลุกๆ ต้องมี
        mini_chart.grid_cell(0,1,2,1, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
        mini_chart.type(CHART_TYPE_LINE);
        mini_chart.bg_opa(0);
        mini_chart.line_opa(0);
        mini_chart.range(0, 100);
        mini_chart.border(0);
        mini_chart.point_count(30);
        mini_chart.clickable(false);
          series = mini_chart.createSerie(TFT_CYAN);
    }
    
    inline void title(String title)                     { create(); lb_title = title;                   }
    inline String title()                               { create(); return lb_title.toString();         }
    inline void addPoint(float v)                       { create(); mini_chart.addPoint(series, (int32_t) v); lb_value = String(v,1); }
    inline void range(int32_t v_min, int32_t v_max)     { create(); mini_chart.range(v_min,v_max);      }    
    inline void color(color_t c)                        { create(); mini_chart.series_color(series, c); }
    inline color_t color()                              { create(); return mini_chart.series_color(series);  }
};

GPage page_minimon(cont_frame);
  GMiniMonitor mini_mon[20];

GLabel setting(SYMBOL_SETTINGS, cont_frame);

uint8_t cur_mini_mon_id = 0;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  
  cont_frame.size(PCT(100), PCT(100));
  cont_frame.bg_opa(0);
  cont_frame.GRID_CELL(GRID_COL{ GRID_FR(1), 200}, GRID_ROW{GRID_FR(1), GRID_FR(1), GRID_FR(1),GRID_FR(1),GRID_FR(1), GRID_FR(1), GRID_FR(1),GRID_FR(1) });
  cont_frame.padding(10);
  
  rect_body.grid_cell(0,0,1,7);
  rect_body.radius(10);
  rect_body.color(TFT_COLOR_HSV(0,0,20));
    chart_body.align(ALIGN_CENTER, 0, 20);
    chart_body.size(PCT(80), PCT(80));
    chart_body.bg_opa(0);
    chart_body.point_count(30);
    chart_body.line_color(TFT_COLOR_HSV(0,0,40));
    chart_body.border_color(TFT_COLOR_HSV(0,0,40));
    chart_body.div_line_count(5, 0);
    chart_body.update_mode(CHART_UPDATE_MODE_SHIFT);
      series_body= chart_body.createSerie(TFT_BLUE);

    scale_y_chart_body.range(0,100);
    scale_y_chart_body.height(chart_body);
    scale_y_chart_body.tick(5, 1);
    scale_y_chart_body.tick_length(0);
    scale_y_chart_body.thickness(0, 2, 0);
    scale_y_chart_body.mode(SCALE_MODE_VERTICAL_LEFT);
    scale_y_chart_body.font_color(TFT_SILVER);
    scale_y_chart_body.align(chart_body, ALIGN_LEFT);

    lb_title_body.align(chart_body, ALIGN_TOP,0,-5);
    lb_title_body.color(TFT_WHITE);
  
  setting.font(google_icon_picture_35, TFT_WHITE);
  setting.font_color( TFT_COLOR_HSV(0, 0, 40), GSTATE_PRESSED);
  setting.grid_cell(0,7, GRID_ALIGN_START, GRID_ALIGN_CENTER);
  setting.clickable(true);
  setting.onClicked(GWIDGET_CB{
    Serial.println("Clicked");
  });
  

  page_minimon.grid_cell(1,0,1,8, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
  page_minimon.radius(0);
  page_minimon.border(0);
  page_minimon.bg_opa(0);
  page_minimon.GRID_CELL( 
    GRID_COL{ GRID_FR(1)}, 
    GRID_ROW{ GRID_CONTENT, GRID_CONTENT, GRID_CONTENT, GRID_CONTENT,
              GRID_CONTENT, GRID_CONTENT, GRID_CONTENT, GRID_CONTENT,
              GRID_CONTENT, GRID_CONTENT, GRID_CONTENT, GRID_CONTENT,
              GRID_CONTENT, GRID_CONTENT, GRID_CONTENT, GRID_CONTENT,
              GRID_CONTENT, GRID_CONTENT, GRID_CONTENT, GRID_CONTENT });
  page_minimon.padding(0,0,0,5,10);
  page_minimon.scroll_snap_y(SCROLL_SNAP_START);

  for(int i=0; i<sizeof(mini_mon)/sizeof(mini_mon[0]); i++) {
    mini_mon[i].parent(page_minimon);
    mini_mon[i].grid_cell(0,i, GRID_ALIGN_STRETCH, GRID_ALIGN_STRETCH);
    mini_mon[i].title(StringX::printf("อุณหภูมิ #%d",i+1));
    mini_mon[i].height((BlynkGO.height()-5*10)/4);
    mini_mon[i].range(20,80);
    mini_mon[i].color(TFT_COLOR_HSV(random(360), 100, 100));
    mini_mon[i].onClicked(GWIDGET_CB{
      int ii = (GMiniMonitor*) widget -&mini_mon[0];
      cur_mini_mon_id = ii;
      chart_body.series_color(series_body, mini_mon[ii].color());
      chart_body.ext_y_array(series_body, mini_mon[ii].series->y_points );
      series_body->start_point = mini_mon[ii].series->start_point;
      lb_title_body = mini_mon[ii].title();
    });
  }

  mini_mon[0].event_send(EVENT_CLICKED);  // จำลองเหมือนว่า mini_mon[0] มีการถูก clicked

  static SoftTimer timer_mini_mon;
  timer_mini_mon.setInterval(200,[](){
    for(int i=0; i<20; i++) {
      mini_mon[i].addPoint(random(20,80)); 
    }

    // ให้ series_body ทำการ link ไปใช้ข้อมูลจาก series ของ mini_mon ที่ถูกเลือกอยู่
    chart_body.ext_y_array(series_body, mini_mon[cur_mini_mon_id].series->y_points );
    series_body->start_point = mini_mon[cur_mini_mon_id].series->start_point;
  });  
}

void loop(){
  BlynkGO.update();
}

