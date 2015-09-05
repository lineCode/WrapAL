#include "wrapal.h"
#include <mmdeviceapi.h>

// WRAPAL: DEFINE_GUID
#define WRAPAL_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    static const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

// WRAPAL: DEFINE_PROPERTYKEY
#define WRAPAL_DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) \
    static const PROPERTYKEY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }

/*
// IID_IAudioClient
WRAPAL_DEFINE_GUID(IID_IAudioClient, 0x1cb9ad4c, 0xdbfa, 0x4c32, 0xb1, 0x78, 0xc2, 0xf5, 0x68, 0xa7, 0x03, 0xb2);
// IID_IAudioCaptureClient
WRAPAL_DEFINE_GUID(IID_IAudioCaptureClient, 0xc8adbd64, 0xe71e, 0x48a0, 0xa4, 0xde, 0x18, 0x5c, 0x39, 0x5c, 0xd3, 0x17);
*/

// WrapAL 命名空间
namespace WrapAL {
    // CLSID_MMDeviceEnumerator
    WRAPAL_DEFINE_GUID(CLSID_MMDeviceEnumerator, 0xBCDE0395, 0xE52F, 0x467C,
        0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E);
    // IID_IMMDeviceEnumerator
    WRAPAL_DEFINE_GUID(IID_IMMDeviceEnumerator, 0xA95664D2, 0x9614, 0x4F35,
        0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6);
    // PKEY_Device_FriendlyName
    WRAPAL_DEFINE_PROPERTYKEY(PKEY_Device_FriendlyName,0xa45c254e, 0xdf1c, 0x4efd, 
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
    // 自动释放?
    if (this->flags & WrapAL::Flag_AutoDestroyEOP) {
        this->playing = false;
        return;
    }
    // 流模式 + 无线轮回
    if (this->flags & (WrapAL::Flag_StreamingReading | WrapAL::Flag_LoopInfinite)) {
        // = 重置
        this->stream->Seek(0);
    }
    else {
        this->playing = false;
    }
}


// 缓冲区结束
void WrapAL::AudioSourceClipReal::OnBufferEnd(void* pBufferContext) noexcept {
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
    return this->ProcessBufferData(buffer, stream->ReadNext(StreamingBufferSize, data) != StreamingBufferSize);
}


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
    // 提前声明
    WrapAL::AudioDeviceInfo devices_info[WrapAL::DeviceMaxCount];
    UINT create_flags = 0;
    UINT32 device_count = 0;
    // 载入 XAudio2 动态链接库
    if (SUCCEEDED(hr)) {
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
#ifdef _DEBUG
        create_flags |= XAUDIO2_DEBUG_ENGINE;
#endif
        const auto* const file_name = L"XAudio2_7.dll";
        if ((m_hXAudio2 = ::LoadLibraryW(file_name))) {
            // 设置API等级
            m_lvAPI = APILevel::Level_XAudio2_7;
            // 创建
            CALAudioEngine::XAudio2Create = [](IXAudio2** ppXAudio2, UINT32 flags, XAUDIO2_PROCESSOR XAudio2Processor) noexcept {
                IXAudio2* pXAudio2 = nullptr;
                // 创建实例
                HRESULT hr = ::CoCreateInstance(
                    (flags & XAUDIO2_DEBUG_ENGINE) ? CLSID_XAudio2Debug : CLSID_XAudio2,
                    nullptr,
                    CLSCTX_INPROC_SERVER,
                    IID_IXAudio2,
                    reinterpret_cast<void**>(&pXAudio2)
                    );
                // 初始化
                if (SUCCEEDED(hr)) {
                    hr = pXAudio2->Initialize(flags, XAudio2Processor);
                    // OK!
                    if (SUCCEEDED(hr)) {
                        *ppXAudio2 = pXAudio2;
                    }
                    else {
                        pXAudio2->Release();
                    }
                }
                return hr;
            };
        }
        // 没有找到
        else {
            hr = E_FAIL;
            std::swprintf(error, ErrorInfoLength,
                this->GetRuntimeMessage(Message_NeedDxRuntime),
                __FUNCTION__
                );
        }
#else
        // XAudio2.9
#ifdef WRAPAL_DEBUG_XAUDIO2_9
        const auto* file_name = L"XAudio2_9D.dll";
        if ((m_hXAudio2 = ::LoadLibraryW(file_name))) {
            m_lvAPI = APILevel::Level_XAudio2_9;
            create_flags |= XAUDIO2_9_DEBUG_ENGINE;
        }
#else
        const auto* file_name = L"XAudio2_9.dll";
        if ((m_hXAudio2 = ::LoadLibraryW(file_name))) {
            m_lvAPI = APILevel::Level_XAudio2_9;
        }
#endif
        // XAudio2.8
        else if ((m_hXAudio2 = ::LoadLibraryW((file_name = L"XAudio2_8.dll")))) {
            m_lvAPI = APILevel::Level_XAudio2_8;
        }
        // 有效
        if (m_hXAudio2) {
            WrapAL::LoadProc(XAudio2Create, m_hXAudio2, "XAudio2Create");
            WrapAL::LoadProc(X3DAudioInitialize, m_hXAudio2, "X3DAudioInitialize");
        }
        // 没有找到
        else {
            this->FormatErrorFoF(error, __FUNCTION__, file_name);
            hr = E_FAIL;
        }
#endif
    }
    // 检查是否自动刷新
    if (SUCCEEDED(hr) && this->configure->IsAutoUpdate()) {
#ifdef WRAPAL_SAME_THREAD_UPDATE
        assert(!"you must undef 'WRAPAL_SAME_THREAD_UPDATE' if want auto-update");
        hr = E_ABORT;
#else

#endif
        assert(!"noimpl");
    }
    // 创建 XAudio2 引擎
    if (SUCCEEDED(hr)) {
        assert(!m_pXAudio2Engine && "m_pXAudio2 must be null");
        hr = CALAudioEngine::XAudio2Create(&m_pXAudio2Engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
    }
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
    // 获取数量设备
    if (SUCCEEDED(hr)) {
        hr = m_pXAudio2Engine->GetDeviceCount(&device_count);
    }
    // 获取设备信息
    if (SUCCEEDED(hr)) {
        for (UINT32 i = 0; i < device_count; ++i) {
            if (SUCCEEDED(hr)) {
                hr = m_pXAudio2Engine->GetDeviceDetails(i, &devices_info[i].details);
            }
        }
    }
#else
    // 枚举输出
    IMMDeviceEnumerator* enumerator = nullptr;
    IMMDeviceCollection * devices = nullptr;
    // 获取枚举器
    if (SUCCEEDED(hr)) {
        hr = ::CoCreateInstance(
            CLSID_MMDeviceEnumerator, 
            nullptr,
            CLSCTX_ALL, 
            IID_IMMDeviceEnumerator, 
            reinterpret_cast<void**>(&enumerator)
            );
    }
    // 获取输出设备集合
    if (SUCCEEDED(hr)) {
        hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices);
    }
    // 获取输出设备数量
    if (SUCCEEDED(hr)) {
        hr = devices->GetCount(&device_count);
    }
    // 获取设备信息
    if (SUCCEEDED(hr)) {
        // 太大
        if (device_count > DeviceMaxCount) {
            device_count = DeviceMaxCount;
        }
        for (UINT i = 0; i < device_count; ++i) {
            // 错误
            if (FAILED(hr)) { device_count = i; break; }
            // 当前
            auto& info = devices_info[i];
            // 数据信息
            IPropertyStore *propStore = nullptr;
            IMMDevice* device = nullptr;
            ::PropVariantInit(&info.id);
            ::PropVariantInit(&info.name);
            // 获取设备
            hr = devices->Item(i, &device);
            // 信息商店
            if (SUCCEEDED(hr)) {
                hr = device->OpenPropertyStore(STGM_READ, &propStore);
            }
            // 路径名称
            if (SUCCEEDED(hr)) {
                hr = propStore->GetValue(PKEY_AudioEndpoint_Path, &info.id);
            }
            // 友好名称
            if (SUCCEEDED(hr)) {
                hr = propStore->GetValue(PKEY_Device_FriendlyName, &info.name);
            }
            // 断言检查
            if (SUCCEEDED(hr)) {
                assert(info.id.vt == VT_LPWSTR && info.name.vt == VT_LPWSTR);
            }
            // 扫尾
            WrapAL::SafeRelease(propStore);
            WrapAL::SafeRelease(device);
        }
    }
#endif
    // 创建 Mastering Voice 接口
    if (SUCCEEDED(hr)) {
        // 选择设备
        auto index = this->configure->ChooseDevice(devices_info, device_count);
        auto device_id = index >= device_count ? nullptr : devices_info[index].Id();
        auto device_namme = index >= device_count ? nullptr : devices_info[index].Name();
#if defined(_DEBUG) && defined(_MSC_VER)
        {
            wchar_t buffer[4096];
            std::swprintf(
                buffer, 4096, 
                L"WrapAL Debug<%S>:    Choose Device:\r\n\t\t"
                L"[  id: %ls]\r\n\t\t[name: %ls]\r\n",
                __FUNCTION__,
                device_id ? device_id : L"<DEFAULT>",
                device_namme ? device_namme : L"<DEFAULT>"
                );
            ::OutputDebugStringW(buffer);
        }
#endif
        // 创建
        hr = m_pXAudio2Engine->CreateMasteringVoice(
            &m_pMasterVoice,
            XAUDIO2_DEFAULT_CHANNELS,
            XAUDIO2_DEFAULT_SAMPLERATE,
            create_flags, 
#ifdef WRAPAL_XAUDIO2_7_SUPPORT
            ((index >= device_count) ? 0 : index),
#else
            device_id,
#endif
            nullptr
            );
        device_id = device_namme = nullptr;
    }
#ifndef WRAPAL_XAUDIO2_7_SUPPORT
    // 扫尾
    for (UINT i = 0; i < device_count; ++i) {
        ::PropVariantClear(&devices_info[i].id);
        ::PropVariantClear(&devices_info[i].name);
    }
    WrapAL::SafeRelease(enumerator);
    WrapAL::SafeRelease(devices);
#endif
    // 初始化X3DAudio
    if (SUCCEEDED(hr)) {
        /*DWORD mask;
        X3DAUDIO_HANDLE instance;
        // 获取通道掩码
        hr = m_pMasterVoice->GetChannelMask(&mask);
        // 初始化 X3D
        if (SUCCEEDED(hr)) {
            hr = CALAudioEngine::X3DAudioInitialize(mask, X3DAUDIO_SPEED_OF_SOUND, instance);
        }*/
    }
    // 获取libmpg123.dll句柄
    if (SUCCEEDED(hr)) {
        wchar_t path[MAX_PATH]; path[0] = 0;
        this->configure->GetLibmpg123Path(path);
        // 载入
        force_cast(this->libmpg123) = ::LoadLibraryW(path);
        // 初始化libmpg123
        if (this->libmpg123) {
            Mpg123::Init(this->libmpg123);
            Mpg123::mpg123_init();
        }
        // 没有 libmpg123.dll 依然允许运行, 提供了路径则报错
        else if(path[0]){
            std::swprintf(error, ErrorInfoLength,
                this->GetRuntimeMessage(Message_NoLibmpg123),
                __FUNCTION__, path
                );
        }
    }
    // 报错
    if (!error[0] && FAILED(hr)) {
        this->FormatErrorHR(error, __FUNCTION__, hr);
    }
    // 输出错误
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
    m_acAllocator.Release([](AudioSourceClipReal* real) { real->Release(); });
#endif
    // 释放
    for (auto& group : m_aGroup) { group.Release(); }
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
auto WrapAL::CALAudioEngine::CreateClip(XALAudioStream* stream, AudioClipFlag flags, const char* group_name) noexcept -> ALHandle {
    assert(stream && "bad argument");
    wchar_t error[ErrorInfoLength]; error[0] = 0;
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
                    hr = this->create_source_voice(*real, group_name);
                }
                // 输入数据
                for (unsigned int i = 0; i < StreamingBufferCount-1; ++i) {
                    if (SUCCEEDED(hr)) {
                        hr = real->LoadAndBufferData(i);
                    }
                }
                // 检查错误
                if (FAILED(hr)) {
                    this->FormatErrorHR(error, __FUNCTION__, hr);
                }
            }
            // 错误
            else {
                this->FormatErrorOOM(error, __FUNCTION__);
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
                this->FormatErrorOOM(error, __FUNCTION__);
            }
        }
    }
    // 有错误的情况
    if (error[0]) {
        // 显示错误信息
        this->configure->OutputError(error);
    }
    return id;
}

