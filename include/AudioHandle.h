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
    // check it
#define CheckHandle assert(m_handle != ALInvalidHandle && "this clip m_handle had been failed")
    // Audio Group Handle Class, managed by engine, don't care about the releasing
    class CALAudioSourceGroup {
    public:
        // ctor
        /*explicit*/ CALAudioSourceGroup(ALHandle data) noexcept : m_handle(data) {};
        // ctor
        ~CALAudioSourceGroup() noexcept { m_handle = ALInvalidHandle; };
        // copy ctor
        CALAudioSourceGroup(const CALAudioSourceGroup& group)noexcept : m_handle(group.m_handle) {}
        // move ctor
        CALAudioSourceGroup(CALAudioSourceGroup&& group)noexcept : m_handle(group.m_handle) { (group.m_handle) = ALInvalidHandle; }
        // operator = copy
        auto operator =(const CALAudioSourceGroup& group) noexcept ->CALAudioSourceGroup& { (m_handle) = group.m_handle; return *this; }
        // operator = move
        auto operator =(CALAudioSourceGroup&& group) noexcept ->CALAudioSourceGroup& { m_handle = group.m_handle;  (group.m_handle) = ALInvalidHandle; return *this; }
    public:
        // get group name
        auto Name() const noexcept { return AudioEngine.ag_name(m_handle); }
        // get/set group volume
        auto Volume(float volume = -1.f) const noexcept { return AudioEngine.ag_volume(m_handle, volume); }
        // ----------------------------------------------------------------------------
#ifdef WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD
        // get group name
        auto name() const noexcept { return AudioEngine.ag_name(m_handle); }
        // get/set group volume
        auto volume(float volume = -1.f) const noexcept { return AudioEngine.ag_volume(m_handle, volume); }
#endif
    private:
        // m_handle for this
        ALHandle            m_handle;
    };
    // Audio Clip Handle Class, managed by engine, don't care about the releasing
    class CALAudioSourceClip {
    public:
        // copy ctor
        CALAudioSourceClip(const CALAudioSourceClip& clip)noexcept : m_handle(clip.m_handle) {}
        // move ctor
        CALAudioSourceClip(CALAudioSourceClip&& clip)noexcept : m_handle(clip.m_handle) { (clip.m_handle) = ALInvalidHandle; }
        // operator = copy
        auto operator =(const CALAudioSourceClip& clip) noexcept ->CALAudioSourceClip& { (m_handle) = clip.m_handle; return *this; }
        // operator = move
        auto operator =(CALAudioSourceClip&& clip) noexcept ->CALAudioSourceClip& { m_handle = clip.m_handle;  (clip.m_handle) = ALInvalidHandle; return *this; }
    public:
        // ctor
        /*explicit*/ CALAudioSourceClip(ALHandle data) noexcept : m_handle(data) {};
        // ctor
        ~CALAudioSourceClip() noexcept { m_handle = ALInvalidHandle; };
        // operatr bool()
        operator bool() const noexcept { return m_handle != ALInvalidHandle; }
        // operatr ->
        auto operator ->() const noexcept { CheckHandle; return reinterpret_cast<const AudioSourceClipReal*>(m_handle); }
        // get group
        auto GetGroup() const noexcept { return CALAudioSourceGroup(m_handle ? reinterpret_cast<ALHandle>((*this)->group) : ALInvalidHandle); }
        // destroy this clip, free the memory in engine
        auto Destroy()noexcept { CheckHandle; AudioEngine.ac_destroy(m_handle);  (m_handle) = ALInvalidHandle; }
        // play this clip
        auto Play() const noexcept { CheckHandle; return AudioEngine.ac_play(m_handle); }
        // stop this clip
        auto Stop() const noexcept { CheckHandle; return AudioEngine.ac_stop(m_handle); }
        // Pause this clip
        auto Pause() const noexcept { CheckHandle; return AudioEngine.ac_pause(m_handle); }
        // tell this clip in sec.
        auto Tell() const noexcept { CheckHandle; return AudioEngine.ac_tell(m_handle); }
        // get the duration in sec.
        auto Duration() const noexcept { CheckHandle; return AudioEngine.ac_duration(m_handle); }
        // set/get this volume
        auto Volume(float volume = -1.f) const noexcept { CheckHandle; return AudioEngine.ac_volume(m_handle, volume); }
        // set/get this ratio
        auto Ratio(float ratio = -1.f) const noexcept { CheckHandle; return AudioEngine.ac_ratio(m_handle, ratio); }
        // Seek this clip in sec.
        auto Seek(float time) const noexcept { CheckHandle; return AudioEngine.ac_seek(m_handle, time); }
        // ----------------------------------------------------------------------------
#ifdef WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD
        // get group
        auto group() const noexcept { return CALAudioSourceGroup(m_handle ? reinterpret_cast<ALHandle>((*this)->group) : ALInvalidHandle); }
        // destroy this clip, free the memory in engine
        auto destroy() noexcept { CheckHandle; AudioEngine.ac_destroy(m_handle);  (m_handle) = ALInvalidHandle; }
        // play this clip
        auto play() const noexcept { CheckHandle; return AudioEngine.ac_play(m_handle); }
        // stop this clip
        auto stop() const noexcept { CheckHandle; return AudioEngine.ac_stop(m_handle); }
        // Pause this clip
        auto pause() const noexcept { CheckHandle; return AudioEngine.ac_pause(m_handle); }
        // tell this clip in sec.
        auto tell() const noexcept { CheckHandle; return AudioEngine.ac_tell(m_handle); }
        // get the duration in sec.
        auto duration() const noexcept { CheckHandle; return AudioEngine.ac_duration(m_handle); }
        // set/get this volume
        auto volume(float volume = -1.f) const noexcept { CheckHandle; return AudioEngine.ac_volume(m_handle, volume); }
        // set/get this ratio
        auto ratio(float ratio = -1.f) const noexcept { CheckHandle; return AudioEngine.ac_ratio(m_handle, ratio); }
        // Seek this clip in sec.
        auto seek(float time) const noexcept { CheckHandle; return AudioEngine.ac_seek(m_handle, time); }
#endif
    private:
        // m_handle for this
        ALHandle                m_handle;
    };
    // create new clip with audio stream wrapped function
    // if using streaming audio, do not release the stream, this clip will do it
    static inline auto CreateAudioClip(XALAudioStream* stream, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return std::move(CALAudioSourceClip(AudioEngine.CreateClip(stream, flags, group)));
    }
    // create new clip with file name wrapped function
    static inline auto CreateAudioClip(AudioFormat f, const wchar_t* p, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return std::move(CALAudioSourceClip(AudioEngine.CreateClip(f, p, flags, group)));
    }
    // create new clip in memory wrapped function
    // for this, can't be in streaming mode
    static inline auto CreateAudioClip(const PCMFormat& format, const uint8_t* src, size_t size, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return std::move(CALAudioSourceClip(AudioEngine.CreateClip(format, src, size, flags, group)));
    }
    // create new clip in memory with move wrapped function
    // for this, can't be in streaming mode
    static inline auto CreateAudioClipMove(const PCMFormat& f, uint8_t*& p, size_t l, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return std::move(CALAudioSourceClip(AudioEngine.CreateClipMove(f, p, l, flags, group)));
    }
    // create new clip with audio stream wrapped function
    // if using streaming audio, do not release the stream, this clip will do it
    static inline auto CreateStreamingAudioClip(XALAudioStream* stream, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return std::move(CALAudioSourceClip(AudioEngine.CreateClip(stream, flags | WrapAL::Flag_StreamingReading, group)));
    }
    // create new clip with file name wrapped function
    static inline auto CreateStreamingAudioClip(AudioFormat f, const wchar_t* p, AudioClipFlag flags = Flag_None, const char* group = "BGM") noexcept {
        return std::move(CALAudioSourceClip(AudioEngine.CreateClip(f, p, flags | WrapAL::Flag_StreamingReading, group)));
    }
}