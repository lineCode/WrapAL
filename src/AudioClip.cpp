#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "AudioClip.h"
#include <AudioEngine.h>


// WRAPAL: DEFINE_GUID
#define WRAPAL_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

// WRAPAL: DEFINE_PROPERTYKEY
#define WRAPAL_DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) \
    const PROPERTYKEY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }



// WrapAL 命名空间
namespace WrapAL {
    // impl
    namespace impl {
        // read any
        template<typename T, size_t I, typename U>
        auto read_any(const U* ptr) noexcept {
            const auto data = reinterpret_cast<const T*>(ptr);
            return data[I];
        }
        // read any
        template<typename T, typename U>
        auto read_any(const U* ptr) noexcept {
            return impl::read_any<T, 0>(ptr);
        }
    }
    // CLSID_MMDeviceEnumerator
    WRAPAL_DEFINE_GUID(CLSID_MMDeviceEnumerator, 0xBCDE0395, 0xE52F, 0x467C,
        0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E);
    // IID_IMMDeviceEnumerator
    WRAPAL_DEFINE_GUID(IID_IMMDeviceEnumerator, 0xA95664D2, 0x9614, 0x4F35,
        0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6);
    // PKEY_Device_FriendlyName
    WRAPAL_DEFINE_PROPERTYKEY(PKEY_Device_FriendlyName, 0xa45c254e, 0xdf1c, 0x4efd,
        0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 14);
    // PKEY_AudioEndpoint_Path
    WRAPAL_DEFINE_PROPERTYKEY(PKEY_AudioEndpoint_Path, 0x9c119480, 0xddc2, 0x4954,
        0xa1, 0x50, 0x5b, 0xd2, 0x40, 0xd4, 0x54, 0xad, 1);
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
    // CLSID_XAudio2
    WRAPAL_DEFINE_GUID(CLSID_XAudio2, 0x5a508685, 0xa254, 0x4fba, 0x9b, 0x82, 0x9a, 0x24, 0xb0, 0x03, 0x06, 0xaf);
    // CLSID_XAudio2Debug
    WRAPAL_DEFINE_GUID(CLSID_XAudio2Debug, 0xdb05ea35, 0x0329, 0x4d4b, 0xa5, 0x3a, 0x6d, 0xea, 0xd0, 0x3d, 0x38, 0x52);
    // IID_IXAudio2
    WRAPAL_DEFINE_GUID(IID_IXAudio2, 0x8bcf1f58, 0x9fe7, 0x4583, 0x8a, 0xc6, 0xe2, 0xad, 0xc4, 0x65, 0xc8, 0xbb);
#endif
}



#ifndef NDEBUG
// V-Table address
void* WrapAL::CALAudioSourceClipImpl::s_vtable = nullptr;
#endif


/// <summary>
/// Releases this instance.
/// </summary>
/// <returns></returns>
auto WrapAL::CALAudioSourceClipImpl::Release() noexcept -> uint32_t {
    assert(m_cRefCount > 0 && m_cRefCount < 256 && "bad range");
    uint32_t count = --m_cRefCount;
    if (!count) this->destroy();
    return count;
}

/// <summary>
/// Destroys this instance.
/// </summary>
/// <returns></returns>
void WrapAL::CALAudioSourceClipImpl::destroy() noexcept {
    auto ptr = this;
    this->~CALAudioSourceClipImpl();
    WrapALAudioEngine.configure->SmallFree(ptr);
}


/// <summary>
/// Cals the audio source clip implementation.
/// </summary>
/// <param name="buf">The buf.</param>
/// <param name="stream">The stream.</param>
/// <param name="flag">The flag.</param>
/// <param name="buflen">The buflen.</param>
/// <returns></returns>
WrapAL::CALAudioSourceClipImpl::CALAudioSourceClipImpl(
    uint8_t*&& buf,
    XALAudioStream* stream,
    AudioClipFlag flag,
    uint32_t buflen
) noexcept : flags(flag),
m_pAudioData(buf),
m_pStream(Acquire(stream)),
m_uBufferLength(buflen) {
    buf = nullptr;
    // 加入调试链表
#ifndef NDEBUG
    this->s_vtable = impl::read_any<void*>(this);
    const auto last_node = &WrapALAudioEngine.last_clip__dbg;
    auto prve = last_node->prev;
    prve->next = this;
    this->prev = prve;
    this->next = last_node;
    last_node->prev = this;
#endif
}


/// <summary>
/// Cals the audio source clip implementation.
/// </summary>
/// <returns></returns>
WrapAL::CALAudioSourceClipImpl::~CALAudioSourceClipImpl() {
    if (m_pSourceVoice) m_pSourceVoice->DestroyVoice();
    if (m_pStream) m_pStream->Release();
    std::free(m_pAudioData);
    m_pAudioData = nullptr;
    // 链接前后指针
#ifndef NDEBUG
    this->prev->next = this->next;
    this->next->prev = this->prev;
#endif
}

