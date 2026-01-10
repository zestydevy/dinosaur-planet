#include "PR/os.h"
#include "sys/acache.h"
#include "sys/asset_thread.h"
#include "sys/memory.h"

static const char str_80098210[] = "acacheInit %d,%d,%d %dbytes\n";
static const char str_80098230[] = "\nfetch %d\n";
static const char str_8009823c[] = "in cache %d\n";
static const char str_8009824c[] = "flush\n";
static const char str_80098254[] = "in load %d\n";

ACache* acache_init(u8 fileID, u8 arg1, u8 arg2, u8 arg3, ACacheCallback arg4) {
    u8* var_a0;
    ACache* cache;
    s32 temp_v0_2[5];
    s32 i;

    temp_v0_2[0] = mmAlign16(sizeof(ACache));
    temp_v0_2[1] = temp_v0_2[0] + (arg1 * arg2);
    temp_v0_2[2] = temp_v0_2[1] + (arg3 * arg2);
    temp_v0_2[3] = temp_v0_2[2] + (arg3 * 2);
    i = temp_v0_2[3] + (arg3 * 4);
    temp_v0_2[4] = i;
    
    cache = mmAlloc(temp_v0_2[4], COLOUR_TAG_GREY, NULL);
    if (cache != NULL) {
        if ((!cache->unkC) && (!cache->unkC)) {}
        cache->fileID = fileID;
        cache->unk1 = arg1;
        cache->unk2 = arg2;
        cache->unk3 = arg3;
        cache->unk18 = arg4;
        cache->unk8 = (u8* ) (temp_v0_2[0] + (u32)cache);
        cache->unkC = (u8* ) (temp_v0_2[1] + (u32)cache);
        cache->unk10 = (s16* ) (temp_v0_2[2] + (u32)cache);
        cache->unk14 = (u8** ) (temp_v0_2[3] + (u32)cache);
        cache->unk6 = -arg2 - 1;
        var_a0 = cache->unkC;
        cache->unk4 = 0;
        for (i = 0; i < arg3; i++) {
            cache->unk10[i] = -1;
            cache->unk14[i] = var_a0;
            var_a0 += arg2;
        }
    }
    
    return cache;
}

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
