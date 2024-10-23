/// @file    NoisePlusPalette.ino
/// @brief   Demonstrates how to mix noise generation with color palettes on a
/// 2D LED matrix
/// @example NoisePlusPalette.ino

// printf
#include <stdio.h>
#include <string>
#include <vector>

#include <FastLED.h>
#include "third_party/arduinojson/json.h"
#include "slice.h"
#include "screenmap.h"
#include "math_macros.h"



#define LED_PIN 3
#define BRIGHTNESS 96
#define COLOR_ORDER GRB
#define NUM_LEDS 256

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];


void make_map(int stepx, int stepy, int num, std::vector<pair_xy16>* _map) {
    int16_t x = 0;
    int16_t y = 0;
    std::vector<pair_xy16>& map = *_map;
    for (int16_t i = 0; i < num; i++) {
        map.push_back(pair_xy16{x, y});
        x += stepx;
        y += stepy;
    }
}

void setup() {
    for (CRGB& c : leds) {
        c = CRGB::Blue;
    }
    for (CRGB& c : leds2) {
        c = CRGB::Red;
    }
    FastLED.setBrightness(255);
    std::vector<pair_xy16> map;
    make_map(1, 1, NUM_LEDS, &map);
    ScreenMap screenmap = ScreenMap(map.data(), map.size());

    std::vector<pair_xy16> map2;
    make_map(-1, -1, NUM_LEDS, &map2);
    ScreenMap screenmap2 = ScreenMap(map2.data(), map2.size());

    FastLED.addLeds<WS2811, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCanvasUi(screenmap);

    FastLED.addLeds<WS2811, LED_PIN, COLOR_ORDER>(leds2, NUM_LEDS)
        .setCanvasUi(screenmap2);
}

void loop() {
    FastLED.show();
}

