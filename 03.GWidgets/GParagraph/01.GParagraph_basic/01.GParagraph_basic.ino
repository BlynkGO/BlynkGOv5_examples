#include <BlynkGOv5.h>

GParagraph paragraph;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  BlynkGO.begin();

  paragraph.width(310);                                 // พารากราฟ กว้าง 300 px
  paragraph.pad_hor(10);                                // ช่องไฟ ซ้าย-ขวา 10px
  paragraph.border(1, TFT_PALETTE(TFT_PALETTE_ORANGE)); // ขอบ 1 px พร้อมกำหนดสีขอบ
  paragraph.text_align(TEXT_ALIGN_LEFT);                // พารากราฟ แบบชิดซ้าย
  paragraph.indent(PCT(8));                             // ระยะ ย่อหน้า แรก 8 % ของความกว้าง (หรือกำหนดด้วย ระยะ pixel ได้)
  paragraph.font(prasanmit_20, TFT_WHITE);              // ฟอนต์หลักของ พารากราฟ
  paragraph.mode(SPAN_MODE_BREAK);                      // SPAN_MODE_BREAK : ยาวเกินกว้าง ให้ตัดขึ้นบรรทัดใหม่อัตโนมัติ ; SPAN_MODE_FIXED : ให้ fixed ตามความกว้าง ; SPAN_MODE_EXPAND : ให้ขยายความกว้าง
  // paragraph.overflow(SPAN_OVERFLOW_ELLIPSIS);        // SPAN_OVERFLOW_CLIP : เวลาล้นให้ตัด ;  SPAN_OVERFLOW_ELLIPSIS : เวลาล้นให้เป็น ...

    GSpan span;
    span = paragraph.span("นี่คือข้อความในย่อหน้า");          // สร้าง span ย่อย ๆ ใน paragraph

    span = paragraph.span("ที่ถูกไฮไลท์");
    span.font(prasanmit_40, TFT_GREEN);

    span = paragraph.span("เพื่อแสดงให้เห็นการใช้งาน ");

    span = paragraph.span("paragraph กับ span.");
    span.font(prasanmit_25, TFT_CYAN);
    span.underline(true);

    span = paragraph.span(" โดยคุณสามารถรวมหลายๆ span ที่ตก แต่งข้อความด้วยฟอนต์ต่างๆกัน เข้าในวิตเจ็ต paragraph ได้.");

    span = paragraph.span("\n    ");    
    span = paragraph.span("ข้อความนี้ เป็นข้อความที่ถูกขีดคล่อม");
    span.color(TFT_RED);
    span.strikethrough(true);
}

void loop()
{
  BlynkGO.update();
}
