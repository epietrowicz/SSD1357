/*
  Use the included SSD1357 driver to run a display
  By: Eric Pietrowicz
  Date: 07/01/2017
*/


#define SSD1357_DONT_USE_DEF_FONT         // Defining this variable removes the default font from the code

#include "SparkFun_RGB_OLED_64x64.h"      // Click here to get the library: http://librarymanager/All#SparkFun_RGB_OLED_64x64
#include "font8x16.h"
#include "fontlargenumber.h"   // This file simply holds a bitmap definition of the 8x16 font in program memory

uint8_t FontScratch8x16[8 * 16 * 2];
uint8_t FontLargeNumber[12 * 48 * 2];      // This declares a space for the 8x16 font to work in.
MicroviewMonochromeProgMemBMPFont SSD1357Font8x16(font8x16, FontScratch8x16, 6);   // Construct an object using the given font definition, the font scratch space, and with 6 bytes in the font header area.
MicroviewMonochromeProgMemBMPFont SSD1357FongLarge(fontlargenumber, FontLargeNumber, 6);

#define myOLED_SPI SPI

#define CS_PIN 14
#define DC_PIN 5
#define RST_PIN 12

RGB_OLED_64x64  myOLED; // Declare OLED object

void setup() {

  myOLED_SPI.begin();
  myOLED.begin(DC_PIN, RST_PIN, CS_PIN, myOLED_SPI, 8000000); // Choosing 8 MHz because Uno at 16 MHz can only do HW SPI at F_CPU/2
  configure();
  myOLED.clearDisplay();
  myOLED.setFont(
    &SSD1357FongLarge,
    SSD1357FongLarge.Wrapper_to_call_getBMP,
    SSD1357FongLarge.Wrapper_to_call_getAlpha,
    SSD1357FongLarge.Wrapper_to_call_getFrameData,
    SSD1357FongLarge.Wrapper_to_call_advanceState,
    SSD1357FongLarge.Wrapper_to_call_setCursorValues);

  printWithLargeFontInt(48, 48);
  
}

void loop() {
 
}

void printWithLargeFontInt( char firstChar, char secondChar )
{
  Serial.println("Printing to display with large font");
  myOLED.setFontCursorValues(OLED_64x64_START_COL + 19, OLED_64x64_START_ROW + 8, OLED_64x64_START_COL, OLED_64x64_START_ROW, OLED_64x64_STOP_COL, OLED_64x64_STOP_ROW); //configure to center of display
  myOLED.print(firstChar);
  myOLED.println(secondChar);
}

void configure( void )
{
  // This is the suggested initialization routine for the 64x64 RGB OLED - Change settings as needed for your particular display
  myOLED.setCommandLock(false);
  myOLED.setSleepMode(true);

  // Initial settings configuration
  myOLED.setClockDivider(0xB0);
  myOLED.setMUXRatio(0x3F);
  myOLED.setDisplayOffset(0x80); //64 by 64 so we need to offset the RAM if we're going to rotate the screen 180 degrees. WAS: 0x40 for normal orientation
  myOLED.setDisplayStartLine(0x00);
  //myOLED.setRemapColorDepth(false, false, false, false, false, SSD1357_COLOR_MODE_65k);
  myOLED.setRemapColorDepth(false /*horizontal address*/, false /*col address*/, true /*color seq*/, false /*COM scan*/, true /*COM split*/, SSD1357_COLOR_MODE_65k); //configure the rotation. 
  myOLED.setContrastCurrentABC(0x88, 0x32, 0x88);
  myOLED.setMasterContrastCurrent(0x0F);
  myOLED.setResetPrechargePeriod(0x02, 0x03);

  uint8_t MLUT[63] = {0x02, 0x03, 0x04, 0x05,
                      0x06, 0x07, 0x08, 0x09,
                      0x0A, 0x0B, 0x0C, 0x0D,
                      0x0E, 0x0F, 0x10, 0x11,
                      0x12, 0x13, 0x15, 0x17,
                      0x19, 0x1B, 0x1D, 0x1F,
                      0x21, 0x23, 0x25, 0x27,
                      0x2A, 0x2D, 0x30, 0x33,
                      0x36, 0x39, 0x3C, 0x3F,
                      0x42, 0x45, 0x48, 0x4C,
                      0x50, 0x54, 0x58, 0x5C,
                      0x60, 0x64, 0x68, 0x6C,
                      0x70, 0x74, 0x78, 0x7D,
                      0x82, 0x87, 0x8C, 0x91,
                      0x96, 0x9B, 0xA0, 0xA5,
                      0xAA, 0xAF, 0xB4
                     };
  myOLED.setMLUTGrayscale(MLUT);

  myOLED.setPrechargeVoltage(0x17);
  myOLED.setVCOMH(0x05);
  myOLED.setColumnAddress(0x20, 0x5F);
  myOLED.setRowAddress(0x00, 0x3F);
  myOLED.setDisplayMode(SSD1357_CMD_SDM_RESET);

  myOLED.setSleepMode(false);

  delay(200);

}