// 创建音频片段
auto WrapAL::CALAudioEngine::CreateClip(EncodingFormat format, const wchar_t* file_path, AudioClipFlag flags, const char* group_name) noexcept ->ALHandle {
    // 创建音频流
    auto file_stream = this->CreatStreamFromFile(file_path);
    // 内存不足?
    if (!file_stream) {
        this->OutputErrorOOM(__FUNCTION__);
        return ALHandle(ALInvalidHandle);
    }
    // 嫁接
    return this->CreateClip(format, file_stream, flags, group_name);
}

// 创建音频片段
auto WrapAL::CALAudioEngine::CreateClip(EncodingFormat format, IALFileStream* file_stream, AudioClipFlag flags, const char* group_name) noexcept ->ALHandle {
    assert(file_stream && "bad argument");
    ALHandle clip(ALInvalidHandle);
    // 错误(已报错)
    if (!file_stream->OK()) {
        file_stream->Release();
        return clip;
    }
    // 创建音频流
    auto audio_stream = this->configure->CreateAudioStream(format, file_stream);
    if (audio_stream) {
        clip = this->CreateClip(audio_stream, flags, group_name);
        // 安全释放
        CALAudioSourceClip clip_handle(clip);
        if (!clip_handle->stream) {
            WrapAL::SafeRelease(audio_stream);
        }
    }
    // 出现错误
    else {
        this->OutputErrorLast(__FUNCTION__);
    }
    return clip;
}

