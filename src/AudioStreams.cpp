#include "wrapal.h"

// Memory leak detector
#if defined(_DEBUG) && defined(_MSC_VER) && defined(WRAPAL_MEMDEBUG_IN_MSC)
struct MemoryLeakDetector {
    // ctor
    MemoryLeakDetector() {
        ::_CrtMemCheckpoint(memstate + 0);
        constexpr int sa = sizeof(_CrtMemState);
    }
    // dtor
    ~MemoryLeakDetector() {
        ::_CrtMemCheckpoint(memstate + 1);
        if (::_CrtMemDifference(memstate + 2, memstate + 0, memstate + 1)) {
            ::_CrtDumpMemoryLeaks();
            assert(!"OOps! Memory leak detected");
        }
    }
    // mem state
    _CrtMemState memstate[3];
} g_detector;
#endif

// Ogg Vorbis
#include "../3rdparty/libvorbis/include/vorbis/codec.h"
#include "../3rdparty/libvorbis/include/vorbis/vorbisfile.h"



#ifdef WRAPAL_INCLUDE_DEFAULT_CONFIGURE

// wrapal namespace
namespace WrapAL {
    // default error code for audio stream
    enum class DefErrorCode : uint32_t {
        // Ok, No Error
        Code_Ok = 0,
        // File Not Found
        Code_FileNotFound,
        // Illegal File
        Code_IllegalFile,
        // Unsupported Format
        Code_UnsupportedFormat,
        // Decode Error
        Code_DecodeError,
        // Out of memory
        Code_OutOfMemory,
    };
#define alignedsize(n, a) (((((n)-1)/(a))+1)*(a))
    // Basic Audio Stream
    class WRAPAL_NOVTABLE BasicAudioStream : public XALAudioStream {
    public:
        // ctor
        BasicAudioStream(const wchar_t*  path) noexcept :
            m_pFile(::_wfopen(path, L"rb")),
            m_code(m_pFile ? DefErrorCode::Code_Ok : DefErrorCode::Code_FileNotFound) {}
    public: // interface impl for XALAudioStream
            // release this
        virtual auto Release() noexcept->int32_t override;
        // get last error infomation, return false if no error
        virtual auto GetLastErrorInfo(wchar_t info[/*ErrorInfoLength*/])noexcept->bool override;
    protected:
        // audio file
        FILE*               m_pFile;
        // last error code
        DefErrorCode        m_code;
        // postion
        uint32_t            m_pos = 0;
    };
    // Audio Stream for wave file(PCM only)
    class  WavAudioStream final : public BasicAudioStream {
        // super class define
        using Super = BasicAudioStream;
    public:
        // ctor
        WavAudioStream(const wchar_t*) noexcept;
    public: // interface impl for XALAudioStream
            // release this
            //virtual auto Release() noexcept->int32_t override;
            // seek stream in byte, return false if out of range
        virtual auto Seek(uint32_t) noexcept->bool override;
        // read stream, return false if end of file/stream
        virtual auto ReadNext(uint32_t l, uint8_t* b) noexcept->bool override { m_pos += l; return ::fread(b, l, 1, m_pFile) > 0; }
    private:
        // zero postion offset
        uint32_t            m_zeroPosOffset = 0;
    };
    // Audio Stream for ogg file
    class  OggAudioStream final : public BasicAudioStream {
        // super class define
        using Super = BasicAudioStream;
    public:
        // ctor
        OggAudioStream(const wchar_t*) noexcept;
    public: // interface impl for XALAudioStream
            // release this
        virtual auto Release() noexcept->int32_t override;
        // seek stream in byte, return false if out of range
        virtual auto Seek(uint32_t) noexcept->bool override;
        // read stream, return false if end of file/stream
        virtual auto ReadNext(uint32_t, uint8_t*) noexcept->bool override;
    private:
        // ogg file
        OggVorbis_File          m_ovfile;
    };
    // Audio Stream for mp3 file
    class  Mp3AudioStream final : public BasicAudioStream {
        // super class define
        using Super = BasicAudioStream;
    public:
        // ctor
        Mp3AudioStream(const wchar_t*) noexcept;
    public: // interface impl for XALAudioStream
            // release this
        virtual auto Release() noexcept->int32_t override;
        // seek stream in byte, return false if out of range
        virtual auto Seek(uint32_t) noexcept->bool override;
        // read stream, return false if end of file/stream
        virtual auto ReadNext(uint32_t, uint8_t*) noexcept->bool override;
    private:
        // mpg123 file
        mpg123_handle*         m_hMpg123 = nullptr;
    };
    // total buffer
    union DefAudioStreamBuffer {
        // using buffer
        size_t              buf[1];
        // buffer for wave
        uint8_t             wav[alignedsize(sizeof(WavAudioStream), sizeof(void*))];
        // buffer for ogg vorbis
        uint8_t             ogg[alignedsize(sizeof(OggAudioStream), sizeof(void*))];
        // buffer for mpg123
        uint8_t             mp3[alignedsize(sizeof(Mp3AudioStream), sizeof(void*))];
    };
    // audio stream pool
    ObjectPool<DefAudioStreamBuffer, AudioStreamBucketSize> ASPool;
}

