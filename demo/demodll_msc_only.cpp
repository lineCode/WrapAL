#define _CRT_SECURE_NO_WARNINGS
#ifndef _MSC_VER
static_assert(false, "MSC support __declspec(dllimport) only");
#endif
#include <cstdio>
#include <clocale>
#define WRAPALAPI __declspec(dllimport)
#include "AudioEngine.h"
#include "AudioHandle.h"

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
                std::wprintf(L"%ls\r\n%ls\r\n", list[uindex].Name(), list[uindex].Id());
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
            auto dur = clip.Duration();
            auto name = clip.GetGroup().Name();
            // play the clip
            clip.Play();
            float value = 1.f;
            wchar_t control = 0;
            std::wprintf(L"Audio clip duration: %5.3f sec.\r\n", clip.Duration());
            while ((std::wscanf(L"%c %f", &control, &value) != EOF)) {
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
            // destroy clip(optional)
            //clip.Destroy();

            // Uninitialize WrapAL WrapALAudioEngine
            AudioEngine.Uninitialize();
        }
        // Uninitialize the COM Interface
        ::CoUninitialize();
    }
    return EXIT_SUCCESS;
}

