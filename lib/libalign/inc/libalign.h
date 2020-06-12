#pragma once

#include <iostream>

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