// mpg123 编码
enum ENCODE_ENUM {
    ENCODE_ENUM_8 = 0x00f, ENCODE_ENUM_16 = 0x040, ENCODE_ENUM_24 = 0x4000, ENCODE_ENUM_32 = 0x100,
    ENCODE_ENUM_SIGNED = 0x080, ENCODE_ENUM_FLOAT = 0xe00,
    ENCODE_ENUM_SIGNED_16 = (ENCODE_ENUM_16 | ENCODE_ENUM_SIGNED | 0x10),
    ENCODE_ENUM_UNSIGNED_16 = (ENCODE_ENUM_16 | 0x20), ENCODE_ENUM_UNSIGNED_8 = 0x01,
    ENCODE_ENUM_SIGNED_8 = (ENCODE_ENUM_SIGNED | 0x02), ENCODE_ENUM_ULAW_8 = 0x04,
    ENCODE_ENUM_ALAW_8 = 0x08, ENCODE_ENUM_SIGNED_32 = ENCODE_ENUM_32 | ENCODE_ENUM_SIGNED | 0x1000,
    ENCODE_ENUM_UNSIGNED_32 = ENCODE_ENUM_32 | 0x2000,
    ENCODE_ENUM_SIGNED_24 = ENCODE_ENUM_24 | ENCODE_ENUM_SIGNED | 0x1000,
    ENCODE_ENUM_UNSIGNED_24 = ENCODE_ENUM_24 | 0x2000,
    ENCODE_ENUM_FLOAT_32 = 0x200,
    ENCODE_ENUM_FLOAT_64 = 0x400,
    ENCODE_ENUM_ANY = (ENCODE_ENUM_SIGNED_16 | ENCODE_ENUM_UNSIGNED_16 | ENCODE_ENUM_UNSIGNED_8
    | ENCODE_ENUM_SIGNED_8 | ENCODE_ENUM_ULAW_8 | ENCODE_ENUM_ALAW_8
    | ENCODE_ENUM_SIGNED_32 | ENCODE_ENUM_UNSIGNED_32
    | ENCODE_ENUM_SIGNED_24 | ENCODE_ENUM_UNSIGNED_24
    | ENCODE_ENUM_FLOAT_32 | ENCODE_ENUM_FLOAT_64)
};

// 基本释放
auto WrapAL::BasicAudioStream::Release() noexcept -> int32_t {
    if (m_pFile) {
        ::fclose(m_pFile);
        m_pFile = nullptr;
    }
    ASPool.Free(reinterpret_cast<WrapAL::DefAudioStreamBuffer*>(this));
    return 0;
}

// 获取最近一次错误
bool WrapAL::BasicAudioStream::GetLastErrorInfo(wchar_t des_info[]) noexcept {
    if (m_code != DefErrorCode::Code_Ok) {
        const wchar_t* info = nullptr;
        switch (m_code)
        {
        /*case WrapAL::DefErrorCode::Code_Ok:
            info = L"No Error";
            break;*/
        case WrapAL::DefErrorCode::Code_FileNotFound:
            info = L"File Not Found";
            break;
        case WrapAL::DefErrorCode::Code_IllegalFile:
            info = L"Illegal File";
            break;
        case WrapAL::DefErrorCode::Code_UnsupportedFormat:
            info = L"Unsupported Format";
            break;
        case WrapAL::DefErrorCode::Code_DecodeError:
            info = L"Decode Error";
            break;
        case WrapAL::DefErrorCode::Code_OutOfMemory:
            info = L"Out Of Memory";
            break;
        default:
            info = L"Unknown Error";
        }
        ::wcscpy(des_info, info);
        return true;
    }
    return false;
}

