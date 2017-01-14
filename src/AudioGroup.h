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

// IXAudio2SubmixVoice
struct IXAudio2SubmixVoice;

// assert
#include <cassert>
// memory
#include <cstring>
// include the config
#include "wrapalconf.h"
// include the config
#include "wrapal_common.h"
// clip
#include "AudioClip.h"



// wrapal namespace
namespace WrapAL {
    // Audio Source Group Handle
    class CALAudioSourceGroup;
    // group of audio clip
    struct AudioSourceGroupImpl {
        // ctor
        AudioSourceGroupImpl() noexcept { std::memset(name, 0, sizeof(name)); };
#ifndef NDEBUG
        // dtor
        ~AudioSourceGroupImpl() { assert(!voice && "not be released!"); }
#endif
        // Release
        auto Release() noexcept { if (voice) voice->DestroyVoice(); voice = nullptr; }
        // name
        char                    name[GroupNameMaxLength + 1];
        // XAudio2
        IXAudio2SubmixVoice*    voice = nullptr;
    };
}