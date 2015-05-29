#include "wrapal.h"

// 音频处理开始
void WrapAL::AudioSourceClipReal::OnVoiceProcessingPassStart(UINT32 SamplesRequired) noexcept {
    if (SamplesRequired && this->playing) {
        // 流模式
        if (this->flags & WrapAL::Flag_StreamingReading) {
            this->source_voice->FlushSourceBuffers();
            auto id = this->buffer_index + 1;
            if (id >= StreamingBufferCount) id -= StreamingBufferCount;
            // [0, StreamingBufferCount)
            assert(id >= 0 && id < StreamingBufferCount && "out of range");
            this->LoadAndBufferData(id);
        }
    }
    
}

// 音频流结束
void WrapAL::AudioSourceClipReal::OnStreamEnd() noexcept {
    // 流模式？
    if (this->flags & WrapAL::Flag_StreamingReading) {
        // 无线轮回?
        if (this->flags & WrapAL::Flag_LoopInfinite) {
            this->stream->Seek(0);
        }
        else {
            this->playing = false;
        }
    }
    else {
        this->playing = false;
    }
}


// 缓冲区结束
void WrapAL::AudioSourceClipReal::OnBufferEnd(void * pBufferContext) noexcept {
    end_of_buffer = true; 
    if (this->flags & WrapAL::Flag_StreamingReading) {
        // todo
    }
    else {
        playing = false;
    }
}

// 加工新的缓冲区
auto WrapAL::AudioSourceClipReal::ProcessBufferData(XAUDIO2_BUFFER& buffer, bool eos) noexcept -> HRESULT {
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
    return this->source_voice->SubmitSourceBuffer(&buffer);
}

// 读取下一段
auto WrapAL::AudioSourceClipReal::LoadAndBufferData(int id) noexcept ->HRESULT {
    assert(stream);
    XAUDIO2_BUFFER buffer;
    ::ZeroMemory(&buffer, sizeof(buffer));
    buffer.AudioBytes = StreamingBufferSize;
    auto data = audio_data + StreamingBufferSize * (buffer_index = id);
    buffer.pAudioData = data;
    return this->ProcessBufferData(buffer, !stream->ReadNext(StreamingBufferSize, data));
}

#define LoadFunction(a, b, c) a = reinterpret_cast<decltype(a)>(::GetProcAddress(c, #b))
// 载入

