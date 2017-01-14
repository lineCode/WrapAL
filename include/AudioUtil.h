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
// config
#include "wrapalconf.h"
// def config
#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE
#include "AudioInterface.h"
#endif

#include <cstdlib>

// wrapal namespace
namespace WrapAL {
    // error code
    using ECode = int32_t;
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
        virtual auto ChooseDevice(const AudioDeviceInfo[], uint32_t count) noexcept ->uint32_t override { return count; };
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