#include <BlynkGOv5.h>

IMAGE_DECLARE(img_sony);

GQRCode qrcode;

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED);
  
  // qrcode.size(300,300);  // กำหนดขนาด กว้าง = ยาว ให้เท่ากัน
  // qrcode.version(17);    // กำหนด version ของ QR code  เพื่อรองรับ ความยาวสูงสุดที่จะรับได้ (ดูตารางด้านล่าง)
  qrcode.encode("สวัสดีครับ");

  // หากต้องการใส่ logo ด้วย ควรมีขนาด 60x60pixels
//  qrcode.encode("https://www.sony.com", img_sony); 
}

void loop() {
  BlynkGO.update();
}


// หมายเหตุ อักษรไทย 1 ตัว = ความยาว 3 bytes (UTF8)
// หากเป็นข้อความไทย จะถูกมองว่าเป็น binary
//-----------------------------------------
// qrcode version & maxinum data length (for eec_Q)
//-----------------------------------------
// version  Alphanumeric  Binary       Thai
// 6           108           74         24
// 7           125           86         28
// 8           157          108         36
// 9           189          130         43
// 10          221          151         50
// 11          259          177         59
// 12          296          203         67
// 13          352          241         80
// 14          376          258         86
// 15          426          292         97
// 16          470          322        107
// 17          531          364        121
// 18          574          394        131
// 19          644          442        147
// 20          702          482        160
// 21          742          509        169
// 22          823          565        188
// 23          890          611        203
// 24          963          661        220
// 25         1041          715        238
// 26         1094          751        250
// 27         1172          805        268
// 28         1263          868        289
// 29         1322          908        302
// 30         1429          982        327
// 31         1499         1030        343
// 
