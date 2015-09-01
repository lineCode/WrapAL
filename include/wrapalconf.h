#pragma once
// THIS FILE IS NONE-LICENSE
#include <Windows.h>

// include CALDefConfigure
#define WRAPAL_INCLUDE_DEFAULT_CONFIGURE

// use AudioEngine marco
#define WRAPAL_USE_AUDIOENGINE_MARCO

// lower case method in clip, group class
//#define WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD

// simple/crude memory leak detector in msc in debug mode
#define WRAPAL_MEMDEBUG_IN_MSC

// update AudioEngine in same thread with playing audio?
#define WRAPAL_SAME_THREAD_UPDATE

// support for XAudio2_7.dll in dxsdk/runtime 2010jun?
// need support vista/win7 ? define it!
//#define WRAPAL_XAUDIO2_7_SUPPORT

// name
namespace WrapAL {
    // some config
    enum WrapALConstant : unsigned int {
        // object pool length in defaultly
        ObjectPoolLength = 1024,
        // error text defaultly length
        ErrorInfoLength = 1024,
        // each audio stream bucket size
        AudioStreamBucketSize = 32 * 1024,
        // streaming buffer size
        StreamingBufferSize = 16 * 1024,
        // streaming buffer count
        StreamingBufferCount = 3,
        // group name max length in char, +1 for null-end character
        GroupNameMaxLength = 15,
        // group max size
        GroupMaxSize = 8,
        // devie max count
        DeviceMaxCount = 256,
    };
    // message for runtime
    enum RuntimeMessage : unsigned int {
        // failed with HRESULT code, 2-arguments[char*, HRESULT]
        Message_FailedHRESULT,
        // file not found
        Message_FileNotFound,
        // libmpg123 not found, 2-arguments[char*, wchar_t*]
        Message_NoLibmpg123,
        // os before win8, XAudio2_7.dll only,1-argument[char*]
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
        Message_NeedDxRuntime,
#endif
        // count of this
        MESSAGE_COUNT,
    };
    // build-in message string
    static const wchar_t* BuildInMessageString[MESSAGE_COUNT] = {
        L"<%S>: Failed with HRESULT code : 0x%08X",
        L"<%S>: file not found ---> %ls",
        L"<%S>: libmpg123 library not found ---> %ls",
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
        L"<%S>: This app need dx-runtime, you should download 'directx_Jun2010_redist.exe' at first",
#endif
    };
    // locker for al
    class CALLocker {
#ifdef WRAPAL_SAME_THREAD_UPDATE
    public:
        // ctor
        CALLocker() noexcept = default;
        // dtor
        ~CALLocker() noexcept = default;
        // lock
        auto Lock() noexcept { }
        // unlock
        auto Unlock() noexcept { }
    private:
        // unused
        size_t          m_unused = 0;
#else
    public:
        // ctor
        CALLocker() noexcept { ::InitializeCriticalSection(&m_cs); }
        // dtor
        ~CALLocker() noexcept { ::DeleteCriticalSection(&m_cs); }
        // lock
        auto Lock() noexcept { ::EnterCriticalSection(&m_cs); }
        // unlock
        auto Unlock() noexcept { ::LeaveCriticalSection(&m_cs); }
    private:
        // cs
        CRITICAL_SECTION        m_cs;
#endif
    };
}