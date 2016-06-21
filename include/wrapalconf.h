#pragma once
// THIS FILE IS NONE-LICENSE

// include WrapAL::CALDefConfigure
#define WRAPAL_INCLUDE_DEFAULT_CONFIGURE

// include WrapAL::DefCreateAudioStream
#define WRAPAL_INCLUDE_DEFAULT_AUDIO_STREAM

// use AudioEngine marco
#define WRAPAL_USE_AUDIOENGINE_MARCO

// lower case method in clip, group class
//#define WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD

// simple/crude memory leak detector in msc in debug mode
#define NO_WRAPAL_MEMDEBUG_IN_MSC

// update AudioEngine in same thread with playing audio?
#define WRAPAL_SAME_THREAD_UPDATE

// support for XAudio2_7.dll in dxsdk/runtime 2010jun?
// need support vista/win7 ? define it!
//#define WRAPAL_XAUDIO2_7_SUPPORT


// [invalid yet] 
// support for creating WrapAL::IALStream from COM IStream
//#define WRAPAL_COM_ISTREAM_SUPPORT

#ifndef NO_WRAPAL_MEMDEBUG_IN_MSC
#define WRAPAL_MEMDEBUG_IN_MSC
#endif


#ifndef WRAPAL_SAME_THREAD_UPDATE
#include <Windows.h>
#endif

// Namespace wrapped std-lib
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _DEBUG
#define NDEBUG
#endif


// 检查
#ifndef WRAPAL_NOVTABLE
#   if (_MSC_VER >= 1100) && defined(__cplusplus)
#       define WRAPAL_NOVTABLE __declspec(novtable)
#   else
#       define WRAPAL_NOVTABLE
#   endif
#endif

#ifndef WRAPAL_NOINLINE
#   if (_MSC_VER >= 1300) && defined(__cplusplus)
#       define WRAPAL_NOINLINE __declspec(noinline)
#   else
#       define WRAPAL_NOINLINE
#   endif
#endif

// API
// #define WRAPALAPI __declspec(dllimport)
#ifndef WRAPALAPI
#ifdef WRAPALAPIDLL
#define WRAPALAPI __declspec(dllexport)
#else
#define WRAPALAPI
#endif
#endif

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
        // small space threshold for IALConfigure::SmallAlloc/SmallFree
        SmallSpaceThreshold = 128,
    };
    // message for runtime
    enum RuntimeMessage : unsigned int {
        // failed with HRESULT code, 2-arguments[char*, HRESULT]
        Message_FailedHRESULT,
        // out of memory, 1-argument[char*]
        Message_OOM,
        // file not found, 2-arguments[char*, wchar_t*]
        Message_FileNotFound,
        // libmpg123 not found, 2-arguments[char*, wchar_t*]
        Message_NoLibmpg123,
        // os before win8, XAudio2_7.dll only, 1-argument[char*]
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
        Message_NeedDxRuntime,
#endif
        // count of this
        MESSAGE_COUNT,
    };
    // build-in message string
    static const wchar_t* BuildInMessageString[MESSAGE_COUNT] = {
        L"<%S>: Failed with HRESULT code : 0x%08X",
        L"<%S>: Out of memory",
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