#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstdio>
#include "wrapal.h"
#include <cmath>

auto create_biiiiii(float f) {
    WrapAL::AudioFormat format;
    format.nSamplesPerSec = 44100;
    format.nBlockAlign = sizeof(float);
    format.nChannels = 1;
    format.nFormatTag = WrapAL::Wave_IEEEFloat;
    auto length = format.nSamplesPerSec * format.nBlockAlign;
    auto* buffer = reinterpret_cast<uint8_t*>(::malloc(length * sizeof(float)));
    if (!buffer) return WrapAL::CALAudioSourceClip(WrapAL::ALInvalidHandle);
    {
        constexpr float pi = float(3.1415926536);
        auto float_buffer = reinterpret_cast<float*>(buffer);
        for (auto i = 0u; i < length; ++i) {
            float time = float(i) / float(length);
            float_buffer[i] = std::sinf(time * 2.f * pi * f);
        }
        float_buffer = nullptr;
    }
    auto clip = WrapAL::CreateAudioClip(
        format, std::move(buffer), length, WrapAL::Flag_LoopInfinite
        );
    return clip;
}

// App Entrance
int main() {
    // Initialize COM Interface and AudioEngine
    ::CoInitialize(nullptr);
    AudioEngine.Initialize();
    {
        // create audio clip
        /*auto clip = WrapAL::CreateAudioClip(
            WrapAL::EncodingFormat::Format_OggVorbis,
            L"NationalAnthemOfRussia.ogg",
            WrapAL::Flag_StreamingReading | WrapAL::Flag_LoopInfinite,
            "BGM"
            );*/
        auto clip = create_biiiiii(500.f);
        // play the clip
        clip.Play();
        // until get char
        std::getchar();
        // destroy clip(optional)
        //clip.Destroy();
    }
    // Uninitialize
    AudioEngine.Uninitialize();
    ::CoUninitialize();
    return EXIT_SUCCESS;
}

