#include <BlynkGOv5.h>

void draw_s_curve(GLine&ln, Point p0, Point p3) {
    // 1. คำนวณระยะห่าง
    float distance = p0.distance(p3);

    // 2. ปรับ resolution ตามระยะ
    int resolution = (int)(distance / 10);
    resolution = constrain(resolution, 10, 50);
    
    // 3. ปรับ offset ให้แปรผันตามระยะห่าง (เพื่อให้โค้งสวยเสมอไม่ว่าจะใกล้หรือไกล)
    // ถ้าใกล้กันมาก offset ก็จะน้อยลง เส้นจะดูไม่ปูด
    int offset_x = (int)(abs(p3.x-p0.x) * 0.4); 
    offset_x = constrain(offset_x,10,100);

    // 4. คำนวณจุดควบคุม
    Point p1, p2;
    if (p3.x >= p0.x) { 
        // node1 อยู่ซ้าย: พุ่งไปทางขวา (บวก), พุ่งมาทางซ้าย (ลบ)
        p1 = p0 + Point(offset_x, 0);
        p2 = p3 - Point(offset_x, 0);
    } else { 
        // node1 อยู่ขวา: พุ่งไปทางซ้าย (ลบ), พุ่งไปทางขวา (บวก)
        p1 = p0 - Point(offset_x, 0);
        p2 = p3 + Point(offset_x, 0);
    }

    // 5. เตรียม Vector ของ Point
    Points pts; 
    
    // 6. วนลูปคำนวณ Bézier
    for (int i = 0; i < resolution; i++) {
        float t = (float)i / (resolution - 1);
        float u = 1 - t;
        Point p = { u*u*u*p0.x + 3*u*u*t*p1.x + 3*u*t*t*p2.x + t*t*t*p3.x, 
                    u*u*u*p0.y + 3*u*u*t*p1.y + 3*u*t*t*p2.y + t*t*t*p3.y };
        pts.push_back(p);
    }

    // 7. อัปเดต GLine
    ln.setPoints(pts);
}

GLine line;
GRect node1, node2;

void setup(){
  BlynkGO.begin();

  node1.size(160,100);
  node1.position(70,80);
  node1.color(TFT_VIOLET);
  node1.drag_n_drop(true);
  node1.onPressing(GWIDGET_CB{
    draw_s_curve( line, node1.top_right() +Point(-10,10), node2.top_left() + Point(10,10) );
  });

  node2.size(160,100);
  node2.align(ALIGN_BOTTOM_RIGHT,-20,-40);
  node2.color(TFT_RED);
  node2.drag_n_drop(true);
  node2.onPressing(GWIDGET_CB{
    draw_s_curve( line, node1.top_right() +Point(-10,10), node2.top_left() + Point(10,10) );
  });

  line.color(TFT_CYAN);
  line.thickness(3);
    draw_s_curve( line, node1.top_right() +Point(-10,10), node2.top_left() + Point(10,10) );
}

void loop(){
  BlynkGO.update();
}
