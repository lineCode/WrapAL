#include "wrapal.h"

// ��Ƶ����ʼ
void WrapAL::AudioSourceClipReal::OnVoiceProcessingPassStart(UINT32 SamplesRequired) noexcept {
    if (SamplesRequired && this->playing) {
        // ��ģʽ
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

// ��Ƶ������
void WrapAL::AudioSourceClipReal::OnStreamEnd() noexcept {
    // ��ģʽ��
    if (this->flags & WrapAL::Flag_StreamingReading) {
        // �����ֻ�?
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


// ����������
void WrapAL::AudioSourceClipReal::OnBufferEnd(void * pBufferContext) noexcept {
    end_of_buffer = true; 
    if (this->flags & WrapAL::Flag_StreamingReading) {
        // todo
    }
    else {
        playing = false;
    }
}

// �ӹ��µĻ�����
auto WrapAL::AudioSourceClipReal::ProcessBufferData(XAUDIO2_BUFFER& buffer, bool eos) noexcept -> HRESULT {
    // ����������
    buffer.LoopCount = 0;
    // EOS?
    if (eos) {
        buffer.Flags = XAUDIO2_END_OF_STREAM;
    }
    // ��ģʽ
    if (flags & WrapAL::Flag_StreamingReading) {
        // todo
    }
    else {
        // �����ֻ�?
        if (flags & WrapAL::Flag_LoopInfinite) {
            buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
        }
    }
    // �ύ
    return this->source_voice->SubmitSourceBuffer(&buffer);
}

// ��ȡ��һ��
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
// ����

// ��ʼ��
auto WrapAL::CAudioEngine::Initialize(IALConfigure* config) noexcept -> HRESULT {
    wchar_t error[ErrorInfoLength]; error[0] = 0;
    HRESULT hr = S_OK;
    // ���������Ϣ
#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE
    force_cast(this->configure) = config ? config : &m_config;
#else
    force_cast(this->configure) = config;
#endif
    assert(this->configure && "none configure");
    if (!this->configure) hr = E_INVALIDARG;
    // ����XAudio��̬���ӿ�
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
    // ���� XAudio2 ����
    if (SUCCEEDED(hr)) {
        assert(!m_pXAudio2Engine && "m_pXAudio2 must be null");
        hr = CAudioEngine::XAudio2Create(&m_pXAudio2Engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
    }
    // ���� Mastering Voice �ӿ�
    if (SUCCEEDED(hr)) {
        hr = m_pXAudio2Engine->CreateMasteringVoice(
            &m_pMasterVoice,
            0, 0, 0, 0, nullptr
            );
    }
    // ��ȡlibmpg123.dll���
    if (SUCCEEDED(hr)) {
        wchar_t path[MAX_PATH]; path[0] = 0;
        this->configure->GetLibmpg123_dllPath(path);
        force_cast(this->libmpg123) = ::LoadLibraryW(path);
        // ��ʼ��libmpg123
        if (this->libmpg123) {
            Mpg123::Init(this->libmpg123);
            Mpg123::mpg123_init();
        }
        // û�� libmpg123.dll ��Ȼ��������, �ṩ��·���򱨴�
        else if(path[0]){
            ::swprintf(error, ErrorInfoLength,
                L"<%S>:libmpg123 library not found ---> %ls",
                __FUNCTION__, path
                );
        }
    }
    // ����
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


// ����ʼ��
void WrapAL::CAudioEngine::UnInitialize() noexcept {
    // �ݻ�������ЧƬ��
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
    // �ͷ�dll�ļ�
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

// ������ƵƬ��
auto WrapAL::CAudioEngine::CreateClip(IALAudioStream* stream, AudioClipFlag flags) noexcept -> ClipID {
    assert(stream && "bad argument");
    wchar_t error[ErrorInfoLength];
    wchar_t error_total[ErrorInfoLength]; error[0] = 0;
    ClipID id = ClipIDError;
    // ��ȡ������Ϣ
    if (!stream->GetLastErrorInfo(error)) {
        // ��ģʽ?
        if (flags & WrapAL::Flag_StreamingReading) {
            auto buffer = reinterpret_cast<uint8_t*>(::malloc(StreamingBufferSize*StreamingBufferCount));
            AudioSourceClipReal* real = m_acAllocator.Alloc();
#ifdef _DEBUG
            if (real) m_listAC.push_back(real);
#endif
            // ����ɹ�
            if (buffer && (id = reinterpret_cast<ClipID>(real))) {
                // �û�����
                new(real) AudioSourceClipReal();
                // ����
                stream->GetFormat().MakeWave(real->wave);
                real->stream = stream;
                real->audio_data = buffer;
                buffer = nullptr;
                real->flags = flags;
                real->buffer_length = StreamingBufferSize*StreamingBufferCount * 2;
                // ����source
                auto hr = m_pXAudio2Engine->CreateSourceVoice(
                    &real->source_voice,
                    &real->wave,
                    0, XAUDIO2_DEFAULT_FREQ_RATIO,
                    real, nullptr, nullptr
                    );
                // ��������
                for (unsigned int i = 0; i < StreamingBufferCount-1; ++i) {
                    if (SUCCEEDED(hr)) {
                        hr = real->LoadAndBufferData(i);
                    }
                }
                // ������
                if (FAILED(hr)) {
                    ::swprintf(error, ErrorInfoLength, L"HRESULT code -> 0x%08X\n", hr);
                }
            }
            // ����
            else {
                ::wcscpy(error, L"Out of memory");
            }
            // �ͷ�
            if (buffer) {
                ::free(buffer);
                buffer = nullptr;
            }
            
        }
        // ��Ƭ��ȡ
        else {
            auto size_in_byte = stream->GetSizeInByte();
            auto buffer = reinterpret_cast<uint8_t*>(::malloc(size_in_byte));
            // ����ɹ�
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
    // �д�������
    if (error[0]) {
        // �����Ϣ
        ::swprintf(
            error_total, ErrorInfoLength, L"<%S>: Error -> %ls\n",
            __func__, error
            );
        // ��ʾ������Ϣ
        this->configure->OutputError(error_total);
    }
    return id;
}

// ������ƵƬ��
auto WrapAL::CAudioEngine::CreateClip(AudioFormat format, const wchar_t* file_path, AudioClipFlag flags) noexcept ->ClipID {
    ClipID clip(ClipIDError);
    IALAudioStream* stream = nullptr;
    // ������Ƶ��
    if ((stream = this->configure->CreateAudioStream(format, file_path))) {
        clip = this->CreateClip(stream, flags);
        // ��ȫ�ͷ�
        AudioSourceClip clip_handle(clip);
        if (!clip_handle->stream) {
            WrapAL::SafeRelease(stream);
        }
    }
    // ���ִ���
    else {
        wchar_t error[ErrorInfoLength]; error[0] = 0;
        wchar_t error_total[ErrorInfoLength];
        // ��ȡ����������Ϣ
        this->configure->GetLastErrorInfo(error);
        // �����Ϣ
        ::swprintf(
            error_total, ErrorInfoLength, L"<%S>: Error -> %ls\n",
            __func__, error
            );
        // ��ʾ������Ϣ
        this->configure->OutputError(error_total);
    }
    return clip;
}

// ������ƵƬ��
auto WrapAL::CAudioEngine::CreateClipMove(const PCMFormat& format, uint8_t*& buf, size_t len, AudioClipFlag flags) noexcept -> ClipID {
    // ֱ��ʹ�û���������ֻ����ģʽ
    assert(!(flags & WrapAL::Flag_StreamingReading) && "directly buffer can't be streaming mode");
    AudioSourceClipReal* real = m_acAllocator.Alloc();
#ifdef _DEBUG
    if (real) m_listAC.push_back(real);
#endif
    // �����ж�
    if (real) {
        // �û�����
        new(real) AudioSourceClipReal();
        // ������Ϣ
        real->flags = flags;
        // ����
        format.MakeWave(real->wave);
        real->buffer_length = len;
        real->audio_data = buf;
        buf = nullptr;
        // ����source
        auto hr = m_pXAudio2Engine->CreateSourceVoice(
            &real->source_voice,
            &real->wave,
            0, XAUDIO2_DEFAULT_FREQ_RATIO,
            real, nullptr, nullptr
            );
        // �ύ������
        if (SUCCEEDED(hr)) {
            XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
            buffer.pAudioData = real->audio_data;
            buffer.AudioBytes = real->buffer_length;
            hr = real->ProcessBufferData(buffer);
        }
        // ������
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


// ����Ƭ��
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

// �ݻ�ָ��Ƭ��
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

// ����ָ��Ƭ��
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

// ��ָͣ��Ƭ��
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

// ��Ƶ��λ
bool WrapAL::CAudioEngine::ac_seek(ClipID id, float pos) noexcept {
    assert(id != ClipIDError);
    if (id != ClipIDError) {
        auto* __restrict clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        // ��������
        bool playing = clip->playing;
        clip->source_voice->Stop(0);
        clip->source_voice->FlushSourceBuffers();
        // ������λ��
        uint32_t pos_in_sample = static_cast<uint32_t>(static_cast<double>(pos) *
            static_cast<double>(clip->wave.nSamplesPerSec));
        // ��ģʽ?
        if (clip->flags & WrapAL::Flag_StreamingReading) {
            // ����
            clip->stream->Seek(pos_in_sample *clip->wave.nBlockAlign);
            for (unsigned int i = 0; i < StreamingBufferCount - 1; ++i) {
                clip->LoadAndBufferData(i);
            }
        }
        // ֱ�Ӳ���
        else {
            XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
            buffer.PlayBegin = pos_in_sample;
            buffer.pAudioData = clip->audio_data;
            buffer.AudioBytes = clip->buffer_length;
            clip->ProcessBufferData(buffer);
        }
        // ����?
        if (playing) {
            clip->source_voice->Start();
        }
        return true;
    }
    return false;
}

// ��ȡƬ��λ��
auto WrapAL::CAudioEngine::ac_tell(ClipID id) noexcept -> float {
    assert(id != ClipIDError);
    float pos = 0.0f;
    if (id != ClipIDError) {
        auto* __restrict clip = reinterpret_cast<AudioSourceClipReal*>(id);
        assert(clip->source_voice);
        XAUDIO2_VOICE_STATE state ;
        state.SamplesPlayed = 0;
        clip->source_voice->GetState(&state);
        // ����
        pos = static_cast<float>(static_cast<double>(state.SamplesPlayed) /
            static_cast<double>(clip->wave.nSamplesPerSec));
    }
    return pos;
}

// ��ȡƬ�γ���ʱ��
auto WrapAL::CAudioEngine::ac_duration(ClipID id) noexcept -> float {
    assert(id != ClipIDError);
    float duration = 0.f;
    if (id != ClipIDError) {
        auto* __restrict clip = reinterpret_cast<AudioSourceClipReal*>(id);
        register uint32_t length;
        // ��ȡ�ֽڳ���
        if (clip->flags & WrapAL::Flag_StreamingReading) {
            length = clip->stream->GetSizeInByte();
        }
        else {
            length = clip->buffer_index;
        }
        // ����ʱ��
        duration = static_cast<float>(static_cast<double>(length)/
            static_cast<double>(clip->wave.nAvgBytesPerSec)
            );
    }
    return duration;
}


// ���û��ȡƬ������
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


// ���û��ȡƬ�λط�����
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


// ���û��ȡ������
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

