#include "BLE_HID.h"

#if defined(USE_NIMBLE)

#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>
#include <NimBLEHIDDevice.h>


#else
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"

#endif // USE_NIMBLE

#include "HIDTypes.h"
#include <driver/adc.h>
#include "sdkconfig.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)

  #include "esp32-hal-log.h"
  #define LOG_TAG ""
#else

  #include "esp_log.h"
  static const char* LOG_TAG = "BLEDevice";

#endif

// Report IDs:
#define KEYBOARD_ID 0x01
#define MEDIA_KEYS_ID 0x02
#define MOUSE_ID 0x03

static const uint8_t _hidReportDescriptor[] = {
  // ------------------------------------------------- Keyboard
  USAGE_PAGE(1),      0x01,          // USAGE_PAGE (Generic Desktop Ctrls)
  USAGE(1),           0x06,          // USAGE (Keyboard)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       KEYBOARD_ID,   //   REPORT_ID (1)
  // ------------------------------------------------- 
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0xE0,          //   USAGE_MINIMUM (0xE0)
  USAGE_MAXIMUM(1),   0xE7,          //   USAGE_MAXIMUM (0xE7)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   Logical Maximum (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x08,          //   REPORT_COUNT (8)
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 1 byte (Reserved)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE (8)
  HIDINPUT(1),        0x01,          //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  REPORT_COUNT(1),    0x05,          //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  USAGE_PAGE(1),      0x08,          //   USAGE_PAGE (LEDs)
  USAGE_MINIMUM(1),   0x01,          //   USAGE_MINIMUM (0x01) ; Num Lock
  USAGE_MAXIMUM(1),   0x05,          //   USAGE_MAXIMUM (0x05) ; Kana
  HIDOUTPUT(1),       0x02,          //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x01,          //   REPORT_COUNT (1) ; 3 bits (Padding)
  REPORT_SIZE(1),     0x03,          //   REPORT_SIZE (3)
  HIDOUTPUT(1),       0x01,          //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  REPORT_COUNT(1),    0x06,          //   REPORT_COUNT (6) ; 6 bytes (Keys)
  REPORT_SIZE(1),     0x08,          //   REPORT_SIZE(8)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
  LOGICAL_MAXIMUM(1), 0x65,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
  USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
  USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
  USAGE_MAXIMUM(1),   0x65,          //   USAGE_MAXIMUM (0x65)
  HIDINPUT(1),        0x00,          //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
  // ------------------------------------------------- Media Keys
  USAGE_PAGE(1),      0x0C,          // USAGE_PAGE (Consumer)
  USAGE(1),           0x01,          // USAGE (Consumer Control)
  COLLECTION(1),      0x01,          // COLLECTION (Application)
  REPORT_ID(1),       MEDIA_KEYS_ID, //   REPORT_ID (2)
  // -------------------------------------------------
  USAGE_PAGE(1),      0x0C,          //   USAGE_PAGE (Consumer)
  LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1), 0x01,          //   LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),     0x01,          //   REPORT_SIZE (1)
  REPORT_COUNT(1),    0x10,          //   REPORT_COUNT (16)
  USAGE(1),           0xB5,          //   USAGE (Scan Next Track)     ; bit 0: 1
  USAGE(1),           0xB6,          //   USAGE (Scan Previous Track) ; bit 1: 2
  USAGE(1),           0xB7,          //   USAGE (Stop)                ; bit 2: 4
  USAGE(1),           0xCD,          //   USAGE (Play/Pause)          ; bit 3: 8
  USAGE(1),           0xE2,          //   USAGE (Mute)                ; bit 4: 16
  USAGE(1),           0xE9,          //   USAGE (Volume Increment)    ; bit 5: 32
  USAGE(1),           0xEA,          //   USAGE (Volume Decrement)    ; bit 6: 64
  USAGE(2),           0x23, 0x02,    //   Usage (WWW Home)            ; bit 7: 128
  USAGE(2),           0x94, 0x01,    //   Usage (My Computer) ; bit 0: 1
  USAGE(2),           0x92, 0x01,    //   Usage (Calculator)  ; bit 1: 2
  USAGE(2),           0x2A, 0x02,    //   Usage (WWW fav)     ; bit 2: 4
  USAGE(2),           0x21, 0x02,    //   Usage (WWW search)  ; bit 3: 8
  USAGE(2),           0x26, 0x02,    //   Usage (WWW stop)    ; bit 4: 16
  USAGE(2),           0x24, 0x02,    //   Usage (WWW back)    ; bit 5: 32
  USAGE(2),           0x83, 0x01,    //   Usage (Media sel)   ; bit 6: 64
  USAGE(2),           0x8A, 0x01,    //   Usage (Mail)        ; bit 7: 128
  HIDINPUT(1),        0x02,          //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  END_COLLECTION(0),                 // END_COLLECTION
  // ------------------------------------------------- Mouse
  USAGE_PAGE(1),      0x01,			 // USAGE_PAGE (Generic Desktop)			
  USAGE(1),           0x02,			 // USAGE (Mouse)
  COLLECTION(1),      0x01,			 // COLLECTION (Application)
  REPORT_ID(1),       MOUSE_ID,		 //   REPORT_ID (3)
  USAGE(1),            0x01, 		 //   USAGE (Pointer)
  COLLECTION(1),       0x00, 		 //   COLLECTION (Physical)
  // ------------------------------------------------- Buttons (Left, Right, Middle, Back, Forward)
  USAGE_PAGE(1),       0x09,   		 //     USAGE_PAGE (Button)
  USAGE_MINIMUM(1),    0x01,   		 //     USAGE_MINIMUM (Button 1)
  USAGE_MAXIMUM(1),    0x05,   		 //     USAGE_MAXIMUM (Button 5)
  LOGICAL_MINIMUM(1),  0x00,   		 //     LOGICAL_MINIMUM (0)
  LOGICAL_MAXIMUM(1),  0x01,   		 //     LOGICAL_MAXIMUM (1)
  REPORT_SIZE(1),      0x01,   		 //     REPORT_SIZE (1)
  REPORT_COUNT(1),     0x05,   		 //     REPORT_COUNT (5)
  HIDINPUT(1),         0x02,   		 //     INPUT (Data, Variable, Absolute) ;5 button bits
  // ------------------------------------------------- Padding
  REPORT_SIZE(1),      0x03, 		 //     REPORT_SIZE (3)
  REPORT_COUNT(1),     0x01, 		 //     REPORT_COUNT (1)
  HIDINPUT(1),         0x03, 		 //     INPUT (Constant, Variable, Absolute) ;3 bit padding
  // ------------------------------------------------- X/Y position, Wheel
  USAGE_PAGE(1),       0x01,         //     USAGE_PAGE (Generic Desktop)
  USAGE(1),            0x30,         //     USAGE (X)
  USAGE(1),            0x31,         //     USAGE (Y)
  USAGE(1),            0x38,         //     USAGE (Wheel)
  LOGICAL_MINIMUM(1),  0x81,         //     LOGICAL_MINIMUM (-127)
  LOGICAL_MAXIMUM(1),  0x7f,         //     LOGICAL_MAXIMUM (127)
  REPORT_SIZE(1),      0x08,         //     REPORT_SIZE (8)
  REPORT_COUNT(1),     0x03,         //     REPORT_COUNT (3)
  HIDINPUT(1),         0x06,         //     INPUT (Data, Variable, Relative) ;3 bytes (X,Y,Wheel)
  // ------------------------------------------------- Horizontal wheel
  USAGE_PAGE(1),       0x0c,         //     USAGE PAGE (Consumer Devices)
  USAGE(2),      0x38, 0x02,         //     USAGE (AC Pan)
  LOGICAL_MINIMUM(1),  0x81,         //     LOGICAL_MINIMUM (-127)
  LOGICAL_MAXIMUM(1),  0x7f,         //     LOGICAL_MAXIMUM (127)
  REPORT_SIZE(1),      0x08,         //     REPORT_SIZE (8)
  REPORT_COUNT(1),     0x01,         //     REPORT_COUNT (1)
  HIDINPUT(1),         0x06,         //     INPUT (Data, Var, Rel)
  END_COLLECTION(0),                 //   END_COLLECTION
  END_COLLECTION(0)                  // END_COLLECTION
};

