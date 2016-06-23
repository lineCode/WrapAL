#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstdio>
#include "AudioEngine.h"
#include "AudioHandle.h"
#include <cmath>

// create normal sine wave
auto create_sine_wave(float frequency) {
    // setup the format
    WrapAL::AudioFormat format;
    // CD
    format.nSamplesPerSec = 44100;
    // a block = a float var
    format.nBlockAlign = sizeof(float);
    // one channel
    format.nChannels = 1;
    // single float
    format.nFormatTag = WrapAL::Wave_IEEEFloat;
    // get length of buffer
    uint32_t countlen = format.nSamplesPerSec * format.nBlockAlign ;
    auto bytelen = countlen * sizeof(float);
    // make & create the clip
    return WrapAL::CreateAudioClip(format, [=](uint8_t* buffer) noexcept {
        constexpr float pi = float(3.1415926536);
        auto float_buffer = reinterpret_cast<float*>(buffer);
        for (auto i = 0u; i < countlen; ++i) {
            float time = float(i) / float(countlen);
            float_buffer[i] = std::sin(time * 2.f * pi * frequency);
        }
    }, bytelen, WrapAL::Flag_LoopInfinite);
}

// App Entrance
int main() {
    // Initialize COM Interface and AudioEngine
    ::CoInitialize(nullptr);
    AudioEngine.Initialize();
    {
        auto clip = create_sine_wave(500.f);
        // play the clip
        clip.Play();
        // until get char
        std::getchar();
        // destroy clip manually(optional)
        // clip.Destroy();
    }
    // Uninitialize
    AudioEngine.Uninitialize();
    ::CoUninitialize();
    return EXIT_SUCCESS;
}

