



    // windows error code to HRESULT
    inline auto WinCode2HRESULT(UINT x) noexcept ->HRESULT {
        return ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT)(((x)& 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)));
    }