// 初始化
auto WrapAL::CAudioEngine::Initialize(IALConfigure* config) noexcept -> HRESULT {
    wchar_t error[ErrorInfoLength]; error[0] = 0;
    HRESULT hr = S_OK;
    // 检查配置信息
#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE
    force_cast(this->configure) = config ? config : &m_config;
#else
    force_cast(this->configure) = config;
#endif
    assert(this->configure && "none configure");
    if (!this->configure) hr = E_INVALIDARG;
    // 载入XAudio动态链接库
    if (SUCCEEDED(hr)) {
        m_hXAudio2 = ::LoadLibraryW(L"XAudio2_8.dll");
        if (m_hXAudio2) {
            LoadFunction(WrapAL::CAudioEngine::XAudio2Create, XAudio2Create, m_hXAudio2);
        }
        else {
            hr = E_FAIL;
            ::swprintf(error, ErrorInfoLength,
                L"<%S>:libmpg123 library not found ---> %ls",
                __FUNCTION__, L"XAudio2_8.dll"
                );
        }
    }
    // 创建 XAudio2 引擎
    if (SUCCEEDED(hr)) {
        assert(!m_pXAudio2Engine && "m_pXAudio2 must be null");
        hr = CAudioEngine::XAudio2Create(&m_pXAudio2Engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
    }
    // 创建 Mastering Voice 接口
    if (SUCCEEDED(hr)) {
        hr = m_pXAudio2Engine->CreateMasteringVoice(
            &m_pMasterVoice,
            0, 0, 0, 0, nullptr
            );
    }
    // 获取libmpg123.dll句柄
    if (SUCCEEDED(hr)) {
        wchar_t path[MAX_PATH]; path[0] = 0;
        this->configure->GetLibmpg123_dllPath(path);
        force_cast(this->libmpg123) = ::LoadLibraryW(path);
        // 初始化libmpg123
        if (this->libmpg123) {
            Mpg123::Init(this->libmpg123);
            Mpg123::mpg123_init();
        }
        // 没有 libmpg123.dll 依然允许运行, 提供了路径则报错
        else if(path[0]){
            ::swprintf(error, ErrorInfoLength,
                L"<%S>:libmpg123 library not found ---> %ls",
                __FUNCTION__, path
                );
        }
    }
    // 报错
    if (!error[0] && FAILED(hr)) {
        ::swprintf(error, ErrorInfoLength,
            L"<%S>: Failed with HRESULT code : 0x%08X",
            __FUNCTION__, hr
            );
    }
    if(error[0] && this->configure)  this->configure->OutputError(error);
    assert(SUCCEEDED(hr));
    return hr;
}


// 反初始化
void WrapAL::CAudioEngine::UnInitialize() noexcept {
    // 摧毁所有有效片段
#ifdef _DEBUG
    m_acAllocator.Release([this](AudioSourceClipReal* real) {
        real->Release();
        auto itr = std::find(m_listAC.begin(), m_listAC.end(), real);
        assert(itr != m_listAC.end());
        m_listAC.erase(itr);
    });
    assert(m_listAC.size() == 0);
#else
    m_acAllocator.Release([](AudioSourceClipReal* real) {
            real->Release();
    });
#endif
    WrapAL::SafeDestroyVoice(m_pMasterVoice);
    WrapAL::SafeRelease(m_pXAudio2Engine);
    WrapAL::SafeRelease(force_cast(this->configure));
    // 释放dll文件
    if (this->libmpg123) {
        Mpg123::mpg123_exit();
        ::FreeLibrary(this->libmpg123);
        force_cast(this->libmpg123) = nullptr;
    }
    if (m_hXAudio2) {
        ::FreeLibrary(m_hXAudio2);
        m_hXAudio2 = nullptr;
    }
}

// 创建音频片段
auto WrapAL::CAudioEngine::CreateClip(IALAudioStream* stream, AudioClipFlag flags) noexcept -> ClipID {
    assert(stream && "bad argument");
    wchar_t error[ErrorInfoLength];
    wchar_t error_total[ErrorInfoLength]; error[0] = 0;
    ClipID id = ClipIDError;
    // 获取错误信息
    if (!stream->GetLastErrorInfo(error)) {
        // 流模式?
        if (flags & WrapAL::Flag_StreamingReading) {
            auto buffer = reinterpret_cast<uint8_t*>(::malloc(StreamingBufferSize*StreamingBufferCount));
            AudioSourceClipReal* real = m_acAllocator.Alloc();
#ifdef _DEBUG
            if (real) m_listAC.push_back(real);
#endif
            // 申请成功
            if (buffer && (id = reinterpret_cast<ClipID>(real))) {
                // 置换构造
                new(real) AudioSourceClipReal();
                // 设置
                stream->GetFormat().MakeWave(real->wave);
                real->stream = stream;
                real->audio_data = buffer;
                buffer = nullptr;
                real->flags = flags;
                real->buffer_length = StreamingBufferSize*StreamingBufferCount * 2;
                // 创建source
                auto hr = m_pXAudio2Engine->CreateSourceVoice(
                    &real->source_voice,
                    &real->wave,
                    0, XAUDIO2_DEFAULT_FREQ_RATIO,
                    real, nullptr, nullptr
                    );
                // 输入数据
                for (unsigned int i = 0; i < StreamingBufferCount-1; ++i) {
                    if (SUCCEEDED(hr)) {
                        hr = real->LoadAndBufferData(i);
                    }
                }
                // 检查错误
                if (FAILED(hr)) {
                    ::swprintf(error, ErrorInfoLength, L"HRESULT code -> 0x%08X\n", hr);
                }
            }
            // 错误
            else {
                ::wcscpy(error, L"Out of memory");
            }
            // 释放
            if (buffer) {
                ::free(buffer);
                buffer = nullptr;
            }
            
        }
        // 整片读取
        else {
            auto size_in_byte = stream->GetSizeInByte();
            auto buffer = reinterpret_cast<uint8_t*>(::malloc(size_in_byte));
            // 申请成功
            if (buffer) {
                stream->ReadNext(size_in_byte, buffer);
                stream->GetLastErrorInfo(error);
                id = this->CreateClipMove(stream->GetFormat(), buffer, size_in_byte, flags);
            }
            // OOM
            else {
                ::wcscpy(error, L"Out of memory");
            }
        }
    }
    // 有错误的情况
    if (error[0]) {
        // 添加信息
        ::swprintf(
            error_total, ErrorInfoLength, L"<%S>: Error -> %ls\n",
            __func__, error
            );
        // 显示错误信息
        this->configure->OutputError(error_total);
    }
    return id;
}

// 创建音频片段
auto WrapAL::CAudioEngine::CreateClip(AudioFormat format, const wchar_t* file_path, AudioClipFlag flags) noexcept ->ClipID {
    ClipID clip(ClipIDError);
    IALAudioStream* stream = nullptr;
    // 创建音频流
    if ((stream = this->configure->CreateAudioStream(format, file_path))) {
        clip = this->CreateClip(stream, flags);
        // 安全释放
        AudioSourceClip clip_handle(clip);
        if (!clip_handle->stream) {
            WrapAL::SafeRelease(stream);
        }
    }
    // 出现错误
    else {
        wchar_t error[ErrorInfoLength]; error[0] = 0;
        wchar_t error_total[ErrorInfoLength];
        // 获取基本错误信息
        this->configure->GetLastErrorInfo(error);
        // 添加信息
        ::swprintf(
            error_total, ErrorInfoLength, L"<%S>: Error -> %ls\n",
            __func__, error
            );
        // 显示错误信息
        this->configure->OutputError(error_total);
    }
    return clip;
}

// 创建音频片段
auto WrapAL::CAudioEngine::CreateClipMove(const PCMFormat& format, uint8_t*& buf, size_t len, AudioClipFlag flags) noexcept -> ClipID {
    // 直接使用缓冲区不能只用流模式
    assert(!(flags & WrapAL::Flag_StreamingReading) && "directly buffer can't be streaming mode");
    AudioSourceClipReal* real = m_acAllocator.Alloc();
#ifdef _DEBUG
    if (real) m_listAC.push_back(real);
#endif
    // 创建判断
    if (real) {
        // 置换构造
        new(real) AudioSourceClipReal();
        // 配置信息
        real->flags = flags;
        // 设置
        format.MakeWave(real->wave);
        real->buffer_length = len;
        real->audio_data = buf;
        buf = nullptr;
        // 创建source
        auto hr = m_pXAudio2Engine->CreateSourceVoice(
            &real->source_voice,
            &real->wave,
            0, XAUDIO2_DEFAULT_FREQ_RATIO,
            real, nullptr, nullptr
            );
        // 提交缓冲区
        if (SUCCEEDED(hr)) {
            XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
            buffer.pAudioData = real->audio_data;
            buffer.AudioBytes = real->buffer_length;
            hr = real->ProcessBufferData(buffer);
        }
        // 检查错误
        if (FAILED(hr)) {
            wchar_t error[ErrorInfoLength];
            ::swprintf(
                error, ErrorInfoLength, L"<%S>: Error with HRESULT code -> 0x%08X\n",
                __func__, hr
                );
            this->configure->OutputError(error);
        }
    }
    return reinterpret_cast<ClipID>(real);
}


// 创建片段
auto WrapAL::CAudioEngine::CreateClip(const PCMFormat & format, const uint8_t* src, size_t size, AudioClipFlag config) noexcept ->ClipID {
    uint8_t* new_src = reinterpret_cast< uint8_t*>(::malloc(size));
    if (new_src) {
        ::memcpy(new_src, src, size);
        return this->CreateClipMove(format, new_src, size, config);
    }
    else {
        this->configure->OutputError(L"Out of memory");
    }
    return ClipIDError;
}

// 摧毁指定片段
bool WrapAL::CAudioEngine::ac_destroy(ClipID id) noexcept {
    assert(id != ClipIDError);
    auto clip = reinterpret_cast<AudioSourceClipReal*>(id);
    assert(clip->source_voice);
    if (clip->source_voice) {
        clip->Release();
    }
    m_acAllocator.Free(clip);
#ifdef _DEBUG
    m_listAC.remove(clip);
#endif
    return true;
}

// 播放指定片段
bool WrapAL::CAudioEngine::ac_play(ClipID id) noexcept {
    assert(id != ClipIDError);
    if (id != ClipIDError) {
        auto clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        clip->source_voice->Start(0);
        clip->playing = true;
        return true;
    }
    return false;
}

// 暂停指定片段
bool WrapAL::CAudioEngine::ac_pause(ClipID id) noexcept{
    assert(id != ClipIDError);
    if (id != ClipIDError) {
        auto clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        clip->source_voice->Stop(0);
        clip->playing = false;
        return true;
    }
    return false;
}

// 音频定位
bool WrapAL::CAudioEngine::ac_seek(ClipID id, float pos) noexcept {
    assert(id != ClipIDError);
    if (id != ClipIDError) {
        auto* __restrict clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        // 保留基本
        bool playing = clip->playing;
        clip->source_voice->Stop(0);
        clip->source_voice->FlushSourceBuffers();
        // 检查采样位置
        uint32_t pos_in_sample = static_cast<uint32_t>(static_cast<double>(pos) *
            static_cast<double>(clip->wave.nSamplesPerSec));
        // 流模式?
        if (clip->flags & WrapAL::Flag_StreamingReading) {
            // 输入
            clip->stream->Seek(pos_in_sample *clip->wave.nBlockAlign);
            for (unsigned int i = 0; i < StreamingBufferCount - 1; ++i) {
                clip->LoadAndBufferData(i);
            }
        }
        // 直接播放
        else {
            XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
            buffer.PlayBegin = pos_in_sample;
            buffer.pAudioData = clip->audio_data;
            buffer.AudioBytes = clip->buffer_length;
            clip->ProcessBufferData(buffer);
        }
        // 播放?
        if (playing) {
            clip->source_voice->Start();
        }
        return true;
    }
    return false;
}

// 获取片段位置
auto WrapAL::CAudioEngine::ac_tell(ClipID id) noexcept -> float {
    assert(id != ClipIDError);
    float pos = 0.0f;
    if (id != ClipIDError) {
        auto* __restrict clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        XAUDIO2_VOICE_STATE state ;
        state.SamplesPlayed = 0;
        clip->source_voice->GetState(&state);
        // 计算
        pos = static_cast<float>(static_cast<double>(state.SamplesPlayed) /
            static_cast<double>(clip->wave.nSamplesPerSec));
    }
    return pos;
}

// 获取片段持续时间
auto WrapAL::CAudioEngine::ac_duration(ClipID id) noexcept -> float {
    assert(id != ClipIDError);
    float duration = 0.f;
    if (id != ClipIDError) {
        auto* __restrict clip = reinterpret_cast<AudioSourceClipReal*>(id);
        register uint32_t length;
        // 获取字节长度
        if (clip->flags & WrapAL::Flag_StreamingReading) {
            length = clip->stream->GetSizeInByte();
        }
        else {
            length = clip->buffer_index;
        }
        // 计算时间
        duration = static_cast<float>(static_cast<double>(length)/
            static_cast<double>(clip->wave.nAvgBytesPerSec)
            );
    }
    return duration;
}


// 设置或获取片段音量
auto WrapAL::CAudioEngine::ac_volume(ClipID id, float volume) noexcept -> float {
    assert(id != ClipIDError);
    if (id != ClipIDError) {
        auto* clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        // Get
        if (volume < 0.f) {
            clip->source_voice->GetVolume(&volume);
        }
        // Set
        else {
            clip->source_voice->SetVolume(volume);
        }
    }
    return volume;
}


// 设置或获取片段回放速率
auto WrapAL::CAudioEngine::ac_ratio(ClipID id, float ratio) noexcept -> float {
    assert(id != ClipIDError);
    if (id != ClipIDError) {
        auto* clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        // Get
        if (ratio < 0.f) {
            clip->source_voice->GetFrequencyRatio(&ratio);
        }
        // Set
        else {
            clip->source_voice->SetFrequencyRatio(ratio);
        }
    }
    return ratio;
}


// 设置或获取总音量
auto WrapAL::CAudioEngine::Master_Volume(float volume) noexcept -> float {
    assert(m_pMasterVoice);
    // Get
    if (volume < 0.f) {
        m_pMasterVoice->GetVolume(&volume);
    }
    // Set
    else {
        m_pMasterVoice->SetVolume(volume);
    }
    return volume;
}

