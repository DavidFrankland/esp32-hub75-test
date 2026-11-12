#include <Arduino.h>
#include <xtensa/core-macros.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include "main.h"
#include "config.h"

// HUB75E pinout
// R1 | G1
// B1 | GND
// R2 | G2
// B2 | E
//  A | B
//  C | D
// CLK| LAT
// OE | GND

// Configure for your panel(s) as appropriate!
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 32
#define PANELS_NUMBER 1 // Number of chained panels, if just a single panel, obviously set to 1
#define PANE_WIDTH PANEL_WIDTH *PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT
#define NUM_LEDS PANE_WIDTH *PANE_HEIGHT

MatrixPanel_I2S_DMA *matrix = nullptr;

const uint16_t centreX = PANE_WIDTH / 2;
const uint16_t centreY = PANE_HEIGHT / 2;

const uint16_t black = matrix->color565(0, 0, 0);
const uint16_t red = matrix->color565(255, 0, 0);
const uint16_t green = matrix->color565(0, 255, 0);
const uint16_t yellow = matrix->color565(255, 255, 0);
const uint16_t blue = matrix->color565(0, 0, 255);
const uint16_t magenta = matrix->color565(255, 0, 255);
const uint16_t cyan = matrix->color565(0, 255, 255);
const uint16_t white = matrix->color565(255, 255, 255);

// serial debug port baud rate
const unsigned int BAUD_RATE = 115200;

void setup()
{
  Serial.begin(BAUD_RATE);

  HUB75_I2S_CFG::i2s_pins pins = {HUB75_R1_PIN, HUB75_G1_PIN, HUB75_B1_PIN, HUB75_R2_PIN, HUB75_G2_PIN, HUB75_B2_PIN,
                                  HUB75_A_PIN, HUB75_B_PIN, HUB75_C_PIN, HUB75_D_PIN, HUB75_E_PIN,
                                  HUB75_LAT_PIN, HUB75_OE_PIN, HUB75_CLK_PIN};
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER, pins);
  mxconfig.driver = HUB75_I2S_CFG::FM6124;
  mxconfig.clkphase = false;
  mxconfig.double_buff = true;

  matrix = new MatrixPanel_I2S_DMA(mxconfig);
  matrix->begin();
  matrix->setBrightness8(100);
}

void loop()
{
  testCard();
  matrix->flipDMABuffer();
  delay(1000);
  matrix->clearScreen();
  matrix->drawRect(0, 0, PANE_WIDTH, PANE_HEIGHT, white);
  matrix->flipDMABuffer();
  delay(1000);
}

void testCard()
{
  int barWidth = PANE_WIDTH / 8;
  uint16_t barColours[] = {black, blue, red, magenta, green, cyan, yellow, white};
  for (int colIndex = 0; colIndex < 8; colIndex++)
  {
    uint16_t barColour = barColours[colIndex];
    for (int x = 0; x < barWidth; x++)
    {
      matrix->drawFastVLine(colIndex * barWidth + x, 0, PANE_HEIGHT, barColour);
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
