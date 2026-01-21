# Presentation Wand - Quick Reference Guide

**Version**: 2.0 (Refactored)  
**Updated**: January 2026

---

## 🎯 MODE ENTRY KEYS

### Enter Command Mode
**Hold All 3 Buttons** (Pointer + Thumbs Up + Thumbs Down)
- Access settings and intensity tuning
- Configure which features are active (Haptics/Laser/BLE HID)
- **Hold again while in Command Mode to restart device**

### Switch Function Modes
**Hold Thumbs Up + Thumbs Down** (outside Command Mode)
- Toggle between **Presentation Mode** ↔ **Media Control Mode**
- Haptic feedback confirms mode switch

### Switch Input Modes (Command Mode)
**Pointer Single Click** (while in Command Mode)
- Toggle between **Simple Input** ↔ **Motion Control**
- Returns to last used input mode when exiting Command Mode

---

---

## 📊 PRESENTATION MODE

### Simple Input Mode (Default)
| Button | Action | Command | Key Sent |
|--------|--------|---------|----------|
| **Pointer** | Press/Hold | Laser On/Off | - |
| **Thumbs Up** | Single Click | Next Page | → |
| **Thumbs Up** | Double Click | Volume Up | Vol+ |
| **Thumbs Up** | Triple Click | Full Screen | F11 |
| **Thumbs Down** | Single Click | Previous Page | ← |
| **Thumbs Down** | Double Click | Volume Down | Vol- |
| **Thumbs Down** | Triple Click | Blank Screen | Space |

### Motion Control Mode
| Input | Action | Command | Key Sent |
|-------|--------|---------|----------|
| **Pointer** | Press/Hold | Laser On/Off | - |
| **Thumbs Up** | Single Click | Full Screen | F11 |
| **Thumbs Up** | Double Click | Blank Screen | Space |
| **Thumbs Up** | Triple Click | Lock/Unlock Motion | - |
| **Thumbs Down** | Press/Hold | Enable Motion Detection | - |
| **Flick** | Up or Right | Next Page | → |
| **Flick** | Down or Left | Previous Page | ← |
| **Rotate** | Clockwise | Continuous Next | → (repeat) |
| **Rotate** | Counter-CW | Continuous Prev | ← (repeat) |

**Motion Detection**:
- Hold Thumbs Down to activate
- Release to deactivate
- Triple-click Thumbs Up to lock motion ON (hands-free)
- Any other motion or deactive detection to stop Continuous Next/Prev

---

## 🎵 MEDIA CONTROL MODE

### Simple Input Mode
| Button | Action | Command | Key Sent |
|--------|--------|---------|----------|
| **Pointer** | Single Click | Play/Pause | Media Play/Pause |
| **Pointer** | Double Click | Next Track | Media Next |
| **Pointer** | Triple Click | Previous Track | Media Prev |
| **Thumbs Up** | Single Click | Volume Up | Vol+ |
| **Thumbs Down** | Single Click | Volume Down | Vol- |

### Motion Control Mode
| Input | Action | Command | Key Sent |
|-------|--------|---------|----------|
| **Pointer** | Single Click | Play/Pause | Media Play/Pause |
| **Thumbs Up** | Single Click | Volume Up | Vol+ |
| **Thumbs Down** | Single Click | Volume Down | Vol- |
| **Thumbs Down** | Press/Hold | Enable Motion | - |
| **Flick** | Up or Right | Next Track | Media Next |
| **Flick** | Down or Left | Previous Track | Media Prev |
| **Rotate** | Clockwise | Volume Up | Vol+ (repeat) |
| **Rotate** | Counter-CW | Volume Down | Vol- (repeat) |

---

## ⚙️ COMMAND MODE (Settings & Configuration)

### Feature Toggles (Pointer Button)
| Action | Function | Effect |
|--------|----------|--------|
| **Single Click** | Switch Input Mode | Toggle Simple ↔ Motion |
| **Double Click** | Haptics Test | Test haptic feedback |
| **Triple Click** | LED Test | Test LED indicators |

### Enable/Disable Features (Thumbs Up Button)
| Action | Function | Status |
|--------|----------|--------|
| **Single Click** | Toggle Haptics | ON/OFF |
| **Double Click** | Toggle Laser | ON/OFF |
| **Triple Click** | Toggle BLE HID | ON/OFF |

**Note**: Features can be enabled/disabled independently. Settings persist until device restart.

