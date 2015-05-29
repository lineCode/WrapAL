#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstdio>

#include "wrapal.h"

// App Entrance
int main() {
    // Initialize COM Interface and AudioEngine
    if (SUCCEEDED(::CoInitialize(nullptr)) && SUCCEEDED(AudioEngine.Initialize())) {
        // create audio clip
        auto clip = WrapAL::CreateAudioClip(
            WrapAL::AudioFormat::Format_OggVorbis,
            L"NationalAnthemOfRussia.ogg",
            WrapAL::AudioClipFlag(WrapAL::Flag_StreamingReading | WrapAL::Flag_LoopInfinite)
            );
        // play the clip
        clip.Play();
        float ratio = 1.f;
        while (::scanf("%f", &ratio) != EOF) { 
            clip.Ratio(ratio); 
        }
        // destroy clip(optional)
        //clip.Destroy();

        // UnInitialize
        AudioEngine.UnInitialize(); CoUninitialize();
    }
    return EXIT_SUCCESS;
}