/// <summary>
/// Seeks the specified position.
/// </summary>
/// <param name="pos">The position.</param>
/// <returns></returns>
void WrapAL::CALAudioSourceClipImpl::Seek(float pos) noexcept {
    assert(m_pSourceVoice);
    // 保留基本
    bool playing = this->IsPlaying();
    m_pSourceVoice->Stop(0);
    m_pSourceVoice->FlushSourceBuffers();
    // 检查采样位置
    uint32_t pos_in_sample = static_cast<uint32_t>(static_cast<double>(pos) *
        static_cast<double>(this->wave.nSamplesPerSec));
    // 流模式?
    if (this->flags & WrapAL::Flag_StreamingReading) {
        // 输入
        m_pStream->Seek(pos_in_sample * this->wave.nBlockAlign);
        for (unsigned int i = 0; i < StreamingBufferCount - 1; ++i) {
            this->LoadAndBufferData(i);
        }
    }
    // 直接播放
    else {
        XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
        buffer.PlayBegin = pos_in_sample;
        buffer.pAudioData = m_pAudioData;
        buffer.AudioBytes = m_uBufferLength;
        this->ProcessBufferData(buffer);
    }
    // 播放?
    if (playing) {
        m_pSourceVoice->Start();
    }
}

/// <summary>
/// Tells this instance.
/// </summary>
/// <returns></returns>
auto WrapAL::CALAudioSourceClipImpl::Tell() const noexcept ->float {
    assert(m_pSourceVoice);
    XAUDIO2_VOICE_STATE state;
    state.SamplesPlayed = 0;
    m_pSourceVoice->GetState(&state);
    // 已经播放
    double p = static_cast<double>(state.SamplesPlayed);
    double s = static_cast<double>(this->wave.nSamplesPerSec);
    // 计算
    return static_cast<float>(p / s);
}

/// <summary>
/// Durations this instance.
/// </summary>
/// <returns></returns>
auto WrapAL::CALAudioSourceClipImpl::Duration() const noexcept ->float {
    uint32_t length;
    // 获取字节长度
    if (this->flags & WrapAL::Flag_StreamingReading) {
        length = m_pStream->GetSizeInByte();
    }
    else {
        length = m_uBufferIndex;
    }
    // 计算
    double l = static_cast<double>(length);
    double n = static_cast<double>(this->wave.nAvgBytesPerSec);
    // 计算时间
    return static_cast<float>(l / n);
}

// 音频处理开始
void WrapAL::CALAudioSourceClipImpl::OnVoiceProcessingPassStart(UINT32 SamplesRequired) noexcept {
    if (SamplesRequired && this->IsPlaying()) {
        // 流模式
        if (this->flags & WrapAL::Flag_StreamingReading) {
            m_pSourceVoice->FlushSourceBuffers();
            auto id = m_uBufferIndex + 1;
            if (id >= StreamingBufferCount) id -= StreamingBufferCount;
#ifndef NDEBUG
            //std::wprintf(L"%d", id);
#endif
            // [0, StreamingBufferCount)
            assert(id >= 0 && id < StreamingBufferCount && "out of range");
            this->LoadAndBufferData(id);
        }
    }

}

// 音频流结束
void WrapAL::CALAudioSourceClipImpl::OnStreamEnd() noexcept {
    // 自动释放?
    if (this->flags & WrapAL::Flag_AutoDestroyEOP) {
        m_bPlaying = false;
        return;
    }
    // 流模式 + 无线轮回
    if (this->flags & (WrapAL::Flag_StreamingReading | WrapAL::Flag_LoopInfinite)) {
        // = 重置
        this->m_pStream->Seek(0);
    }
    else {
        m_bPlaying = false;
    }
}

/// <summary>
/// Stops this instance.
/// </summary>
/// <returns></returns>
void WrapAL::CALAudioSourceClipImpl::Terminate() noexcept {
    m_pSourceVoice->Stop(0);
    m_pSourceVoice->FlushSourceBuffers();
    m_bPlaying = false;
    m_bEOB = false;
}

// 缓冲区结束
void WrapAL::CALAudioSourceClipImpl::OnBufferEnd(void* pBufferContext) noexcept {
    m_bEOB = true;
    if (this->flags & WrapAL::Flag_StreamingReading) {
        // todo
    }
    else {
        m_bPlaying = false;
    }
}

// 将XAUDIO2_BUFFER暴露在顶级命名空间
namespace WrapAL { using XAUDIO2_BUFFER_T = XAUDIO2_BUFFER; }

// 加工新的缓冲区
auto WrapAL::CALAudioSourceClipImpl::ProcessBufferData(
    XAUDIO2_BUFFER_T& buffer, bool eos) noexcept -> HRESULT {
    // 缓冲区配置
    buffer.LoopCount = 0;
    // EOS?
    if (eos) {
        buffer.Flags = XAUDIO2_END_OF_STREAM;
    }
    // 流模式
    if (flags & WrapAL::Flag_StreamingReading) {
        // todo
    }
    else {
        // 无限轮回?
        if (flags & WrapAL::Flag_LoopInfinite) {
            buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
        }
    }
    // 提交
    return this->m_pSourceVoice->SubmitSourceBuffer(&buffer);
}

/// <summary>
/// Loads the and buffer data.
/// 读取下一段
/// </summary>
/// <param name="id">The identifier.</param>
/// <returns></returns>
auto WrapAL::CALAudioSourceClipImpl::LoadAndBufferData(uint16_t id) noexcept ->HRESULT {
    assert(m_pStream);
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = StreamingBufferSize;
    auto data = m_pAudioData + StreamingBufferSize * (m_uBufferIndex = id);
    buffer.pAudioData = data;
    // 已读取
    auto read = m_pStream->ReadNext(StreamingBufferSize, data);
    return this->ProcessBufferData(buffer, read != StreamingBufferSize);
}