// -- BLEHID class --
BLEHID::BLEHID(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) 
    : hid(0)
    , deviceName(std::string(deviceName).substr(0, 15))
    , deviceManufacturer(std::string(deviceManufacturer).substr(0,15))
    , batteryLevel(batteryLevel) {}

void BLEHID::begin(void)
{
  BLEDevice::init(deviceName);
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(this);

  hid = new BLEHIDDevice(pServer);
  keyboard.inputKeyboard = hid->inputReport(KEYBOARD_ID);
  keyboard.outputKeyboard = hid->outputReport(KEYBOARD_ID);
  keyboard.inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID); 

  keyboard.outputKeyboard->setCallbacks(this);

  mouse.inputMouse = hid->inputReport(MOUSE_ID);

  hid->manufacturer()->setValue(deviceManufacturer);

  hid->pnp(0x02, vid, pid, version);
  hid->hidInfo(0x00, 0x01);

  // Bind connection state to share connection flag
  keyboard.bindConnectionState(&this->connected);
  mouse.bindConnectionState(&this->connected);

#if defined(USE_NIMBLE)

  BLEDevice::setSecurityAuth(true, true, true);

#else

  BLESecurity* pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);

#endif // USE_NIMBLE

  hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  hid->startServices();

  onStarted(pServer);

  advertising = pServer->getAdvertising();	
  advertising->setAppearance(HID_KEYBOARD);
  advertising->addServiceUUID(hid->hidService()->getUUID());
  advertising->setScanResponse(false);
  advertising->start();
  hid->setBatteryLevel(batteryLevel);

  ESP_LOGD(LOG_TAG, "Advertising started!");
}

