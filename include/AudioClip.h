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


// for [u]intXX_t
#include <cstdint>
// for assert
#include <cassert>
// for atomic
#include <atomic>

// include the config
#include "wrapalconf.h"
// WrapAL interface
#include "AudioInterface.h"

// XAudio
//#include <xaudio2.h>
#include "XAudio2_diy.h"
#include "X3DAudio_diy.h"


// wrapal namespace
namespace WrapAL {
    // Audio Source Clip Handle
    class CALAudioSourceClip;
    // impl for group
    struct AudioSourceGroupImpl;
    // Audio Source Clip implement
    class CALAudioSourceClipImpl final : public IXAudio2VoiceCallback {
    public: // impl for callback
        // Called just before this voice's processing pass begins.
        void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) noexcept override;
        // Called just after this voice's processing pass ends.
        void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() noexcept override {}
        // Called when this voice has just finished playing a buffer stream
        void STDMETHODCALLTYPE OnStreamEnd() noexcept override;
        // Called when this voice is about to start processing a new buffer.
        void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) noexcept override { m_bEOB = false; }
        // Called when this voice has just finished processing a buffer.
        // The buffer can now be reused or destroyed.
        void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) noexcept override;
        // Called when this voice has just reached the end position of a loop.
        void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) noexcept override { m_bPlaying = false; }
        // Called in the event of a critical error during voice processing,
        // such as a failing xAPO or an error from the hardware XMA decoder.
        // The voice may have to be destroyed and re-created to recover from
        // the error.  The callback arguments report which buffer was being
        // processed when the error occurred, and its HRESULT code.
        void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) noexcept override { }
    public:
        // Acquire stream
        static inline auto Acquire(XALAudioStream* steam) noexcept {
            if (steam) steam->AddRef(); return steam;
        }
        // ctor
        CALAudioSourceClipImpl(
            uint8_t*&& buf,
            XALAudioStream* stream,
            AudioClipFlag flag,
            uint32_t buflen
        ) noexcept : flags(flag), 
            m_pAudioData(buf),
            m_pStream(Acquire(stream)),
            m_uBufferLength(buflen) {
            buf = nullptr;
#ifdef _DEBUG
            this->s_vtable = *reinterpret_cast<void**>(this);
#endif
        }
    public:
        // dtor
        ~CALAudioSourceClipImpl() noexcept;
        // seek in sec.
        void Seek(float pos) noexcept;
        // tell position
        auto Tell() const noexcept ->float;
        // get duration
        auto Duration() const noexcept ->float;
        // is playing
        bool IsPlaying() const noexcept { return m_bPlaying; }
        // is end of buufer
        bool IsEndOfBuffer() const noexcept { return m_bEOB; }
        // set volume
        auto SetVolume(float v) noexcept { return m_pSourceVoice->SetVolume(v); }
        // set frequency ratio
        auto SetFrequencyRatio(float f) noexcept { 
            return m_pSourceVoice->SetFrequencyRatio(f); 
        }
        // get volume
        auto GetVolume() const noexcept { 
            float v = 0.f; m_pSourceVoice->GetVolume(&v); return v;
        }
        // get frequency ratio
        auto GetFrequencyRatio() const noexcept { 
            float f = 0.f; m_pSourceVoice->GetFrequencyRatio(&f); return f;
        }
        // play
        void Play() noexcept { m_pSourceVoice->Start(0); m_bPlaying = true; }
        // stop
        void Stop() noexcept { m_pSourceVoice->Stop(0); m_bPlaying = false; }
        // terminate
        void Terminate() noexcept;
        // add ref-count
        auto AddRef() noexcept { return ++m_cRefCount; }
        // Release
        auto Release() noexcept ->uint32_t;
        // buffer the data
        auto ProcessBufferData(XAUDIO2_BUFFER&, bool = true) noexcept ->HRESULT;
        // load next data and buffer it for streaming
        auto LoadAndBufferData(uint16_t id) noexcept ->HRESULT;
        // isok
        bool IsOK() const noexcept { return !!m_pStream; }
        // has source
        bool HasSource() const noexcept { return !!m_pSourceVoice; }
        // SetOutputVoices
        template<typename T> auto SetOutputVoices(T t) noexcept {
            return m_pSourceVoice->SetOutputVoices(t);
        }
        // CreateSourceVoice
        template<typename T> auto CreateSourceVoice(T eng) noexcept {
            return eng->CreateSourceVoice(
                &m_pSourceVoice,
                &wave,
                0, XAUDIO2_DEFAULT_FREQ_RATIO,
                this, nullptr, nullptr
            );
        }
    private:
        // destroy this clip
        void destroy() noexcept;
#ifdef _DEBUG
    public:
        // v-table address
        static void* s_vtable;
        // Check_debug
        bool Check_debug() const noexcept {
            auto ptr = reinterpret_cast<const size_t*>(this);
            assert(reinterpret_cast<size_t>(s_vtable) == ptr[0]);
            return true;
        }
#endif
    private:
        // source
        IXAudio2SourceVoice*        m_pSourceVoice = nullptr;
        // audio stream for streaming
        XALAudioStream*             m_pStream = nullptr;
    public:
        // group of this
        AudioSourceGroupImpl*       group = nullptr;
    private:
        // audio data
        uint8_t*                    m_pAudioData = nullptr;
        // audio length in byte
        uint32_t             const  m_uBufferLength = 0;
        // buffer index for streaming
        uint32_t                    m_uBufferIndex = 0;
        // ref-count
        uint32_t                    m_cRefCount = 1;
    public:
        // flags
        AudioClipFlag        const  flags;
        // wave format
        WAVEFORMATEX                wave;
    private:
        // end of buffer
        std::atomic_bool            m_bEOB = false;
        // is playing
        std::atomic_bool            m_bPlaying = false;
#if defined _M_IX86

#elif defined _M_X64

#else
#error "Unknown Target Platform"
#endif
    };
}