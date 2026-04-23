#include <Arduino.h>
#include "xtensa/core-macros.h"
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "main.h"

// HUB75E pinout
// R1 | G1
// B1 | GND
// R2 | G2
// B2 | E
//  A | B
//  C | D
// CLK| LAT
// OE | GND

/* Default library pin configuration for the reference. You can redefine only ones you need later on object creation
#define R1 25
#define G1 26
#define BL1 27
#define R2 14
#define G2 12
#define BL2 13
#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 17
#define CH_E -1 // assign to any available pin if using panels with 1/32 scan
#define CLK 16
#define LAT 4
#define OE 15
*/

// Configure for your panel(s) as appropriate!
#define PIN_E 18
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64 // Panel height of 64 will required PIN_E to be defined.
#define PANELS_NUMBER 1 // Number of chained panels, if just a single panel, obviously set to 1
#define PANE_WIDTH PANEL_WIDTH *PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT
#define NUM_LEDS PANE_WIDTH *PANE_HEIGHT

MatrixPanel_I2S_DMA *matrix = nullptr;

const uint16_t centreX = PANE_WIDTH / 2;
const uint16_t centreY = PANE_HEIGHT / 2;

const uint16_t red = matrix->color565(255, 0, 0);
const uint16_t green = matrix->color565(0, 255, 0);
const uint16_t blue = matrix->color565(0, 0, 255);
const uint16_t white = matrix->color565(255, 255, 255);
const uint16_t black = matrix->color565(0, 0, 0);

void setup()
{
  Serial.begin(BAUD_RATE);

  // redefine pins if required
  // HUB75_I2S_CFG::i2s_pins _pins={R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK};
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER);
  mxconfig.gpio.e = PIN_E;
  mxconfig.driver = HUB75_I2S_CFG::FM6124;
  mxconfig.clkphase = false;
  mxconfig.double_buff = true;

  matrix = new MatrixPanel_I2S_DMA(mxconfig);
  matrix->begin();
  matrix->setBrightness8(100);

  testCard();
  matrix->flipDMABuffer();
  delay(5000);
}

void loop()
{
  for (uint16_t sec = 0; sec < 60; sec++)
  {
    matrix->clearScreen();
    clockFaceOuter();
    const uint16_t innerRadius = 0;
    const uint16_t outerRadius = 28;
    clockHand(innerRadius, outerRadius, sec / 60.0, green);
    clockFaceInner();
    matrix->flipDMABuffer();
    delay(1000);
  }
}

void clockFaceOuter()
{
  matrix->fillCircle(centreX, centreY, 31, white);
  matrix->fillCircle(centreX, centreY, 29, black);
}

void clockFaceInner()
{
  matrix->fillCircle(centreX, centreY, 5, white);
}

void clockHand(uint16_t innerRadius, uint16_t outerRadius, float angle, uint16_t colour)
{
  float theta = angle * TWO_PI;
  float sin_theta = sin(theta);
  float cos_theta = cos(theta);
  uint16_t x1 = centreX + innerRadius * sin_theta + 0.5;
  uint16_t y1 = centreY - innerRadius * cos_theta + 0.5;
  uint16_t x2 = centreX + outerRadius * sin_theta + 0.5;
  uint16_t y2 = centreY - outerRadius * cos_theta + 0.5;
  matrix->drawLine(x1, y1, x2, y2, colour);
}

void testCard()
{
  int barWidth = PANE_WIDTH / 8;
  CRGB barColours[] = {CRGB::Black, CRGB::Blue, CRGB::Red, CRGB::Magenta, CRGB::Lime, CRGB::Cyan, CRGB::Yellow, CRGB::White};
  for (int colIndex = 0; colIndex < 8; colIndex++)
  {
    CRGB barColour = barColours[colIndex];
    for (int x = 0; x < barWidth; x++)
    {
      matrix->drawFastVLine(colIndex * barWidth + x, 0, PANE_HEIGHT, barColour.r, barColour.g, barColour.b);
    }
  }
}

// Power supply tester - slowly fills matrix with white, stressing PSU
void powerSupplyTester()
{
  for (int y = 0; y < PANE_HEIGHT; ++y)
  {
    for (int x = 0; x < PANE_WIDTH; ++x)
    {
      matrix->drawPixelRGB888(x, y, 255, 255, 255);
      // matrix->drawPixelRGB888(x, y - 1, 255, 0, 0); // pls, be gentle :)
      delay(10);
    }
  }
}

void drawText(int colorWheelOffset, char *str, int x, int y)
{
  // draw some text
  matrix->setTextSize(2); // size 1 == 8 pixels high
  matrix->setTextWrap(false);
  matrix->setCursor(x, y);
  uint8_t w = 0;
  for (w = 0; w < strlen(str); w++)
  {
    char c = str[w];
    matrix->setTextColor(colorWheel(w * 4 + c * 32 + colorWheelOffset));
    matrix->print(c);
  }
}

uint16_t colorWheel(uint8_t pos)
{
  if (pos < 85)
  {
    return matrix->color565(pos * 3, 255 - pos * 3, 0);
  }
  else if (pos < 170)
  {
    pos -= 85;
    return matrix->color565(255 - pos * 3, 0, pos * 3);
  }
  else
  {
    pos -= 170;
    return matrix->color565(0, pos * 3, 255 - pos * 3);
  }
}
