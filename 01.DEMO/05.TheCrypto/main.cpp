#include <BlynkGOv5.h>

IMAGE_DECLARE(img_bitcoin_btc_s);
IMAGE_DECLARE(img_bnb_bnb_s);
IMAGE_DECLARE(img_cardano_ada_s);
IMAGE_DECLARE(img_dogecoin_doge_s);
IMAGE_DECLARE(img_ethereum_eth_s);
IMAGE_DECLARE(img_solana_sol_s);
IMAGE_DECLARE(img_tether_usdt_s);
IMAGE_DECLARE(img_tron_trx_s);
IMAGE_DECLARE(img_usd_coin_usdc_s);
IMAGE_DECLARE(img_xrp_xrp_s);

#define ZONE_BOX_COLOR      TFT_PALETTE_DARKEN(TFT_PALETTE_BLUE_GRAY,3)

/* ฟังกชั่น แปลงค่าเลขใหญ่ๆ ให้เหลือเพียง 4 หลัก มีพ่วง K, M, B, T, Q แทน */
String formatLargeNumber(uint64_t v);

/* วิตเจ็ตสำหรับใช้เป็น element ของ ลิสต์ Crypto */
class GCryptoBox : public GRect {
  public:
    GCryptoBox(GWidget&parent=GScreen) : GRect(parent)   {}

    GLabel lb_no;                 /* # order */
    GImage image_coin_logo;       /* รูป logo */
    GLabel lb_coin_abbrev;        /* ชื่อ coin แบบย่อ */
    GLabel lb_coin_full;          /* ชื่อ coin แบบเต็ม */
    GLabel lb_price;              /* สำหรับแสดงราคาปัจจุบัน */
    GLabel lb_1hour;              /* การเปลี่ยนแปลง 1 ชม.*/
    GLabel lb_24hour;             /* การเปลี่ยนแปลง 24 ชม.*/
    GLabel lb_7day;               /* การเปลี่ยนแปลง 7 วัน */
    GLabel lb_1hour_trend;        /* ^ หรือ v สำหรับบอก trend 1 ชม*/
    GLabel lb_24hour_trend;       /* ^ หรือ v สำหรับบอก trend 24 ชม*/
    GLabel lb_7day_trend;         /* ^ หรือ v สำหรับบอก trend 7 วัน*/
    GLabel lb_volume_24h;         /* volume 24 h */
    GLabel lb_marketcap;          /* Market Cap */
    GChart chart;                 /* กราฟแบบ mini */
    chart_series_t *series;
    float _vmin = 0;
    float _vmax = 10000;

