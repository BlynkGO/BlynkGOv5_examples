#include <BlynkGOv5.h>

FONT_DECLARE(AftikaBold_25);
FONT_DECLARE(google_icon_trend_20);


#define SYMBOL_TRENDING_UP        "\ue8e5"
#define SYMBOL_TRENDING_FLAT      "\ue8e4"
#define SYMBOL_TRENDING_DOWN      "\ue8e3"
#define SYMBOL_TIMELINE           "\ue922"
#define SYMBOL_CHART_DATA         "\ue473"

//======================= FRAME & ZONE =============================
GContainer cont_frame;
  GLine  ln_seperator(cont_frame);
  GLabel lb_overview("OVERVIEW", cont_frame);
  GRect rect_zone[8];
    #define ZONE_A        rect_zone[0]
    #define ZONE_B        rect_zone[1]
    #define ZONE_C        rect_zone[2]
    #define ZONE_D        rect_zone[3]
    #define ZONE_E        rect_zone[4]
    #define ZONE_F        rect_zone[5]
    #define ZONE_G        rect_zone[6]
    #define ZONE_H        rect_zone[7]
//==================================================================

class GTrend : public GContainer {
  public:
    GTrend(GWidget&parent=GScreen) : GContainer(parent)  {}

    GLabel lb_title;
    GLabel lb_value;
    GLabel lb_trend;
    GLabel lb_trend_icon;

    void create(){
      if(this->isCreated()) return;
      GContainer::create();
      GContainer::size(PCT(100), SIZE_CONTENT);
      GRect::max_width(250);
      GRect::min_width(100);
      GContainer::GRID_CELL(GRID_COL{GRID_FR(1)}, GRID_ROW{GRID_CONTENT, GRID_CONTENT,GRID_CONTENT});
      GContainer::padding(0,0,10,10,5);
      GContainer::bg_opa(0);
      GContainer::font_color(TFT_WHITE);
        lb_title.parent(this); lb_title.grid_cell(0,0); lb_title.width(100);
        lb_value.parent(this); lb_value.grid_cell(0,1); lb_value.text_align(TEXT_ALIGN_CENTER); 
        lb_trend_icon.parent(this); lb_trend_icon.grid_cell(0,2);
        lb_trend.parent(this);  lb_trend.grid_cell(0,2, GRID_ALIGN_START, GRID_ALIGN_CENTER, 30);

        lb_title = "title"; lb_title.font(prasanmit_20);
        lb_value = "1,000"; lb_value.font(AftikaBold_25);
        lb_trend_icon = SYMBOL_TRENDING_UP; lb_trend_icon.font(google_icon_trend_20, TFT_WHITE);
        lb_trend = "30% as this quater"; lb_trend.font(prasanmit_15, TFT_SILVER);
    }

    inline void title(String title)           { create(); lb_title = title; }
    inline void value(String value)           { create(); lb_value = value; }
    inline void trend_icon(String trend_icon) {
      create(); 
      lb_trend_icon = trend_icon;
      lb_trend.grid_cell(0,2, GRID_ALIGN_START, GRID_ALIGN_CENTER, (lb_trend_icon=="")? 0: 30);
    }
    inline void trend(String trend) { 
      create(); 
      lb_trend = trend; 
      lb_trend.grid_cell(0,2, GRID_ALIGN_START, GRID_ALIGN_CENTER, (lb_trend_icon=="")? 0: 30);
    }

    private:
};


class GGaugeGRAFANA : GScale {      // สร้าง class แตกมาจากวิตเจ็ต GScale
  public:
    GGaugeGRAFANA(GWidget&parent= GScreen) : GScale(parent), _ranges{0, 40, 60, 80, 100}, _sect_colors{ TFT_COLOR_HSV(220,100,100), TFT_GREEN, TFT_YELLOW, TFT_RED} {}

