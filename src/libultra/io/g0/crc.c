#include <PR/ultratypes.h>

u8 __osContAddressCrc(u16 addr) {
    u32 i;
    s32 temp;

    temp = 0;
    i = 0x400;
    
    do {
        temp = temp * 2;
        
        if (((u32)addr & i) != 0) {
            if ((temp & 0x20) != 0) {
                temp = temp ^ 0x14;
            } else {
                temp = temp + 1;
            }
        } else if ((temp & 0x20) != 0) {
            temp = temp ^ 0x15;
        }
        
        i = i / 2;
    } while (i != 0);

    for (i = 0; i < 5; ++i) {
        temp = temp * 2;
        
        if ((temp & 0x20) != 0) {
            temp = temp ^ 0x15;
        }
    }

    return temp & 0x1F;
}

u8 __osContDataCrc(u8 *data) {
    u32 j;
    s32 temp;
    u32 i;

    temp = 0;
    
    for (i = 32; i != 0; data++, i--) {
        for (j = 0x80; j != 0; j >>= 1) {
            temp = temp * 2;
            
            if ((*data & j) != 0) {
                if ((temp & 0x100) != 0) {
                    temp = temp ^ 0x84;
                } else {
                    temp = temp + 1;
                }
            } else if ((temp & 0x100) != 0) {
                temp = temp ^ 0x85;
            }
        }
    }
    
    do {
        temp = temp * 2;
        
        if ((temp & 0x100) != 0) {
            temp = temp ^ 0x85;
        }
    } while (++i < 8);
    
    return temp;
}