// 创建音频片段
auto WrapAL::CALAudioEngine::CreateClipMove(const AudioFormat& format, uint8_t*& buf, size_t len, AudioClipFlag flags, const char* group_name) noexcept -> ALHandle {
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
            hr = this->create_source_voice(*real, group_name);
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
            this->OutputErrorHR(__FUNCTION__, hr);
        }
    }
    return reinterpret_cast<ALHandle>(real);
}


// 创建片段
auto WrapAL::CALAudioEngine::CreateClip(const AudioFormat & format, const uint8_t* src, size_t size, AudioClipFlag config, const char* group_name) noexcept ->ALHandle {
    uint8_t* new_src = reinterpret_cast< uint8_t*>(::malloc(size));
    if (new_src) {
        ::memcpy(new_src, src, size);
        return this->CreateClipMove(format, new_src, size, config, group_name);
    }
    else {
        this->OutputErrorOOM(__FUNCTION__);
    }
    return ALInvalidHandle;
}

// 摧毁指定片段(正式)
void WrapAL::CALAudioEngine::destroy_clip_real(AudioSourceClipReal* clip) noexcept {
    assert(clip && "bad argument");
    assert(clip->source_voice);
    if (clip->source_voice) {
        clip->Release();
    }
    m_acAllocator.Free(clip);
#ifdef _DEBUG
    m_listAC.remove(clip);
#endif
}

