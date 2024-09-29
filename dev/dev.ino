

#include <FastLED.h>
#include <iostream>

// How many leds in your strip?
#define NUM_LEDS 10

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];

// Time scaling factors for each component
#define TIME_FACTOR_HUE 60
#define TIME_FACTOR_SAT 100
#define TIME_FACTOR_VAL 100

#define DELAY 200
#define BRIGHNESS 8

// #define COLOR_ORDER_TEST
// #define TIMING_TEST

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812, DATA_PIN, BRG>(leds, NUM_LEDS).setRgbw(RgbwDefault());
    FastLED.setBrightness(BRIGHNESS);  // Set global brightness to 50%
    delay(2000);  // If something ever goes wrong this delay will allow upload.
}

void fill(CRGB color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
    }
}

void Blink(CRGB color, int times) {
    for (int i = 0; i < times; i++) {
        fill(color);
        FastLED.show();
        delay(DELAY);
        fill(CRGB::Black);
        FastLED.show();
        delay(DELAY);
    }
    delay(DELAY*2);
}




void blink_loop() {
    Blink(CRGB::Red, 1);
    Blink(CRGB::Green, 2);
    Blink(CRGB::Blue, 3);
    Blink(CRGB::White, 4);
    delay(DELAY);
    // long delay to make the cycle visible
    delay(DELAY * 4);
}

void timing_loop() {
    // Tests how long it takes to set all leds to a single color and draw them
    FastLED.setBrightness(8);
    fill(CRGB::White);
    FastLED.show();
    uint32_t us = micros();
    // Serial.printf("Time to set all leds to white: %lu\n", millis() - ms);
    std::cout << "Microseconds to set all leds to white: " << micros() - us << std::endl;
    delay(250);  // now allow the leds to draw so that the time can be measured.
}

void hue_loop() {
    uint32_t ms = millis();
    
    for(int i = 0; i < NUM_LEDS; i++) {
        // Use different noise functions for each LED and each color component
        uint8_t hue = inoise16(ms * TIME_FACTOR_HUE, i * 1000, 0) >> 8;
        uint8_t sat = inoise16(ms * TIME_FACTOR_SAT, i * 2000, 1000) >> 8;
        uint8_t val = inoise16(ms * TIME_FACTOR_VAL, i * 3000, 2000) >> 8;
        
        // Map the noise to full range for saturation and value
        sat = map(sat, 0, 255, 30, 255);
        val = map(val, 0, 255, 100, 255);
        
        leds[i] = CHSV(hue, sat, val);
    }

    FastLED.show();
}

void loop() {
    #ifdef COLOR_ORDER_TEST
    blink_loop();
    #elif defined(TIMING_TEST)
    timing_loop();
    #else
    hue_loop();
    #endif
}