void BLEHID::end(void) {}

bool BLEHID::isConnected(void) {
  return this->connected;
}

void BLEHID::setBatteryLevel(uint8_t level) {
  this->batteryLevel = level;
  if (hid != 0)
    this->hid->setBatteryLevel(this->batteryLevel);
}

//must be called before begin in order to set the name
void BLEHID::setName(std::string deviceName) {
  this->deviceName = deviceName;
}

void BLEHID::set_vendor_id(uint16_t vid) { 
	this->vid = vid; 
}

void BLEHID::set_product_id(uint16_t pid) { 
	this->pid = pid; 
}

void BLEHID::set_version(uint16_t version) { 
	this->version = version; 
}

void BLEHID::onConnect(BLEServer* pServer) {
  this->connected = true;

#if !defined(USE_NIMBLE)

  BLE2902* desc = (BLE2902*)this->inputKeyboard->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(true);
  desc = (BLE2902*)this->inputMediaKeys->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(true);
  desc = (BLE2902*)this->inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(true);

#endif // !USE_NIMBLE

}

void BLEHID::onDisconnect(BLEServer* pServer) {
  this->connected = false;

#if !defined(USE_NIMBLE)

  BLE2902* desc = (BLE2902*)this->inputKeyboard->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(false);
  desc = (BLE2902*)this->inputMediaKeys->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(false);
  desc = (BLE2902*)this->inputMouse->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  desc->setNotifications(false);

  advertising->start();

#endif // !USE_NIMBLE
}

void BLEHID::onWrite(BLECharacteristic* me) {
  uint8_t* value = (uint8_t*)(me->getValue().c_str());
  (void)value;
  ESP_LOGI(LOG_TAG, "special keys: %d", *value);
}