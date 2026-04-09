# Temp-Control-Mattress

ESP32 firmware for a water-based temperature control system that can cool or heat a mattress loop using Peltier modules, pumps, fans, temperature sensors, and pressure sensors.

The project is built with PlatformIO on top of the Arduino framework and runs multiple FreeRTOS tasks for sensing, control, and actuation.

## What This Project Does

- Reads 4x DS18B20 temperature sensors (OneWire bus)
- Reads 4x HX710B pressure sensors
- Controls:
  - 4x Peltier modules through H-bridge outputs (cooling/heating/off direction control)
  - Heat exchanger fans via PWM MOSFET
  - Mattress pump via PWM MOSFET
  - Recirculation pump via ON/OFF MOSFET
- Applies a fuzzy temperature controller for Peltier power level selection
- Applies protection logic for:
  - Hot-side overheating
  - Condensation risk (too cold water)
  - Missing heat-exchanger temperature sensor
- Logs all task events through a centralized debug queue

## Tech Stack

- MCU: ESP32 (`esp32dev`)
- Framework: Arduino + FreeRTOS (ESP32 core)
- Build system: PlatformIO
- Libraries:
  - `OneWire`
  - `DallasTemperature`
  - `HX710B`
  - `eFLL` (fuzzy logic)

## Repository Structure

- `src/main.cpp` - startup, initialization, and task creation
- `src/SharedMemory/` - shared system state + mutex-protected access helpers
- `src/Constants/` - global constants, enums, thresholds, task IDs
- `src/Tasks/IO/` - sensor reading and actuator output drivers
- `src/Tasks/Controllers/` - temperature and fan control logic
- `src/Tasks/Debug/` - async debug message queue and serial print task
- `src/Tasks/Test/` - test helper task (currently optional/not enabled by default)
- `platformio.ini` - board/framework/libs setup
- `diagram.json` + `wokwi.toml` - Wokwi simulation configuration

## Runtime Architecture

The firmware uses one shared `ControllerParameters` structure protected by mutexes.

Main tasks started in `setup()`:

- `print_task` - serial logger for all debug queue messages
- `temperatureSensorsTask` - reads DS18B20 temperatures each second
- `pressureSensorsTask` - reads HX710B pressures every 2 seconds
- `powerMosfetsTask` - writes fan/pump PWM and recirculation ON/OFF outputs
- `hBridgeTask` - applies desired Peltier states with anti-fast-cycling transition logic
- `tempCtrlTask` - emergency checks + fuzzy temperature control (every 10 cycles)
- `fanCtrlTask` - fan command from feed-forward and proportional strategies

## Hardware Mapping (GPIO)

From `src/Tasks/IO/GPIO_Definition.h`:

- DS18B20 bus: `GPIO25`
- Pressure sensors:
  - Sensor 1: `SCK GPIO23`, `DT GPIO22`
  - Sensor 2: `SCK GPIO21`, `DT GPIO19`
  - Sensor 3: `SCK GPIO18`, `DT GPIO5`
  - Sensor 4: `SCK GPIO17`, `DT GPIO16`
- Peltier H-bridges:
  - Cell 1: `FWD GPIO4`, `REV GPIO12`
  - Cell 2: `FWD GPIO13`, `REV GPIO14`
  - Cell 3: `FWD GPIO15`, `REV GPIO26`
  - Cell 4: `FWD GPIO27`, `REV GPIO32`
- MOSFET outputs:
  - Recirculation pump: `GPIO2`
  - Mattress pump: `GPIO33`
  - Peltier fans: `GPIO0`

Important ESP32 boot-pin notes already documented in code:

- `GPIO12` is a strapping pin (can break boot if pulled high)
- `GPIO0` is a boot mode pin (must stay low for flash mode)

Use proper external circuitry to avoid boot instability.

## Getting Started

### 1) Prerequisites

- [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html) installed
- USB connection to an ESP32 dev board (for real hardware)

### 2) Install dependencies

Dependencies are declared in `platformio.ini` and downloaded automatically on first build.

### 3) Build

```bash
pio run
```

### 4) Flash firmware

