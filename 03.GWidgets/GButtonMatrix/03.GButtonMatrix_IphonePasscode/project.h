#pragma once

#include <BlynkGOv5.h>

namespace PASSCODE {
  void    passcode(uint32_t passcode);
  String  passcode();
  void    onReady( uint32_t passcode, void (*fn)(void));
  void    onCancel(void (*fn)(void));
}
