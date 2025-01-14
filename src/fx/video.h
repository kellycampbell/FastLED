#pragma once

#include <stdint.h>

#include "namespace.h"
#include "fl/ptr.h"
#include "fx/fx1d.h"
#include "fx/time.h"
#include "fl/str.h"


FASTLED_NAMESPACE_BEGIN
struct CRGB;
FASTLED_NAMESPACE_END

namespace fl {

// Forward declare classes
FASTLED_SMART_PTR(FileHandle);
FASTLED_SMART_PTR(ByteStream);
FASTLED_SMART_PTR(Frame);
FASTLED_SMART_PTR(VideoImpl);
FASTLED_SMART_PTR(VideoFxWrapper);
FASTLED_SMART_PTR(ByteStreamMemory);


// Video represents a video file that can be played back on a LED strip.
// The video file is expected to be a sequence of frames. You can either use
// a file handle or a byte stream to read the video data.
class Video : public Fx1d {
public:
    static size_t DefaultFrameHistoryCount() {
        #ifdef __AVR__
        return 1;
        #else
        return 2;  // Allow interpolation by default.
        #endif
    }
    // frameHistoryCount is the number of frames to keep in the buffer after draw. This
    // allows for time based effects like syncing video speed to audio triggers.
    Video(size_t pixelsPerFrame, float fps = 30.0f, size_t frameHistoryCount = DefaultFrameHistoryCount());  // Please use FileSytem to construct a Video.
    ~Video();
    Video(const Video&);
    Video& operator=(const Video&);

    // Fx Api
    void draw(DrawContext context) override;
    Str fxName() const override;

    // Api
    bool begin(fl::FileHandlePtr h);
    bool beginStream(fl::ByteStreamPtr s);
    bool draw(uint32_t now, CRGB* leds);
    bool draw(uint32_t now, Frame* frame);
    void end();
    bool finished();
    bool rewind();
    void setTimeScale(float timeScale);
    float timeScale() const;
    Str error() const;
    void setError(const Str& error) { mError = error; }
    size_t pixelsPerFrame() const;
    void pause(uint32_t now) override;
    void resume(uint32_t now) override;

    // make compatible with if statements
    operator bool() const { return mImpl.get(); }
private:
    bool mFinished = false;
    VideoImplPtr mImpl;
    Str mError;
    Str mName;
};



// Wraps an Fx and stores a history of video frames. This allows
// interpolation between frames.
class VideoFxWrapper : public Fx1d {
  public:
    VideoFxWrapper(FxPtr fx);
    ~VideoFxWrapper() override;
    void draw(DrawContext context) override;
    Str fxName() const override;

  private:
    FxPtr mFx;
    VideoImplPtr mVideo;
    ByteStreamMemoryPtr mByteStream;
    float mFps = 30.0f;
};


}  // namespace fl