    void create(){
      if(GScale::isCreated()) return;
      GScale::create();
      GScale::size(200,200); 
      GScale::range(0,100);
      GScale::angle_offset(170);
      GScale::angle_range(200);
      GScale::mode(SCALE_MODE_ROUND);
      GScale::tick_opa(0); 
      GScale::showLabel(false);

      for(int i=0; i<4; i++) {
        GScaleSection* section = GScale::addSection();
        section->range(this->_ranges[i],this->_ranges[i+1]);
        section->thickness(2, this->_sect_colors[i]);
      }

      arc.parent(this);
      arc.size(PCT(90), PCT(90));
      arc.thickness( 0.1* GScale::width(), TFT_GREEN);
      arc.angle_offset(GScale::angle_offset());
      arc.angle_range(GScale::angle_range());
      arc.bg_enable(true);
      arc.bg_color(TFT_COLOR_HEX(0x2d2f34));
      arc.range(0, 100);
      arc.clickable(false);

      lb_value.parent(this);
      lb_value.font(AftikaBold_25, TFT_WHITE);
      lb_value = 0;

      lb_title.parent(this);
      lb_title.font(prasanmit_15, TFT_SILVER);
      lb_title = "Title";

      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,0,5);

      this->onSizeChanged(GWIDGET_CB{
        GGaugeGRAFANA* _gauge = (GGaugeGRAFANA*) widget;
        _gauge->refresh_size();
      });

      this->refresh_size();
    }

    void title(String title){
      create();
      lb_title = title;
      this->refresh_size();
    }
    String title(){ create(); return lb_title.c_str(); }

    void value(float value){  // value 0-100
      create();
      _value = value;
      lb_value = String(_value,(unsigned int)_decimal);
      arc = (int32_t) value;
      this->refresh_size();
    }
    
    float value()             { create(); return _value;  }

    void decimal(uint8_t d)   { create(); _decimal = d; this->value(_value); }
    uint8_t decimal()         { create(); return _decimal; }

    void range(int32_t min_val, int32_t max_val)  {
      create();
      GScale::range(min_val, max_val);
      arc.range(min_val, max_val);
      this->value(this->_value);
    }

    void section(const std::vector<uint32_t>& ranges, const std::vector<color_t>&sect_colors)
    {
      create();
      GScale::clearSection();
      this->_ranges = ranges;
      this->_sect_colors = sect_colors;
      int8_t section_num = min( this->_ranges.size() - 1, this->_sect_colors.size());
      for(int i=0; i< section_num; i++){
        GScaleSection* section = GScale::addSection();
        section->range(ranges[i], ranges[i+1]);
        section->thickness(2, sect_colors[i]);
      }
      this->value(this->_value);
    }

    void operator=(float f) {
      create();
      this->value(f);
    }

    void font(const font_t& font)   { create(); lb_value.font(font); lb_title.align(lb_value, ALIGN_TOP,0,5); }

    void refresh_size(){
      if(!this->isCreated()) return;
      arc.size(PCT(90), PCT(90));
      arc.thickness( 0.1* this->width(), TFT_GREEN);
      lb_value.align_center();
      lb_title.align(lb_value, ALIGN_TOP,0,5);
      int8_t section_num = min( _ranges.size() - 1, _sect_colors.size());
      for(int i=1; i<=section_num; i++){
        if( _value <= _ranges[i]) {
          arc.color(_sect_colors[i-1]) ;
          break;
        }
      }
    }

    GArc arc;
    GLabel lb_value;
    GLabel lb_title;

    using GObject::size;
    using GObject::parent;
    using GObject::align;
    using GObject::width;
    using GObject::height;

  private:
    std::vector<uint32_t> _ranges;
    std::vector<color_t>  _sect_colors;    
    float _value;
    uint8_t _decimal = 0;
};



GTrend trend_NetQR(ZONE_A);
GTrend trend_ARR(ZONE_B);
GTrend trend_QRG(ZONE_C);
GArc   arc_QRG(ZONE_C);
GTrend trend_NewO(ZONE_D);

GLabel lb_sales_overview("Sales Overview", ZONE_E);
GScale scale_sales_overview(ZONE_E);
  GScaleSection scale_section[4];
  uint32_t scale_section_range[] = { 0, 15, 35, 50, 100};
  color_t scale_section_color[] = { 
    TFT_PALETTE_LIGHTEN(TFT_PALETTE_GREEN,2),
    TFT_PALETTE(TFT_PALETTE_GREEN), 
    TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN,2),
    TFT_WHITE
  };
  GLabel lb_scale_overview_val("102k", scale_sales_overview);

