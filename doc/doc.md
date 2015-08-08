# C++ API Reference

### Singleton
WrapAL use a singleton within marco `AudioEngine`, defined as
```cpp
#define AudioEngine (WrapAL::CALAudioEngine::s_instance)
```
this singleton initialized in the bottom of [AudioStreams.cpp](../src/AudioStreams.cpp):
```cpp
// 单例类
WrapAL::CALAudioEngine AudioEngine;
```

### Initialize & UnInitialize
WrapAL used some COM object on Windows, you should Initialize COM object within `CoInitialize` or `CoInitializeEx` on Windows, and use `AudioEngine.Initialize()` init the WrapAL. you may check the return code in `HRESULT`.

much similarly. use `AudioEngine.UnInitialize()` to UnInitialize this, and `CoUninitialize()` on Windows.

### Audio Clip
in WrapAL, Audio was abstracted as "Clip" in `WrapAL::CALAudioSourceClip`

### Audio Group
in WrapAL, clip was grouped in "Group", like "BGM" group, "BGS" group, "SE" group, etc.
a group could set volume(or other operator) to affect all clip under this group.
if a clip created with `''` or `nullptr` group, the group of this clip is `TOPLEVEL`, this group could do some operation to **affect all clip**.

### CALAudioEngine
core management class in WrapAL, see [CALAudioEngine.md](/CALAudioEngine.md)

### IALConfigure
in `WrapAL::CALAudioEngine::Initialize`, yeah, you use `AudioEngine.Initialize()` in easy way, but, IALConfigure provided some **Configure**, you may implement it. more detail see [IALConfigure.md](/CALAudioEngine.md)
