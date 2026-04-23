#include <FastLED.h>

#define BAUD_RATE 115200 // serial debug port baud rate

uint16_t colorWheel(uint8_t pos);
void drawText(int colorWheelOffset, char *str, int x, int y);
void testCard();
void clockFaceOuter();
void clockFaceInner();
void clockHand(uint16_t innerRadius, uint16_t outerRadius, float angle, uint16_t colour);