```bash
pio run -t upload
```

### 5) Open serial monitor

```bash
pio device monitor -b 115200
```

## Wokwi Simulation

This project includes Wokwi files:

- `diagram.json` - virtual hardware wiring (ESP32 + DS18B20 + HX711-based pressure simulation + LED indicators)
- `wokwi.toml` - points to PlatformIO build artifact

Typical flow:

1. Build firmware with `pio run`
2. Start Wokwi simulation from your Wokwi/IDE integration using project files
3. Observe serial logs and virtual peripheral behavior

Note: `wokwi.toml` currently points both `firmware` and `elf` to `firmware.bin`.

## Control Behavior Overview

- Temperature controller computes error from:
  - `setpoint` (`setpoints.mattressTemperature_f`)
  - measured tank temperature (`sensors.tankWaterTemp_f`)
- Fuzzy output is mapped to one of discrete levels:
  `-100, -75, -50, -25, 0, 25, 50, 75, 100`
- Discrete level maps to 4-stage heating/cooling Peltier activation
- Fan controller computes candidate PWM from:
  - feed-forward (based on active Peltier level)
  - hot-side vs water `deltaT`
  - heat-sink temperature table (heating/cooling mode aware)
- Final fan PWM = max of candidate strategies

## Safety and Protection Logic

- Overheat warning threshold: `70 C`
- Overheat shutdown threshold: `80 C`
- Condensation threshold: `5 C` (water too cold)
- If emergency flags are active, Peltier command is forced OFF
- H-bridge task includes an intermediate disabling state to reduce rapid on/off direction toggling

See thresholds in `src/Constants/constants.h`.

## Current Development Status

This repository is actively in development. Based on the current code:

- Core task architecture and drivers are present and integrated
- Wokwi simulation files are included
- No dedicated API/UI/config interface yet for runtime setpoint management
- Unit tests are not yet implemented in `test/`

## Suggested First Improvements

- Add a setpoint interface (serial command, BLE, Wi-Fi, or persistent config)
- Add sensor calibration/scaling for pressure readings
- Add CI build checks (`pio run`) and unit tests
- Add hardware safety interlocks for pump flow and pressure fault handling

## Troubleshooting

- Build fails because `pio` is missing:
  - install PlatformIO Core and ensure `pio` is in your PATH
- Board does not boot:
  - review external pull-ups/pull-downs on `GPIO0` and `GPIO12`
- Temperature readings show disconnected:
  - verify OneWire bus pull-up, addressing, and wiring to `GPIO25`
- Pressure readings undefined:
  - verify HX710B power, DT/SCK wiring, and sensor initialization logs

## License

No license file is currently present in the repository.
If this project is intended to be open source, add a `LICENSE` file (for example MIT, Apache-2.0, or GPL).

## Follow-Up Items To Address

The items below are intentionally listed as follow-up checks for future maintenance. They are not fixed in this change.

- `src/Tasks/Controllers/FanCtrl/fanCtrl.cpp`
  - `checkEmergencyConditions()` defines `l_returnValue_u16` but does not reliably return it in all paths. This should be reviewed to avoid undefined behavior.
- `src/Tasks/IO/PWM_Mosfets/PWM_Mosfets.cpp`
  - In fan and mattress pump handling, bounds checks compare `pwmValue` before it is assigned from `pwmPercent`. Recheck the clamp logic order.
- `src/Tasks/Controllers/TempCtrl/tempCtrl.cpp`
  - Setpoint reads for `setpoints.mattressTemperature_f` appear to use `tankWaterTemp_mutex` instead of the setpoint mutex. Confirm intended mutex usage.
- `src/Tasks/IO/DS18B20_TemperatureSensors/DS18B20_TemperatureSensors.cpp`
  - `returnDeviceAddressFromName()` sets `deviceAddress_p = NULL` locally when not found, which does not propagate to the caller. Review the API contract.
- `src/main.cpp` and controller flow
  - `setpoints.mattressTemperature_f` starts as `UNDEFINED_FLOAT`, and there is no runtime interface yet to set it (serial/BLE/Wi-Fi/config). Add a setpoint input path before production use.