// WavAudioStream 构造函数
WrapAL::WavAudioStream::WavAudioStream(const wchar_t* file_path) noexcept : Super(file_path) {
    // 检查错误
    if (m_code != DefErrorCode::Code_Ok) return;
    // 解析数据结构
#pragma pack(push)
    // 保存对齐状态
#pragma pack(1)
    // WAVE 文件头
    struct {
        // RIFF 文件头
        char    szRiffID[4];        // 'R','I','F','F'
        DWORD   dwRiffSize;         // 整个wav文件大小减去 "RIFF"以及本对象(文件大小-8)
        char    szRiffFormat[4];    // 'W','A','V','E'
        // FMT 区块
        char    szFmtID[4];         // 'f','m','t', 0x20
        DWORD   dwFmtSize;          // fmt 区块大小(16?18?)
        // WAVE格式
        WORD    wFormatTag;         // format type
        WORD    nChannels;          // number of channels (i.e. mono, stereo...)
        DWORD   nSamplesPerSec;     // sample rate
        DWORD   nAvgBytesPerSec;    // for buffer estimation
        WORD    nBlockAlign;        // block size of data 
        WORD    wBitsPerSample;     // number of bits per sample of mono data
    } header;
    // FACT 块 可选 / 数据大小
    union {       
        struct {
            char    szFactID[4];    // 'f','a','c','t'
            DWORD   dwFactSize;     // fact 大小(一般为4?)
        };
        struct {
            char    szDataID[4];    // 'd','a','t','a'
            DWORD   dwDataSize;     // 数据大小
        } ;
    } fact_data;
    // 恢复对齐状态
#pragma pack(pop)
    auto code = DefErrorCode::Code_Ok;
    // 读取文件头
    if (!::fread(&header, sizeof(header), 1, m_pFile)) {
        code = DefErrorCode::Code_IllegalFile;
    }
    // 检查合法性
    if (code == DefErrorCode::Code_Ok) {
        if ((*reinterpret_cast<uint32_t*>(header.szRiffID) !=  "RIFF"_wrapal32)   ||
            (*reinterpret_cast<uint32_t*>(header.szRiffFormat) != "WAVE"_wrapal32) ||
            (*reinterpret_cast<uint32_t*>(header.szFmtID) != "fmt\x20"_wrapal32))
        code = DefErrorCode::Code_IllegalFile;
    }
    // 检查格式支持
    if (code == DefErrorCode::Code_Ok) {
        if (header.wFormatTag != WAVE_FORMAT_PCM) {
            code = DefErrorCode::Code_UnsupportedFormat;
        }
    }
    // 检查剩余部分
    if (code == DefErrorCode::Code_Ok) {
        ::fseek(m_pFile, header.dwFmtSize - 16, SEEK_CUR);
        if (!::fread(&fact_data, sizeof(fact_data), 1, m_pFile)) {
            code = DefErrorCode::Code_IllegalFile;
        }
    }
    // fact 还是 data
    if (code == DefErrorCode::Code_Ok) {
        // fact 块?
        if ((*reinterpret_cast<uint32_t*>(fact_data.szFactID) == "face"_wrapal32)) {
            ::fseek(m_pFile, fact_data.dwFactSize - 4, SEEK_CUR);
            ::fread(&fact_data, sizeof(fact_data), 1, m_pFile);
        }
        // data 块?
        if ((*reinterpret_cast<uint32_t*>(fact_data.szDataID) == "data"_wrapal32)) {
            m_cTotalSize = fact_data.dwDataSize;
            m_zeroPosOffset = ::ftell(m_pFile);
        }
        else {
            code = DefErrorCode::Code_IllegalFile;
        }
    }
    // 复制数据
    if (code == DefErrorCode::Code_Ok) {
        m_pcmFormat.nSamplesPerSec = header.nSamplesPerSec;
        m_pcmFormat.nAvgBytesPerSec = header.nAvgBytesPerSec;
        m_pcmFormat.nBlockAlign = header.nBlockAlign;
        m_pcmFormat.nChannels = header.nChannels;
    }
    m_code = code;
}

// 设置读取位置
auto WrapAL::WavAudioStream::Seek(uint32_t pos) noexcept -> bool {
    if (pos >= m_cTotalSize) return false;
    m_pos = pos;
    ::fseek(m_pFile, pos + m_zeroPosOffset, SEEK_SET);
    return true;
}

