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

// DEBUG
#ifdef _DEBUG
#include <list>
#else
#define NDEBUG
#endif

// wrapal namespace
namespace WrapAL {
    // API level
    enum class APILevel : size_t {
        // NO API
        Level_Unknown = 0,
        // XAudio ver2.7, user need install DirectX Runtime
        Level_XAudio2_7,
        // XAudio ver2.8, system component in Windows 8
        Level_XAudio2_8,
        // XAudio ver2.9, system component in Windows 10
        Level_XAudio2_9,
        // [invalid yet]Open Audio Lib, user need install openal-runtime
        Level_OpenAL,
        // [invalid yet]Direct Sound in early sdk
        Level_DirectSound,
    };
#ifdef WRAPAL_INCLUDE_DEFAULT_PCM_STREAM
    // create default pcm audio stream
    auto DefCreatePCMStream(AudioFormat format, IALFileStream* stream, wchar_t error_info[ErrorInfoLength]) noexcept->XALPCMStream* ;
#endif
    // Audio Engine
    class CALAudioEngine {
        // audio clip pool
        using ACPool = ObjectPool<AudioSourceClipReal, AudioStreamBucketSize>;
        // friend class
        friend class CALAudioSourceClip;
        // friend class
        friend class CALAudioSourceGroup;
        // friend class
        friend class CALDefConfigure;
    public:
        // get version
        auto GetVersion() const noexcept -> const char* { return "0.2.2"; }
        // get api level
        auto GetAPILevel() const noexcept { return m_lvAPI; }
        // get message
        auto GetRuntimeMessage(RuntimeMessage msg) const noexcept { return this->configure->GetRuntimeMessage(msg); }
        // init
        auto Initialize(IALConfigure* config=nullptr) noexcept ->HRESULT;
        // un-init
        void UnInitialize() noexcept;
        // update audio engine if you want to do some auto-task, call this more than 20Hz
        void Update() noexcept;
        // create stream form file
        static auto CreatStreamFromFile(const wchar_t* file_name) noexcept->IALFileStream*;
#ifdef WRAPAL_COM_ISTREAM_SUPPORT
        // create alstream form istream
        static auto CreatStreamFromStream(IStream* stream) noexcept->IALStream*;
        // create stream form stream
        //auto CreatStreamFromStream(IALStream* stream) noexcept->IStream*;
#endif
    public:
        // ctor
        CALAudioEngine() = default;
        // dtor
        ~CALAudioEngine() noexcept { if(this->configure) this->UnInitialize(); }
    private:
        // audio api leve
        APILevel                    m_lvAPI = APILevel::Level_Unknown;
    public: // Audio Clip
        // create new clip with audio stream
        // if using streaming audio, do not release the stream, this clip will do it
        auto CreateClip(XALPCMStream*, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
        // create new clip with file name
        auto CreateClip(AudioFormat, const wchar_t*, AudioClipFlag, const char* group_name) noexcept->ALHandle;
        // create new clip in memory with move
        auto CreateClipMove(const PCMFormat&, uint8_t*&, size_t, AudioClipFlag, const char* group_name) noexcept->ALHandle;
        // create new clip in memory
        auto CreateClip(const PCMFormat& format, const uint8_t* src, size_t size, AudioClipFlag, const char* group_name) noexcept->ALHandle;
    private: // Audio Clip
        // recreate with file
        bool ac_recreate(ALHandle clip_id, const wchar_t* file_name) noexcept;
        // recreate with stream
        bool ac_recreate(ALHandle clip_id, XALPCMStream* stream) noexcept;
        // recreate with memory: same format with new buffer!
        bool ac_recreate(ALHandle clip_id, uint8_t* buffer, size_t length) noexcept;
        // recreate with move-able memory : same format with new buffer!
        bool ac_recreate_move(ALHandle clip_id, uint8_t*& buffer, size_t length) noexcept;
        // destroy the clip
        bool ac_destroy(ALHandle clip_id) noexcept;
        // play the clip
        bool ac_play(ALHandle clip_id) noexcept;
        // pause the clip
        bool ac_pause(ALHandle clip_id) noexcept;
        // stop the clip
        bool ac_stop(ALHandle clip_id) noexcept { this->ac_pause(clip_id); return this->ac_seek(clip_id, 0.0f); }
        // seek the clip with time in sec.
        bool ac_seek(ALHandle clip_id, float) noexcept;
        // tell the postion of clip in sec.
        auto ac_tell(ALHandle clip_id) noexcept ->float;
        // get the duration of clip  in sec.
        auto ac_duration(ALHandle clip_id) noexcept ->float;
        // set or get volume of clip
        auto ac_volume(ALHandle clip_id, float = -1.f) noexcept->float;
        // set or get ratio of clip
        auto ac_ratio(ALHandle clip_id, float ratio) noexcept -> float;
    public: // Master
        // set or get master volume
        auto Volume(float volume=-1.f) noexcept -> float;
    public: // Group
        // find group by group name
        auto GetGroup(const char* name) noexcept ->CALAudioSourceGroup;
    private:
        // get group name
        auto ag_name(ALHandle group_id) const noexcept -> const char*;
        // get/set group volume
        auto ag_volume(ALHandle group_id, float volume = -1.f) noexcept -> float;
    private:
        // find group by group name
        auto find_group(const char* name) noexcept->AudioSourceGroupReal*;
        // real destroy the clip
        void destroy_clip_real(AudioSourceClipReal* clip) noexcept;
        // set clip group
        auto set_clip_group(AudioSourceClipReal& clip, const char* group) noexcept->HRESULT;
    public: // XAudio2 API
        // XAudio2Create
        static decltype(&::XAudio2Create) XAudio2Create;
        // X3DAudioInitialize
        static decltype(&::X3DAudioInitialize) X3DAudioInitialize;
    private: // XAudio2 API
        // create source void
        auto create_source_voice(AudioSourceClipReal& clip, const char* group_name) noexcept->HRESULT;
        // XAudio2
        HMODULE                     m_hXAudio2 = nullptr;
        // XAudio2 interface
        IXAudio2*                   m_pXAudio2Engine = nullptr;
        // XAudio2 Mastering Voice interface
        IXAudio2MasteringVoice*     m_pMasterVoice = nullptr;
    private: // OpenAL -- maybe support
    private: // DirectSound -- maybe not support
    public:
        // now config
        IALConfigure*       const   configure = nullptr;
        // libmpg123.dll handle
        HMODULE             const   libmpg123 = nullptr;
    private: // Common
        // group
        AudioSourceGroupReal        m_aGroup[GroupMaxSize];
        // count of it
        size_t                      m_cGroupCount = 0;
#ifdef _DEBUG
        // debug list
        std::list<void*>            m_listAC;
#endif
        // allocator for audio clip
        ACPool                      m_acAllocator;
#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE
        // default config
        CALDefConfigure             m_config;
#endif
    public: // format helper 
        // format error with hr code
        static void FormatErrorHR(wchar_t err_buf[], const char* func_name, HRESULT hr) noexcept;
        // format error with file not found
        static void FormatErrorFoF(wchar_t err_buf[], const char* func_name, const wchar_t* file_name) noexcept;
        // format error with out of memory
        static void FormatErrorOOM(wchar_t err_buf[], const char* func_name) noexcept;
    public: // output helper
        // output error with hr code
        inline auto OutputErrorHR(const char* func_name, HRESULT hr) noexcept { 
            wchar_t err_buf[ErrorInfoLength]; 
            this->FormatErrorHR(err_buf, func_name, hr);
            this->configure->OutputError(err_buf);
        }
        // output error with file not found
        inline auto OutputErrorFoF(const char* func_name, const wchar_t* file_name) noexcept { 
            wchar_t err_buf[ErrorInfoLength]; 
            this->FormatErrorFoF(err_buf,func_name, file_name); 
            this->configure->OutputError(err_buf);
        }
        // output error with out of memory
        inline auto OutputErrorOOM(const char* func_name) noexcept { 
            wchar_t err_buf[ErrorInfoLength]; 
            this->FormatErrorOOM(err_buf,func_name); 
            this->configure->OutputError(err_buf);
        }
        // output last error
        void OutputErrorLast(const char* func_name) noexcept;
    public:
        // instance for this
        static CALAudioEngine s_instance;
    };
    // marco define
#define WrapALAudioEngine (WrapAL::CALAudioEngine::s_instance)
#ifdef WRAPAL_USE_AUDIOENGINE_MARCO
#define AudioEngine (WrapAL::CALAudioEngine::s_instance)
#endif
}