# Presentation Wand (ESP32-C3 Super Mini)

A compact handheld Bluetooth presentation wand with BLE HID and muli-functionality. Built around the ESP32-C3 Super Mini, with optional OTA updates.

---

## Features
- **Elegant OTA**
  - OTA upload firmware

- **Bluetooth Presentation Control (BLE HID)**
  - Simulates keyboard keys: F5, PageUp, PageDown, Left/Right arrows
  - Uses BLE 5.0 HID protocol (no Bluetooth Classic required)

<!-- - **Motion / Gesture Control**
  - MPU6050 accelerometer & gyroscope (I2C)
  - Left/Right swings trigger slide forward/back
  - Optional up/down or rotation gestures for extra functions -->

<!-- - **Capacitive Touch Buttons**
  - Uses ESP32-C3 touch channels (T0~T9, e.g., GPIO0/2)
  - Single touch triggers slide control or laser pointer
  - Includes debounce logic -->

<!-- - **LED Indicators**
  - Shows BLE pairing status, gesture triggers, and slide actions
  - GPIO2 (PWM or simple HIGH/LOW blink) -->

<!-- - **Laser Pointer Control**
  - GPIO controlled (100~220Ω current-limiting resistor)
  - Can sync with gestures or touch events -->

<!-- - **Rechargeable Design**
  - 3.7V LiPo battery
  - TP4056 charging module (micro-USB, overcharge protection)
  - Optional battery voltage monitoring via ADC -->

<!-- - **Low Power Design**
  - Deep sleep + wakeup via touch or MPU6050 interrupt
  - BLE interval adjustable, LED/laser only active when triggered -->

---

## Partition Table Notes

- Currently using defualt
- Consider using huge_app.csv if flash storage isn't enough

---

## Development

- **Board:** WEMOS LOLIN C3 Mini
- **IDE:** PlatformIO
- **Libraries:**
  - `BLE_Keyboard`
  - `MPU6050`
  - `ElegantOTA`
  - `NimBLE-Arduino`
- **Source Structure:**
  - `src/` → main program
  - `lib/` → modular libraries (BLE, OTA...)
