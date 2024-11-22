#pragma once

#include <stdint.h>

#include "crgb.h"
#include "fixed_vector.h"
#include "namespace.h"
#include "singleton.h"
#include "pixelset.h"
#include "scoped_ptr.h"

FASTLED_NAMESPACE_BEGIN


// Note that this is a work in progress. The API will change and things right
// now are overly strict and hard to compile on purpose as the driver is undergoing
// development. I don't want to be dealing with memory errors because of all the
// the raw pointers so types are strict and the API is strict. This will change
// in the future.

class YvesI2SImpl;


class YvesI2S {
  public:
    // Safe to initialize in static memory because the driver will be instantiated
    // on first call to showPixels().
    YvesI2S(CRGB* leds, const FixedVector<int, 6>& pins, int clock_pin, int latch_pin);

    void initOnce();
    void showPixels();

    // Disable copy constructor and assignment operator
    YvesI2S() = delete;
    YvesI2S(YvesI2S &&) = delete;
    YvesI2S &operator=(const YvesI2S &) = delete;
    ~YvesI2S();

  private:
    scoped_ptr<YvesI2SImpl> mDriver;
    //Pins mPins;
    int mClockPin;
    int mLatchPin;
    CRGB* mLeds;
    FixedVector<int, 6> mPins;
};

FASTLED_NAMESPACE_END