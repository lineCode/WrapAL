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

// int type
#include <cstdint>

// mpg123
#include <sys\types.h>
#ifdef _MSC_VER
using ssize_t = std::intptr_t;
#endif
// MY CONFIGURE
#if 0
#define NO_ICY 1
#define NO_STRING 1
#define NO_ID3V2 1
#define NO_WARNING 1
#define NO_ERRORMSG 1
#define NO_ERETURN 1
#define NO_FEEDER 1
#define USE_NEW_HUFFTABLE 1
#endif
#define MPG123_NO_CONFIGURE
#include "../3rdparty/mpg123/mpg123.h.in"


// wrapal namespace
namespace WrapAL {
    // Mpg123 dll, using dynamic-linking to avoid LGPL
    class Mpg123 {
    public:
        // init
        static void Init(HMODULE)  noexcept;
    public: // function zone
        // mpg123 : init
        static decltype(&::mpg123_init) mpg123_init;
        // mpg123 : exit
        static decltype(&::mpg123_exit) mpg123_exit;
        // mpg123 : new
        static decltype(&::mpg123_new) mpg123_new;
        // mpg123 : delete
        static decltype(&::mpg123_delete) mpg123_delete;
        // mpg123 : read
        static decltype(&::mpg123_read) mpg123_read;
        // mpg123 : seek
        static decltype(&::mpg123_seek) mpg123_seek;
        // mpg123 : tell
        static decltype(&::mpg123_tell) mpg123_tell;
        // mpg123 : length
        static decltype(&::mpg123_length) mpg123_length;
        // mpg123 : format
        static decltype(&::mpg123_format) mpg123_format;
        // mpg123 : get format
        static decltype(&::mpg123_getformat) mpg123_getformat;
        // mpg123 : format none
        static decltype(&::mpg123_format_none) mpg123_format_none;
        // mpg123 : open with handle
        static decltype(&::mpg123_open_handle) mpg123_open_handle;
        // mpg123 : replace reader handle
        static decltype(&::mpg123_replace_reader_handle) mpg123_replace_reader_handle;
    };
}