### Intensity Tuning (Thumbs Down Button)
| Action | Target | Range |
|--------|--------|-------|
| **Single Click** | Tune Haptics Intensity | 0-100% |
| **Double Click** | Tune Laser Intensity | 0-100% |
| **Triple Click** | Tune LED Intensity | 0-100% |

**Tuning Controls**:
- **Thumbs Up**: Increase intensity (+10%)
- **Thumbs Down**: Decrease intensity (-10%)
- **Hold Any Button**: Exit tuning and save

**Settings are saved to EEPROM and persist across power cycles.**

---

## ⏱️ BUTTON TIMING & PATTERNS

| Pattern | Timing | Description |
|---------|--------|-------------|
| **Single Click** | < 150ms | Quick tap and release |
| **Double Click** | 2nd click within 200ms | Two rapid taps |
| **Triple Click** | 3rd click within 200ms | Three rapid taps |
| **Long Press** | > 150ms | Press and hold briefly |
| **Hold** | Continuous press | Hold until action complete |

**Debounce**: 50ms hardware debounce prevents false triggers

---

## 🎛️ HARDWARE CONFIGURATION

### Pin Assignments
| Component | GPIO Pin | Type |
|-----------|----------|------|
| Pointer Button | 10 | Input (Pull-up) |
| Thumbs Up Button | 7 | Input (Pull-up) |
| Thumbs Down Button | 6 | Input (Pull-up) |
| Haptic Motor | A0 | PWM Output |
| Laser Pointer | A1 | PWM Output |
| MPU6050 SDA | Default | I2C Data |
| MPU6050 SCL | Default | I2C Clock |

### Motion Detection Parameters
| Parameter | Value | Purpose |
|-----------|-------|---------|
| **Flick Threshold** | 250°/s | Minimum angular velocity for flick |
| **Rotate Threshold** | 700°/s | Minimum angular velocity for rotation |
| **Tilt Threshold** | 30° | Angle change for tilt detection |
| **Motion Debounce** | 200ms | Minimum time between gestures |
| **Continuous Throttle** | 500ms | Delay between rotation repeats |

### Intensity Settings
| Setting | Range | Default | Saved |
|---------|-------|---------|-------|
| Haptics | 0-100% | 50% | ✓ EEPROM |
| Laser | 0-100% | 50% | ✓ EEPROM |
| LED | 0-100% | 50% | ✓ EEPROM |

**Internal Conversion**: Settings are stored as 0-100% and converted to 0-255 PWM internally.

---

## 🔧 DEBUG MODE

### Serial Logging (115200 baud)
Enable debug output by defining `DEBUG_MODE` in `platformio.ini`:

```ini
build_flags = 
    -D DEBUG_MODE
```

### Debug Output Examples
```
[MPU Task] Initialized successfully
Entered Command Mode
Switched to Motion Control Mode
Motion Detect Lock: LOCKED
Haptics Intensity: 60
Laser: OFF
BLE HID: ON
Presentation Motion: Flick Up -> Next Page
Media Motion: Rotate CW -> Volume Up
```

### Debug Macros (Utils.h)
```cpp
DEBUG_PRINT(x)      // Print without newline
DEBUG_PRINTLN(x)    // Print with newline
DEBUG_PRINTF(...)   // Formatted print
```

**Production Build**: Comment out `-D DEBUG_MODE` to:
- Remove all logging code at compile time
- Reduce binary size (~10-15KB)
- Eliminate runtime overhead

---

## 📊 SYSTEM STATE TRACKING

### Feature Flags (Event Bits)
| Feature | Bit | Default | Toggle In Command Mode |
|---------|-----|---------|------------------------|
| USING_HAPTICS | 0 | ON | Thumbs Up Single |
| USING_LASER | 1 | ON | Thumbs Up Double |
| USING_HID | 2 | ON | Thumbs Up Triple |
| USING_MPU | 3 | OFF | Motion Detection Toggle |
| USING_OTA | 5 | OFF | Auto (WiFi available) |

### System Modes
**Function Modes**:
- Presentation (default)
- Media Control
- Mouse (future)
- Keyboard (future)

**Input Modes**:
- Simple Input (button-only)
- Motion Control (button + gesture)
- Command (settings)

---

## 🚨 TROUBLESHOOTING

### Buttons Not Responding
**Symptoms**: No feedback from button presses
- ✓ Check GPIO connections (pins 6, 7, 10)
- ✓ Verify pull-up resistors (10kΩ recommended)
- ✓ Test with serial debug output enabled
- ✓ Check button timing (may need longer press)