// OggAudioStream 构造函数
WrapAL::OggAudioStream::OggAudioStream(const wchar_t* file_path) noexcept : Super(file_path) {
    // 检查错误
    if (m_code != DefErrorCode::Code_Ok) return;
    if (::ov_open_callbacks(m_pFile, &m_ovfile, nullptr, 0, OV_CALLBACKS_NOCLOSE) >= 0) {
        //char **ptr = ov_comment(&ovfile, -1)->user_comments;
        vorbis_info *vi = ov_info(&m_ovfile, -1);
        // 获取声道数
        m_pcmFormat.nChannels = vi->channels;
        // 获取采样率
        m_pcmFormat.nSamplesPerSec = vi->rate;
        // 区块对齐
        m_pcmFormat.nBlockAlign = m_pcmFormat.nChannels * sizeof(int16_t);
        // bps
        m_pcmFormat.nAvgBytesPerSec = m_pcmFormat.nSamplesPerSec * m_pcmFormat.nBlockAlign;
        // 数据大小
        m_cTotalSize = static_cast<decltype(m_cTotalSize)>(::ov_pcm_total(&m_ovfile, -1)) *
            static_cast<decltype(m_cTotalSize)>(m_pcmFormat.nBlockAlign);
    }
    // 非法文件
    else {
        m_code = DefErrorCode::Code_IllegalFile;
    }
}

// OggAudioStream 释放数据
auto WrapAL::OggAudioStream::Release() noexcept -> int32_t {
    ::ov_clear(&m_ovfile);
    return Super::Release();
}

// OggAudioStream 定位
auto WrapAL::OggAudioStream::Seek(uint32_t pos) noexcept -> bool {
    if (pos >= m_cTotalSize) return false;
    auto code = ::ov_pcm_seek(&m_ovfile, pos / m_pcmFormat.nBlockAlign);
    // ERROR
    if (code) {

    }
    return true;
}

// OggAudioStream 读取数据
auto WrapAL::OggAudioStream::ReadNext(uint32_t len, uint8_t* buf) noexcept -> bool {
#ifdef _DEBUG
    uint32_t total_len = 0, target_len = len;
    bool recode = true;
    // 循环读取数据
    while (len) {
        int bitstream = 0;
        auto code = ::ov_read(&m_ovfile, reinterpret_cast<char*>(buf), len, 0, 2, 1, &bitstream);
        // EOF?
        if (!code) {
            recode = false;
            break;
        }
        // Error?
        if (code < 0) {
            m_code = DefErrorCode::Code_DecodeError;
            break;
        }
        // OK!
        else {
            len -= code;
            buf += code;
            total_len += code;
            m_pos += code;
        }
    }
    if (recode && target_len != total_len) {
        ::printf("X");
        m_code = DefErrorCode::Code_DecodeError;
    }
    return recode;
#else
    // 循环读取数据
    while (len) {
        int bitstream = 0;
        auto code = ::ov_read(&m_ovfile, reinterpret_cast<char*>(buf), len, 0, 2, 1, &bitstream);
        // EOF?
        if (!code) return false;
        // Error?
        if (code < 0) {
            m_code = DefErrorCode::Code_DecodeError;
            break;
        }
        // OK!
        else {
            len -= code;
            buf += code;
            m_pos += code;
        }
    }
    return true;
#endif
}

