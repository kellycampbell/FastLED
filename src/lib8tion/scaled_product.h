
#include <stdint.h>
#include "namespace.h"
#include "lib8static.h"

FASTLED_NAMESPACE_BEGIN


LIB8STATIC_ALWAYS_INLINE uint16_t map8_to_16(uint8_t x) {
    return uint16_t(x) * 257;
}

// Has the property that quadradic_norm8(0xff, 0xff) == 0xffff
LIB8STATIC_ALWAYS_INLINE uint16_t scaled_product8_8_16(uint8_t a, uint8_t b) {
    if (!a || !b) {
        return 0;
    }
    if (a == 0xff) {
        return map8_to_16(b);
    }
    if (b == 0xff) {
        return map8_to_16(a);
    }
    uint16_t out = static_cast<uint16_t>(a) + 1;
    out *= static_cast<uint16_t>(b) + 1;
    out -= 1;
    return out;
}

// Has the property that quadradic_norm8(0xff, 0xff) == 0xff
LIB8STATIC_ALWAYS_INLINE uint8_t scaled_product8_8_8(uint8_t a, uint8_t b) {
    uint16_t out = scaled_product8_8_16(a, b);
    return out >> 8;
}

FASTLED_NAMESPACE_END