### Motion Gestures Not Working
**Symptoms**: Flicks/rotations not detected
- ✓ Ensure Motion Control mode is active (not Simple Input)
- ✓ Hold Thumbs Down to enable motion detection
- ✓ Check MPU6050 I2C connections
- ✓ Calibrate MPU on flat surface at startup
- ✓ Perform gestures with sufficient speed (>250°/s for flicks)
- ✓ Check `[MPU Task] Initialized successfully` in serial output

### Haptics/Laser Not Working
**Symptoms**: No haptic feedback or laser output
- ✓ Verify feature is enabled (Command Mode → Thumbs Up click)
- ✓ Check intensity is not zero (tune in Command Mode)
- ✓ Verify GPIO connections (A0 for haptics, A1 for laser)
- ✓ Check power supply (haptic motor needs adequate current)
- ✓ Test with debug output to see if commands are sent

### BLE Not Connecting
**Symptoms**: Device not visible or won't pair
- ✓ Look for "Tu's Wand" in Bluetooth devices list
- ✓ Ensure BLE HID is enabled (Command Mode → Thumbs Up Triple)
- ✓ Check `-D USE_NIMBLE` flag is set in platformio.ini
- ✓ Restart device and computer
- ✓ Remove existing pairing and re-pair
- ✓ Verify BLE is supported on target device

### Serial Debug Not Showing
**Symptoms**: No output in serial monitor
- ✓ Verify `-D DEBUG_MODE` is uncommented in platformio.ini
- ✓ Check baud rate is 115200
- ✓ Rebuild and re-upload firmware
- ✓ Try different USB cable/port

---

## 💡 USAGE EXAMPLES

### Example 1: Basic Presentation
```
1. Power on → Device starts in Presentation Mode (Simple Input)
2. Pair Bluetooth → Search for "Tu's Wand"
3. Open PowerPoint/Google Slides
4. Thumbs Up (click) → Next slide
5. Thumbs Down (click) → Previous slide
6. Pointer (hold) → Laser pointer ON
```

### Example 2: Gesture-Based Presentation
```
1. Ensure Presentation Mode is active
2. Switch to Motion Control:
   - Command Mode (hold all 3) → Pointer (single) → Exit
3. Hold Thumbs Down → Enables motion detection
4. Flick up/right → Next slide
5. Flick down/left → Previous slide
6. Rotate wrist CW → Continuous advance
7. Release Thumbs Down → Disable motion
```

### Example 3: Hands-Free Gesture Mode
```
1. In Presentation Motion mode
2. Triple-click Thumbs Up → Lock motion detection ON
3. Now motion is always active (no need to hold button)
4. Use gestures freely while presenting
5. Triple-click Thumbs Up again → Unlock motion
```

### Example 4: Adjust Haptic Feedback
```
1. Hold all 3 buttons → Enter Command Mode
2. Thumbs Down (single) → "Tuning Haptics Intensity"
3. Thumbs Up multiple times → Increase strength
   OR Thumbs Down → Decrease strength
4. Hold any button → Exit tuning, settings saved
5. Changes apply immediately and persist
```

### Example 5: Media Control with Gestures
```
1. Hold Thumbs Up + Down → Switch to Media Control Mode
2. Pointer (double) → Skip to next track
3. Switch to Motion Control (Command → Pointer)
4. Hold Thumbs Down → Enable gestures
5. Rotate CW → Volume up
6. Rotate CCW → Volume down
7. Flick up → Next track
```

### Example 6: Disable Unwanted Features
```
1. Hold all 3 buttons → Command Mode
2. Thumbs Up (single) → Disable Haptics (for quiet operation)
3. Thumbs Up (double) → Disable Laser (if not needed)
4. Features stay off until toggled back on
5. Settings reset on device restart
```

### Example 7: OTA Firmware Update
```
1. Ensure device is on WiFi network (configure SSID/password)
2. Hold all 3 buttons → Command Mode
3. Device starts OTA server automatically
4. Open browser → http://<device_ip>/update
5. Upload new firmware .bin file
6. Device reboots with new firmware
```

---

## 🎓 ADVANCED TIPS

### Efficient Mode Switching
- **Quick Function Toggle**: Hold Thumbs Up+Down for 1 second
- **Input Mode Memory**: Device remembers Simple vs Motion per function mode
- **Visual Feedback**: Haptic pulses confirm successful mode changes

