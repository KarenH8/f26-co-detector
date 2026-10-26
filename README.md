# F26 CO Detector

CO detector project using an **Adafruit Circuit Playground Classic**,
an **MQ7 sensor module**, and an external common-cathode **RGB LED**.

The source in [`src/main.cpp`](src/main.cpp) is the original supplied code,
preserved unchanged. It compares raw ADC readings against thresholds of 40
and 100; those readings are not PPM measurements.

## Hardware connections

| Signal | Circuit Playground pad | AVR pin |
| --- | --- | --- |
| Red LED | 10 | PB6 |
| Green LED | 6 | PD7 |
| Blue LED, kept off | 12 | PD6 |
| MQ7 analog input through divider | 9 | PB5 / ADC12 |

The stated voltage divider is 10k from MQ7 A0 to ADC12 and 15k from ADC12 to
ground. The LED channels need appropriate current-limiting resistors and
the components must share a ground.

## Build in VS Code

1. Install the PlatformIO IDE extension.
2. Open this repository folder in VS Code.
3. Select **PlatformIO: Build**.

Or, with PlatformIO Core installed:

```sh
pio run
```

The project targets `circuitplay_classic`, an ATmega32U4 at **8 MHz**, using
`atmelavr@5.3.0` and the Arduino framework. GPIO, Timer1 and ADC configuration
use AVR registers.

## Program behavior

- `SetupIO()` configures all three LED channels and initializes them off.
- `SetupTimer()` uses Timer1 mode 9, TOP 31250 and prescaler 64 for a
  **500-ms overflow period at 8 MHz**.
- `SetupADC()` selects ADC12, AVCC reference, right adjustment,
  divide-by-4 prescaling and automatic triggering from Timer1 overflow.
- The empty Timer1 overflow interrupt handler allows the overflow flag to
  clear on interrupt entry, permitting subsequent trigger edges.
- `loop()` calls `UpdateLED()` and then waits 1000 ms.

| Raw ADC count | LED state |
| --- | --- |
| 0-40 | Green |
| 41-100 | Red + green (yellow) |
| 101-1023 | Red |

Blue remains off. These are raw-reading demonstration thresholds; the code
does not establish calibrated CO concentrations.

## Validation

This unchanged source compiled successfully with PlatformIO for the Circuit
Playground Classic on October 23, 2026. A successful build checks compilation;
it does not validate physical wiring, sensor calibration or CO concentrations.

## References

- [MQ7 tutorial](https://www.makerguides.com/arduino-uno-and-mq7-co-sensor/)
- [ATmega32U4 datasheet](https://ww1.microchip.com/downloads/en/devicedoc/atmel-7766-8-bit-avr-atmega16u4-32u4_datasheet.pdf)
- [PlatformIO Circuit Playground Classic board](https://docs.platformio.org/en/latest/boards/atmelavr/circuitplay_classic.html)
