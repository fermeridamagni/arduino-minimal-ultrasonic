# Installation

Complete guide to installing MinimalUltrasonic on your system.

## Arduino IDE

### Method 1: Library Manager (Recommended)

The easiest way to install:

1. **Open Arduino IDE**
2. **Navigate to Library Manager**
   - Click `Sketch` → `Include Library` → `Manage Libraries...`
   - Or press `Ctrl+Shift+I` (Windows/Linux) / `Cmd+Shift+I` (Mac)

3. **Search for Library**
   - Type "MinimalUltrasonic" in the search box
   - Look for "Minimal Ultrasonic by fermeridamagni"

4. **Install**
   - Click the "Install" button
   - Wait for installation to complete
   - You'll see "INSTALLED" when done

5. **Verify Installation**
   - Go to `Sketch` → `Include Library`
   - You should see "MinimalUltrasonic" in the list

### Method 2: Manual Installation (ZIP)

For offline installation or specific versions:

1. **Download Library**
   - Go to [GitHub Releases](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/releases)
   - Download the latest `.zip` file
   - Or download [master branch](https://github.com/fermeridamagni/arduino-minimal-ultrasonic/archive/master.zip)

2. **Install ZIP Library**
   - Open Arduino IDE
   - Click `Sketch` → `Include Library` → `Add .ZIP Library...`
   - Select the downloaded ZIP file
   - Click "Open"

3. **Restart IDE**
   - Close and reopen Arduino IDE
   - The library should now be available

### Method 3: Manual Copy

For development or custom installations:

1. **Locate Libraries Folder**
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`

2. **Download and Extract**
   - Download library from GitHub
   - Extract the ZIP file
   - Rename folder to `MinimalUltrasonic` (if needed)

3. **Copy to Libraries Folder**
   - Copy `MinimalUltrasonic` folder to libraries directory
   - Folder should contain `src/`, `examples/`, etc.

4. **Restart IDE**
   - Close and reopen Arduino IDE

## PlatformIO

### Using platformio.ini

Add to your `platformio.ini` file:

```ini
[env:your_board]
platform = atmelavr
board = uno  ; or your board
framework = arduino
lib_deps =
    fermeridamagni/MinimalUltrasonic@^2.0.0
```

### Using PlatformIO Library Manager

1. Open PlatformIO Home
2. Go to Libraries
3. Search for "MinimalUltrasonic"
4. Click "Add to Project"
5. Select your project

### Using CLI

```bash
pio lib install "fermeridamagni/MinimalUltrasonic"
```

## Verification

After installation, verify it works:

### Test Compilation

Create a new sketch:

```cpp
#include <MinimalUltrasonic.h>

MinimalUltrasonic sensor(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distance = sensor.read();
  Serial.println(distance);
  delay(1000);
}
```

Click "Verify" (✓) button. Should compile without errors.

### Check Examples

1. Go to `File` → `Examples` → `MinimalUltrasonic`
2. You should see:
   - UltrasonicSimple
   - Timeout
   - MultipleUltrasonicSensors
   - AllUnits

### Check Library Version

In your sketch:

```cpp
#include <MinimalUltrasonic.h>

void setup() {
  Serial.begin(9600);
  Serial.println("MinimalUltrasonic v2.0.0");
  // If this compiles, you have v2.0+
  MinimalUltrasonic::Unit unit = MinimalUltrasonic::METERS;
}

void loop() {}
```

## Updating

### Arduino IDE

1. **Via Library Manager**
   - Open Library Manager
   - Find MinimalUltrasonic
   - Click "Update" if available

2. **Manual Update**
   - Download new version
   - Delete old folder from libraries directory
   - Install new version

### PlatformIO

Update platformio.ini:

```ini
lib_deps =
    fermeridamagni/MinimalUltrasonic@^2.0.1  ; new version
```

Then run:

```bash
pio lib update
```

## Troubleshooting

### Library Not Found

**Problem:** Arduino can't find the library after installation.

**Solution:**

1. Verify library is in correct folder
2. Check folder name is exactly `MinimalUltrasonic`
3. Restart Arduino IDE
4. Check File → Preferences → Sketchbook location

### Compilation Errors

**Problem:** `MinimalUltrasonic.h: No such file or directory`

**Solution:**

1. Reinstall library
2. Check include statement: `#include <MinimalUltrasonic.h>`
3. Verify library in Sketch → Include Library menu

### Wrong Version

**Problem:** Old version installed.

**Solution:**

1. Delete old version from libraries folder
2. Install new version
3. Restart IDE

### Permission Errors (Linux/Mac)

**Problem:** Can't write to libraries folder.

**Solution:**

```bash
# Fix permissions
chmod -R 755 ~/Arduino/libraries/

# Or install with sudo (not recommended)
sudo cp -r MinimalUltrasonic ~/Arduino/libraries/
```

## Dependencies

MinimalUltrasonic has **zero external dependencies**. It only requires:

- Arduino core library (included with Arduino IDE)
- Standard C++ library

No need to install additional libraries!

## Platform Compatibility

Tested and working on:

- Arduino Uno
- Arduino Mega
- Arduino Nano
- Arduino Pro Mini
- ESP32
- ESP8266
- Teensy 3.x/4.x

Should work on any Arduino-compatible board with digital pins.

## Next Steps

Once installed:

1. **[Getting Started](/guide/getting-started)** - Wire up your sensor
2. **[Basic Usage](/guide/basic-usage)** - Learn the basics
3. **[Examples](/examples/basic)** - See working code

::: tip Quick Test
Load `File` → `Examples` → `MinimalUltrasonic` → `UltrasonicSimple` to quickly test your setup!
:::
