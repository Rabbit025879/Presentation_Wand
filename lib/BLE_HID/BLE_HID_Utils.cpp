#include "BLE_HID_Utils.h"

// -- BLEHIDUtils class --
void BLEHIDUtils::delay_ms(uint64_t ms) {
  uint64_t m = esp_timer_get_time();
  if(ms) {
    uint64_t e = (m + (ms * 1000));
	if(m > e) { //overflow
	  while(esp_timer_get_time() > e) {}
	}
    while(esp_timer_get_time() < e) {}
  }
}