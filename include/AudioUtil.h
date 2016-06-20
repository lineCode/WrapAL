#pragma once
/**
* Copyright (c) 2014-2015 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

// int type
#include <cstdint>
// windows
#include <Windows.h>
// config
#include "wrapalconf.h"
// def config
#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE
#include "AudioInterface.h"
#endif

// mpg123
#include <sys\types.h>
#ifdef _MSC_VER
using ssize_t = std::intptr_t;
#endif
// MY CONFIGURE
#if 0
#define NO_ICY 1
#define NO_STRING 1
#define NO_ID3V2 1
#define NO_WARNING 1
#define NO_ERRORMSG 1
#define NO_ERETURN 1
#define NO_FEEDER 1
#define USE_NEW_HUFFTABLE 1
#endif
#define MPG123_NO_CONFIGURE
#include "../3rdparty/mpg123/mpg123.h.in"


// wrapal namespace
namespace WrapAL {
    // windows error code to HRESULT
    inline auto WinCode2HRESULT(UINT x) noexcept ->HRESULT {
        return ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT)(((x)& 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)));
    }
    // Mpg123 dll, using dynamic-linking to avoid LGPL
    class Mpg123 {
    public:
        // init
        static void Init(HMODULE)  noexcept;
    public: // function zone
        // mpg123 : init
        static decltype(&::mpg123_init) mpg123_init;
        // mpg123 : exit
        static decltype(&::mpg123_exit) mpg123_exit;
        // mpg123 : new
        static decltype(&::mpg123_new) mpg123_new;
        // mpg123 : delete
        static decltype(&::mpg123_delete) mpg123_delete;
        // mpg123 : read
        static decltype(&::mpg123_read) mpg123_read;
        // mpg123 : seek
        static decltype(&::mpg123_seek) mpg123_seek;
        // mpg123 : tell
        static decltype(&::mpg123_tell) mpg123_tell;
        // mpg123 : length
        static decltype(&::mpg123_length) mpg123_length;
        // mpg123 : format
        static decltype(&::mpg123_format) mpg123_format;
        // mpg123 : get format
        static decltype(&::mpg123_getformat) mpg123_getformat;
        // mpg123 : format none
        static decltype(&::mpg123_format_none) mpg123_format_none;
        // mpg123 : open with handle
        static decltype(&::mpg123_open_handle) mpg123_open_handle;
        // mpg123 : replace reader handle
        static decltype(&::mpg123_replace_reader_handle) mpg123_replace_reader_handle;
    };
    // small buffer
    template<typename T, size_t LEN>
    class SmallBuffer {
    public:
        // ctor
        SmallBuffer() noexcept = default;
        // copy ctor
        SmallBuffer(const SmallBuffer&) = delete;
        // move ctor
        SmallBuffer(SmallBuffer&&) = delete;
        // dtor
        ~SmallBuffer() noexcept { 
            if (m_pData != m_buffer) std::free(m_pData); 
            m_pData = nullptr; 
        }
        // init
        auto Init(size_t length) {
            assert(!m_pData && "call Init() only once");
            m_pData = (length > LEN) ? reinterpret_cast<T*>(::malloc(sizeof(T)*length)) : m_buffer;
            assert(m_pData && "out of memory");
        }
        // get data
        auto GetData() const noexcept { assert(m_pData); return m_pData; }
    private:
        // buffer
        T*          m_pData = nullptr;
        // fixed buffer
        T           m_buffer[LEN];
    };
#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE
    // default al configure
    class WRAPALAPI CALDefConfigure : public IALConfigure {
    public:
        // cotr
        CALDefConfigure() { m_szLastError[0] = 0; };
        // dotr
        ~CALDefConfigure() = default;
    public: // infterface impl for IALConfigure
        // add ref-count this
        virtual auto AddRef() noexcept ->uint32_t override { return 2; }
        // release this
        virtual auto Release() noexcept ->uint32_t override { return 1; }
        // auto update? return true if you want and must undef "WRAPAL_SAME_THREAD_UPDATE"
        virtual auto IsAutoUpdate() noexcept ->bool override { return false; };
        // alloc a small space less than 128-byte
        virtual void*SmallAlloc(size_t length) noexcept override;
        // free space that alloced via "SmallAlloc"
        virtual void SmallFree(void* address) noexcept override;
        // choose device, return index, if out of range, choose default device
        virtual auto ChooseDevice(const AudioDeviceInfo[], UINT count) noexcept ->UINT override { return count; };
        // create audio stream from file stream
        virtual auto CreateAudioStream(EncodingFormat, IALFileStream*) noexcept ->XALAudioStream* override;
        // get last error infomation, return false if no error
        virtual auto GetLastErrorInfo(wchar_t info[/*ErrorInfoLength*/]) noexcept ->bool override;
        // output error infomation
        virtual void OutputError(const wchar_t* err) noexcept override;
        // get message in runtime
        virtual auto GetRuntimeMessage(RuntimeMessage msg) noexcept ->const wchar_t* override { return BuildInMessageString[msg]; }
        // get the "libmpg123.dll" path on windows
        virtual void GetLibmpg123Path(wchar_t path[/*MAX_PATH*/]) noexcept;
    private:
        // last error infomation
        wchar_t             m_szLastError[ErrorInfoLength];
    };
#endif
}