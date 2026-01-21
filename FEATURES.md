# Presentation Wand - Feature Implementation Guide

## Overview
The Presentation Wand is a multi-functional BLE HID controller with three independent buttons (Pointer, Thumbs Up, Thumbs Down) supporting multiple input modes, function modes, and interactive features.

---

## System Architecture

### Input Modes
1. **Simple Input**: Direct button clicks for immediate actions
2. **Motion Control**: Uses accelerometer/gyroscope for gesture recognition
3. **Command Mode**: Settings, customization, and OTA mode

### Function Modes
1. **Presentation Mode**: Slideshow/presentation control with PDF navigation
2. **Media Control Mode**: Music/media player control with volume management

---

## Feature Details

### 1. COMMAND MODE (Settings & Configuration)
**Entry**: Hold all three buttons simultaneously
**Exit**: Press Pointer button or exit mode

#### Pointer Button (Command Mode)
- **Single Click**: Switch function mode (Presentation ↔ Media Control)
- **Double Click**: Toggle Haptics On/Off
- **Triple Click**: Toggle LED On/Off

#### Thumbs Up Button (Command Mode)
- **Single Click**: Toggle Haptics On/Off
- **Double Click**: Toggle Laser On/Off
- **Triple Click**: Toggle BLE HID On/Off

#### Thumbs Down Button (Command Mode)
- **Single Click**: Enter Haptics Intensity Tuning
  - Thumbs Up: Increase intensity (+10)
  - Thumbs Down: Decrease intensity (-10)
  - Pointer Press: Exit tuning
  
- **Double Click**: Enter Laser Intensity Tuning
  - Same controls as haptics tuning
  
- **Triple Click**: Enter LED Intensity Tuning
  - Same controls as haptics tuning

### 2. PRESENTATION MODE - Simple Input

#### Pointer Button
- **Press/Release**: Turn on/off laser pointer
- Haptics feedback on press/release

#### Thumbs Up Button
- **Single Click**: Next page (→ Key)
- **Double Click**: Volume up
- **Triple Click**: Full screen (F11)

#### Thumbs Down Button
- **Single Click**: Previous page (← Key)
- **Double Click**: Volume down
- **Triple Click**: White space/Black screen (Space)

### 3. PRESENTATION MODE - Motion Control

#### Pointer Button
- **Press/Release**: Turn on/off laser pointer

#### Thumbs Up Button
- **Single Click**: Full screen (F11)
- **Double Click**: White space (Space)
- **Triple Click**: Lock/Unlock motion detection

#### Thumbs Down Button
- **Press/Release**: Activate/Deactivate motion detection

#### Motion Gestures
- **Flick Up**: Next page (→ Key)
- **Flick Down**: Previous page (← Key)
- **Flick Right**: Next page (→ Key)
- **Flick Left**: Previous page (← Key)
- **Rotate Clockwise**: Continuous next page
- **Rotate CounterClockwise**: Continuous previous page

### 4. MEDIA CONTROL MODE - Simple Input

#### Pointer Button
- **Single Click**: Play/Pause
- **Double Click**: Next track
- **Triple Click**: Previous track

#### Thumbs Up Button
- **Single Click**: Volume up

#### Thumbs Down Button
- **Single Click**: Volume down

### 5. MEDIA CONTROL MODE - Motion Control

#### Pointer Button
- **Single Click**: Play/Pause

#### Thumbs Up Button
- **Single Click**: Volume up
- **Double Click**: Lock/Unlock motion detection

#### Thumbs Down Button
- **Single Click**: Volume down
- **Double Click**: Lock/Unlock motion detection

#### Motion Gestures
- **Flick Up**: Next track
- **Flick Down**: Previous track
- **Flick Right**: Next track
- **Flick Left**: Previous track
- **Rotate Clockwise**: Volume up
- **Rotate CounterClockwise**: Volume down

### 6. MODE SWITCHING
**Hold Thumbs Up + Thumbs Down** (outside Command Mode)
- Toggles between Presentation and Media Control modes
- Only works in Simple Input or Motion Control modes

---

## Device Settings & Status

### Tracked Settings
- **Haptics Intensity**: 0-255 (default: 100)
- **Laser Intensity**: 0-255 (default: 100)
- **LED Intensity**: 0-255 (default: 100)
- **Haptics Enabled**: Boolean (default: On)
- **Laser Enabled**: Boolean (default: On)
- **LED Enabled**: Boolean (default: On)
- **Motion Detect Lock**: Boolean (default: Off)

