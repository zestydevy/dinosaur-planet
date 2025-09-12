#include "PR/os.h"
#include "sys/acache.h"
#include "sys/asset_thread.h"
#include "sys/memory.h"

static const char str_80098210[] = "acacheInit %d,%d,%d %dbytes\n";
static const char str_80098230[] = "\nfetch %d\n";
static const char str_8009823c[] = "in cache %d\n";
static const char str_8009824c[] = "flush\n";
static const char str_80098254[] = "in load %d\n";

// https://decomp.me/scratch/Uiv2B
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/acache/acache_init.s")
#else
ACache* acache_init(u8 fileID, u8 arg1, u8 arg2, u8 arg3, ACacheCallback arg4) {
    ACache* cache;
    s32 i;
    s32 temp_a0; // sp+44
    s32 temp_t4; // sp+40
    s32 temp_t3; // sp+3c
    s32 temp_t2; // sp+38
    s32 temp_v0_2; // sp+34
    u8* var_a0;

    temp_v0_2 = mmAlign16(sizeof(ACache));
    
    temp_t2 = temp_v0_2;
    temp_t2 += (arg1 * arg2);
    
    temp_t3 = temp_t2;
    temp_t3 += (arg3 * arg2);
    
    temp_t4 = temp_t3;
    temp_t4 += (arg3 * 2);
    
    temp_a0 = temp_t4;
    temp_a0 += (arg3 * 4);
    
    cache = mmAlloc(temp_a0, COLOUR_TAG_GREY, NULL);
    if (cache != NULL) {
        cache->fileID = fileID;
        cache->unk1 = arg1;
        cache->unk2 = arg2;
        cache->unk3 = arg3;
        cache->unk18 = arg4;
        cache->unk8 = (void* ) (temp_v0_2 + (u32)cache);
        cache->unkC = (void* ) (temp_t2 + (u32)cache);
        cache->unk10 = (void* ) (temp_t3 + (u32)cache);
        cache->unk14 = (void* ) (temp_t4 + (u32)cache);
        cache->unk6 = -arg2 - 1;
        cache->unk4 = 0;
        var_a0 = cache->unkC;
        for (i = 0; i < arg3; i++) {
            cache->unk10[i] = -1;
            cache->unk14[i] = var_a0;
            var_a0 += arg2;
        }
    }
    
    if ((!temp_a0) && (!temp_a0)){} // @fake
    
    return cache;
}
#endif

void acache_free(ACache *cache) {
    mmFree(cache);
}

void acache_func_80000D50(ACache *cache) {
    s32 i;

    cache->unk6 = -cache->unk2 - 1;
    cache->unk4 = 0;

    for (i = 0; i < cache->unk3; ++i) {
       cache->unk10[i] = -1;
    }
}

void *acache_get(ACache *cache, s32 arg1) {
    u8* sp34;
    s32 var_s0;
    s32 var_s2_3;
    s32 var_v1;
    void *temp_v0_2;

    sp34 = NULL;
    cache->unk4--;
    if (cache->unk4 < 0) {
        cache->unk4 = cache->unk3 - 1;
    }
    for (var_s2_3 = 0; var_s2_3 < cache->unk3; var_s2_3++) {
        if (arg1 == cache->unk10[var_s2_3]) {
            sp34 = cache->unk14[var_s2_3];
            break;
        }
    }
    if (sp34 != NULL) {
        var_s0 = var_s2_3;
        var_v1 = var_s2_3 + 1;
        while (var_s0 != cache->unk4) {
            if (var_v1 == cache->unk3) {
                var_v1 = 0;
            }
            cache->unk10[var_s0] = cache->unk10[var_v1];
            cache->unk14[var_s0] = cache->unk14[var_v1];
            var_s0 += 1;
            var_v1 += 1;
            if (var_s0 == cache->unk3) {
                var_s0 = 0;
            }
        }
    } else {
        sp34 = cache->unk14[cache->unk4];
        var_s2_3 = arg1 - cache->unk6;
        if ((var_s2_3 < 0) || (var_s2_3 >= cache->unk1)) {
            // Requested item is outside of the currently cached region
            var_s2_3 = arg1 - (cache->unk1 >> 1);
            if (var_s2_3 < 0) {
                var_s2_3 = 0;
            }
            cache->unk6 = var_s2_3;
            if (cache->unk18 != NULL) {
                // Load from callback
                for (var_s0 = 0; var_s0 < (s32) cache->unk1; var_s0 += 1) {
                    // Get thing from callback
                    temp_v0_2 = cache->unk18(var_s2_3);
                    // Copy it into cache
                    bcopy(temp_v0_2, &cache->unk8[var_s0 * cache->unk2], cache->unk2);
                    mmFree(temp_v0_2);
                    var_s2_3 += 1;
                }
            } else {
                // Load from file
                queue_load_file_region_to_ptr(
                    /*dest*/   (void** ) cache->unk8, 
                    /*fileID*/ (s32) cache->fileID, 
                    /*offset*/ cache->unk2 * var_s2_3, 
                    /*length*/ cache->unk1 * cache->unk2);
            }
            var_s2_3 = arg1 - cache->unk6;
        }
        bcopy(&cache->unk8[var_s2_3 * cache->unk2], sp34, cache->unk2);
    }
    cache->unk10[cache->unk4] = arg1;
    cache->unk14[cache->unk4] = sp34;
    return sp34;
}