// Mp3AudioStream 构造函数
WrapAL::Mp3AudioStream::Mp3AudioStream(const wchar_t* path) noexcept: Super(path)  {
    // 检查错误
    if (m_code != DefErrorCode::Code_Ok) return;
    // 检查dll
    if (!WrapALAudioEngine.libmpg123) { m_code = DefErrorCode::Code_FileNotFound; return; }
    // 创建新的句柄
    int error_code = 0;
    m_hMpg123 = Mpg123::mpg123_new(nullptr, &error_code);
    // 替换新的READ/SEEK函数
    if (!error_code) {
        error_code = Mpg123::mpg123_replace_reader_handle(
            m_hMpg123,
            [](void* file, void* buf, size_t len) ->ssize_t {
                return ::fread(buf, 1, len, static_cast<FILE*>(file));
            },
            [](void* file, off_t off, int org) ->off_t {
                ::fseek(static_cast<FILE*>(file), off, org);
                return ::ftell(static_cast<FILE*>(file));
            },
            nullptr
            );
    }
    // 以句柄方式打开mpg123
    if (!error_code) {
        error_code = Mpg123::mpg123_open_handle(m_hMpg123, m_pFile);
    }
    // 获取格式
    int encoding = 0;
    if (!error_code) {
        long rate = 0; int ch = 0;
        error_code = Mpg123::mpg123_getformat(m_hMpg123, &rate, &ch, &encoding);
        m_pcmFormat.nChannels = ch;
        m_pcmFormat.nSamplesPerSec = rate;
    }
    // 检查编码支持
    if (!error_code) {
        if (encoding != ENCODE_ENUM_SIGNED_16 && encoding != ENCODE_ENUM_FLOAT_32) {
            m_code = DefErrorCode::Code_UnsupportedFormat;
            return;
        }
    }
    // 保证编码不再改变
    if (!error_code) {
        error_code = Mpg123::mpg123_format_none(m_hMpg123);
    }
    // 保证编码不再改变
    if (!error_code) {
        error_code = Mpg123::mpg123_format(
            m_hMpg123, 
            m_pcmFormat.nSamplesPerSec,
            m_pcmFormat.nChannels,
            encoding
            );
    }
    // 填写格式
    if (!error_code) {
        // 获取深度
        if (encoding & ENCODE_ENUM_FLOAT_64) {
            m_pcmFormat.nBlockAlign = 8 * m_pcmFormat.nChannels;
        }
        else if (encoding & ENCODE_ENUM_FLOAT_32) {
            m_pcmFormat.nBlockAlign = 4 * m_pcmFormat.nChannels;
        }
        else if (encoding & ENCODE_ENUM_16) {
            m_pcmFormat.nBlockAlign = 2 * m_pcmFormat.nChannels;
        }
        else {
            m_pcmFormat.nBlockAlign = 1 * m_pcmFormat.nChannels;
        }
        // 数据率
        m_pcmFormat.nAvgBytesPerSec = m_pcmFormat.nSamplesPerSec * m_pcmFormat.nBlockAlign;
    }
    // 计算长度
    if (!error_code) {
        auto length_in_sample = Mpg123::mpg123_length(m_hMpg123);
        // 有效
        if (length_in_sample > 0) {
            m_cTotalSize = length_in_sample * m_pcmFormat.nBlockAlign / m_pcmFormat.nChannels;
        }
        // 错误
        else {
            m_code = DefErrorCode::Code_IllegalFile;
            return;
        }
    }
    // 检查错误
    if (error_code == MPG123_OUT_OF_MEM) {
        m_code = DefErrorCode::Code_OutOfMemory;
    }
    else if(error_code) {
        m_code = DefErrorCode::Code_IllegalFile;
    }
}

// Mp3AudioStream 释放
auto WrapAL::Mp3AudioStream::Release() noexcept -> int32_t {
    if (m_hMpg123) {
        Mpg123::mpg123_delete(m_hMpg123);
        m_hMpg123 = nullptr;
    }
    return Super::Release();
}

// 定位
auto WrapAL::Mp3AudioStream::Seek(uint32_t pos) noexcept -> bool {
    if (pos >= m_cTotalSize) return false;
    auto pos_in_sample = pos / m_pcmFormat.nBlockAlign ;
    Mpg123::mpg123_seek(m_hMpg123, pos_in_sample, SEEK_SET);
    return false;
}

// 读取下一部分
auto WrapAL::Mp3AudioStream::ReadNext(uint32_t len, uint8_t* buf) noexcept -> bool {
    size_t real_size = 0;
    if (auto i = Mpg123::mpg123_read(m_hMpg123, buf, len, &real_size)) {
        if (i == MPG123_ERR || i > 0)
            m_code = DefErrorCode::Code_DecodeError;
    }
    return real_size == len;
}