### SettingsManager Class
Located in `lib/Utils/SettingsManager.h/cpp`

Methods:
- `increaseSetting(uint8_t setting_id, uint8_t step)`: Increase intensity
- `decreaseSetting(uint8_t setting_id, uint8_t step)`: Decrease intensity
- `toggleHaptics()`, `toggleLaser()`, `toggleLed()`: Toggle features
- `toggleMotionDetectLock()`: Lock motion detection
- `printCurrentSettings()`: Output current settings to Serial

---

## Hardware Components

### Buttons
- **Pointer Button**: Pin 10 (Primary interaction)
- **Thumbs Up Button**: Pin 7 (Secondary - increment/next)
- **Thumbs Down Button**: Pin 6 (Tertiary - decrement/previous)

### Output Devices
- **Haptics Motor**: Pin A0 (Tactile feedback)
- **Laser Pointer**: Pin A1 (Presentation pointer)
- **LED**: Controlled via OutputDevice class
- **BLE HID**: Wireless keyboard/media control

### Motion Sensor
- **MPU6050/MPU9250**: 6-axis IMU for gesture recognition
- Detects flicks, rotations, and tilt attitudes

---

## FreeRTOS Task Structure

### Button Task (`button_task.cpp`)
- Polls three buttons using MultiButton class
- Detects single/double/triple clicks and holds
- Routes events to appropriate queues based on input mode
- Manages mode transitions

### HID Task (`hid_task.cpp`)
- Converts button/motion events to BLE HID commands
- Implements mode-specific keyboard shortcuts
- Supports keyboard and media keys

### Laser Task (`laser_task.cpp`)
- Controls laser pointer on/off states
- Uses pointerButtonState for control

### Haptics Task (`haptics_task.cpp`)
- Provides tactile feedback for button interactions
- Intensity controlled by settings

### MPU Task (`mpu_task.cpp`)
- Reads motion sensor data
- Detects flicks and rotations
- Updates motionState in InputEvent
- Sends motion events to HID queue

### OTA Task (`ota_task.cpp`)
- Over-the-air firmware updates
- Entered via Command Mode + all buttons

---

## Classes & Data Structures

### MultiButton Class
Manages three independent buttons simultaneously.
```cpp
MultiButton multi_button;
InputEvent event = multi_button.getState();
bool both_thumbs = multi_button.bothThumbButtonsPressed();
bool all_three = multi_button.allThreeButtonsPressed();
```

### InputEvent Structure
```cpp
struct InputEvent {
  ButtonState pointerButtonState;
  ButtonState thumbsUpButtonState;
  ButtonState thumbsDownButtonState;
  MotionState motionState;
};
```

### DeviceSettings Structure
```cpp
struct DeviceSettings {
  uint8_t hapticsIntensity;
  uint8_t laserIntensity;
  uint8_t ledIntensity;
  bool hapticsEnabled;
  bool laserEnabled;
  bool ledEnabled;
  bool motionDetectLocked;
};
```

---

## Event Types

### Button Events
- `None`: No event
- `SingleClick`: Single press and release
- `DoubleClick`: Two clicks within 700ms
- `TripleClick`: Three clicks within 700ms
- `SingleLongPress`: Press held >200ms, release <700ms
- `DoubleLongPress`: Two holds within 700ms
- `TripleLongPress`: Three holds within 700ms
- `Hold`: Button held down continuously
- `OtherPattern`: Unrecognized pattern

### Motion Events
- `None`: No motion
- `FlickUp`: Fast upward motion
- `FlickDown`: Fast downward motion
- `FlickLeft`: Fast leftward motion
- `FlickRight`: Fast rightward motion
- `RotateClockwise`: CW rotation
- `RotateCounterClockwise`: CCW rotation

---

## BLE HID Keys Used

### Keyboard Keys
- Navigation: LEFT_ARROW, RIGHT_ARROW, UP_ARROW, DOWN_ARROW
- Function: F11 (fullscreen)
- Characters: Space

### Media Keys
- `KEY_MEDIA_PLAY_PAUSE`: Play/Pause
- `KEY_MEDIA_NEXT_TRACK`: Next track
- `KEY_MEDIA_PREVIOUS_TRACK`: Previous track
- `KEY_MEDIA_VOLUME_UP`: Volume up
- `KEY_MEDIA_VOLUME_DOWN`: Volume down
- `KEY_MEDIA_MUTE`: Mute

---

## Usage Examples

