#include "wrapal.h"

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
    class WRAPAL_NOVTABLE BasicAudioStream : public IALAudioStream {
    public:
        // ctor
        BasicAudioStream(const wchar_t*  path) noexcept :
            m_pFile(::_wfopen(path, L"rb")),
            m_code(m_pFile ? DefErrorCode::Code_Ok : DefErrorCode::Code_FileNotFound) {}
    public: // interface impl for IALAudioStream
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
    public: // interface impl for IALAudioStream
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
    public: // interface impl for IALAudioStream
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
    public: // interface impl for IALAudioStream
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

// mpg123 ����
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

// �����ͷ�
auto WrapAL::BasicAudioStream::Release() noexcept -> int32_t {
    if (m_pFile) {
        ::fclose(m_pFile);
        m_pFile = nullptr;
    }
    ASPool.Free(reinterpret_cast<WrapAL::DefAudioStreamBuffer*>(this));
    return 0;
}

// ��ȡ���һ�δ���
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

// WavAudioStream ���캯��
WrapAL::WavAudioStream::WavAudioStream(const wchar_t* file_path) noexcept : Super(file_path) {
    // ������
    if (m_code != DefErrorCode::Code_Ok) return;
    // �������ݽṹ
#pragma pack(push)
    // �������״̬
#pragma pack(1)
    // WAVE �ļ�ͷ
    struct {
        // RIFF �ļ�ͷ
        char    szRiffID[4];        // 'R','I','F','F'
        DWORD   dwRiffSize;         // ����wav�ļ���С��ȥ "RIFF"�Լ�������(�ļ���С-8)
        char    szRiffFormat[4];    // 'W','A','V','E'
        // FMT ����
        char    szFmtID[4];         // 'f','m','t', 0x20
        DWORD   dwFmtSize;          // fmt �����С(16?18?)
        // WAVE��ʽ
        WORD    wFormatTag;         // format type
        WORD    nChannels;          // number of channels (i.e. mono, stereo...)
        DWORD   nSamplesPerSec;     // sample rate
        DWORD   nAvgBytesPerSec;    // for buffer estimation
        WORD    nBlockAlign;        // block size of data 
        WORD    wBitsPerSample;     // number of bits per sample of mono data
    } header;
    // FACT �� ��ѡ / ���ݴ�С
    union {       
        struct {
            char    szFactID[4];    // 'f','a','c','t'
            DWORD   dwFactSize;     // fact ��С(һ��Ϊ4?)
        };
        struct {
            char    szDataID[4];    // 'd','a','t','a'
            DWORD   dwDataSize;     // ���ݴ�С
        } ;
    } fact_data;
    // �ָ�����״̬
#pragma pack(pop)
    auto code = DefErrorCode::Code_Ok;
    // ��ȡ�ļ�ͷ
    if (!::fread(&header, sizeof(header), 1, m_pFile)) {
        code = DefErrorCode::Code_IllegalFile;
    }
    // ���Ϸ���
    if (code == DefErrorCode::Code_Ok) {
        if ((*reinterpret_cast<uint32_t*>(header.szRiffID) !=  "RIFF"_wrapal32)   ||
            (*reinterpret_cast<uint32_t*>(header.szRiffFormat) != "WAVE"_wrapal32) ||
            (*reinterpret_cast<uint32_t*>(header.szFmtID) != "fmt\x20"_wrapal32))
        code = DefErrorCode::Code_IllegalFile;
    }
    // ����ʽ֧��
    if (code == DefErrorCode::Code_Ok) {
        if (header.wFormatTag != WAVE_FORMAT_PCM) {
            code = DefErrorCode::Code_UnsupportedFormat;
        }
    }
    // ���ʣ�ಿ��
    if (code == DefErrorCode::Code_Ok) {
        ::fseek(m_pFile, header.dwFmtSize - 16, SEEK_CUR);
        if (!::fread(&fact_data, sizeof(fact_data), 1, m_pFile)) {
            code = DefErrorCode::Code_IllegalFile;
        }
    }
    // fact ���� data
    if (code == DefErrorCode::Code_Ok) {
        // fact ��?
        if ((*reinterpret_cast<uint32_t*>(fact_data.szFactID) == "face"_wrapal32)) {
            ::fseek(m_pFile, fact_data.dwFactSize - 4, SEEK_CUR);
            ::fread(&fact_data, sizeof(fact_data), 1, m_pFile);
        }
        // data ��?
        if ((*reinterpret_cast<uint32_t*>(fact_data.szDataID) == "data"_wrapal32)) {
            this->total_size = fact_data.dwDataSize;
            m_zeroPosOffset = ::ftell(m_pFile);
        }
        else {
            code = DefErrorCode::Code_IllegalFile;
        }
    }
    // ��������
    if (code == DefErrorCode::Code_Ok) {
        this->pcm_format.nSamplesPerSec = header.nSamplesPerSec;
        this->pcm_format.nAvgBytesPerSec = header.nAvgBytesPerSec;
        this->pcm_format.nBlockAlign = header.nBlockAlign;
        this->pcm_format.nChannels = header.nChannels;
    }
    m_code = code;
}

// ���ö�ȡλ��
auto WrapAL::WavAudioStream::Seek(uint32_t pos) noexcept -> bool {
    if (pos >= this->total_size) return false;
    m_pos = pos;
    ::fseek(m_pFile, pos + m_zeroPosOffset, SEEK_SET);
    return true;
}

// OggAudioStream ���캯��
WrapAL::OggAudioStream::OggAudioStream(const wchar_t* file_path) noexcept : Super(file_path) {
    // ������
    if (m_code != DefErrorCode::Code_Ok) return;
    if (::ov_open_callbacks(m_pFile, &m_ovfile, nullptr, 0, OV_CALLBACKS_NOCLOSE) >= 0) {
        //char **ptr = ov_comment(&ovfile, -1)->user_comments;
        vorbis_info *vi = ov_info(&m_ovfile, -1);
        // ��ȡ������
        this->pcm_format.nChannels = vi->channels;
        // ��ȡ������
        this->pcm_format.nSamplesPerSec = vi->rate;
        // �������
        this->pcm_format.nBlockAlign = this->pcm_format.nChannels * sizeof(int16_t);
        // bps
        this->pcm_format.nAvgBytesPerSec = this->pcm_format.nSamplesPerSec * this->pcm_format.nBlockAlign;
        // ���ݴ�С
        this->total_size = static_cast<decltype(this->total_size)>(::ov_pcm_total(&m_ovfile, -1)) *
            static_cast<decltype(this->total_size)>(this->pcm_format.nBlockAlign);
    }
    // �Ƿ��ļ�
    else {
        m_code = DefErrorCode::Code_IllegalFile;
    }
}

// OggAudioStream �ͷ�����
auto WrapAL::OggAudioStream::Release() noexcept -> int32_t {
    ::ov_clear(&m_ovfile);
    return Super::Release();
}

// OggAudioStream ��λ
auto WrapAL::OggAudioStream::Seek(uint32_t pos) noexcept -> bool {
    if (pos >= this->total_size) return false;
    auto code = ::ov_pcm_seek(&m_ovfile, pos / this->pcm_format.nBlockAlign);
    // ERROR
    if (code) {

    }
    return true;
}

// OggAudioStream ��ȡ����
auto WrapAL::OggAudioStream::ReadNext(uint32_t len, uint8_t* buf) noexcept -> bool {
#ifdef _DEBUG
    uint32_t total_len = 0, target_len = len;
    bool recode = true;
    // ѭ����ȡ����
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
    // ѭ����ȡ����
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

// Mp3AudioStream ���캯��
WrapAL::Mp3AudioStream::Mp3AudioStream(const wchar_t* path) noexcept: Super(path)  {
    // ������
    if (m_code != DefErrorCode::Code_Ok) return;
    // ���dll
    if (!AudioEngine.libmpg123) { m_code = DefErrorCode::Code_FileNotFound; return; }
    // �����µľ��
    int error_code = 0;
    m_hMpg123 = Mpg123::mpg123_new(nullptr, &error_code);
    // �滻�µ�READ/SEEK����
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
    // �Ծ����ʽ��mpg123
    if (!error_code) {
        error_code = Mpg123::mpg123_open_handle(m_hMpg123, m_pFile);
    }
    // ��ȡ��ʽ
    int encoding = 0;
    if (!error_code) {
        long rate = 0; int ch = 0;
        error_code = Mpg123::mpg123_getformat(m_hMpg123, &rate, &ch, &encoding);
        this->pcm_format.nChannels = ch;
        this->pcm_format.nSamplesPerSec = rate;
    }
    // ������֧��
    if (!error_code) {
        if (encoding != ENCODE_ENUM_SIGNED_16 && encoding != ENCODE_ENUM_FLOAT_32) {
            m_code = DefErrorCode::Code_UnsupportedFormat;
            return;
        }
    }
    // ��֤���벻�ٸı�
    if (!error_code) {
        error_code = Mpg123::mpg123_format_none(m_hMpg123);
    }
    // ��֤���벻�ٸı�
    if (!error_code) {
        error_code = Mpg123::mpg123_format(
            m_hMpg123, 
            this->pcm_format.nSamplesPerSec,
            this->pcm_format.nChannels,
            encoding
            );
    }
    // ��д��ʽ
    if (!error_code) {
        // ��ȡ���
        if (encoding & ENCODE_ENUM_FLOAT_64) {
            pcm_format.nBlockAlign = 8 * pcm_format.nChannels;
        }
        else if (encoding & ENCODE_ENUM_FLOAT_32) {
            pcm_format.nBlockAlign = 4 * pcm_format.nChannels;
        }
        else if (encoding & ENCODE_ENUM_16) {
            pcm_format.nBlockAlign = 2 * pcm_format.nChannels;
        }
        else {
            pcm_format.nBlockAlign = 1 * pcm_format.nChannels;
        }
        // ������
        pcm_format.nAvgBytesPerSec = pcm_format.nSamplesPerSec * pcm_format.nBlockAlign;
    }
    // ���㳤��
    if (!error_code) {
        auto length_in_sample = Mpg123::mpg123_length(m_hMpg123);
        // ��Ч
        if (length_in_sample > 0) {
            this->total_size = length_in_sample * pcm_format.nBlockAlign / pcm_format.nChannels;
        }
        // ����
        else {
            m_code = DefErrorCode::Code_IllegalFile;
            return;
        }
    }
    // ������
    if (error_code == MPG123_OUT_OF_MEM) {
        m_code = DefErrorCode::Code_OutOfMemory;
    }
    else if(error_code) {
        m_code = DefErrorCode::Code_IllegalFile;
    }
}

// Mp3AudioStream �ͷ�
auto WrapAL::Mp3AudioStream::Release() noexcept -> int32_t {
    if (m_hMpg123) {
        Mpg123::mpg123_delete(m_hMpg123);
        m_hMpg123 = nullptr;
    }
    return Super::Release();
}

// ��λ
auto WrapAL::Mp3AudioStream::Seek(uint32_t pos) noexcept -> bool {
    if (pos >= this->total_size) return false;
    auto pos_in_sample = pos / pcm_format.nBlockAlign ;
    Mpg123::mpg123_seek(m_hMpg123, pos_in_sample, SEEK_SET);
    return false;
}

// ��ȡ��һ����
auto WrapAL::Mp3AudioStream::ReadNext(uint32_t len, uint8_t* buf) noexcept -> bool {
    size_t real_size = 0;
    if (auto i = Mpg123::mpg123_read(m_hMpg123, buf, len, &real_size)) {
        if (i == MPG123_ERR || i > 0)
            m_code = DefErrorCode::Code_DecodeError;
    }
    return real_size == len;
}







// Ĭ����Ƶ����������
auto WrapAL::CALDefConfigure::DefCreateAudioStream(
    AudioFormat format, const wchar_t * file_path, wchar_t error_info[]) noexcept -> IALAudioStream * {
    IALAudioStream* stream = reinterpret_cast<IALAudioStream*>(ASPool.Alloc());
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
        // ��ʽ��֧��
        ::swprintf(error_info, ErrorInfoLength, L"Unsupported Format : 0x%08X", format);
    }
    // ������
    if (*reinterpret_cast<size_t*>(stream)) {
        stream->GetLastErrorInfo(error_info);
    }
    return stream;
}


// ������Ƶ��
auto WrapAL::CALDefConfigure::CreateAudioStream(
    AudioFormat format, const wchar_t * file_path) noexcept -> IALAudioStream* {
    return CALDefConfigure::DefCreateAudioStream(format, file_path, m_szLastError);
}

// ��ȡ���һ�δ���
auto WrapAL::CALDefConfigure::GetLastErrorInfo(wchar_t info[]) noexcept -> bool {
    if (*m_szLastError) {
        ::wcscpy(info, m_szLastError);
        return true;
    }
    return false;
}

// ��ȡĬ��libmpg123λ��
auto WrapAL::CALDefConfigure::GetLibmpg123_dllPath(wchar_t path[]) noexcept -> void {
    path += ::GetCurrentDirectoryW(MAX_PATH, path);
    const auto* __restrict real = L"\\libmpg123.dll";
    while ((*path = *real)) { ++path; ++real; }
}

#endif

// ����
#define LoadFunction(a, b, c) a = reinterpret_cast<decltype(a)>(::GetProcAddress(c, #b))

// ��ʼ��
void WrapAL::Mpg123::Init(HMODULE hModule) noexcept {
    assert(hModule); if (!hModule) return;
    LoadFunction(WrapAL::Mpg123::mpg123_init, mpg123_init, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_exit, mpg123_exit, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_new, mpg123_new, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_delete, mpg123_delete, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_read, mpg123_read, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_seek, mpg123_seek, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_tell, mpg123_tell, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_length, mpg123_length, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_format, mpg123_format, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_getformat, mpg123_getformat, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_format_none, mpg123_format_none, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_open_handle, mpg123_open_handle, hModule);
    LoadFunction(WrapAL::Mpg123::mpg123_replace_reader_handle, mpg123_replace_reader_handle, hModule);
}


// ��ʼ����̬����
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
InitStaticVar(WrapAL::CAudioEngine::XAudio2Create);



// ������
WrapAL::CAudioEngine AudioEngine;