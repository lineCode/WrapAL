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


// wrapal namespace
namespace WrapAL {
    // Audio Source Clip Handle
    class CALAudioSourceClip;
    // Audio Source Clip Real
    struct AudioSourceClipReal final : public IXAudio2VoiceCallback {
    public: // impl for callback
        // Called just before this voice's processing pass begins.
        void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) noexcept override;
        // Called just after this voice's processing pass ends.
        void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() noexcept override { }
        // Called when this voice has just finished playing a buffer stream
        void STDMETHODCALLTYPE OnStreamEnd() noexcept override;
        // Called when this voice is about to start processing a new buffer.
        void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext)noexcept override { end_of_buffer = false; }
        // Called when this voice has just finished processing a buffer.
        // The buffer can now be reused or destroyed.
        void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) noexcept override;
        // Called when this voice has just reached the end position of a loop.
        void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext)noexcept override { playing = false; }
        // Called in the event of a critical error during voice processing,
        // such as a failing xAPO or an error from the hardware XMA decoder.
        // The voice may have to be destroyed and re-created to recover from
        // the error.  The callback arguments report which buffer was being
        // processed when the error occurred, and its HRESULT code.
        void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error)noexcept override { }
    public:
        // ctor
        AudioSourceClipReal() { end_of_buffer = false; playing = false; };
        // dtor
#ifdef _DEBUG
        ~AudioSourceClipReal() { if (stream || source_voice) assert(!"not released!"); }
#endif
        // Release
        void Release() noexcept { WrapAL::SafeDestroyVoice(source_voice); WrapAL::SafeRelease(stream); ::free(audio_data); audio_data = nullptr; }
        // buffer the data
        auto ProcessBufferData(XAUDIO2_BUFFER&, bool = true) noexcept->HRESULT;
        // load next data and buffer it for streaming
        auto LoadAndBufferData(int id) noexcept->HRESULT;
    public:
        // source
        IXAudio2SourceVoice*        source_voice = nullptr;
        // audio stream for streaming
        XALPCMStream*             stream = nullptr;
        // group of this
        AudioSourceGroupReal*           group = nullptr;
        // audio data
        uint8_t*                    audio_data = nullptr;
        // audio length in byte
        size_t                      buffer_length = 0;
        // buffer index for streaming
        uint32_t                    buffer_index = 0;
        // flags
        AudioClipFlag               flags;
        // wave format
        WAVEFORMATEX                wave;
        // end of buffer
        std::atomic_bool            end_of_buffer;
        // is playing
        std::atomic_bool            playing;
#if defined _M_IX86

#elif defined _M_X64

#else
#error "Unknown Target Platform"
#endif
    };
}