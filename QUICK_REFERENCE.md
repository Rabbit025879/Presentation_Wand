# Presentation Wand - Quick Reference Guide

## MODE ENTRY KEYS

### Enter Command Mode
**Hold All 3 Buttons** (Pointer + Thumbs Up + Thumbs Down)
- Access settings and intensity tuning
- Configure which features are active
- Enter OTA firmware update mode

### Switch Function Modes
**Hold Thumbs Up + Thumbs Down** (outside Command Mode)
- Toggle between **Presentation Mode** ↔ **Media Control Mode**

---

## PRESENTATION MODE

### Simple Input (Default)
| Button | Action | Command |
|--------|--------|---------|
| Pointer | Press/Release | Laser On/Off |
| Thumbs Up | Single | Next Page (→) |
| Thumbs Up | Double | Volume Up |
| Thumbs Up | Triple | Full Screen (F11) |
| Thumbs Down | Single | Prev Page (←) |
| Thumbs Down | Double | Volume Down |
| Thumbs Down | Triple | White Space |

### Motion Control (Requires Pointer Hold)
| Gesture | Direction | Command |
|---------|-----------|---------|
| Flick | Up or Right | Next Page |
| Flick | Down or Left | Prev Page |
| Rotate | Clockwise | Continuous Next |
| Rotate | Counter-CW | Continuous Prev |

**Additional Controls:**
- Thumbs Up, Single: F11 (Fullscreen)
- Thumbs Up, Double: White Space
- Thumbs Up, Triple: Lock/Unlock Motion
- Thumbs Down, Press: Toggle Motion Detection

---

## MEDIA CONTROL MODE

### Simple Input
| Button | Action | Command |
|--------|--------|---------|
| Pointer | Single | Play/Pause |
| Pointer | Double | Next Track |
| Pointer | Triple | Prev Track |
| Thumbs Up | Single | Volume Up |
| Thumbs Down | Single | Volume Down |

### Motion Control
| Gesture | Direction | Command |
|---------|-----------|---------|
| Flick | Up or Right | Next Track |
| Flick | Down or Left | Prev Track |
| Rotate | Clockwise | Volume Up |
| Rotate | Counter-CW | Volume Down |

**Additional Controls:**
- Thumbs Up, Double: Lock/Unlock Motion
- Thumbs Down, Double: Lock/Unlock Motion

---

## COMMAND MODE (Settings)

### Pointer Button
| Action | Function |
|--------|----------|
| Single Click | Switch Function Mode |
| Double Click | Toggle Haptics On/Off |
| Triple Click | Toggle LED On/Off |

### Thumbs Up Button
| Action | Function |
|--------|----------|
| Single Click | Toggle Haptics On/Off |
| Double Click | Toggle Laser On/Off |
| Triple Click | Toggle BLE HID On/Off |

### Thumbs Down Button
| Action | Function |
|--------|----------|
| Single Click | Tune Haptics Intensity |
| Double Click | Tune Laser Intensity |
| Triple Click | Tune LED Intensity |

### Tuning Mode
**During Intensity Tuning:**
- Thumbs Up: Increase (+10)
- Thumbs Down: Decrease (-10)
- Pointer Press: Exit Tuning

---

## BUTTON TIMING

- **Single Click**: Press and release normally (< 200ms press)
- **Double Click**: Two clicks within 700ms
- **Triple Click**: Three clicks within 700ms
- **Long Press**: Hold > 200ms (various durations)
- **Hold**: Continuous button hold during motion control

---

## INTENSITY TUNING STEPS

Default Range: **0-255** (8-bit PWM)
- Each button press: **±10**
- Minimum: 0 (Off)
- Maximum: 255 (Full)
- Default Startup: 100

**Example**: From 100
- Press Thumbs Up 5 times: 100 → 150
- Press Thumbs Down 3 times: 150 → 120

---

## PIN CONFIGURATION

| Component | Pin |
|-----------|-----|
| Pointer Button | 10 |
| Thumbs Up Button | 7 |
| Thumbs Down Button | 6 |
| Haptics Motor | A0 |
| Laser Pointer | A1 |

---

## MOTION GESTURE PARAMETERS

