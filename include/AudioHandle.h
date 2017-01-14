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


// assert
#include <cassert>
// std::move
#include <utility>
// common
#include "wrapal_common.h"
// engine
#include "AudioEngine.h"

// wrapal namespace
namespace WrapAL {
    // check it
#define CheckHandle assert(m_handle != ALInvalidHandle && "this clip m_handle had been failed")
    // Audio Group Handle Class, managed by engine, don't care about the releasing
    class CALAudioSourceGroup {
    public:
        // ctor
        CALAudioSourceGroup(ALHandle data) noexcept : m_handle(data) {};
        // ctor
        ~CALAudioSourceGroup() noexcept { m_handle = ALInvalidHandle; };
        // copy ctor
        CALAudioSourceGroup(const CALAudioSourceGroup& group) noexcept : m_handle(group.m_handle) {}
        // move ctor
        CALAudioSourceGroup(CALAudioSourceGroup&& group) noexcept : m_handle(group.m_handle) { (group.m_handle) = ALInvalidHandle; }
        // operator = copy
        auto operator =(const CALAudioSourceGroup& group) noexcept ->CALAudioSourceGroup& { (m_handle) = group.m_handle; return *this; }
        // operator = move
        auto operator =(CALAudioSourceGroup&& group) noexcept ->CALAudioSourceGroup& { m_handle = group.m_handle;  (group.m_handle) = ALInvalidHandle; return *this; }
    public:
        // get group name
        auto Name() const noexcept { return WrapALAudioEngine.ag_name(m_handle); }
        // get/set group volume
        auto Volume(float volume = -1.f) const noexcept { return WrapALAudioEngine.ag_volume(m_handle, volume); }
        // ----------------------------------------------------------------------------
#ifdef WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD
        // get group name
        auto name() const noexcept { return WrapALAudioEngine.ag_name(m_handle); }
        // get/set group volume
        auto volume(float volume = -1.f) const noexcept { return WrapALAudioEngine.ag_volume(m_handle, volume); }
#endif
    private:
        // m_handle for this
        ALHandle            m_handle;
    };
    // Audio Clip Handle Class, managed by engine, don't care about the releasing
    class CALAudioSourceClip {
        // safe release
        void safe_release() noexcept { if (*this) WrapALAudioEngine.ac_release(m_handle); }
        // safe add ref
        void safe_addref() noexcept { if (*this) WrapALAudioEngine.ac_addref(m_handle); }
        // operatr ->
        auto operator ->() const noexcept { CheckHandle; return reinterpret_cast<const CALAudioSourceClipImpl*>(m_handle); }
    public:
        // copy ctor
        CALAudioSourceClip(const CALAudioSourceClip& clip) noexcept : m_handle(clip.m_handle) { 
            this->safe_addref();
        }
        // move ctor
        CALAudioSourceClip(CALAudioSourceClip&& clip) noexcept : m_handle(clip.m_handle) { 
            clip.m_handle = ALInvalidHandle; 
        }
        // operator = copy
        auto operator =(const CALAudioSourceClip& clip) noexcept ->CALAudioSourceClip& { 
            this->safe_release();
            (m_handle) = clip.m_handle; 
            this->safe_addref();
            return *this; 
        }
        // operator = move
        auto operator =(CALAudioSourceClip&& clip) noexcept ->CALAudioSourceClip& { 
            this->safe_release();
            m_handle = clip.m_handle;
            (clip.m_handle) = ALInvalidHandle; 
            return *this; 
        }
    public:
        // ctor
        CALAudioSourceClip(ALHandle data) noexcept : m_handle(data) {};
        // ctor
        ~CALAudioSourceClip() noexcept { this->Dispose(); };
        // operatr!
        bool operator !() const noexcept { return m_handle == ALInvalidHandle; }
        // operatr bool()
        operator bool() const noexcept { return m_handle != ALInvalidHandle; }
        // get group
        auto GetGroup() const noexcept->CALAudioSourceGroup;
        // dispose this handle
        void Dispose() noexcept { this->safe_release();  (m_handle) = ALInvalidHandle; }
    public:
        // play this clip
        auto Play() const noexcept { CheckHandle; return WrapALAudioEngine.ac_play(m_handle); }
        // stop this clip
        auto Stop() const noexcept { CheckHandle; return WrapALAudioEngine.ac_stop(m_handle); }
        // Pause this clip
        auto Pause() const noexcept { CheckHandle; return WrapALAudioEngine.ac_pause(m_handle); }
        // tell this clip in sec.
        auto Tell() const noexcept { CheckHandle; return WrapALAudioEngine.ac_tell(m_handle); }
        // get the duration in sec.
        auto Duration() const noexcept { CheckHandle; return WrapALAudioEngine.ac_duration(m_handle); }
        // set/get this volume
        auto Volume(float volume = -1.f) const noexcept { CheckHandle; return WrapALAudioEngine.ac_volume(m_handle, volume); }
        // set/get this ratio
        auto Ratio(float ratio = -1.f) const noexcept { CheckHandle; return WrapALAudioEngine.ac_ratio(m_handle, ratio); }
        // Seek this clip in sec.
        auto Seek(float time) const noexcept { CheckHandle; return WrapALAudioEngine.ac_seek(m_handle, time); }
        // ----------------------------------------------------------------------------
#ifdef WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD
        // get group
        auto group() const noexcept { return CALAudioSourceGroup(m_handle ? reinterpret_cast<ALHandle>((*this)->group) : ALInvalidHandle); }
        // destroy this clip, free the memory in engine
        auto destroy() noexcept { CheckHandle; WrapALAudioEngine.ac_destroy(m_handle);  (m_handle) = ALInvalidHandle; }
        // play this clip
        auto play() const noexcept { CheckHandle; return WrapALAudioEngine.ac_play(m_handle); }
        // stop this clip
        auto stop() const noexcept { CheckHandle; return WrapALAudioEngine.ac_stop(m_handle); }
        // Pause this clip
        auto pause() const noexcept { CheckHandle; return WrapALAudioEngine.ac_pause(m_handle); }
        // tell this clip in sec.
        auto tell() const noexcept { CheckHandle; return WrapALAudioEngine.ac_tell(m_handle); }
        // get the duration in sec.
        auto duration() const noexcept { CheckHandle; return WrapALAudioEngine.ac_duration(m_handle); }
        // set/get this volume
        auto volume(float volume = -1.f) const noexcept { CheckHandle; return WrapALAudioEngine.ac_volume(m_handle, volume); }
        // set/get this ratio
        auto ratio(float ratio = -1.f) const noexcept { CheckHandle; return WrapALAudioEngine.ac_ratio(m_handle, ratio); }
        // Seek this clip in sec.
        auto seek(float time) const noexcept { CheckHandle; return WrapALAudioEngine.ac_seek(m_handle, time); }
#endif
    private:
        // m_handle for this
        ALHandle                m_handle;
    };
    // create new clip with audio stream wrapped function
    // if using streaming audio, do not release the stream, this clip will do it
    inline auto CreateAudioClip(XALAudioStream* stream, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return (CALAudioSourceClip(WrapALAudioEngine.CreateClip(stream, flags, group)));
    }
    // create new clip with file name wrapped function
    inline auto CreateAudioClip(EncodingFormat format, const wchar_t* name, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return (CALAudioSourceClip(WrapALAudioEngine.CreateClip(format, name, flags, group)));
    }
    // create new clip with file stream wrapped function
    inline auto CreateAudioClip(EncodingFormat format, IALFileStream* stream, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return (CALAudioSourceClip(WrapALAudioEngine.CreateClip(format, stream, flags, group)));
    }
    // create new clip in memory wrapped function
    // for this, can't be in streaming mode
    inline auto CreateAudioClip(const AudioFormat& format, const uint8_t* src, size_t size, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return (CALAudioSourceClip(WrapALAudioEngine.CreateClip(format, src, size, flags, group)));
    }
    // create new clip in memory with given memory
    // for this, can't be in streaming mode
    template<typename T>
    inline auto CreateAudioClip(const AudioFormat& f, T call, size_t l, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        ALHandle clip = ALInvalidHandle;
        if (auto buffer = reinterpret_cast<uint8_t*>(std::malloc(l))) {
            call(buffer);
            clip = WrapALAudioEngine.CreateClip(f, std::move(buffer), l, flags, group);
        }
        return (CALAudioSourceClip(clip));
    }
}