GLabel lb_NoS("Number of Sales", ZONE_E);
GLabel lb_NoS_val("$71,020", ZONE_E);
GLine  ln_seperator2(ZONE_E);

GLabel lb_electronics("• Electronics", ZONE_E);
GLabel lb_electronics_val("$55,640", ZONE_E);
GLabel lb_furniture ("• Furniture", ZONE_E);
GLabel lb_furniture_val ("$11,420", ZONE_E);
GLabel lb_cloths("• Cloths", ZONE_E);
GLabel lb_cloths_val("$1,840", ZONE_E);
GLabel lb_shoes("• Shoes", ZONE_E);
GLabel lb_shoes_val("$2,120", ZONE_E);

GGaugeGRAFANA gauge1(ZONE_F);

GGaugeGRAFANA gauge2(ZONE_G);

GLabel lb_total_profit_title("Total Profits", ZONE_H);
GChart chart_total_profit(ZONE_H);
  chart_series_t* series;


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  //======================= FRAME & ZONE =============================
  cont_frame.size(PCT(100), PCT(100));
  cont_frame.bg_opa(0);
  cont_frame.GRID_CELL(
    // แบ่ง GRID เป็น 12 x 5 ช่อง
    GRID_COL{GRID_FR(1),GRID_FR(1),GRID_FR(1), GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1),GRID_FR(1)},
    GRID_ROW{10,GRID_CONTENT, GRID_FR(1),GRID_FR(1),GRID_FR(1)}
  );
  cont_frame.padding(10);
  cont_frame.align(ALIGN_TOP);

  ln_seperator.setPoints( Point(0,0), cont_frame.content_width());
  ln_seperator.grid_cell(0,1,9,1, GRID_ALIGN_START, GRID_ALIGN_START);
  lb_overview.grid_cell(0,1,6,1, GRID_ALIGN_START,GRID_ALIGN_CENTER,0,5);  // วางที่ grid_cell(0,1) ขยายไปแนวนอน 6 ช่อง แนวตั้ง 1 ช่อง แนวตั้งชิดเริ่มต้น และแนวตอนชิดกลาง ปรับลงมา 5px ภายใน grid cell
  lb_overview.font(prasanmit_30, TFT_WHITE);

  for(int i=0; i<8; i++){
    rect_zone[i].parent(cont_frame);
    rect_zone[i].cell_design();
  }

  ZONE_A.grid_cell(0,2,3,1);
  ZONE_B.grid_cell(3,2,3,1);
  ZONE_C.grid_cell(6,2,3,1);
  ZONE_D.grid_cell(9,2,3,1);
  ZONE_E.grid_cell(0,3,8,2);
  ZONE_F.grid_cell(8,3,2,1);
  ZONE_G.grid_cell(10,3,2,1);
  ZONE_H.grid_cell(8,4,4,1);
  //==================================================================

  trend_NetQR.title("Net Quarterly Revenue");
  trend_NetQR.value("$3,131,021");

  trend_ARR.title("ARR");
  trend_ARR.value("$1,511,121");

  trend_QRG.title("Quarterly Revenue Goal");
  trend_QRG.value("71%");
  trend_QRG.lb_value.text_align(TEXT_ALIGN_LEFT); 
  trend_QRG.trend_icon("");
  trend_QRG.trend("Goal ฿1.3M");

  arc_QRG.size(120,120);
  arc_QRG.align(ALIGN_BOTTOM_RIGHT, 25,55);
  arc_QRG.bg_enable(true);           // ให้แสดง arc พื้นหลังด้วย
  arc_QRG.angle_offset(180);         // มุมเริ่มต้น นับจากแกน X-axis ทางคณิตศาสตร์ (หมุนตามเข็ม)
  arc_QRG.bg_angle(0,120);
  arc_QRG.angle(0,120);
  arc_QRG.range(0,100);
  arc_QRG.color(TFT_PALETTE_DARKEN(TFT_PALETTE_GREEN, 1));
  arc_QRG = 71;  

  trend_NewO.title("New Orders");
  trend_NewO.value("18,221");

  ZONE_E.GRID_CELL(
    GRID_COL { GRID_FR(2),GRID_FR(1),GRID_FR(1),20},
    GRID_ROW { GRID_CONTENT,GRID_FR(6),10,GRID_FR(5),GRID_FR(5),GRID_FR(3)}
  );
  ZONE_E.padding(10,10,10,10,0);
  ZONE_E.font(prasanmit_20, TFT_WHITE);

    lb_sales_overview.grid_cell(0,0,4,1);
    lb_sales_overview.font(prasanmit_30, TFT_WHITE);

    scale_sales_overview.size(150,150); 
    scale_sales_overview.range(0,100);
    scale_sales_overview.angle_offset(270);
    scale_sales_overview.angle_range(360);
    scale_sales_overview.mode(SCALE_MODE_ROUND);
    scale_sales_overview.tick_opa(0); 
    scale_sales_overview.showLabel(false);
    scale_sales_overview.grid_cell(0,1,1,5, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
    for(int i=0; i<4; i++) {
      scale_section[i] = scale_sales_overview.add_section();
      scale_section[i].thickness(20, scale_section_color[i]);
      scale_section[i].range(scale_section_range[i],scale_section_range[i+1]);
    }
    lb_scale_overview_val.font(prasanmit_35, TFT_WHITE);

    lb_NoS.grid_cell(1,1,3,1, GRID_ALIGN_START, GRID_ALIGN_START); 
    ln_seperator2.setPoints(Point(0,0), 200);  
    ln_seperator2.grid_cell(1,1,2,1, GRID_ALIGN_START, GRID_ALIGN_END); 
    ln_seperator2.thick(1, TFT_RED);

    lb_electronics.grid_cell(1,3, GRID_ALIGN_START, GRID_ALIGN_START); 
    lb_furniture.grid_cell(2,3, GRID_ALIGN_START, GRID_ALIGN_START);
    lb_cloths.grid_cell(1,4, GRID_ALIGN_START, GRID_ALIGN_START);
    lb_shoes.grid_cell(2,4, GRID_ALIGN_START, GRID_ALIGN_START);

    lb_NoS_val.grid_cell(1,1,3,1, GRID_ALIGN_START, GRID_ALIGN_END);  lb_NoS_val.font(prasanmit_30);
    lb_electronics_val.grid_cell(1,3, GRID_ALIGN_START, GRID_ALIGN_END,10); 
    lb_furniture_val.grid_cell(2,3, GRID_ALIGN_START, GRID_ALIGN_END,10); 
    lb_cloths_val.grid_cell(1,4, GRID_ALIGN_START, GRID_ALIGN_END,10); 
    lb_shoes_val.grid_cell(2,4, GRID_ALIGN_START, GRID_ALIGN_END,10); 

  // ZONE_F
  gauge1.size(PCT(80),PCT(80));
  gauge1.align(ALIGN_CENTER,0,20);
  gauge1.title("Profit");
  gauge1 = 89;

  // ZONE_G
  gauge2.size(PCT(80),PCT(80));
  gauge2.align(ALIGN_CENTER,0,20);
  gauge2.title("Sales");
  gauge2 = 93;

  // ZONE_H
  ZONE_H.font_color(TFT_WHITE);
  lb_total_profit_title.position(10,0); lb_total_profit_title.font(prasanmit_20);
  chart_total_profit.size(PCT(100), PCT(100));
  chart_total_profit.bg_opa(0);
  chart_total_profit.border(0);
  chart_total_profit.line_width(0);
  chart_total_profit.point_count(20);
  chart_total_profit.chart_type(CHART_TYPE_LINE);
  chart_total_profit.draw_grad(true);

  series = chart_total_profit.createSerie(TFT_PALETTE(TFT_PALETTE_GREEN));
  for(int i=0; i<20; i++){
    chart_total_profit.addPoint(series, random(10,80));
  }

}

void loop(){
  BlynkGO.update();
}