// 摧毁指定片段
bool WrapAL::CALAudioEngine::ac_destroy(ALHandle id) noexcept {
    assert(id != ALInvalidHandle);
    auto clip = reinterpret_cast<AudioSourceClipReal*>(id);
    // 自动释放?
    assert(!(clip->flags & Flag_AutoDestroyEOP) && "cannot destroy clip with Flag_AutoDestroyEOP");
    this->destroy_clip_real(clip);
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

// namesapce!
namespace WrapAL {
    // stop clip anyway
    WRAPAL_NOINLINE auto StopClipAnyway(ALHandle clip_id) {
        assert(clip_id != ALInvalidHandle);
        if (clip_id != ALInvalidHandle) {
            auto clip = reinterpret_cast<AudioSourceClipReal*>(clip_id);
            clip->source_voice->Stop(0);
            clip->source_voice->FlushSourceBuffers();
            clip->playing = false;
            clip->end_of_buffer = false;
        }
    }
}

#ifdef WRAPAL_IN_PLAN
// 利用文件重建片段
bool WrapAL::CALAudioEngine::ac_recreate(ALHandle clip_id, const wchar_t* file_name) noexcept {
    assert(clip_id != ALInvalidHandle && file_name);
    // 创建文件流
    auto file_stream = this->CreatStreamFromFile(file_name);
    // 有效
    if (file_stream) {
        return this->ac_recreate(clip_id, file_stream);
    }
    // 内存不足
    else {
        this->OutputErrorOOM(__FUNCTION__);
        return false;
    }
}

// 利用文件流重建片段
bool WrapAL::CALAudioEngine::ac_recreate(ALHandle clip_id, IALFileStream* file_stream) noexcept {
    assert(clip_id != ALInvalidHandle && file_stream);
    assert(!"noimpl");
    return false;
}

// 利用音频流重建片段
bool WrapAL::CALAudioEngine::ac_recreate(ALHandle clip_id, XALAudioStream* stream) noexcept {
    assert(clip_id != ALInvalidHandle && stream);
    if (stream) {
        WrapAL::StopClipAnyway(clip_id);
        auto clip = reinterpret_cast<AudioSourceClipReal*>(clip_id);
        // 检查新旧格式是否一致
        WAVEFORMATEX format; stream->GetFormat().MakeWave(format);
        // 不一致?
        if (std::memcmp(&format, &clip->wave, sizeof(format))) {
            clip->wave = format;
        }
    }
    assert(!"noimpl");
    return false;
}

// 利用缓冲片段重建片段
bool WrapAL::CALAudioEngine::ac_recreate(ALHandle id, uint8_t* buf, size_t len) noexcept {
    auto new_buffer = reinterpret_cast<uint8_t*>(::malloc(len));
    if (new_buffer) {
        ::memcpy(new_buffer, buf, len);
        return this->ac_recreate_move(id, new_buffer, len);
    }
    assert(!"noimpl");
    return false;
}

// 利用可移动缓冲片段重建片段
bool WrapAL::CALAudioEngine::ac_recreate_move(ALHandle id, uint8_t*& buf, size_t len) noexcept {
    assert(id && buf && "bad arguments");
    if (buf) {
        assert(!"noimpl");
        ::free(buf);
        buf = nullptr;
    }
    return false;
}
#endif

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

// 创建源音
auto WrapAL::CALAudioEngine::create_source_voice(
    AudioSourceClipReal& clip, const char* group_name) noexcept->HRESULT {
    HRESULT hr = S_OK;
    // 创建源音
    if (SUCCEEDED(hr)) {
        hr = m_pXAudio2Engine->CreateSourceVoice(
            &clip.source_voice,
            &clip.wave,
            0, XAUDIO2_DEFAULT_FREQ_RATIO,
            &clip, nullptr, nullptr
            );
    }
    // 设置组别
    if (SUCCEEDED(hr)) {
        hr = this->set_clip_group(clip, group_name);
    }
    return hr;
}

// 刷新
void WrapAL::CALAudioEngine::Update() noexcept {

}


// ----------------------------------------------------------------------------
// -------------------------------- Error Helper ------------------------------
// ----------------------------------------------------------------------------


// 输出最新错误
void WrapAL::CALAudioEngine::OutputErrorLast(const char* func_name) noexcept {
    wchar_t error[ErrorInfoLength]; error[0] = 0;
    wchar_t error_total[ErrorInfoLength];
    // 获取基本错误信息
    this->configure->GetLastErrorInfo(error);
    // 有效
    if (error[0]) {
        // 添加信息
        std::swprintf(
            error_total, ErrorInfoLength, L"<%S>: Error -> %ls\n",
            func_name, error
            );
        // 显示错误信息
        this->configure->OutputError(error_total);
    }
}

// 错误 HR 代码
WRAPAL_NOINLINE void WrapAL::CALAudioEngine::FormatErrorHR(wchar_t err_buf[], const char* func_name, HRESULT hr) noexcept {
    // 失败
    if (FAILED(hr)) {
        std::swprintf(
            err_buf, ErrorInfoLength,
            WrapALAudioEngine.configure->GetRuntimeMessage(Message_FailedHRESULT),
            func_name, hr
            );
    }
    // 成功
    else {
        err_buf[0] = 0;
    }
}

// 错误 文件没找到
WRAPAL_NOINLINE void WrapAL::CALAudioEngine::FormatErrorFoF(wchar_t err_buf[], const char* func_name, const wchar_t* file_name) noexcept {
    std::swprintf(
        err_buf, ErrorInfoLength,
        WrapALAudioEngine.configure->GetRuntimeMessage(Message_FileNotFound),
        func_name, file_name
        );
}

// 内存不足
WRAPAL_NOINLINE void WrapAL::CALAudioEngine::FormatErrorOOM(wchar_t err_buf[], const char* func_name) noexcept {
    std::swprintf(
        err_buf, ErrorInfoLength,
        WrapALAudioEngine.configure->GetRuntimeMessage(Message_OOM),
        func_name
        );
}