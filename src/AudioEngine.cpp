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


// 载入函数地址
#define LoadFunction(a, b, c) a = reinterpret_cast<decltype(a)>(::GetProcAddress(c, #b))
// 载入

// 初始化
auto WrapAL::CALAudioEngine::Initialize(IALConfigure* config) noexcept -> HRESULT {
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
            LoadFunction(WrapAL::CALAudioEngine::XAudio2Create, XAudio2Create, m_hXAudio2);
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
        hr = CALAudioEngine::XAudio2Create(&m_pXAudio2Engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
    }
    // 创建 Mastering Voice 接口
    if (SUCCEEDED(hr)) {
        hr = m_pXAudio2Engine->CreateMasteringVoice(
            &m_pMasterVoice,
            XAUDIO2_DEFAULT_CHANNELS,
            XAUDIO2_DEFAULT_SAMPLERATE,
            0, 
            nullptr, 
            nullptr
            );
    }
    // 获取libmpg123.dll句柄
    if (SUCCEEDED(hr)) {
        wchar_t path[MAX_PATH]; path[0] = 0;
        this->configure->GetLibmpg123Path(path);
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
void WrapAL::CALAudioEngine::UnInitialize() noexcept {
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
    // 释放
    for (auto& group : m_aGroup) {
        group.Release();
    }
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
auto WrapAL::CALAudioEngine::CreateClip(IALAudioStream* stream, AudioClipFlag flags, const char* group_name) noexcept -> ALHandle {
    assert(stream && "bad argument");
    wchar_t error[ErrorInfoLength];
    wchar_t error_total[ErrorInfoLength]; error[0] = 0;
    ALHandle id = ALInvalidHandle;
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
            if (buffer && (id = reinterpret_cast<ALHandle>(real))) {
                // 置换构造
                new(real) AudioSourceClipReal();
                // 设置
                stream->GetFormat().MakeWave(real->wave);
                real->stream = stream;
                real->audio_data = buffer;
                buffer = nullptr;
                real->flags = flags;
                real->buffer_length = StreamingBufferSize*StreamingBufferCount * 2;
                auto hr = S_OK;
                // 创建source
                if (SUCCEEDED(hr)) {
                    hr = m_pXAudio2Engine->CreateSourceVoice(
                        &real->source_voice,
                        &real->wave,
                        0, XAUDIO2_DEFAULT_FREQ_RATIO,
                        real, nullptr, nullptr
                        );
                }
                // 设置组别
                if (SUCCEEDED(hr)) {
                    hr = this->set_clip_group(*real, group_name);
                }
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
                id = this->CreateClipMove(stream->GetFormat(), buffer, size_in_byte, flags, group_name);
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
auto WrapAL::CALAudioEngine::CreateClip(AudioFormat format, const wchar_t* file_path, AudioClipFlag flags, const char* group_name) noexcept ->ALHandle {
    ALHandle clip(ALInvalidHandle);
    IALAudioStream* stream = nullptr;
    // 创建音频流
    if ((stream = this->configure->CreateAudioStream(format, file_path))) {
        clip = this->CreateClip(stream, flags, group_name);
        // 安全释放
        CALAudioSourceClip clip_handle(clip);
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
auto WrapAL::CALAudioEngine::CreateClipMove(const PCMFormat& format, uint8_t*& buf, size_t len, AudioClipFlag flags, const char* group_name) noexcept -> ALHandle {
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
        auto hr = S_OK;
        // 创建source
        if (SUCCEEDED(hr)) {
            hr = m_pXAudio2Engine->CreateSourceVoice(
                &real->source_voice,
                &real->wave,
                0, XAUDIO2_DEFAULT_FREQ_RATIO,
                real, nullptr, nullptr
                );
        }
        // 设置组别
        if (SUCCEEDED(hr)) {
            hr = this->set_clip_group(*real, group_name);
        }
        // 提交缓冲区
        if (SUCCEEDED(hr)) {
            XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
            buffer.pAudioData = real->audio_data;
            buffer.AudioBytes = static_cast<UINT32>(real->buffer_length);
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
    return reinterpret_cast<ALHandle>(real);
}


// 创建片段
auto WrapAL::CALAudioEngine::CreateClip(const PCMFormat & format, const uint8_t* src, size_t size, AudioClipFlag config, const char* group_name) noexcept ->ALHandle {
    uint8_t* new_src = reinterpret_cast< uint8_t*>(::malloc(size));
    if (new_src) {
        ::memcpy(new_src, src, size);
        return this->CreateClipMove(format, new_src, size, config, group_name);
    }
    else {
        this->configure->OutputError(L"Out of memory");
    }
    return ALInvalidHandle;
}

// 摧毁指定片段
bool WrapAL::CALAudioEngine::ac_destroy(ALHandle id) noexcept {
    assert(id != ALInvalidHandle);
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
bool WrapAL::CALAudioEngine::ac_play(ALHandle id) noexcept {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        clip->source_voice->Start(0);
        clip->playing = true;
        return true;
    }
    return false;
}

// 暂停指定片段
bool WrapAL::CALAudioEngine::ac_pause(ALHandle id) noexcept{
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        clip->source_voice->Stop(0);
        clip->playing = false;
        return true;
    }
    return false;
}

// 音频定位
bool WrapAL::CALAudioEngine::ac_seek(ALHandle id, float pos) noexcept {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
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
            buffer.AudioBytes = static_cast<UINT32>(clip->buffer_length);
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
auto WrapAL::CALAudioEngine::ac_tell(ALHandle id) noexcept -> float {
    assert(id != ALInvalidHandle);
    float pos = 0.0f;
    // OK
    if (id != ALInvalidHandle) {
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
auto WrapAL::CALAudioEngine::ac_duration(ALHandle id) noexcept -> float {
    assert(id != ALInvalidHandle);
    float duration = 0.f;
    // OK
    if (id != ALInvalidHandle) {
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
auto WrapAL::CALAudioEngine::ac_volume(ALHandle id, float volume) noexcept -> float {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
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
auto WrapAL::CALAudioEngine::ac_ratio(ALHandle id, float ratio) noexcept -> float {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
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
auto WrapAL::CALAudioEngine::Volume(float volume) noexcept -> float {
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

// find group by group name
auto WrapAL::CALAudioEngine::GetGroup(const char* name) noexcept ->CALAudioSourceGroup {
    return CALAudioSourceGroup(reinterpret_cast<ALHandle>(this->find_group(name)));
}

// 获取组名称
auto WrapAL::CALAudioEngine::ag_name(ALHandle id) const noexcept -> const char* {
    // 句柄有效?
    if (id != ALInvalidHandle) {
        return reinterpret_cast<AudioSourceGroupReal*>(id)->name;
    }
    // TOP-LEVEL!
    return "TOPLEVEL";
}

// 设置或获取组别音量
auto WrapAL::CALAudioEngine::ag_volume(ALHandle group_id, float volume) noexcept -> float {
    IXAudio2Voice* voice = nullptr;
    if (group_id != ALInvalidHandle) {
        auto* group = reinterpret_cast<AudioSourceGroupReal*>(group_id);
        assert(group->voice);
        voice = group->voice;
    }
    else {
        voice = m_pMasterVoice;
    }
    // 设置
    if (voice) {
        // Get
        if (volume < 0.f) {
            voice->GetVolume(&volume);
        }
        // Set
        else {
            voice->SetVolume(volume);
        }
    }
    return volume;
}

// 获取组指针
auto WrapAL::CALAudioEngine::find_group(const char* name) noexcept->AudioSourceGroupReal* {
    // 循环
    for (size_t i = 0u; i < m_cGroupCount; ++i) {
        if (!::strncmp(m_aGroup[i].name, name, GroupNameMaxLength)) {
            return m_aGroup + i;
        }
    }
    return nullptr;
}

// 设置clip的组ID
auto WrapAL::CALAudioEngine::set_clip_group(AudioSourceClipReal& clip, const char* group_name) noexcept->HRESULT {
    assert(clip.source_voice);
    // 参数无效
    if (!clip.source_voice) return E_INVALIDARG;
    // 参数将就
    clip.group = nullptr;
    if (!group_name || !*group_name) return S_FALSE;
    // 查找clip组别
    auto group_id = this->find_group(group_name);
    // 没有就添加组别
    if (!group_id) {
        // 满了?
        if (m_cGroupCount == GroupMaxSize) return S_FALSE;
        XAUDIO2_VOICE_DETAILS details = { 0 };
        m_pMasterVoice->GetVoiceDetails(&details);
        // 创建submix
        auto hr = m_pXAudio2Engine->CreateSubmixVoice(
            &m_aGroup[m_cGroupCount].voice,
            details.InputChannels,
            details.InputSampleRate,
            0, 0,
            nullptr,
            nullptr
            );
        // 失败
        assert(SUCCEEDED(hr));
        if (FAILED(hr)) return S_FALSE;
        group_id = m_aGroup + m_cGroupCount;
        ::strncpy(group_id->name, group_name, GroupNameMaxLength);
        ++m_cGroupCount;
    }
    // 设置组别
    clip.group = group_id;
    // 设置输出链
    XAUDIO2_SEND_DESCRIPTOR descriptors[] = {
        { 0, group_id->voice }
    };
    XAUDIO2_VOICE_SENDS sends = {
        sizeof(descriptors)/sizeof(*descriptors),
        descriptors
    };
    auto hr = clip.source_voice->SetOutputVoices(&sends);
    assert(SUCCEEDED(hr));
    return hr;
}

