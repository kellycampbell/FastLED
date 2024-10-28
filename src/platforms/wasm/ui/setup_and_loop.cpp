#ifdef __EMSCRIPTEN__


#include <emscripten.h>
#include <emscripten/emscripten.h> // Include Emscripten headers
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "events.h"
#include "engine_events.h"
#include "platforms/wasm/fs_wasm.h"


extern void setup();
extern void loop();

// Frame time for 60 fps.
#define SIXTY_FPS 16

inline void setup_once() {
    static bool g_setup_called = false;
    if (g_setup_called) {
        return;
    }
    EngineListener::Init();
    g_setup_called = true;
    setup();
}



EMSCRIPTEN_KEEPALIVE extern "C" void fastled_inject_files(std::string jsonStr) {
    ArduinoJson::JsonDocument doc;
    ArduinoJson::deserializeJson(doc, jsonStr);
    auto files = doc["files"];
    if (files.isNull()) {
        return;
    }
    auto files_array = files.as<ArduinoJson::JsonArray>();
    if (files_array.isNull()) {
        return;
    }

    for (auto file : files_array) {
        auto size_obj = file["size"];
        if (size_obj.isNull()) {
            continue;
        }
        auto size = size_obj.as<int>();
        auto path_obj = file["path"];
        if (path_obj.isNull()) {
            continue;
        }
        printf("Declaring file %s with size %d. These will become available as File system paths within the app.\n", path_obj.as<const char*>(), size);
        jsDeclareFile(path_obj.as<const char*>(), size);
    }
}


EMSCRIPTEN_BINDINGS(_fastled_inject_files) {
    emscripten::function("_fastled_inject_files", &fastled_inject_files);
};

//////////////////////////////////////////////////////////////////////////
// BEGIN EMSCRIPTEN EXPORTS
EMSCRIPTEN_KEEPALIVE extern "C" int extern_setup() {
    setup_once();
    return 0;
}

EMSCRIPTEN_KEEPALIVE extern "C" int extern_loop() {

    setup_once();
    //fastled_resume_timer();
    EngineEvents::onPlatformPreLoop();
    loop();
    //fastled_pause_timer();
    return 0;
}


void interval_loop(void* userData) {
    extern_loop();
}


EMSCRIPTEN_KEEPALIVE extern "C" void async_start_loop() {
  // Receives a function to call and some user data to provide it.
  //emscripten_request_animation_frame_loop(on_request_animation_frame_loop, 0);
  emscripten_set_interval(interval_loop, SIXTY_FPS, nullptr);
}

#endif  // __EMSCRIPTEN__
