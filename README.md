# Testing ESP32 with HUB75 LED panel

Sample code to check you've wired up your ESP32 and HUB75 LED panel correctly.

Displays a "test card" to check colours are wired up correctly.

Also has the beginnings of a clock, but so far only show a second hand.

## How to setup

### Hardware

Connect the HUB75 panel directly to the ESP32. You'll need 16 "Dupont" (or similar) wires.

The HUB75 panel needs its own beefy power supply, at least 4A for full brightness white with all LEDs on (64x64 LED panel). Use the (usually included) red/black cable for this. A smaller power supply is OK for testing, just don't turn all the LEDs on at once.

This one should be sufficient: [Amazon link](https://www.amazon.co.uk/dp/B07PPPF1R5). 

I have this one: [Amazon link](https://www.amazon.co.uk/dp/B07PQT2Q7L), but it's way overkill for just one panel.

The sample code also includes a power supply tester, which gradually lights up all the LEDs white to check if the power supply can cope. If they start to turn pink, your power supply is not supplying enough current.

Wire the ESP32 to panel as follows:

| HUB75 pin | Purpose | ESP32   |
| --------- | ------- | ------- |
| 1         | R1      | GPIO 25 |
| 2         | G1      | GPIO 26 |
| 3         | B1      | GPIO 27 |
| 4         | GND     | GND     |
| 5         | R2      | GPIO 14 |
| 6         | G2      | GPIO 12 |
| 7         | B2      | GPIO 13 |
| 8         | E       | GPIO 18 |
| 9         | A       | GPIO 23 |
| 10        | B       | GPIO 19 |
| 11        | C       | GPIO 5  |
| 12        | D       | GPIO 17 |
| 13        | CLK     | GPIO 16 |
| 14        | LAT     | GPIO 4  |
| 15        | OE      | GPIO 15 |
| 16        | GND     | GND     |

### Software

The project uses [PlatformIO](https://platformio.org/). I use Visual Studio Code (VS Code) with the PlatformIO extension.

- Clone the Git repository (or download as a zip and unzip)
- Open the folder in VS Code, it should be recognised as a PlatformIO project
- Amend [platformio.ini](platformio.ini) if required
- Build the project (click ![PlatformIO: Build](img/build.png) on the toolbar or `Ctrl` + `Alt` + `B`)

The first build will take a couple of minutes as all the required libraries are downloaded and compiled. Subsequent build will only take a few seconds.

If all goes well, upload to youe ESP32 (click ![PlatformIO: Upload](img/upload.png) on the toolbar or `Ctrl` + `Alt` + `U`).

All the code is contained within [main.cpp](src/main.cpp).