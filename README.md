# Presentation Wand (ESP32-C3 Super Mini)

A compact, multi-functional handheld wireless presentation controller with motion gesture support, haptic feedback, and laser pointer. Built on ESP32-C3 Super Mini with BLE HID and OTA firmware updates.

---

## ✨ Features

### Hardware
- **ESP32-C3 Super Mini** - Compact BLE 5.0 microcontroller
- **3-Button Control** - Pointer, Thumbs Up, Thumbs Down
- **MPU6050 IMU** - 6-axis accelerometer & gyroscope for motion detection
- **Laser Pointer** - PWM-controlled laser module
- **Haptic Motor** - Tactile feedback for user interactions
- **OTA Updates** - Wireless firmware updates via WiFi

### Software Capabilities
- **Multi-Mode Operation**
  - **Presentation Mode** - Slide navigation and presentation control
  - **Media Control Mode** - Music/video playback control
  - **Command Mode** - Settings and configuration
  
- **Dual Input Methods**
  - **Simple Input** - Button-based control (clicks, holds)
  - **Motion Control** - Gesture-based control (flicks, rotations)

- **Advanced Features**
  - BLE HID keyboard emulation (no dongle required)
  - Motion gesture recognition (6 gesture types)
  - Customizable intensity settings (haptics, laser)
  - Persistent settings storage (EEPROM)
  - Debug logging system (conditional compilation)
  - FreeRTOS multi-tasking architecture

---

---

## 🎮 Quick Control Reference

### Mode Switching
- **Enter Command Mode**: Hold all 3 buttons simultaneously
- **Switch Function Mode**: Hold Thumbs Up + Thumbs Down (Presentation ↔ Media)
- **Switch Input Mode**: In Command Mode, single click Pointer button

### Presentation Mode (Simple Input)
| Button | Action | Function |
|--------|--------|----------|
| Pointer | Press/Hold | Laser On/Off |
| Thumbs Up | Single Click | Next Slide (→) |
| Thumbs Up | Double Click | Volume Up |
| Thumbs Up | Triple Click | Full Screen (F11) |
| Thumbs Down | Single Click | Previous Slide (←) |
| Thumbs Down | Double Click | Volume Down |
| Thumbs Down | Triple Click | Blank Screen (Space) |

### Presentation Mode (Motion Control)
| Gesture | Function |
|---------|----------|
| Flick Up/Right | Next Slide |
| Flick Down/Left | Previous Slide |
| Rotate CW | Continuous Next |
| Rotate CCW | Continuous Previous |
| Thumbs Down Press | Enable Motion Detection |
| Thumbs Up Triple | Lock Motion Detection |

### Media Control Mode
| Button | Action | Function |
|--------|--------|----------|
| Pointer | Single Click | Play/Pause |
| Pointer | Double Click | Next Track |
| Pointer | Triple Click | Previous Track |
| Thumbs Up | Single Click | Volume Up |
| Thumbs Down | Single Click | Volume Down |

**Motion Gestures**: Flick Up/Right (Next), Flick Down/Left (Prev), Rotate CW/CCW (Volume)

### Command Mode (Settings)
| Button | Action | Function |
|--------|--------|----------|
| Pointer | Single Click | Switch Input Mode |
| Pointer | Double Click | Test Haptics |
| Thumbs Up | Single Click | Toggle Haptics |
| Thumbs Up | Double Click | Toggle Laser |
| Thumbs Up | Triple Click | Toggle BLE HID |
| Thumbs Down | Single Click | Tune Haptics Intensity |
| Thumbs Down | Double Click | Tune Laser Intensity |

**Intensity Tuning**: Use Thumbs Up/Down to adjust (±10), Hold any button to exit

---

## 🛠️ Development

### Hardware Setup
- **Board**: ESP32-C3 Super Mini (WEMOS LOLIN C3 Mini compatible)
- **Sensors**: MPU6050 (I2C)
- **Output**: Laser module, Haptic motor
- **Buttons**: 3x tactile switches (with pull-up resistors)

### Pin Configuration
| Component | GPIO Pin |
|-----------|----------|
| Pointer Button | 10 |
| Thumbs Up Button | 7 |
| Thumbs Down Button | 6 |
| Haptic Motor | A0 |
| Laser Pointer | A1 |
| MPU6050 SDA | Default I2C |
| MPU6050 SCL | Default I2C |

### Software Stack
- **IDE**: PlatformIO (VS Code)
- **Framework**: Arduino ESP32
- **RTOS**: FreeRTOS (built-in)

### Libraries
```ini
lib_deps = 
    ayushsharma82/ElegantOTA@^3.1.7
    h2zero/NimBLE-Arduino@^1.4.0
    rfetick/MPU6050_light@^1.1.0
```

### Build Flags
```ini
build_flags = 
    -D USE_NIMBLE          # Use NimBLE instead of Bluedroid
    -D USING_WIFI_STA      # WiFi station mode for OTA
    -D DEBUG_MODE          # Enable debug logging (comment out for production)
```

