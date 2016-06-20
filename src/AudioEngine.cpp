#include "AudioEngine.h"
#include "AudioClip.h"
#include "AudioHandle.h"
#include <mmdeviceapi.h>

// 获取API等级字符串
auto WrapAL::GetApiLevelString(APILevel level) noexcept -> const char* {
    switch (level)
    {
    case WrapAL::APILevel::Level_Unknown:
        return "Unknown";
    case WrapAL::APILevel::Level_XAudio2_7:
        return "XAudio 2.7";
    case WrapAL::APILevel::Level_XAudio2_8:
        return "XAudio 2.8";
    case WrapAL::APILevel::Level_XAudio2_9:
        return "XAudio 2.9";
    case WrapAL::APILevel::Level_OpenAL:
        return "OpenAL";
    case WrapAL::APILevel::Level_DirectSound:
        return "Direct Sound";
    default:
        return "ERROR";
    }
}

// WRAPAL: DEFINE_GUID
#define WRAPAL_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

// WRAPAL: DEFINE_PROPERTYKEY
#define WRAPAL_DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) \
    const PROPERTYKEY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }



/// <summary>
/// Gets the group.
/// </summary>
/// <returns></returns>
auto WrapAL::CALAudioSourceClip::GetGroup() const noexcept -> CALAudioSourceGroup {
    return CALAudioSourceGroup(
        m_handle ? reinterpret_cast<ALHandle>((*this)->group) : ALInvalidHandle
    );
}

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

#ifdef _DEBUG
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
/// <returns></returns>
WrapAL::CALAudioSourceClipImpl::~CALAudioSourceClipImpl() {
    WrapAL::SafeDestroyVoice(m_pSourceVoice); 
    WrapAL::SafeRelease(m_pStream); 
    std::free(m_pAudioData); 
    m_pAudioData = nullptr;
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
    XAUDIO2_VOICE_STATE state ;
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
#ifdef _DEBUG
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

// 加工新的缓冲区
auto WrapAL::CALAudioSourceClipImpl::ProcessBufferData(XAUDIO2_BUFFER& buffer, bool eos) noexcept -> HRESULT {
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
void WrapAL::CALAudioEngine::Uninitialize() noexcept {
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
            constexpr size_t blen = StreamingBufferSize*StreamingBufferCount;
            auto buffer = reinterpret_cast<uint8_t*>(std::malloc(blen));
            auto* real = this->configure->SmallAlloc<CALAudioSourceClipImpl>();
            // 申请成功
            if (buffer && (id = reinterpret_cast<ALHandle>(real))) {
                // 置换构造
                new (real) CALAudioSourceClipImpl(
                    std::move(buffer), stream, flags,
                    StreamingBufferSize*StreamingBufferCount
                );
                // 设置
                stream->GetFormat().MakeWave(real->wave);
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
            if (buffer) {
                std::free(buffer);
                buffer = nullptr;
            }
            
        }
        // 整片读取
        else {
            auto size_in_byte = stream->GetSizeInByte();
            auto buffer = reinterpret_cast<uint8_t*>(std::malloc(size_in_byte));
            // 申请成功
            if (buffer) {
                stream->ReadNext(size_in_byte, buffer);
                stream->GetLastErrorInfo(error);
                id = this->CreateClip(stream->GetFormat(), std::move(buffer), size_in_byte, flags, group_name);
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
    if (const auto as = this->configure->CreateAudioStream(format, file_stream)) {
        clip = this->CreateClip(as, flags, group_name);
        as->Release();
    }
    // 出现错误
    else {
        this->OutputErrorLast(__FUNCTION__);
    }
    return clip;
}

// 创建音频片段
auto WrapAL::CALAudioEngine::CreateClip(
    const AudioFormat& format, 
    uint8_t*&& buf, size_t len, 
    AudioClipFlag flags, 
    const char* group_name) noexcept -> ALHandle {
    // 直接使用缓冲区不能只用流模式
    assert(!(flags & WrapAL::Flag_StreamingReading) && "directly buffer can't be streaming mode");
    auto* real = this->configure->SmallAlloc<CALAudioSourceClipImpl>();
    // 创建判断
    if (real) {
        auto oldbuf = buf;
        // 置换构造
        new (real) CALAudioSourceClipImpl(
            std::move(buf), nullptr, flags, len
        );
        // 设置
        format.MakeWave(real->wave);
        auto hr = S_OK;
        // 创建source
        if (SUCCEEDED(hr)) {
            hr = this->create_source_voice(*real, group_name);
        }
        // 提交缓冲区
        if (SUCCEEDED(hr)) {
            XAUDIO2_BUFFER buffer; ZeroMemory(&buffer, sizeof(buffer));
            buffer.pAudioData = oldbuf;
            buffer.AudioBytes = static_cast<UINT32>(len);
            hr = real->ProcessBufferData(buffer);
        }
        // 检查错误
        if (FAILED(hr)) {
            this->OutputErrorHR(__FUNCTION__, hr);
        }
    }
    // 依然有效?
    if (buf) {
        std::free(buf);
        buf = nullptr;
    }
    return reinterpret_cast<ALHandle>(real);
}


// 创建片段
auto WrapAL::CALAudioEngine::CreateClip(
    const AudioFormat & format, 
    const uint8_t* src, size_t size, 
    AudioClipFlag config, 
    const char* group_name) noexcept ->ALHandle {
    // 申请空间
    if (const auto new_src = reinterpret_cast< uint8_t*>(std::malloc(size))) {
        std::memcpy(new_src, src, size);
        return this->CreateClip(format, std::move(new_src), size, config, group_name);
    }
    else {
        this->OutputErrorOOM(__FUNCTION__);
    }
    return ALInvalidHandle;
}

// 摧毁指定片段
bool WrapAL::CALAudioEngine::ac_addref(ALHandle id) noexcept {
    assert(id != ALInvalidHandle);
    auto clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
    assert(clip->Check_debug());
    clip->AddRef();
    return true;
}

// 摧毁指定片段
bool WrapAL::CALAudioEngine::ac_release(ALHandle id) noexcept {
    assert(id != ALInvalidHandle);
    auto clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
    assert(clip->Check_debug());
    clip->Release();
    return true;
}

// 播放指定片段
bool WrapAL::CALAudioEngine::ac_play(ALHandle id) noexcept {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        assert(clip->Check_debug());
        clip->Play();
        return true;
    }
    return false;
}

// 暂停指定片段
bool WrapAL::CALAudioEngine::ac_pause(ALHandle id) noexcept{
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        assert(clip->Check_debug());
        clip->Stop();
        return true;
    }
    return false;
}

// 音频定位
bool WrapAL::CALAudioEngine::ac_seek(ALHandle id, float pos) noexcept {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto* clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        assert(clip->Check_debug());
        clip->Seek(pos);
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
        auto* clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        assert(clip->Check_debug());
        pos = clip->Tell();
    }
    return pos;
}

// 获取片段持续时间
auto WrapAL::CALAudioEngine::ac_duration(ALHandle id) noexcept -> float {
    assert(id != ALInvalidHandle);
    float duration = 0.f;
    // OK
    if (id != ALInvalidHandle) {
        auto* clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        duration = clip->Duration();
    }
    return duration;
}


// 设置或获取片段音量
auto WrapAL::CALAudioEngine::ac_volume(ALHandle id, float volume) noexcept -> float {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto* clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        if (volume < 0.f) volume = clip->GetVolume();
        else clip->SetVolume(volume);
    }
    return volume;
}


