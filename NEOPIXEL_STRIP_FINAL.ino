#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 7

// Create a NeoPixel strip object
// Parameters: 
// 1 = number of pixels in strip
// 2 = Arduino pin number
// 3 = pixel type flags, combine as needed:
//    NEO_KHZ800  800 KHz bitstream (most WS2812 LEDs)
//    NEO_KHZ400  400 KHz (classic 'v1' FLORA pixels, WS2811 drivers)
//    NEO_GRB     Pixels wired for GRB (most NeoPixel stuff)
//    NEO_RGB     Pixels wired for RGB (v1 FLORA pixels)
//    NEO_RGBW    Pixels wired for RGBW (NeoPixel RGBW)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Trinket 5V 16MHz setup, can be removed if not using Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of Trinket special code

  strip.begin();
  strip.setBrightness(50);  // set brightness (0-255)
  strip.show();             // initialise all pixels to off
}

void loop() {
  // Simple colour wipes
  colorWipe(strip.Color(255, 0, 0), 50);   // Red
  colorWipe(strip.Color(0, 255, 0), 50);   // Green
  colorWipe(strip.Color(0, 0, 255), 50);   // Blue
  colorWipe(strip.Color(255, 127, 0), 50); // Orange
  colorWipe(strip.Color(128, 0, 128), 50); // Purple

  // Theatre chase effects
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50);     // Red
  theaterChase(strip.Color(0, 255, 0), 50);     // Green
  theaterChase(strip.Color(0, 0, 255), 50);     // Blue
  theaterChase(strip.Color(255, 127, 0), 50);   // Orange
  theaterChase(strip.Color(128, 0, 128), 50);   // Purple
  theaterChaseRainbow(50);

  // Rainbow effects
  rainbow(20);
  rainbowCycle(20);
}

// Fill pixels one by one with a colour
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait); // use wait for delay
  }
}

// Standard rainbow across all pixels
void rainbow(uint8_t wait) {
  for(uint16_t j = 0; j < 256; j++) {
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Rainbow distributed evenly across strip
void rainbowCycle(uint8_t wait) {
  for(uint16_t j = 0; j < 256*5; j++) { // 5 cycles
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Theatre-style crawling lights
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) {  // do 10 cycles
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, c); // turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, 0); // turn every third off
      }
    }
  }
}

// Theatre chase with rainbow
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {     // cycle colours
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, Wheel((i + j) % 255));
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, 0);
      }
    }
  }
}

// Input 0-255 to get colour value.
// Colours transition rgb-back to r
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}