### Project Structure
```
Presentation_Wand/
├── src/
│   ├── main.cpp                    # Application entry point
│   ├── input_task/
│   │   ├── button_task.cpp        # Button input handling
│   │   └── mpu_task.cpp           # Motion detection
│   └── output_task/
│       ├── hid_task.cpp           # BLE HID output
│       ├── haptics_task.cpp       # Haptic feedback
│       ├── laser_task.cpp         # Laser control
│       └── ota_task.cpp           # OTA updates
├── include/
│   ├── main.h
│   ├── input_task/
│   └── output_task/
├── lib/
│   ├── BLE_HID/                   # BLE HID implementation
│   ├── Button/                    # Button debouncing & events
│   ├── DeviceManager/             # System state management
│   ├── MPU/                       # Motion processing
│   ├── OutputDevice/              # Haptics & Laser drivers
│   ├── OTA_Service/               # OTA update handler
│   └── Utils/                     # Common definitions & macros
└── platformio.ini                 # Build configuration
```

---

## 📝 Key Design Patterns

### Debug Logging System
All `Serial.print()` statements are wrapped in conditional macros:
```cpp
#ifdef DEBUG_MODE
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```
- **Production**: Remove `-D DEBUG_MODE` → Zero overhead, smaller binary
- **Development**: Keep `-D DEBUG_MODE` → Full logging at 115200 baud

### Task Architecture
FreeRTOS tasks for concurrent operation:
- **Input Tasks**: Button monitoring, motion detection
- **Output Tasks**: HID commands, haptics, laser, OTA
- **Queue-based Communication**: Lock-free inter-task messaging

### State Management
Centralized `DeviceManager` class:
- Feature enable/disable flags (event groups)
- Intensity settings (0-100%, persistent storage)
- System mode tracking (function mode, input mode)

---

## 🚀 Getting Started

### 1. Build & Upload
```bash
# Build firmware
pio run

# Upload via USB
pio run --target upload

# Monitor serial output (optional)
pio device monitor --baud 115200
```

### 2. OTA Update (After Initial Upload)
1. Hold all 3 buttons to enter Command Mode
2. Connect to WiFi network (configure in code)
3. Access OTA web interface at device IP
4. Upload new firmware binary

### 3. First Use
1. Power on device
2. Pair via Bluetooth: Look for "Tu's Wand"
3. Device starts in Presentation Mode with Simple Input
4. Press buttons to control presentation

---

## 🎯 Usage Examples

### Example 1: Basic Presentation
```
1. Pair device via Bluetooth
2. Open PowerPoint/Google Slides
3. Use Thumbs Up (next) / Thumbs Down (previous)
4. Hold Pointer button for laser pointer
```

### Example 2: Gesture-Based Navigation
```
1. Hold Thumbs Down to enable motion detection
2. Flick wrist up/right for next slide
3. Flick wrist down/left for previous slide
4. Rotate wrist clockwise for continuous advance
```

### Example 3: Adjust Settings
```
1. Hold all 3 buttons → Command Mode
2. Thumbs Down (Single) → Tune Haptics
3. Thumbs Up multiple times → Increase intensity
4. Hold any button → Exit tuning mode
```

### Example 4: Media Control
```
1. Hold Thumbs Up + Down → Switch to Media Mode
2. Pointer (Single) → Play/Pause
3. Pointer (Double) → Next track
4. Rotate wrist for volume control (Motion mode)
```

---

## ⚙️ Configuration

### Motion Detection Thresholds
In [lib/Utils/Utils.h](lib/Utils/Utils.h):
```cpp
#define TILT_THRESHOLD   30.0  // degrees
#define FLICK_THRESHOLD  250.0 // degrees/second
#define ROTATE_THRESHOLD 700.0 // degrees/second
```

### Button Timing
```cpp
#define BUTTON_DEBOUNCE_DEFAULT       50  // ms
#define BUTTON_PRESS_WAIT_TIME       200  // ms
#define BUTTON_SHORT_PRESS_THRESHOLD 150  // ms
```

### Task Priorities
```cpp
#define BUTTON_TASK_PRIORITY    4
#define HID_TASK_PRIORITY       5
#define MPU_TASK_PRIORITY       3
#define HAPTICS_TASK_PRIORITY   2
#define LASER_TASK_PRIORITY     1
#define OTA_TASK_PRIORITY       5
```

---

## 🔧 Troubleshooting

### Issue: Buttons not responding
- Check GPIO pin connections
- Verify pull-up resistors (10kΩ recommended)
- Check debounce timing in code

### Issue: Motion gestures not detected
- Calibrate MPU6050 on flat surface at startup
- Check I2C connections (SDA/SCL)
- Adjust threshold values if too sensitive/insensitive

### Issue: BLE not connecting
- Ensure "Tu's Wand" appears in Bluetooth devices
- Check BLE is enabled (toggle in Command Mode)
- Restart both device and computer

### Issue: Laser/Haptics not working
- Check feature is enabled (Command Mode → Thumbs Up)
- Verify intensity is not zero (tune in Command Mode)
- Check power supply and GPIO connections

### Issue: OTA not accessible
- Verify WiFi credentials in code
- Check device is in same network
- Use serial monitor to see IP address

---

## 📚 Further Reading

- [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - Detailed button mapping and gesture guide
- PlatformIO documentation: https://docs.platformio.org
- ESP32-C3 datasheet: https://www.espressif.com/en/products/socs/esp32-c3
- NimBLE documentation: https://github.com/h2zero/NimBLE-Arduino

---

## 📄 License

This project is open source. Feel free to modify and distribute.

---

## 🙏 Acknowledgments

- ElegantOTA library by Ayush Sharma
- NimBLE-Arduino by h2zero
- MPU6050_light by rfetick
- ESP32 Arduino core by Espressif
- Code refactoring and documentation assisted by GitHub Copilot AI

---

**Built with ❤️ for better presentations**


