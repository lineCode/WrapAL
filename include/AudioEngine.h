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
        // init
        auto Initialize(IALConfigure* config=nullptr) noexcept ->HRESULT;
        // un-init
        void UnInitialize() noexcept;
        // ctor
        CALAudioEngine() = default;
        // dtor
        ~CALAudioEngine() noexcept { if(this->configure) this->UnInitialize(); }
        // get version
        auto GetVersion() const noexcept -> const char* { return "0.1.1" ; }
    public: // Audio Clip
        // create new clip with audio stream
        // if using streaming audio, do not release the stream, this clip will do it
        auto CreateClip(IALAudioStream*, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
        // create new clip with file name
        auto CreateClip(AudioFormat, const wchar_t*, AudioClipFlag, const char* group_name) noexcept->ALHandle;
        // create new clip in memory with move
        auto CreateClipMove(const PCMFormat&, uint8_t*&, size_t, AudioClipFlag, const char* group_name) noexcept->ALHandle;
        // create new clip in memory
        auto CreateClip(const PCMFormat& format, const uint8_t* src, size_t size, AudioClipFlag, const char* group_name) noexcept->ALHandle;
    private: // Audio Clip
        // destroy the clip
        bool ac_destroy(ALHandle) noexcept;
        // play the clip
        bool ac_play(ALHandle) noexcept;
        // pause the clip
        bool ac_pause(ALHandle) noexcept;
        // stop the clip
        bool ac_stop(ALHandle id) noexcept { this->ac_pause(id); return this->ac_seek(id, 0.0f); }
        // seek the clip with time in sec.
        bool ac_seek(ALHandle, float) noexcept;
        // tell the postion of clip in sec.
        auto ac_tell(ALHandle) noexcept ->float;
        // get the duration of clip  in sec.
        auto ac_duration(ALHandle) noexcept ->float;
        // set or get volume of clip
        auto ac_volume(ALHandle, float = -1.f) noexcept->float;
        // set or get ratio of clip
        auto ac_ratio(ALHandle id, float ratio) noexcept -> float;
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
        // find group by group name
        auto find_group(const char* name) noexcept->AudioSourceGroupReal*;
        // set clip group
        auto set_clip_group(AudioSourceClipReal& clip, const char* group) noexcept->HRESULT;
    public: // XAudio2 API
        // XAudio2Create
        static decltype(&::XAudio2Create) XAudio2Create;
    private: // XAudio2 API
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
    public:
        // instance for this
        static CALAudioEngine s_instance;
    };
    // marco define
#define AudioEngine (WrapAL::CALAudioEngine::s_instance)

}