### Example 1: Quick Settings Access
1. Hold all three buttons → Enter Command Mode
2. Press Thumbs Down (Single) → Enter Haptics Tuning
3. Press Thumbs Up to increase or Thumbs Down to decrease
4. Press Pointer button → Exit tuning and show settings

### Example 2: Switch Modes & Give Presentation
1. Hold Thumbs Up + Thumbs Down → Switch to Presentation Mode
2. Use Pointer button to control laser
3. Use Thumbs Up/Down for navigation and controls
4. Use Motion gestures for hands-free navigation

### Example 3: Control Music
1. Hold Thumbs Up + Thumbs Down → Switch to Media Control Mode
2. Use Pointer for play/pause
3. Use Thumbs Up/Down for volume
4. Use motion gestures for track selection

---

## Configuration & Customization

### Button Debounce Time
Default: 50ms (BUTTON_DEBOUNCE_DEFAULT in Utils.h)

### Click Detection Thresholds
- Short Press: <200ms
- Long Press: >200ms but <700ms
- Press Wait Time: 700ms (max time between clicks for multi-click)

### Motion Detection
- Flick Threshold: 250°/second
- Tilt Threshold: 30°
- Motion Debounce: 700ms (min time between motion events)

### Intensity Levels
- Range: 0-255 (8-bit PWM)
- Tuning Step: ±10 per button press
- Storage: In DeviceSettings structure

---

## Future Enhancements

1. **EEPROM Storage**: Save settings and intensity levels across power cycles
2. **LED Status Indicators**: Visual feedback for mode and settings
3. **Gesture Customization**: User-definable gesture mappings
4. **Multiple Profiles**: Save/load different configuration profiles
5. **Adaptive Sensitivity**: Automatic gesture detection tuning
6. **Advanced Motion**: Tap detection, shake detection, free-form drawing

---

## Troubleshooting

### Motion Events Not Working
- Ensure MPU task is enabled (USING_MPU flag)
- Check Motion Control input mode is active
- Verify flick velocity exceeds FLICK_THRESHOLD
- Check tilt angle exceeds TILT_THRESHOLD for attitude events

### Button Not Responding
- Check button debounce settings
- Verify button pin configuration in Utils.h
- Ensure MultiButton instance is initialized

### HID Commands Not Sending
- Check BLE connection status
- Verify USING_HID flag is set
- Ensure function mode matches intended commands
- Check InputMode is not Command mode

### Settings Not Persisting
- Settings are currently stored in RAM only
- Implement EEPROM storage for persistence (future feature)

---

## Code Organization

```
├── src/
│   ├── main.cpp                 # Task initialization
│   ├── button_task.cpp          # Button handling (3 buttons)
│   ├── hid_task.cpp             # BLE HID keyboard/media
│   ├── laser_task.cpp           # Laser control
│   ├── haptics_task.cpp         # Haptics feedback
│   ├── mpu_task.cpp             # Motion detection
│   └── ota_task.cpp             # OTA updates
├── lib/
│   ├── Button/
│   │   ├── Button.h/cpp         # Single button class
│   │   └── MultiButton.h/cpp    # Multi-button support (NEW)
│   ├── BLE_HID/                 # BLE keyboard/mouse
│   ├── Utils/
│   │   ├── Utils.h              # Enums & structures
│   │   ├── SettingsManager.h/cpp# Settings management (NEW)
│   └── [Other libraries]
└── include/
    ├── button_task.h
    ├── hid_task.h
    └── [Other task headers]
```

---

## Implementation Summary

✅ **Completed Features**
- Multi-button support (Pointer, Thumbs Up, Thumbs Down)
- Three input modes (Simple IO, Motion Control, Command)
- Two function modes (Presentation, Media Control)
- Command mode with all settings controls
- Intensity tuning for haptics, laser, and LED
- Motion gesture recognition (flicks, rotations)
- BLE HID keyboard and media control
- Haptics and laser feedback
- Mode switching between Presentation and Media Control
- FreeRTOS task-based architecture

✅ **Ready for Testing**
- All button combinations implemented
- All motion gesture handlers in place
- BLE HID command mapping complete
- Settings management system active
- Serial debugging output enabled

---

## Next Steps

1. **Hardware Testing**: Verify all buttons and sensors work
2. **BLE Connection**: Test Bluetooth pairing and command sending
3. **Motion Calibration**: Tune gesture thresholds for user preference
4. **Feedback Testing**: Verify haptics and LED feedback
5. **Integration Testing**: Test all mode transitions and feature interactions
6. **Optimization**: Profile performance and optimize critical paths
7. **Documentation**: Create user manual with gesture diagrams