    void create(){
      if(this->isCreated()) return;
      GRect::create();
      GRect::size(780,60);
      GRect::min_size(500,60);
      GRect::font(prasanmit_25, TFT_WHITE);
      GRect::bg_color(ZONE_BOX_COLOR);
      GRect::bg_color(TFT_COLOR_MIX(ZONE_BOX_COLOR, TFT_BLACK, 180), GSTATE_PRESSED);
      GRect::GRID_CELL( GRID_COL {20,105, 125, 55, 55, 55, 100, 100, 5, GRID_FR(1) }, GRID_ROW {GRID_FR(1)} );
      GRect::padding(0,0,5,15,5);
        lb_no.parent(this);
        lb_no.font(prasanmit_15, TFT_SILVER);
        lb_no = "1";
        lb_no.grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

        image_coin_logo.parent(this);
        image_coin_logo = img_bitcoin_btc_s;
        image_coin_logo.grid_cell(1,0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

        lb_coin_abbrev.parent(this);
        lb_coin_abbrev = "BTC";
        lb_coin_abbrev.font(prasanmit_20);
        lb_coin_abbrev.grid_cell(1,0, GRID_ALIGN_START, GRID_ALIGN_CENTER,45,-5);

        lb_coin_full.parent(this);
        lb_coin_full = "Bitcoin";
        lb_coin_full.font(prasanmit_15, TFT_SILVER);
        lb_coin_full.grid_cell(1,0, GRID_ALIGN_START, GRID_ALIGN_CENTER,45,10);

        lb_price.parent(this);
        lb_price = "$83,518.83";
        lb_price.grid_cell(2,0, GRID_ALIGN_END, GRID_ALIGN_CENTER,-10);
        lb_price.text_align(TEXT_ALIGN_RIGHT);

        lb_1hour_trend.parent(this);
        lb_1hour_trend = SYMBOL_UP;
        lb_1hour_trend.font(prasanmit_15, TFT_PALETTE(TFT_PALETTE_LIGHT_GREEN));
        lb_1hour_trend.grid_cell(3,0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

        lb_1hour.parent(this);
        lb_1hour = "0.7%";
        lb_1hour.font(prasanmit_18,TFT_PALETTE(TFT_PALETTE_LIGHT_GREEN));
        lb_1hour.grid_cell(3,0, GRID_ALIGN_START, GRID_ALIGN_CENTER,15);

        lb_24hour_trend.parent(this);
        lb_24hour_trend = SYMBOL_DOWN;
        lb_24hour_trend.font(prasanmit_15, TFT_PALETTE(TFT_PALETTE_RED));
        lb_24hour_trend.grid_cell(4,0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

        lb_24hour.parent(this);
        lb_24hour = "4.1%";
        lb_24hour.font(prasanmit_18,TFT_PALETTE(TFT_PALETTE_RED));
        lb_24hour.grid_cell(4,0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 15);

        lb_7day_trend.parent(this);
        lb_7day_trend = SYMBOL_DOWN;
        lb_7day_trend.font(prasanmit_15, TFT_PALETTE(TFT_PALETTE_RED));
        lb_7day_trend.grid_cell(5,0, GRID_ALIGN_START, GRID_ALIGN_CENTER);

        lb_7day.parent(this);
        lb_7day = "18.5%";
        lb_7day.font(prasanmit_18,TFT_PALETTE(TFT_PALETTE_RED));
        lb_7day.grid_cell(5,0, GRID_ALIGN_START, GRID_ALIGN_CENTER, 15);

        lb_volume_24h.parent(this);
        lb_volume_24h = "$" + formatLargeNumber(1655037182466);
        lb_volume_24h.grid_cell(6,0, GRID_ALIGN_END, GRID_ALIGN_CENTER,-5);
        lb_volume_24h.font(prasanmit_25);
        lb_volume_24h.text_align(TEXT_ALIGN_RIGHT);

        lb_marketcap.parent(this);
        lb_marketcap = "$" + formatLargeNumber(1642200616682);
        lb_marketcap.grid_cell(7,0, GRID_ALIGN_END, GRID_ALIGN_CENTER,-5);
        lb_marketcap.font(prasanmit_25);
        lb_marketcap.text_align(TEXT_ALIGN_RIGHT);

        chart.parent(this);
        chart.grid_cell(9,0,GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
        chart.size(100, PCT(80));
        chart.type(CHART_TYPE_LINE);
        chart.bg_opa(0);
        chart.line_opa(0);
        chart.range(_vmin,_vmax);
        chart.border(0);
        chart.point_count(30);
        chart.draw_grad(true);
        chart.clickable(false);
        chart.align(ALIGN_BOTTOM_RIGHT);
    
        series = chart.createSerie(TFT_PALETTE(TFT_PALETTE_GREEN));
        chart.line_width(1, GPART_ITEMS);                 // ความหนาของเส้นกราฟ (เส้น series)
        chart.line_opa(180, GPART_ITEMS);                 // ความโปร่งใสของเส้นกราฟ (เส้น series);
      }

      inline void set(int32_t n, const img_t* img, String abbrev, String full)   { create(); lb_no = n; image_coin_logo = img; lb_coin_abbrev = abbrev; lb_coin_full = full; }
      inline void range(float vmin, float vmax)   { create(); chart.range((_vmin = vmin)*100, (_vmax=vmax)*100);                      }
      inline void addValue(float v)               { create(); chart.addPoint(series, map_f(v, _vmin, _vmax, _vmin*100, _vmax*100));   }
      inline void line_color(color_t c)           { create(); chart.series_color(series, c);    }
      inline void line_width(int32_t w)           { create(); chart.line_width(w, GPART_ITEMS); }
      inline void line_opa(opa_t v)               { create(); chart.line_opa( v, GPART_ITEMS);  }
      inline void point_count(int32_t n)          { create(); chart.point_count(n);             }

      inline void price(uint64_t v)               { create(); lb_price      = "$" + String(v);            }
      inline void volume24h(uint64_t v)           { create(); lb_volume_24h = "$" + formatLargeNumber(v); }
      inline void marketcap(uint64_t v)           { create(); lb_marketcap  = "$" + formatLargeNumber(v); }
      inline void trend_1h(float v, bool is_up)   { create(); lb_1hour  = StringX::printf("%.1f%%", v); auto c = TFT_PALETTE((is_up)? TFT_PALETTE_LIGHT_GREEN : TFT_PALETTE_RED); lb_1hour.color(c);  lb_1hour_trend.color(c);  lb_1hour_trend  = (is_up)? SYMBOL_UP : SYMBOL_DOWN; }
      inline void trend_24h(float v, bool is_up)  { create(); lb_24hour = StringX::printf("%.1f%%", v); auto c = TFT_PALETTE((is_up)? TFT_PALETTE_LIGHT_GREEN : TFT_PALETTE_RED); lb_24hour.color(c); lb_24hour_trend.color(c); lb_24hour_trend = (is_up)? SYMBOL_UP : SYMBOL_DOWN; }
      inline void trend_7d(float v, bool is_up)   { create(); lb_7day   = StringX::printf("%.1f%%", v); auto c = TFT_PALETTE((is_up)? TFT_PALETTE_LIGHT_GREEN : TFT_PALETTE_RED); lb_7day.color(c);   lb_7day_trend.color(c);   lb_7day_trend   = (is_up)? SYMBOL_UP : SYMBOL_DOWN; }

};

typedef struct _crypto_setting {
  const img_t * logo;
  String abbrev;
  String full;
} crypto_setting_t;

crypto_setting_t crypto_setting[] = {
  { &img_bitcoin_btc_s, "BTC", "Bitcoin"},
  { &img_ethereum_eth_s, "ETH", "Ethereum" },
  { &img_tether_usdt_s, "USDT", "Tether" },
  { &img_xrp_xrp_s, "XRP", "XRP" },
  { &img_bnb_bnb_s, "BNB", "BNB" },
  { &img_solana_sol_s, "SOL", "Solana" },
  { &img_usd_coin_usdc_s, "USDC", "USDC" },
  { &img_dogecoin_doge_s, "DOGE", "Dogecoin" },
  { &img_cardano_ada_s, "ADA", "Dogecoin" },
  { &img_tron_trx_s, "TRX", "TRON"},
};


GContainer app_crypto;
  GLabel   lb_crypto_title("The คลิปโต", app_crypto);
  GContainer cont_titlebar(app_crypto);
    GLabel lb_no("#", cont_titlebar);
    GLabel lb_coin_header("Coin", cont_titlebar);
    GLabel lb_price_header("ราคา", cont_titlebar);
    GLabel lb_1h_header("1h", cont_titlebar);
    GLabel lb_24h_header("24h", cont_titlebar);
    GLabel lb_7d_header("7d", cont_titlebar);
    GLabel lb_volume_header( "ซื้อ-ขาย 24h"/*"Volume 24h"*/, cont_titlebar);
    GLabel lb_marketcap_header("ขนาดตลาด"/*"Market Cap"*/, cont_titlebar);
    GLabel lb_7days_header("7 วันล่าสุด", cont_titlebar);

  GPage page_coin_list(app_crypto);
    GCryptoBox crypto_box[10];


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  app_crypto.layout(LAYOUT_COL_M_REVERSE);    // เรียงลูกๆแบบ column ตรงกลางตรงกัน และไล่จากล่างขึ้นบน
  app_crypto.padding(10,0,0,0,2);
  app_crypto.bg_opa(0);
  app_crypto.radius(0);

    page_coin_list.size(SIZE_CONTENT, 350); // กว้างเท่า content สูง 350
    page_coin_list.bg_opa(0);
    page_coin_list.layout(LAYOUT_COL_M);
    page_coin_list.padding( 0,0,0,0,2);
    page_coin_list.scrollbar(SCROLLBAR_AUTO, false, true);

    for(int i=0; i< 10; i++){
      crypto_box[i].parent(page_coin_list);
      crypto_box[i].set(i+1, crypto_setting[i].logo, crypto_setting[i].abbrev, crypto_setting[i].full);
      crypto_box[i].line_color( TFT_PALETTE(random(2)?TFT_PALETTE_GREEN : TFT_PALETTE_RED));
      crypto_box[i].trend_1h(random(20)/10.0, random(2)? true : false);
      crypto_box[i].trend_24h(random(40)/10.0, random(2)? true : false);
      crypto_box[i].trend_7d(random(200)/10.0, random(2)? true : false);
    }

    cont_titlebar.size(crypto_box[0].width(), 40);
    cont_titlebar.radius(0);
    cont_titlebar.GRID_CELL( GRID_COL {20, 105, 125, 55,55,55, 100, 100, 5, GRID_FR(1) }, GRID_ROW {GRID_FR(1)} );
    cont_titlebar.padding(0,0,5,15,5);
    cont_titlebar.color( ZONE_BOX_COLOR );
    cont_titlebar.font(prasanmit_20, TFT_WHITE);      // กำหนด font หากลูกๆ ไม่ได้กำหนด จะใช้ font ของ แม่นี้
      lb_no               .grid_cell(0,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_coin_header      .grid_cell(1,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER,-10);
      lb_price_header     .grid_cell(2,0, GRID_ALIGN_END, GRID_ALIGN_CENTER, -30);
      lb_1h_header        .grid_cell(3,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_24h_header       .grid_cell(4,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_7d_header        .grid_cell(5,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);
      lb_volume_header    .grid_cell(6,0, GRID_ALIGN_END, GRID_ALIGN_CENTER);
      lb_marketcap_header .grid_cell(7,0, GRID_ALIGN_END, GRID_ALIGN_CENTER,-5);
      lb_7days_header     .grid_cell(9,0, GRID_ALIGN_CENTER, GRID_ALIGN_CENTER);

    lb_crypto_title.font(prasanmit_40, TFT_WHITE);
    lb_crypto_title.translate_y(-10);


  static SoftTimer timer;
  timer.setInterval(50,[](){
    static int32_t t;
    for(int i=0; i< 10; i++) {
      crypto_box[i].addValue(random(100,900)/10.0);
    }
    if( t<= millis()){
      t = millis()+ 5000;
      for(int i=0; i<10; i++){
        crypto_box[i].line_color( TFT_PALETTE(random(2)?TFT_PALETTE_GREEN : TFT_PALETTE_RED));
        crypto_box[i].trend_1h(random(20)/10.0, random(2)? true : false);
        crypto_box[i].trend_24h(random(40)/10.0, random(2)? true : false);
        crypto_box[i].trend_7d(random(200)/10.0, random(2)? true : false);
      }
    }
  });
}

void loop() {
  BlynkGO.update();
}

/* ฟังกชั่น แปลงค่าเลขใหญ่ๆ ให้เหลือเพียง 4 หลัก มีพ่วง K, M, B, T, Q แทน */
String formatLargeNumber(uint64_t v) {
  const char* units[] = {"", "K", "M", "B", "T", "Q"}; // Q = Quadrillion
  int unitIndex = 0;
  double val = v;
  // แปลงให้เหลือค่าระหว่าง 1.000 - 999.9 และเลือกหน่วย
  while (val >= 1000 && unitIndex < 5) {
    val /= 1000.0;
    unitIndex++;
  }
  // แปลงเป็น string โดยจำกัดให้รวมแล้วไม่เกิน 4 ตัว (ไม่นับตัวอักษรหน่วย)
  String result;
  if (val >= 100) {
    result = String(val, 1); // เช่น 123.4
  } else if (val >= 10) {
    result = String(val, 2); // เช่น 12.34
  } else {
    result = String(val, 3); // เช่น 1.234
  }
  result += " ";
  result += units[unitIndex];
  return result;
}
