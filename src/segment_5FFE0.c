#include "common.h"

char *strcpy_2(char *dest, char *src) {
    char *start = dest;

    while ((*(dest++) = *(src++)) != '\0') { }

    return start;
}

char *strcat(char *dest, const char *src) {
    char *start = dest;

    while (*dest != '\0') {
        dest++;
    }

    while ((*(dest++) = *(src++)) != '\0') { }

    return start;
}

/**
 * Case insensitive strcmp.
 */
int strcasecmp(const char *lhs, const char *rhs) {
    char c1, c2;
    
    while (*lhs != '\0' || *rhs != '\0') {
        // To uppercase
        c1 = *lhs;
        if (c1 >= 'a' && c1 <= 'z') {
            c1 -= 32;
        }

        c2 = *rhs;
        if (c2 >= 'a' && c2 <= 'z') {
            c2 -= 32;
        }

        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }

        lhs++;
        rhs++;
    }

    return 0;
}

/**
 * Case insensitive strncmp.
 */
int strncasecmp(const char *lhs, const char *rhs, size_t count) {
    char c1, c2;
    
    while ((*lhs != '\0' || *rhs != '\0') && count != 0) {
        // To uppercase
        c1 = *lhs;
        if (c1 >= 'a' && c1 <= 'z') {
            c1 -= 32;
        }

        c2 = *rhs;
        if (c2 >= 'a' && c2 <= 'z') {
            c2 -= 32;
        }

        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }

        lhs++;
        rhs++;
        count--;
    }

    return 0;
}

void *memset(void *dest, int ch, size_t count) {
    unsigned char *ptr = dest;

    while (count-- > 0) {
        *(ptr++) = (unsigned char)ch;
    }

    return dest;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_8005F60C.s")
#else
extern char D_8009ADF0[];
extern char D_8009AE18[]; 

char *func_8005F60C(u64 param1, char *param3, u32 param4, s32 bUppercase) {
    char *chars;

    if (bUppercase) {
        chars = D_8009AE18;
    } else {
        chars = D_8009ADF0;
    }

    if (param1 != 0) {
        do {
            param3--;

            *param3 = chars[param1 % param4];
        } while ((param1 / param4) != 0);
    }

    return param3;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_8005F6CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_8005F6DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_printing_null_nil.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060A80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/dummied_print_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/osSyncPrintf_.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060B5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060B78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060B94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060DE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060E10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060EB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060F60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80060FD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80061068.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_800610D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80061210.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80061700.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_with_bid_bad_byte_array.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80061E48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80061ED8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5FFE0/func_80061F00.s")
