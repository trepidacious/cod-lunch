#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN   15  // Digital IO pin connected to the NeoPixels, labelled D8 on the NodeMCU v2, this should be GPIO 15
#define PIXEL_COUNT 8   // Number of NeoPixels

Servo servo;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  servo.attach(2);
  servo.write(0);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  delay(2000);
}

uint16_t phase = 0;

void rainbow(uint16_t firstPixelHue);

void loop() {


  
  rainbow(phase);             // Flowing rainbow cycle along the whole strip
  phase += 256;
  delay(10);

  long angle = 0;
  if (phase < 32768) {
    angle = phase * 180 / 32768;
  } else {
    angle = (65535 - phase) * 180 / 32768;
  }
  servo.write(angle);

  // servo.write(180);
  // delay(1000);
  // servo.write(90);
  // delay(1000);
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(uint16_t firstPixelHue) {

  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    // Offset pixel hue by an amount to make one full revolution of the
    // color wheel (range of 65536) along the length of the strip
    // (strip.numPixels() steps):
    int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
    // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
    // optionally add saturation and value (brightness) (each 0 to 255).
    // Here we're using just the single-argument hue variant. The result
    // is passed through strip.gamma32() to provide 'truer' colors
    // before assigning to each pixel:
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show(); // Update strip with new contents
  
}
