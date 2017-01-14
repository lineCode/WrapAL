#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <Windows.h>
#include <cstdio>
#include <cwchar>
#include <cstdlib>
#include <clocale>
#include <utility>
#include "AudioHandle.h"
#include "AudioEngine.h"

// App Entrance
int main(int argc, char* argv[]) {
    std::srand(::GetTickCount());
    // my os is in chinese, std::wprintf is in the bad way
    std::setlocale(LC_ALL, "chs");
    // my config
    class MyConfig : public WrapAL::CALDefConfigure {
    public:
        // choose device, return index, if out of range, choose default device
        virtual auto ChooseDevice(
            const WrapAL::AudioDeviceInfo list[], 
            UINT count) noexcept ->uint32_t override {
            std::wprintf(L"Please choose the device(type index. set default device if out of range):\r\n");
            for (uint32_t i = 0; i < count; ++i) {
                std::wprintf(L"%d. %ls\r\n", int(i), list[i].name);
            }
            uint32_t uindex = 0u;
            {
                int index = count;
                std::scanf("%d", &index);
                uindex = uint32_t(index);
            }
            if (uindex < count) {
                std::wprintf(L"%ls\r\n%ls\r\n", list[uindex].name, list[uindex].id);
            }
            else {
                std::wprintf(L"Set default device.\r\n");
            }
            return uindex; 
        };
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
                WrapAL::EncodingFormat::Format_OggVorbis,
                //L"NationalAnthemOfRussia.ogg",
                L"Sakura.ogg",
                WrapAL::Flag_StreamingReading 
                //| WrapAL::Flag_AutoDestroyEOP
                | WrapAL::Flag_LoopInfinite
                , "BGM"
                );
            {
                auto test_refcount = clip;
                decltype(clip) rest_ref1(test_refcount);
                decltype(clip) rest_ref2(std::move(test_refcount));
                rest_ref1 = std::move(rest_ref2);
                int bk = 9;
            }
            {
                auto test_refcount = clip;
            }
            auto dur = clip.Duration();
            auto g = clip.GetGroup();
            auto name = g.Name();
            // play the clip
            clip.Play();
            float value = 1.f;
            char control = 0;
            std::wprintf(L"Audio clip duration: %5.3f sec.\r\n", clip.Duration());
            std::fseek(stdin, 0, SEEK_END);
            while ((std::scanf("%c %f", &control, &value) != EOF)) {
                switch (control)
                {
                case 'r':
                    clip.Ratio(value);
                    break;
                case 'v':
                    clip.Volume(value);
                    break;
                case 's':
                    clip.Seek(value);
                    break;
                case 'S':
                    clip.Stop();
                    break;
                case 'p':
                    clip.Pause();
                    break;
                case 'P':
                    clip.Play();
                    break;
                }
            }
            // dispose all clips before uninitialize the audio-engine
            if (std::rand() & 1) { clip.Dispose(); }
            else { decltype(clip) release_clip(std::move(clip)); }
            // Uninitialize WrapAL WrapALAudioEngine
            AudioEngine.Uninitialize();
        }
        // Uninitialize the COM Interface
        ::CoUninitialize();
    }
    return EXIT_SUCCESS;
}

