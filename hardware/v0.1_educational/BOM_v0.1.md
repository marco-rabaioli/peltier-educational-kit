## Bill of Materials – Version v0.1


| RefDes             | Component            | Value / Part Number | Qty | Notes                                     |
|-------:            |----------------------|---------------------|-----|-------------------------------------------|
| R1 R4              | Resistor             | 10 Ω 1/4W           | 2   | MOS gate resistor                         |
| R2 R3 R6 R7 R8     | Resistor             | 470 Ω 1/4W          | 5   | LED and BJT base resistor                 |
| R5                 | Resistor             | 4.7 kΩ 1/4W         | 1   | Pull-up resistor                          |
| Q1 Q3              | NPN Transistor       | BC547               | 2   | Relè and FAN driver                       |
| Q2 Q4              | N-MOSFET             | IRFZ44N             | 2   | Peltier and resistor driver               |
| D1                 | Diode                | 1N4007              | 1   | Flyback diode                             |
| LED1 LED2 LED3     | Led diode            | Led 5mm             | 3   | Indicator leds                            |
| C1 C2              | REMOVED              | REMOVED             | 0   | REMOVED                                   |
| C3                 | Capacitor            | 4.7uF 50V           | 1   | Voltage stabilizer                        |
| R1                 | Relay                | RTE24012            | 1   | H-Bridge for Heating/Cooling inversion    |


| RefDes | Component            | Value / Part Number | Package | Qty | Notes |
|-------:|----------------------|---------------------|---------|-----|-------|
| R1     | Resistor             | 10 Ω                | 1206    | 1   | Power resistor |
| R2     | Resistor             | 10 kΩ               | 0603    | 2   | Pull-up |
| C1     | Capacitor            | 100 nF              | 0603    | 2   | Decoupling |
| U1     | Microcontroller      | ATmega328P          | TQFP-32 | 1   | Arduino-compatible |
| Q1     | N-MOSFET             | IRLZ44N             | TO-220 | 1   | Peltier driver |
