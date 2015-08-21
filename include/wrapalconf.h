#pragma once
// THIS FILE IS NONE-LICENSE

// include CALDefConfigure
#define WRAPAL_INCLUDE_DEFAULT_CONFIGURE

// lower case method in clip, group class
//#define WRAPAL_HADNLE_CLASS_WITH_LOWERCASE_METHOD

// simple/crude memory leak detector in msc in debug mode
#define WRAPAL_MEMDEBUG_IN_MSC

// name
namespace WrapAL {
    // some config
    enum WrapALConstant : unsigned int {
        // object pool length in defaultly
        ObjectPoolLength = 1024,
        // error text defaultly length
        ErrorInfoLength = 1024,
        // each audio stream bucket size
        AudioStreamBucketSize = 32 * 1024,
        // streaming buffer size
        StreamingBufferSize = 16 * 1024,
        // streaming buffer count
        StreamingBufferCount = 3,
        // group name max length in char, +1 for null-end character
        GroupNameMaxLength = 15,
        // group max size
        GroupMaxSize = 8,
        // devie max count
        DeviceMaxCount = 256,
    };
}