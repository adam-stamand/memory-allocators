#pragma once

#include <iostream>
#define ADD_TO_POINTER(ptr, add) (reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(ptr) + add ))
#define SUB_POINTER_POINTER_UINT(ptr1, ptr2) (reinterpret_cast<uintptr_t>( reinterpret_cast<uintptr_t>(ptr1) - reinterpret_cast<uintptr_t>(ptr2) ))


namespace align{


typedef size_t alignment_t;



inline void* alignForward(void* address, alignment_t alignment)
{
    return (void*)( reinterpret_cast<uintptr_t>( reinterpret_cast<uint8_t*>(address) + static_cast<alignment_t>(alignment-1) ) & static_cast<alignment_t>(~(alignment-1)) );
}

inline alignment_t alignForwardAdjustment(const void* address, alignment_t alignment)
{
    if (alignment == 0) return 0;

    alignment_t adjustment = alignment - ( reinterpret_cast<uintptr_t>(address) & static_cast<alignment_t>(alignment-1) ); 
  
    if(adjustment == alignment) return 0; 
  
    //already aligned 
    return adjustment; 

}

/** 
 * @brief Get alignment adjustment with header
 * 
 * This call will return the number of bytes necessary to adjust
 * provided address to an aligned address, accounting for a header.
 * 
 * For example, if a header is 0x5 bytes in size, the alignment is 0x4 bytes,
 * and the address is 0x21, the returned adjustment will be 0x2 bytes.
 * 
 * @param address 
 * @param alignment 
 * @param headerSize 
 * @return alignment_t 
 */
inline alignment_t alignForwardAdjustmentWithHeader(const void* address, alignment_t alignment, alignment_t headerSize)
{
    if (alignment == 0)
    {
        return 0;
    }
    
    alignment_t adjustment = alignForwardAdjustment(address, alignment); 
    alignment_t neededSpace = headerSize; 
    
    if(adjustment < neededSpace) 
    {
        neededSpace -= adjustment; 

        //Increase adjustment to fit header 
        adjustment += alignment * (neededSpace / alignment); 

        if(neededSpace % alignment > 0)
        {
            adjustment += alignment;
        } 
    }

    return adjustment;
}

}