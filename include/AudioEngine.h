﻿#pragma once
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

// STDMETHODCALLTYPE

// include the config
#include "wrapalconf.h"
// include the config
#include "wrapal_common.h"
// WrapAL interface
#include "AudioInterface.h"
// util
#include "AudioUtil.h"


// wrapal namespace
namespace WrapAL {
    // impl for AudioSourceGroup
    struct AudioSourceGroupImpl;
    // group
    class CALAudioSourceGroup;
    // impl for engine
    struct engine_impl;
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
    // get api level string
    auto GetApiLevelString(APILevel) noexcept -> const char*;
#ifdef WRAPAL_INCLUDE_DEFAULT_AUDIO_STREAM
    // create default audio stream
    auto DefCreateAudioStream(EncodingFormat format, IALFileStream* stream, wchar_t error_info[ErrorInfoLength]) noexcept ->XALAudioStream* ;
#endif
    // Audio Engine
    class WRAPALAPI CALAudioEngine {
        // friend class
        friend class CALAudioSourceGroup;
        // friend class
        friend class CALAudioSourceClip;
        // friend class
        friend class CALDefConfigure;
    public:
        // get version
        auto GetVersion() const noexcept -> const char* { return "0.3.0"; }
        // get api level
        auto GetAPILevel() const noexcept { return m_lvAPI; }
        // get message
        auto GetRuntimeMessage(RuntimeMessage msg) const noexcept { return this->configure->GetRuntimeMessage(msg); }
        // init
        auto Initialize(IALConfigure* config=nullptr) noexcept ->ECode;
        // un-init
        void Uninitialize() noexcept;
        // update audio engine if you want to do some auto-task, call this more than 20Hz
        void Update() noexcept;
        // create stream form file
        static auto CreatStreamFromFile(const wchar_t* file_name) noexcept ->IALFileStream*;
#ifdef WRAPAL_COM_ISTREAM_SUPPORT
        // create alstream form istream
        static auto CreatStreamFromStream(IStream* stream) noexcept ->IALStream*;
        // create stream form stream
        //auto CreatStreamFromStream(IALStream* stream) noexcept ->IStream*;
#endif
    public:
        // ctor
        CALAudioEngine() = default;
        // dtor
        ~CALAudioEngine() noexcept { if(this->configure) this->Uninitialize(); }
    private:
        // audio api leve
        APILevel                    m_lvAPI = APILevel::Level_Unknown;
    public: // Audio Clip
        // create new clip with audio stream
        auto CreateClip(XALAudioStream*, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
        // create new clip with file stream
        auto CreateClip(EncodingFormat, IALFileStream*, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
        // create new clip with file name
        auto CreateClip(EncodingFormat, const wchar_t*, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
        // create new clip in memory
        auto CreateClip(const AudioFormat&, const uint8_t*, size_t, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
        // create new clip in memory
        auto CreateClip(const AudioFormat&, uint8_t*&&, size_t, AudioClipFlag, const char* group_name) noexcept ->ALHandle;
    private: // Audio Clip
#ifdef WRAPAL_IN_PLAN
        // recreate with file name
        bool ac_recreate(ALHandle clip_id, const wchar_t* file_name) noexcept;
        // recreate with file stream
        bool ac_recreate(ALHandle clip_id, IALFileStream* file_stream) noexcept;
        // recreate with audio stream
        bool ac_recreate(ALHandle clip_id, XALAudioStream* stream) noexcept;
        // recreate with memory: same format with new buffer!
        bool ac_recreate(ALHandle clip_id, const uint8_t* buffer, size_t length) noexcept;
        // recreate with malloc-ed move-able memory : same format with new buffer!
        bool ac_recreate(ALHandle clip_id, uint8_t*& buffer, size_t length) noexcept;
#endif
        // add ref-count for the clip
        bool ac_addref(ALHandle clip_id) noexcept;
        // release the clip
        bool ac_release(ALHandle clip_id) noexcept;
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
        auto ac_volume(ALHandle clip_id, float = -1.f) noexcept ->float;
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
        auto find_group(const char* name) noexcept ->AudioSourceGroupImpl*;
        // set clip group
        auto set_clip_group(CALAudioSourceClipImpl& clip, const char* group) noexcept ->ECode;
    private: 
        // create source void
        auto create_source_voice(CALAudioSourceClipImpl& clip, const char* group_name) noexcept ->ECode;
    public:
        // now config
        IALConfigure*   const   configure = nullptr;
#ifndef NDEBUG
        // debug linked node: first
        Node                    first_clip__dbg;
        // debug linked node: last
        Node                    last_clip__dbg;
#endif
    private:
        // pimpl
        engine_impl*            m_pImpl = nullptr;
    public: // format helper 
        // format error with hr code
        static void FormatErrorHR(wchar_t err_buf[], const char* func_name, ECode hr) noexcept;
        // format error with file not found
        static void FormatErrorFoF(wchar_t err_buf[], const char* func_name, const wchar_t* file_name) noexcept;
        // format error with out of memory
        static void FormatErrorOOM(wchar_t err_buf[], const char* func_name) noexcept;
    public: // output helper
        // output error with hr code
        inline auto OutputErrorHR(const char* func_name, ECode hr) noexcept {
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