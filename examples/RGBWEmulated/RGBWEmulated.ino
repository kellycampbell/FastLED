

#define FASTLED_RMT_BUILTIN_DRIVER 0
#define FASTLED_EXPERIMENTAL_ESP32_RGBW_ENABLED 0
#define FASTLED_EXPERIMENTAL_ESP32_RGBW_MODE kRGBWExactColors

#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 12

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 9

// Define the array of leds
CRGB leds[NUM_LEDS];

// Time scaling factors for each component
#define TIME_FACTOR_HUE 60
#define TIME_FACTOR_SAT 100
#define TIME_FACTOR_VAL 100

typedef WS2812<DATA_PIN, RGB> ControllerT;  // RGB mode must be RGB, no re-ordering allowed.
static RGBWEmulatedController<ControllerT, GRB> rgbwEmu(leds, NUM_LEDS);

void setup() {
    Serial.begin(115200);
    //FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());
    FastLED.addLeds(&rgbwEmu, leds, NUM_LEDS);
    FastLED.setBrightness(128);  // Set global brightness to 50%
    delay(2000);  // If something ever goes wrong this delay will allow upload.
}

void loop() {
    uint32_t ms = millis();
    static size_t frame_count = 0;
    int frame_cycle = frame_count % 3;

    CRGB pixel;
    switch (frame_cycle) {
        case 0:
            pixel = CRGB::Red;
            break;
        case 1:
            pixel = CRGB::Green;
            break;
        case 2:
            pixel = CRGB::Blue;
            break;
    }

    for (int i = -1; i < frame_cycle; ++i) {
        for (int j = 0; j < NUM_LEDS; ++j) {
            leds[j] = pixel;
        }
        FastLED.show();
        delay(500);
        for (int j = 0; j < NUM_LEDS; ++j) {
            leds[j] = CRGB::Black;
        }
        FastLED.show();
        delay(500);
    }
}