#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstdio>
#include <clocale>
#include "wrapal.h"

// App Entrance
int main() {
    // my os is in chinese, std::wprintf is on the bad way
    std::setlocale(LC_ALL, "chs");
    // my config
    class MyConfig : public WrapAL::CALDefConfigure {
    public:
        // choose device, return index, if out of range, choose default device
        virtual auto ChooseDevice(const WrapAL::AudioDeviceInfo list[], UINT count) noexcept ->UINT override {
            std::wprintf(L"Please choose the device(type index. set default device if out of range):\r\n");
            for (UINT i = 0; i < count; ++i) {
                std::wprintf(L"%d. %ls\r\n", int(i), list[i].Name());
            }
            UINT uindex = 0u;
            {
                int index = count;
                std::wscanf(L"%d", &index);
                uindex = UINT(index);
            }
            if (uindex < count) {
                std::wprintf(L"[%ls][%ls]\r\n", list[uindex].Name(), list[uindex].Id() );
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
                WrapAL::AudioFormat::Format_OggVorbis,
                //L"NationalAnthemOfRussia.ogg",
                L"Sakura.ogg",
                WrapAL::Flag_StreamingReading 
                //| WrapAL::Flag_AutoDestroyEOP
                | WrapAL::Flag_LoopInfinite
                , "BGM"
                );
            auto dur = clip.Duration();
            auto name = clip.GetGroup().Name();
            // play the clip
            clip.Play();
            clip.Volume(0.5f);
            float ratio = 1.f;
            while (std::wscanf(L"%f", &ratio) != EOF) {
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

