# Metal Detector - Conveyor Belt Length Measurement

This project measures the length of metal objects on a conveyor belt using 6 infrared sensors.

## Hardware Requirements

- Arduino UNO (or compatible)
- 6x Infrared Sensors (analog input)
- Conveyor Belt

## Pin Configuration

| Sensor | Arduino Pin | Position |
|--------|------------|----------|
| 1 | A0 | 0 mm |
| 2 | A1 | 50 mm |
| 3 | A2 | 100 mm |
| 4 | A3 | 150 mm |
| 5 | A4 | 200 mm |
| 6 | A5 | 250 mm |

## Installation

### Prerequisites
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)

### Setup

1. Open the project folder in VS Code
2. PlatformIO will automatically detect `platformio.ini`
3. Adjust the following in `platformio.ini`:
   - `upload_port`: Your Arduino COM port (e.g., COM3, /dev/ttyUSB0)
   - `monitor_port`: Same as upload_port
   - `board`: Change if using different Arduino model

## Usage

### Building
```bash
PlatformIO: Build (Ctrl+Alt+B)
```

### Uploading to Arduino
```bash
PlatformIO: Upload (Ctrl+Alt+U)
```

### Serial Monitor
```bash
PlatformIO: Monitor (Ctrl+Alt+M)
```

## Commands

- `c` or `C`: Recalibrate all sensors
- `r` or `R`: Reset system

## Output

The system outputs only the measured metal length:
```
Length: 45.8 mm
```

## Configuration

Edit `src/main.cpp` to adjust:

- **DETECTION_THRESHOLD**: Sensor sensitivity (default: 100)
- **CONVEYOR_SPEED**: mm per millisecond (adjust based on actual belt speed)
- **UPDATE_INTERVAL**: Polling interval in milliseconds (default: 50ms)
- **Sensor positions**: Change the position values in `setSensorPin()` calls

## Calibration

1. Power on the system
2. Send `c` command via serial monitor
3. System will read baseline values from all sensors
4. Ready to measure

## Troubleshooting

- **No measurements**: Check sensor connections and calibration
- **Inaccurate length**: Adjust `DETECTION_THRESHOLD` or sensor positions
- **Serial issues**: Verify COM port in platformio.ini matches your device