### Motion Detection Best Practices
- **Calibration**: Keep device still for 2-3 seconds at startup
- **Gesture Speed**: Flicks need >250°/s, rotations >700°/s
- **Consistent Direction**: Flick in clear up/down/left/right directions
- **Lock for Demos**: Use motion lock to avoid holding button during long presentations

### Battery Optimization (Future)
- Disable unused features (haptics, laser) to save power
- Simple Input mode uses less power than Motion Control
- BLE HID is power-efficient (no active scanning)

### Customization
Edit thresholds in [lib/Utils/Utils.h](../lib/Utils/Utils.h):
```cpp
#define FLICK_THRESHOLD  250.0  // Lower = more sensitive flicks
#define ROTATE_THRESHOLD 700.0  // Lower = easier rotation detection
#define TILT_THRESHOLD   30.0   // Angle for tilt gestures
```

Rebuild firmware after changes: `pio run --target upload`

---

## 📋 TASK ARCHITECTURE (Developer Reference)

### FreeRTOS Task Overview
| Task | Priority | Stack | Function |
|------|----------|-------|----------|
| button_task | 4 | 3KB | Button input processing |
| mpu_task | 3 | 4KB | Motion detection & processing |
| hid_task | 5 | 4KB | BLE HID keyboard output |
| haptics_task | 2 | 3KB | Haptic motor control |
| laser_task | 1 | 3KB | Laser PWM control |
| ota_task | 5 | 4KB | OTA update handler |

### Inter-Task Communication
- **Queues**: Lock-free message passing between tasks
  - `haptics_queue`: Button/motion → Haptics (size: 3)
  - `laser_queue`: Button → Laser (size: 3)
  - `hid_queue`: Button/motion → HID (size: 3)
- **Shared State**: DeviceManager singleton
  - Feature flags (event groups)
  - Intensity settings
  - System mode tracking

### Code Organization
```
lib/Utils/Utils.h          → Constants, enums, structs, DEBUG macros
lib/DeviceManager/         → System state & feature management
lib/Button/                → Button debouncing & event detection
lib/MPU/                   → Motion processing algorithms
lib/BLE_HID/               → BLE HID protocol implementation
lib/OutputDevice/          → Haptics & Laser drivers
lib/OTA_Service/           → OTA update web interface
src/main.cpp               → Setup & task initialization
src/input_task/            → Button & motion input tasks
src/output_task/           → HID, haptics, laser, OTA tasks
```

---

## 🔮 FUTURE ENHANCEMENTS

### Planned Features
- [ ] **LED Status Indicators** - Visual feedback for modes and states
- [ ] **Custom Gesture Recording** - User-defined gesture patterns
- [ ] **Profile System** - Save/load different button mappings
- [ ] **Advanced Motion** - Tap detection, shake gestures
- [ ] **Battery Monitoring** - ADC-based voltage tracking
- [ ] **Deep Sleep** - Ultra-low power mode with wakeup
- [ ] **Web Configuration** - Change settings via web UI
- [ ] **Multi-Device Support** - Switch between paired devices

### Contribution Ideas
- Additional function modes (Mouse, Keyboard emulation)
- Custom HID profiles for different applications
- Mobile app for configuration
- Vibration patterns for different events

---

## 📚 ADDITIONAL RESOURCES

- **Main Documentation**: [README.md](../README.md)
- **PlatformIO**: https://docs.platformio.org
- **ESP32-C3**: https://www.espressif.com/en/products/socs/esp32-c3
- **NimBLE**: https://github.com/h2zero/NimBLE-Arduino
- **MPU6050**: https://github.com/rfetick/MPU6050_light
- **FreeRTOS**: https://www.freertos.org/

---

## ⚖️ VERSION HISTORY

### v2.0 (January 2026) - Refactored
- ✅ DEBUG_MODE conditional compilation
- ✅ Clean code structure and organization
- ✅ Improved comments and documentation
- ✅ EEPROM settings persistence
- ✅ Enhanced error handling
- ✅ Better task synchronization
- 🤖 Refactoring assisted by GitHub Copilot AI

### v1.0 (Original)
- Basic button and motion control
- BLE HID implementation
- OTA update support

---

**Last Updated**: January 21, 2026  
**Maintainer**: Tu123  
**Device**: Tu's Wand (ESP32-C3)

---

*For detailed API documentation and code examples, see source files in `lib/` and `src/` directories.*