// 设置或获取片段回放速率
auto WrapAL::CALAudioEngine::ac_ratio(ALHandle id, float ratio) noexcept -> float {
    assert(id != ALInvalidHandle);
    // OK
    if (id != ALInvalidHandle) {
        auto* clip = reinterpret_cast<CALAudioSourceClipImpl*>(id);
        if (ratio < 0.f) ratio = clip->GetFrequencyRatio();
        else clip->SetFrequencyRatio(ratio);
    }
    return ratio;
}

// namesapce!
namespace WrapAL {
    // stop clip anyway
    WRAPAL_NOINLINE auto TerminateClipAnyway(ALHandle clip_id) {
        assert(clip_id != ALInvalidHandle);
        if (clip_id != ALInvalidHandle) {
            auto clip = reinterpret_cast<CALAudioSourceClipImpl*>(clip_id);
            assert(clip->Check_debug());
            clip->Terminate();
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
bool WrapAL::CALAudioEngine::ac_recreate(ALHandle clip_id, XALAudioStream* m_pStream) noexcept {
    assert(clip_id != ALInvalidHandle && m_pStream);
    if (m_pStream) {
        WrapAL::StopClipAnyway(clip_id);
        auto clip = reinterpret_cast<CALAudioSourceClipImpl*>(clip_id);
        // 检查新旧格式是否一致
        WAVEFORMATEX format; m_pStream->GetFormat().MakeWave(format);
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
        std::memcpy(new_buffer, buf, len);
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
        std::free(buf);
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
        return reinterpret_cast<AudioSourceGroupImpl*>(id)->name;
    }
    // TOP-LEVEL!
    return "TOPLEVEL";
}

// 设置或获取组别音量
auto WrapAL::CALAudioEngine::ag_volume(ALHandle group_id, float volume) noexcept -> float {
    IXAudio2Voice* voice = nullptr;
    if (group_id != ALInvalidHandle) {
        auto* group = reinterpret_cast<AudioSourceGroupImpl*>(group_id);
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
auto WrapAL::CALAudioEngine::find_group(const char* name) noexcept ->AudioSourceGroupImpl* {
    // 循环
    for (size_t i = 0u; i < m_cGroupCount; ++i) {
        if (!::strncmp(m_aGroup[i].name, name, GroupNameMaxLength)) {
            return m_aGroup + i;
        }
    }
    return nullptr;
}

// 设置clip的组ID
auto WrapAL::CALAudioEngine::set_clip_group(CALAudioSourceClipImpl& clip, const char* group_name) noexcept ->HRESULT {
    assert(clip.HasSource());
    // 参数无效
    if (!clip.HasSource()) return E_INVALIDARG;
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
        std::strncpy(group_id->name, group_name, GroupNameMaxLength);
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
    auto hr = clip.SetOutputVoices(&sends);
    assert(SUCCEEDED(hr));
    return hr;
}

// 创建源音
auto WrapAL::CALAudioEngine::create_source_voice(
    CALAudioSourceClipImpl& clip, const char* group_name) noexcept ->HRESULT {
    HRESULT hr = S_OK;
    // 创建源音
    if (SUCCEEDED(hr)) {
        hr = clip.CreateSourceVoice(m_pXAudio2Engine);
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