| Parameter | Value |
|-----------|-------|
| Flick Threshold | 250°/second |
| Tilt Threshold | 30° |
| Motion Debounce | 700ms |
| Button Debounce | 50ms |

---

## FEATURE STATUS TRACKING

### Per-Feature Toggles
- ✓ Haptics (Default: ON)
- ✓ Laser (Default: ON)
- ✓ LED (Default: ON)
- ✓ BLE HID (Default: ON)
- ✓ Motion Detection (Default: OFF, toggled by button hold)

### Persistence
⚠️ **NOTE**: Settings are stored in RAM only
- Settings reset on power cycle
- Future version will add EEPROM storage

---

## SERIAL DEBUG OUTPUT

Enable Serial Monitor at **115200 Baud** to see:
- Mode transitions
- Button event detection
- Motion event notifications
- Settings changes
- Intensity adjustments
- Connection status

Example Output:
```
Switched to Media Control Mode
Tuning Haptics Intensity - Use Thumb Up (+) / Down (-) to adjust
Haptics Intensity: 110
Exited tuning mode
========== Device Settings ==========
Haptics: ON | Intensity: 110
Laser: ON | Intensity: 100
LED: ON | Intensity: 100
Motion Detect Locked: NO
=====================================
```

---

## TROUBLESHOOTING QUICK TIPS

**Buttons Not Responding**
- Check button press duration (consider timing)
- Verify GPIO pins not in use by other devices
- Check debounce settings if too sensitive

**Motion Gestures Not Working**
- Ensure Motion Control input mode is active
- Hold Pointer button to enable motion detection
- Make flick motions with sufficient speed (>250°/s)
- Check MPU sensor orientation and calibration

**Haptics/Laser Not Responding**
- Verify they're not disabled in Command Mode
- Check intensity value (may be too low)
- Verify power supply to motors

**BLE Not Connecting**
- Pair device via Bluetooth settings
- Check USING_HID flag is set (toggle in Command Mode)
- Look for "Tu's Wand" in BLE device list
- Try power cycle if connection drops

---

## FEATURE INTERACTION EXAMPLES

### Example 1: Give a Presentation
1. Power on device
2. Hold Thumbs Up + Down → Ensure Presentation Mode
3. Use Pointer button to control laser
4. Use Thumbs Up/Down for slide navigation
5. Optional: Hold Thumbs Down to enable motion-based navigation

### Example 2: Adjust Haptics Strength
1. Hold all 3 buttons → Enter Command Mode
2. Press Thumbs Down (Single) → Enter Haptics Tuning
3. Press Thumbs Up multiple times to increase or Down to decrease
4. Press Pointer button → Exit and confirm settings
5. Settings apply immediately to feedback

### Example 3: Control Music with Gestures
1. Hold Thumbs Up + Down twice (if in Presentation Mode) → Switch to Media Control
2. Hold Thumbs Down to enable motion detection
3. Use flick gestures for next/previous tracks
4. Use rotation gestures for volume control
5. Release Thumbs Down to disable motion detection

### Example 4: Customize Feature Set
1. Hold all 3 buttons → Enter Command Mode
2. Press Pointer (Double) → Disable Haptics (if you want quiet)
3. Press Thumbs Up (Double) → Disable Laser (if not needed)
4. Press Thumbs Up (Triple) → Disable BLE if testing locally
5. Settings persist until next power cycle

---

## ADVANCED TIPS

- **Rapid Mode Switching**: Quickly toggle between Presentation and Media by holding Thumbs Up+Down
- **Continuous Paging**: In Presentation Motion, rotate wrist continuously for auto-advance
- **Volume Ramping**: In Media Motion, rotate wrist to ramp volume smoothly
- **Haptic Feedback Timing**: Haptics provide feedback on mode entry and settings changes
- **Motion Lock**: Lock motion detection to prevent accidental gestures during presentation

---

## NEXT FIRMWARE UPDATES (Future)

- [ ] EEPROM persistence for settings
- [ ] LED status indicators
- [ ] Gesture recording and customization
- [ ] Profile switching (work/presentation/gaming)
- [ ] Adaptive gesture sensitivity
- [ ] Gesture feedback visualization
- [ ] Advanced motion: tap detection, shake
- [ ] Voice feedback for setting changes

