#include "common.h"

/* Declaring func_80088E80 up here, as other funcs use it before its implementation which causes redeclaration compile errors*/
void func_80088E80();

extern s32* rmonbrk_bss_0000[32]; /* size may be wrong, element 32 referenced in __rmonSetBreak */
extern s32* rmonbrk_bss_0088[32]; /* size may be wrong, element 32 referenced in __rmonSetBreak */

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088480.s")
#else
extern s32* D_800D3E60;
extern s32 D_800D3E64;

void func_80088480(s32* arg0, s32* arg1) {
    rmonbrk_bss_0000[1] = (s32) *arg0;
    *arg0 = 0x40D;
    osWritebackDCache(arg0, 4);
    osInvalICache(arg0, 4);
    rmonbrk_bss_0000[0] = arg0;
    if (arg1 != NULL) {
        D_800D3E64 = *arg1;
        *arg1 = 0x40D;
        osWritebackDCache(arg1, 4);
        osInvalICache(arg1, 4);
        D_800D3E60 = arg1;
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_8008852C.s")
#else
extern s32* D_800D3E60;
extern s32 D_800D3E64;

void func_8008852C(void) {
    s32 sp1C;

    if (rmonbrk_bss_0000[0] != NULL) {
        sp1C = *rmonbrk_bss_0000[0];
        if ((sp1C & 0xFC00003F) == 0xD) {
            *rmonbrk_bss_0000[0] = rmonbrk_bss_0000[1];
            osWritebackDCache(rmonbrk_bss_0000[0], 4);
            osInvalICache(rmonbrk_bss_0000[0], 4);
        }
        rmonbrk_bss_0000[0] = NULL;
    }
    if (D_800D3E60 != NULL) {
        sp1C = *D_800D3E60;
        if ((sp1C & 0xFC00003F) == 0xD) {
            *D_800D3E60 = D_800D3E64;
            osWritebackDCache(D_800D3E60, 4);
            osInvalICache(D_800D3E60, 4);
        }
        D_800D3E60 = NULL;
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonSetBreak.s")
#else
int __rmonSetBreak(UnkStruct__rmonSetBreak* arg0) {
    register UnkStruct__rmonSetBreak* copy_arg0 = arg0;
    //dont know if using register keywokrd is right here
    register s32 var_s1; //assumed, but need to remove anyway, seems to be a refernece to either array
    register s32** var_s2; //maybe reference to element one or 2
    register s32** var_s3; //maybe reference to the last element, or size
    /*none of these are used until the bottom*/
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 unused30;
    s16 sp2E;
    u8 unused2D;
    u8 sp2C;
    s32 sp28; //assumed
    

//some sort of nest functionality going on here that choses one of the two arrays
    if (arg0->var9 == 1) {
        var_s1 = &rmonbrk_bss_0088;
        var_s2 = &rmonbrk_bss_0088[2];
        var_s3 = &rmonbrk_bss_0088[32];
    } else {
        var_s1 = &rmonbrk_bss_0000;
        var_s2 = &rmonbrk_bss_0000[2];
        var_s3 = &rmonbrk_bss_0000[32];
    }

    //my gut tells me this is a for loop, could use on of the five seemingly unused stack variables, the loop looks for the array element that is  equal to 0 and equal to arg0->var10
    while ((u32) var_s2 < (u32) var_s3) {//would make sense if this means if current element is not the last, or within size
        // if ((*var_s2 != 0) && (*(u32*)var_s2 != copy_arg0->var10)) { // if var10 non pointer
        if ((*var_s2 == 0) || (*var_s2 == copy_arg0->var10)) { //if var10 pointer, also changed the ands to ors and fliped equals
            break;
        }
        var_s2 += 2;
    }
    
    if (var_s2 == var_s3) {
        return -0xA;
    }
    
    if (*var_s2 == NULL) {
        if (arg0->var9 == 1) {
            var_s2[1] = __rmonReadWordAt(copy_arg0->var10);
            __rmonWriteWordTo(copy_arg0->var10, (((((s32) ((u32)var_s2 - (u32)var_s1) >> 3) + 0x10) & 0xFFFFF) << 6) | 0xD);
        } else {
            var_s2[1] = *(copy_arg0->var10);
            *copy_arg0->var10 = ((((((s32) ((u32)var_s2 - (u32)var_s1) >> 3) + 0x10) & 0xFFFFF) << 6) | 0xD);
            
            osWritebackDCache(copy_arg0->var10, 4);
            osInvalICache(copy_arg0->var10, 4);
        }
        var_s2[0] = copy_arg0->var10;
    }
    
    sp2C = copy_arg0->var4;
    sp2E = 0;
    sp34 = copy_arg0->varC;
    sp38 = (s32)((u32)var_s2 - (u32)var_s1) >> 0x3;
    sp3C = var_s2[1];
    __rmonSendReply(&sp28, 0x18, 1);
    return 0;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_800887D4.s")
#else
/* Must be unused*/
int func_800887D4(s32 arg0) {
    return -1;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonClearBreak.s")
#else
int __rmonClearBreak(UnkStruct__rmonSetBreak* arg0) {
    register UnkStruct__rmonSetBreak* copy_arg0;
    register s32** var_s1; /*could be something other than double pointer, array doesnt seem to work*/
    s32 unused3C;
    s32 sp38;
    s32 sp34;
    s32 unused30;
    s16 sp2E;
    u8 unused2D;
    u8 sp2C;
    s32 sp28;
    s32 sp24;
    
    
    
    copy_arg0 = arg0; /* no idea why this was done*/

    if ((s32)copy_arg0->var10 >= 16) {
        return -2;
    }
    if (arg0->var9 == 1) {
        var_s1 = ((s32)copy_arg0->var10 * 8) + (u32)rmonbrk_bss_0088;

        if (*var_s1 == NULL) {
            return -2;
        }
        sp24 = __rmonReadWordAt(*var_s1);
        if ((sp24 & 0xFC00003F) == 0xD) {
            __rmonWriteWordTo(*var_s1, var_s1[1]);
        }

    }else{
        var_s1 = ((s32)copy_arg0->var10 * 8) + (u32)rmonbrk_bss_0000;
        if (*var_s1 == NULL) {
            return -2;
        }
    
        if (((sp24 = **var_s1) & 0xFC00003F) == 0xD) { /* huh */
            **var_s1 = var_s1[1];
            osWritebackDCache(*var_s1, 4);
            osInvalICache(*var_s1, 4);
        }
    }
    

    *var_s1 = NULL;
    
    sp2C = copy_arg0->var4;
    sp2E = 0;
    sp34 = copy_arg0->varC;
    sp38 = copy_arg0->var10;
    __rmonSendReply(&sp28, 0x18, 1);
    return 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonGetBranchTarget.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088B64.s")
#else
    
    int func_80088B64(u32 arg0) {
    
    switch ((arg0 >> 0x1A) & 0x3F) {                              /* irregular */
        case 0:
            if (!((arg0 >> 5) & 0x7FFF) && ((arg0 & 0x3F) == 8)) {
                return 1;
            }
            if (!((arg0 >> 0x10) & 0x1F) && ((arg0 & 0x7FF) == 9)) {
                return 1;
            }
        break;
        case 2:
        case 3:
            return 1;
        }
        
    return 0;
    }

#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonSetSingleStep.s")
#else
int __rmonSetSingleStep(s32 arg0, s32* arg1) {
    s32 sp1C;

    sp1C = __rmonGetBranchTarget(0, arg0, arg1);
    if (sp1C & 3) {
        /* changed the + 4 to + 1, compiler seems to convert appropratly (pretty sure it means 1 word or byte) */
        func_80088480((s32) (arg1 + 1), NULL);
        
    }else if ((arg1+0) == (void*)sp1C) { /* had to add the + 0 to arg1 to get function to match */
        return 0;
    }
    /* changed the + 8 to + 2, compiler seems to convert appropratly (pretty sure it means 2 words or bytes) */
    else if ((func_80088B64(*arg1) != 0) || ((arg1 + 2) == (void*)sp1C)) { 
        func_80088480(sp1C, NULL);
    } else {
        /* Same here*/
        func_80088480(sp1C, arg1 + 2);
    }
    
    
    return 1;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonGetExceptionStatus.s")
#else

void __rmonGetExceptionStatus(UnkStruct_rmonGetExceptionStatus* arg0) {
    arg0->varC = 1;
    arg0->var10 = 2;
    arg0->var12 = 0;
    arg0->var30 = 0;
    arg0->var24 = 2;
    arg0->var26 = 4;
    arg0->var4 = 4;
    arg0->var6 = 0;
    arg0->var0 = 0x4C;
}

#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088CFC.s")
#else
void func_80088CFC(s32* arg0, s32 arg1) {
   
    /*Unused varibles to padd out stakc frame, feels a bit hackey as for some reason
    the stack frame size doesnt always change when a new variable is added*/
    s32 unused68;
    s32 unused64;
    s32 unused60;
    s32 unused5C;
    s32 unused5A;
    s32 unused58;
    s32 unused54;
    s32 unused50;
    s32 unused4C;
    s16 sp4A;
    s16 sp48;
    s32 unused44;
    s32 sp40;
    s32 unused3C;
    s32 unused38;
    s32 unused34;
    s32 unused30;
    s32 unused2c;
    s32 unused28;
    s32 sp24; /* assuming variable type, it's an address anyway */
    
    
    /* Hey future me, the inline conditional expression was how you solved the saved register problem*/
    __rmonGetThreadStatus(0, (arg0 ? arg0 : 0x3EB), &sp24);
    __rmonGetExceptionStatus(&sp24);
    if (arg1 == 0xF) {
        sp48 = 1;
        sp4A = 2;
    }
    if (arg1 < 0x10) {
        arg1 = 0;
    } else {
        arg1 -= 0x10;
    }
    if (arg1 != 0) {
        sp40 = 0xD;
    }
    __rmonSendReply(&sp24, 0x4C, 2);
}
#endif




#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonHitBreak.s")
#else

void __rmonHitBreak(void) {
    func_8008852C();
    __rmonStopUserThreads(0);
    func_80088E80();
}
#endif


#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/__rmonHitSpBreak.s")
#else
extern s8 __rmonRcpAtBreak;
void __rmonHitSpBreak(void) {
    /* Only thing that's wrong is the stack frame size and __rmonRcpAtBreak*/
    s32 unused[18]; /* This is to get the stack frame size to match*/
    
    s32 sp24; /* this has got to be an address to struct*/
    
    __rmonWriteWordTo(0x04080000, __rmonReadWordAt(0x04080000) - 4);
    __rmonGetThreadStatus(1, 0x3E8, &sp24);
    __rmonGetExceptionStatus(&sp24); /* this function uses a struct im sure*/
    __rmonSendReply(&sp24, 0x4C, 2);
    __rmonRcpAtBreak = 1;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088E50.s")
#else
void func_80088E50(void) {
        __rmonMaskIdleThreadInts();
        __rmonStopUserThreads(0);
        func_80088E80();
    }
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_89080/O1/segment_89080/func_80088E80.s")
#else
void func_80088E80() {

    /*Seems right to use register keyword, got compiler to use 'save' register*/
    register struct OSThread * var_s0 = __osGetActiveQueue();
    s32 sp20;


    while (var_s0->priority != -1){
        if ((var_s0->priority > 0) && (var_s0->priority < 0x80)) {
            if (var_s0->flags & 1) {
                sp20 = *(var_s0->context.pc);
                if ((sp20 & 0xFC00003F) == 0xD) {
                    func_80088CFC(var_s0->id, sp20 >> 6);
                } else {
                    func_80088CFC(var_s0->id, 0);
                }
            }
            if (var_s0->flags & 2) {
                __rmonSendFault(var_s0);
                func_80088CFC(var_s0->id, 0xF);
            }
        }
        var_s0 = var_s0->tlnext;/*gives warning but should be fine*/
    } 
    
}
#endif