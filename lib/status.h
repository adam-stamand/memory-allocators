#pragma once


namespace alloc
{

typedef enum
{
    kStatusSuccess = 0x00,
    kStatusOutOfMemory,
    kStatusFailure,
    kStatusInvalidParam
} AllocatorStatus_t;

}
