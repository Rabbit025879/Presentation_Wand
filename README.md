# Presentation Wand (ESP32-C3 Super Mini)

A compact handheld Bluetooth presentation wand with BLE HID and muli-functionality. Built around the ESP32-C3 Super Mini, with OTA updates.

---

## Features
- **Elegant OTA**
  - OTA upload firmware

- **Bluetooth Presentation Control (BLE HID)**
  - Uses BLE 5.0 HID protocol (no Bluetooth Classic required)

- **Motion / Gesture Control**
  - MPU6050 accelerometer & gyroscope (I2C)

<!-- - **LED Indicators**
  - Shows BLE pairing status, gesture triggers, and slide actions
  - GPIO2 (PWM or simple HIGH/LOW blink) -->

- **Laser Pointer Control**
  - GPIO controlled

- **Haptics Feedback**

<!-- - **Rechargeable Design**
  - 3.7V LiPo battery
  - TP4056 charging module (micro-USB, overcharge protection)
  - Optional battery voltage monitoring via ADC -->

<!-- - **Low Power Design**
  - Deep sleep + wakeup via touch or MPU6050 interrupt
  - BLE interval adjustable, LED/laser only active when triggered -->

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

## Function Details
- **Input Mode**
  - **Simple IO** : Using buttons for control
    - Events : Single/Double/Triple Click & Hold
  - **Motion Control** : Detect motion events for control
    - Events : Flick Up/Down/Left/Right, Rotate Clockwise/CounterClockwise
    - Attitude : Tilt Up/Down/Left/Right/Clockwise/CounterClockwise
  - **Command** : Used for apply function changes & settings
    - Events : Single/Double/Triple Click & Hold
      - Hold all three button at once to enter OTA mode
      - Pointer Button :
        - Single Click to switch Function Mode
        - Double Click to turn on/off haptics
        - Triple Click to turn on/off all the LEDs
      - Thumbs Up Button : 
        - Single Click for activate/deactivate haptics
        - Double Click for activate/deactivate laser
        - Triple Click for activate/deactivate BLE HID
      - Thumbs Down Button :
        - Single Click to tune/save haptics intensity with Thumb Up/Down Button
        - Double Click to tune/save laser intensity with Thumb Up/Down Button
        - Triple Click to tune/save LED intensity with Thumb Up/Down Button
  - Hold Thumbs Up/Down button at the same time to switch mode

- **Function Mode**
  - **Presentation**
    - Simple IO : 
      - Pointer Button : Press/Release to turn on/off the laser pointer
      - Thumbs Up Button : 
        - Single Click for next page (arrow right)
        - Double Click for volume up (volume up)
        - Triple Click for full screen (F11)
      - Thumbs Down Button :
        - Single Click for previous page (arrow left)
        - Double Click for volume down (volume down)
        - Triple Click for white space (' ')
    - Motion Control :
      - Pointer Button : Press/Release to turn on/off the laser pointer
      - Thumbs Up Button :
        - Single Click for full screen (F11)
        - Double Click for white space (' ')
        - Triple Click for lock motion detect activate/deactivate
      - Thumbs Down Button : Press/Release to activate/deactivate motion detect
      - Motion Detect :
        - Flick Up/Right for next page (arrow right)
        - Flick Down/Left for previous page (arrow left)
        - Rotate Clockwise for continuously next page (arrow right) until Rotate CounterClockwise
        - Rotate CounterClockwise for continuously previous page (arrow left) until Rotate Clockwise
- **Media Control**
    - Simple IO : 
      - Pointer Button :
        - Single Click for play/pause
        - Double Click for next song
        - Triple Click for previous song
      - Thumbs Up Button : 
        - Single Click for volume up (volume up)
      - Thumbs Down Button :
        - Single Click for volume down (volume down)
    - Motion Control :
      - Pointer Button : Single Click for play/pause
      - Thumbs Up Button :
        - Single Click for volume up (volume up)
        - Double Click for lock motion detect activate/deactivate
        - Hold for activate motion detect until release
      - Thumbs Down Button : 
        - Single Click for volume up (volume down)
        - Double Click for lock motion detect activate/deactivate
        - Hold for activate motion detect until release
      - Motion Detect :
        - Flick Up/Right for next song
        - Flick Down/Left for previous song
        - Rotate Clockwise for volume up (volume up)
        - Rotate CounterClockwise for volume down (volume down)
- **Mouse**
  - To be implement
- **Keyboard**
  - To be implement