// 默认音频流创建函数
auto WrapAL::CALDefConfigure::DefCreateAudioStream(
    AudioFormat format, const wchar_t * file_path, wchar_t error_info[]) noexcept -> XALAudioStream * {
    XALAudioStream* stream = reinterpret_cast<XALAudioStream*>(ASPool.Alloc());
    if (!stream) {
        ::wcscpy(error_info, L"Out of memory!");
        return nullptr;
    }
    *reinterpret_cast<size_t*>(stream) = 0;
    assert(format != AudioFormat::Format_ByteStream && file_path && error_info && "bad arguments");
    switch (format)
    {
    case WrapAL::AudioFormat::Format_Wave:
        new(stream) WavAudioStream(file_path);
        break;
    case WrapAL::AudioFormat::Format_OggVorbis:
        new(stream) OggAudioStream(file_path);
        break;
    case WrapAL::AudioFormat::Format_Mpg123:
        new(stream) Mp3AudioStream(file_path);
        break;
    case WrapAL::AudioFormat::Format_ByteStream:
    default:
        // 格式不支持
        std::swprintf(error_info, ErrorInfoLength, L"Unsupported Format : 0x%08X", format);
    }
    // 检查错误
    if (*reinterpret_cast<size_t*>(stream)) {
        stream->GetLastErrorInfo(error_info);
    }
    return stream;
}


// 创建音频流
auto WrapAL::CALDefConfigure::CreateAudioStream(
    AudioFormat format, const wchar_t * file_path) noexcept -> XALAudioStream* {
    return CALDefConfigure::DefCreateAudioStream(format, file_path, m_szLastError);
}

// 获取最近一次错误
auto WrapAL::CALDefConfigure::GetLastErrorInfo(wchar_t info[]) noexcept -> bool {
    if (*m_szLastError) {
        ::wcscpy(info, m_szLastError);
        return true;
    }
    return false;
}

// 获取默认libmpg123位置
auto WrapAL::CALDefConfigure::GetLibmpg123Path(wchar_t path[]) noexcept -> void {
    path += ::GetCurrentDirectoryW(MAX_PATH, path);
    const auto* __restrict real = L"\\libmpg123.dll";
    while ((*path = *real)) { ++path; ++real; }
}

#endif

// 初始化
void WrapAL::Mpg123::Init(HMODULE hModule) noexcept {
    assert(hModule); if (!hModule) return;
    WrapAL::LoadProc(Mpg123::mpg123_init, hModule, "mpg123_init");
    WrapAL::LoadProc(Mpg123::mpg123_exit, hModule, "mpg123_exit");
    WrapAL::LoadProc(Mpg123::mpg123_new, hModule, "mpg123_new");
    WrapAL::LoadProc(Mpg123::mpg123_delete, hModule, "mpg123_delete");
    WrapAL::LoadProc(Mpg123::mpg123_read, hModule, "mpg123_read");
    WrapAL::LoadProc(Mpg123::mpg123_seek, hModule, "mpg123_seek");
    WrapAL::LoadProc(Mpg123::mpg123_tell, hModule, "mpg123_tell");
    WrapAL::LoadProc(Mpg123::mpg123_length, hModule, "mpg123_length");
    WrapAL::LoadProc(Mpg123::mpg123_format, hModule, "mpg123_format");
    WrapAL::LoadProc(Mpg123::mpg123_getformat, hModule, "mpg123_getformat");
    WrapAL::LoadProc(Mpg123::mpg123_format_none, hModule, "mpg123_format_none");
    WrapAL::LoadProc(Mpg123::mpg123_open_handle, hModule, "mpg123_open_handle");
    WrapAL::LoadProc(Mpg123::mpg123_replace_reader_handle, hModule, "mpg123_replace_reader_handle");
}


// 初始化静态变量
#define InitStaticVar(v)  decltype(v) v = nullptr
InitStaticVar(WrapAL::Mpg123::mpg123_init);
InitStaticVar(WrapAL::Mpg123::mpg123_exit);
InitStaticVar(WrapAL::Mpg123::mpg123_new);
InitStaticVar(WrapAL::Mpg123::mpg123_delete);
InitStaticVar(WrapAL::Mpg123::mpg123_read);
InitStaticVar(WrapAL::Mpg123::mpg123_seek);
InitStaticVar(WrapAL::Mpg123::mpg123_tell);
InitStaticVar(WrapAL::Mpg123::mpg123_length);
InitStaticVar(WrapAL::Mpg123::mpg123_format);
InitStaticVar(WrapAL::Mpg123::mpg123_getformat);
InitStaticVar(WrapAL::Mpg123::mpg123_format_none);
InitStaticVar(WrapAL::Mpg123::mpg123_open_handle);
InitStaticVar(WrapAL::Mpg123::mpg123_replace_reader_handle);
InitStaticVar(WrapAL::CALAudioEngine::XAudio2Create);
InitStaticVar(WrapAL::CALAudioEngine::X3DAudioInitialize);



// 单例类
WrapAL::CALAudioEngine WrapALAudioEngine;