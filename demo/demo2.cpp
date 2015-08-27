#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstdio>

#include "wrapal.h"

// App Entrance
int main() {
    // my config
    class MyConfig : public WrapAL::CALDefConfigure {
    public:
        // get the "libmpg123.dll" path on windows, now, we don't need it
        auto GetLibmpg123Path(wchar_t path[/*MAX_PATH*/]) noexcept ->void override {
            path[0] = wchar_t(0);
        }
    } config;
    // Initialize COM Interface
    if (SUCCEEDED(::CoInitialize(nullptr))) {
        // Initialize WrapAL WrapALAudioEngine
        if (SUCCEEDED(AudioEngine.Initialize(&config))) {
            // create audio clip
            auto clip = WrapAL::CreateAudioClip(
                WrapAL::AudioFormat::Format_OggVorbis,
                //L"NationalAnthemOfRussia.ogg",
                L"Sakura.ogg",
                WrapAL::Flag_StreamingReading | WrapAL::Flag_LoopInfinite,
                "BGM"
                );
            auto dur = clip.Duration();
            auto name = clip.GetGroup().Name();
            // play the clip
            clip.Play();
            float ratio = 1.f;
            while (::scanf("%f", &ratio) != EOF) {
                clip.Ratio(ratio);
            }
            // destroy clip(optional)
            //clip.Destroy();

            // Uninitialize WrapAL WrapALAudioEngine
            AudioEngine.UnInitialize();
        }
        // Uninitialize the COM Interface
        ::CoUninitialize();
    }
    return EXIT_SUCCESS;
}

