#include "common.h"
#include "sys/voxmap.h"
#include "sys/map.h"

static const char str_80098510[] = "VOXMAP: Size overflow on load->Ignoring defined voxmap\n";
static const char str_80098548[] = "VOXMAP: attempt to load invalid object voxmap '%d'\n";
static const char str_8009857c[] = "VOXMAP: attempt to load invalid object voxmap '%d'\n";
static const char str_800985b0[] = "VOXMAP: Size overflow on object voxmap load->Ignoring defined voxmap\n";
static const char str_800985f8[] = "overflow in 'genVoxelCircle'\n";
static const char str_80098618[] = "****** GOAL FOUND Iterations=%d ******\n";
static const char str_80098640[] = "route: routepoints overflow\n";
static const char str_80098660[] = "Childnode Null\n";
static const char str_80098670[] = "**** HEAP INSERT ****\n";
static const char str_80098688[] = "VOXMAPS: route nodes list overflow\n";
static const char str_800986ac[] = "**** NODE FIND ****\n";

#define CACHE_TTL 0x40000000
#define SOME_FACTOR 10
#define SLOT_COUNT 6

/* -------- .bss start 800a7c70 -------- */
s32* gVoxmapTextureIndices; // D_800A7C70
s32* gVoxmapObjectIndices; // D_800A7C74
void* D_800A7C78; // gVoxelCircleBuffer?
VoxmapSlot* D_800A7C80[SLOT_COUNT];
s32* D_800A7C98; // gPathfindingRingBuffer?
s32 _bss_800a7c9c;
VoxmapSlot gVoxmapLastSearchedSlot;
s32 gVoxmapBlockIDs[SLOT_COUNT];
s32 gVoxmapCacheTimers[SLOT_COUNT];
u8 gVoxmapSlotUsed[SLOT_COUNT]; // gVoxmapInUseFlags?
s16 D_800A7CF0[6][2]; // gVoxmapOriginPositions?
Object *gVoxmapCurrentObject;
s32 D_800A7D0C; // gVoxmapObjectCount?
/* -------- .bss end 800a7d10 -------- */

void voxmap_init(void) {
    s32 i;

    queue_alloc_load_file((void**)&gVoxmapTextureIndices, TEXPRE_TAB);
    queue_alloc_load_file((void**)&gVoxmapObjectIndices, VOXOBJ_TAB);
    for (i = 0; gVoxmapObjectIndices[i] != -1; i++) {}

    D_800A7D0C = --i;
    D_800A7C78 = mmAlloc(0x280, ALLOC_TAG_VOX_COL, NULL);
    i = 0;
    while (i < SLOT_COUNT) {
        D_800A7C80[i] = NULL;
        gVoxmapBlockIDs[i] = -2;
        gVoxmapCacheTimers[i] = CACHE_TTL;
        gVoxmapSlotUsed[i] = FALSE;
        D_800A7CF0[i][0] = 0;
        D_800A7CF0[i][1] = 0;
        i++;
    }
    D_800A7C98 = D_800A7C78;
    gVoxmapCurrentObject = NULL;
}

void voxmap_free(void) {
    VoxmapSlot *temp_a0;
    s32 i;

    for (i = 0; i < SLOT_COUNT; i++) {
        temp_a0 = D_800A7C80[i];
        if (temp_a0 != NULL) {
            mmFree(temp_a0);
            D_800A7C80[i] = NULL;
        }
    }
    mmFree(gVoxmapTextureIndices);
    mmFree(gVoxmapObjectIndices);
    mmFree(D_800A7C78);
}

void voxmap_update_cache_timers(void) {
    s32 i;

    for (i = 0; i < SLOT_COUNT; i++) {
        if (CACHE_TTL-1 > gVoxmapCacheTimers[i]) {
            gVoxmapCacheTimers[i]++;
        }
    }
}

VoxmapSlot *voxmap_reserve_slot(void) {
    s32 minTTL;
    s32 cachedItemIdx;
    s32 i;

    minTTL = -1;
    cachedItemIdx = -1;
    for (i = 0; i < SLOT_COUNT; i++) {
        if (gVoxmapSlotUsed[i] == FALSE && minTTL < gVoxmapCacheTimers[i]) {
            cachedItemIdx = i;
            minTTL = gVoxmapCacheTimers[i];
        }
    }

    if (cachedItemIdx != -1) {
        gVoxmapCacheTimers[cachedItemIdx] = CACHE_TTL;
        gVoxmapBlockIDs[cachedItemIdx] = -1;
        gVoxmapSlotUsed[cachedItemIdx] = TRUE;
        return D_800A7C80[cachedItemIdx];
    }

    return NULL;
}

void voxmap_release_slot(VoxmapSlot *slot) {
    s32 i;

    for (i = 0; i < SLOT_COUNT; i++) {
        if (slot == D_800A7C80[i]) {
            gVoxmapSlotUsed[i] = FALSE;
            i = SLOT_COUNT;
        }
    }
}

s32 voxmap_reload_slot(s32 blockID, s32 slotIndex, s32 trkBlkIndex, s32 blockIndex) {
    if ((gVoxmapTextureIndices[blockID + 1] == gVoxmapTextureIndices[blockID]) != 0) {
        return 0;
    }

    if (D_800A7C80[slotIndex]) {
        mmFree(D_800A7C80[slotIndex]);
    }

    D_800A7C80[slotIndex] = (VoxmapSlot*) voxmap_load_slot(blockID, slotIndex, trkBlkIndex, blockIndex);
    return 1;
}

u8 *voxmap_load_slot(s32 blockID, s32 slotIndex, s32 trkBlkIndex, s32 blockIndex) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 temp_v1;
    u8* sp2C;
    u8* temp_v0;

    sp34 = gVoxmapTextureIndices[blockID];
    sp3C = gVoxmapTextureIndices[blockID + 1] - sp34;
    if (sp3C == 0) {
        return 0;
    }
    sp38 = rarezip_uncompress_size_rom(TEXPRE_BIN, gVoxmapTextureIndices[blockID], 1);
    if (sp38 > 0x5000) {
        return 0;
    }
    temp_v0 = mmAlloc(sp38 + 0x80, ALLOC_TAG_VOX_COL, NULL);
    temp_v1 = ((temp_v0 + sp38) - sp3C) + 0x80;
    sp2C = (u8*)(temp_v1 - (temp_v1 % 16));
    read_file_region(TEXPRE_BIN, sp2C, sp34, sp3C);
    rarezip_uncompress(sp2C, temp_v0, sp38 + 0x80);
    temp_v0 = mmRealloc(temp_v0, sp38, NULL);
    for (temp_v1 = 0; temp_v1 < 2; ) {
        ((s32*)temp_v0)[temp_v1 + 7] = ((s32*)temp_v0)[temp_v1 + 7] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 9] = ((s32*)temp_v0)[temp_v1 + 9] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 5] = ((s32*)temp_v0)[temp_v1 + 5] + temp_v0;
        temp_v1++;
    }
    return temp_v0;
}

s32 func_800075B0(s32 arg0, s32 arg1) {
    if ((arg0 < 0) || (arg0 >= D_800A7D0C)) {
        return 0;
    }
    func_80012584(0x3C, 6U, (u32* ) &D_800A7C80[arg1], (ObjSetup* ) arg0, 0, 0, NULL, 0);
    return 1;
}

u8 *func_80007620(s32 arg0, s32 arg1) {
    s32 sp34;
    u8* temp_v0;
    s32 sp2C;
    s32 temp_v0_3;
    u8* sp24;
    s32 temp_v1;

    if (arg0 < 0 || arg0 >= D_800A7D0C) {
        return 0;
    }
    sp2C = gVoxmapObjectIndices[arg0];
    sp34 = gVoxmapObjectIndices[arg0 + 1] - sp2C;
    if (sp34 == 0) {
        return 0;
    }
    temp_v0_3 = rarezip_uncompress_size_rom(VOXOBJ_BIN, gVoxmapObjectIndices[arg0], 1);
    if (temp_v0_3 > 0x5000) {
        return 0;
    }
    temp_v0 = mmAlloc(temp_v0_3 + 0x80, ALLOC_TAG_VOX_COL, NULL);
    temp_v1 = ((temp_v0 + temp_v0_3) - sp34) + 0x80;
    sp24 = (u8*)(temp_v1 - (temp_v1 % 16));
    read_file_region(VOXOBJ_BIN, sp24, (u32) sp2C, sp34);
    rarezip_uncompress(sp24, temp_v0, temp_v0_3);
    temp_v0 = mmRealloc(temp_v0, temp_v0_3, NULL);
    for (temp_v1 = 0; temp_v1 < 2; ) {
        ((s32*)temp_v0)[temp_v1 + 7] = ((s32*)temp_v0)[temp_v1 + 7] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 9] = ((s32*)temp_v0)[temp_v1 + 9] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 5] = ((s32*)temp_v0)[temp_v1 + 5] + temp_v0;
        temp_v1++;
    }
    return temp_v0;
}

VoxmapSlot* voxmap_find_closest_slot(Vec3s16 *position) {
    s32 var_s1;
    s32 sp40;
    s32 pad2;
    s32 pad;
    s32 var_a1;
    s32 var_s0;
    s32 var_v0;
    s32 i;
    GlobalMapCell* mapCell;
    s8 arg3;

    if (gVoxmapCurrentObject != NULL) {
        return voxmap_find_closest_object_slot(position);
    }

    var_v0 = ((position->s[0] * SOME_FACTOR) - D_80092A60) + 5;
    var_s1 = ((position->s[2] * SOME_FACTOR) - D_80092A64) + 5;
    var_s0 = floor_f(var_v0 / BLOCKS_GRID_UNIT_F);
    sp40 = floor_f(var_s1 / BLOCKS_GRID_UNIT_F);
    gVoxmapLastSearchedSlot.unk0 = (var_s0 * BLOCKS_GRID_UNIT) + D_80092A60;
    gVoxmapLastSearchedSlot.unk4 = (sp40 * BLOCKS_GRID_UNIT) + D_80092A64;
    gVoxmapLastSearchedSlot.unk8 = gVoxmapLastSearchedSlot.unk0 / SOME_FACTOR;
    gVoxmapLastSearchedSlot.unkC = gVoxmapLastSearchedSlot.unk4 / SOME_FACTOR;
    var_s1 = -1;
    if (func_80044B18(var_s0, sp40, 0) != 0) {
        mapCell = func_80046698(var_s0, sp40);
        var_s1 = mapCell->blockID;
    }
    if (var_s1 != -1) {
        i = 0;
        var_s0 = -1;
        for (; i < SLOT_COUNT; i++) {
            if (var_s1 == gVoxmapBlockIDs[i]) {
                var_s0 = i;
                i = SLOT_COUNT;
            }
        }
        if (var_s0 != -1) {
            gVoxmapCacheTimers[var_s0] = 0;
            gVoxmapLastSearchedSlot.data = D_800A7C80[var_s0];
        } else {
            var_s0 = -1;
            var_a1 = -1;
            i = 0;
            for (; i < SLOT_COUNT; i++) {
                if (gVoxmapSlotUsed[i] == FALSE) {
                    if (var_a1 < gVoxmapCacheTimers[i]) {
                        var_s0 = i;
                        var_a1 =  gVoxmapCacheTimers[i];
                    }
                }
            }
            if (voxmap_reload_slot(var_s1, var_s0, mapCell->trkBlkIndex, mapCell->loadedBlockIndex) != 0) {
                gVoxmapBlockIDs[var_s0] = var_s1;
                gVoxmapCacheTimers[var_s0] = 0;
                D_800A7CF0[var_s0][0] = gVoxmapLastSearchedSlot.unk8;
                D_800A7CF0[var_s0][1] = gVoxmapLastSearchedSlot.unkC;
            }
            gVoxmapLastSearchedSlot.data = 0;
        }
    } else {
        gVoxmapLastSearchedSlot.data = 0;
    }
    return &gVoxmapLastSearchedSlot;
}

VoxmapSlot* voxmap_find_closest_object_slot(Vec3s16* position) {
    ObjDef* temp_v1;
    s32 temp_t5;
    s32 temp_t8;
    s32 var_a1;
    s32 i;
    s32 temp_s0;
    s32 sp24;

    temp_v1 = gVoxmapCurrentObject->def;
    temp_t8 = (position->s[0] - temp_v1->unk80) / 64;
    temp_t5 = (position->s[2] - temp_v1->unk82) / 64;
    gVoxmapLastSearchedSlot.unk0 = (temp_t8 * 0x280) + (temp_v1->unk80 * SOME_FACTOR);
    gVoxmapLastSearchedSlot.unk4 = (temp_t5 * 0x280) + (temp_v1->unk82 * SOME_FACTOR);
    gVoxmapLastSearchedSlot.unk8 = gVoxmapLastSearchedSlot.unk0 / SOME_FACTOR;
    gVoxmapLastSearchedSlot.unkC = gVoxmapLastSearchedSlot.unk4 / SOME_FACTOR;
    temp_s0 = temp_v1->unk7E;
    if (temp_s0 == -1) {
        gVoxmapLastSearchedSlot.data = 0;
        return &gVoxmapLastSearchedSlot;
    }
    i = 0;
    sp24 = -1;
    temp_s0 += (temp_t5 * (temp_v1->unk7F & 0xF)) + temp_t8;
    for (; i < SLOT_COUNT; i++) {
        if ((temp_s0 + 30000) == gVoxmapBlockIDs[i]) {
            sp24 = i;
            i = SLOT_COUNT;
        }
    }
    if (sp24 != -1) {
        gVoxmapCacheTimers[sp24] = 0;
        gVoxmapLastSearchedSlot.data = D_800A7C80[sp24];
    } else {
        i = 0;
        var_a1 = -1;
        sp24 = -1;
        for (; i < SLOT_COUNT; i++) {
            if (var_a1 < gVoxmapCacheTimers[i]) {
                sp24 = i;
                var_a1 = gVoxmapCacheTimers[i];
            }
        }
        if (func_800075B0(temp_s0, sp24) != 0) {
            gVoxmapBlockIDs[sp24] = (temp_s0 + 30000);
            gVoxmapCacheTimers[sp24] = 0;
            gVoxmapLastSearchedSlot.data = D_800A7C80[sp24];
        } else {
            gVoxmapLastSearchedSlot.data = 0;
        }
    }
    return &gVoxmapLastSearchedSlot;
}

VoxmapSlot* voxmap_get_last_found_slot(void) {
    return &gVoxmapLastSearchedSlot;
}

void func_80007CCC(s32 arg0) {
    rarezip_uncompress_size_rom(TEXPRE_BIN, gVoxmapTextureIndices[arg0], 1);
}

u8 *func_80007D08(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4, s32 arg5) {
    u32 var_v1;
    u8 *temp_v0;
    u8 *var_a1;
    s32 var_a0;
    s32 new_var;
    u8 new_var2;

    new_var = arg4 * 2 + arg4;
    if ((arg5 >> 3) != 0) {
        var_v1 = arg0[new_var + 1];
        var_v1 >>= 4;
        var_v1 |= arg0[new_var + 2] << 4;
        var_a1 = &arg2[(arg4 << 5) | 0x10];
    } else {
        var_v1 = arg0[new_var];
        var_v1 |= ((arg0[new_var + 1] & 0xF) << 8);
        var_a1 = &arg2[arg4 << 5];
    }
    temp_v0 = &arg2[(arg4 << 5) | ((arg5 << 1) + (arg3 >> 3))];
    while (var_a1 < temp_v0) {
        var_a0 = *var_a1;
        var_a1++;
        while (var_a0 != 0) {
            var_a0 &= (var_a0 - 1) & 0xFF;
            var_v1++;
        }
    }
    var_a0 = *var_a1;
    new_var2 = 0xFFU >> (8 - (arg3 & 7));
    var_a0 &= new_var2 & 0xFF;
    while (var_a0 != 0) {
        var_a0 &= (var_a0 - 1) & 0xFF;
        var_v1++;
    }
    return &arg1[var_v1 * 4];
}

void func_80007E2C(Vec3f* arg0, Vec3s16 *arg1) {
    arg0->x = (arg1->s[0] * SOME_FACTOR) + 5;
    arg0->y = (arg1->s[1] * SOME_FACTOR) + 5;
    arg0->z = (arg1->s[2] * SOME_FACTOR) + 5;
    if (gVoxmapCurrentObject != NULL) {
        transform_point_by_object(arg0->x, arg0->y, arg0->z, arg0->f, &arg0->y, &arg0->z, gVoxmapCurrentObject);
    }
}

void func_80007EE0(Vec3f* arg0, Vec3s16 *arg1) {
    Vec3f sp44;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;

    sp44.z = arg0->x;
    sp44.y = arg0->y;
    sp44.x = arg0->z;
    if (gVoxmapCurrentObject != NULL) {
        inverse_transform_point_by_object(sp44.z, sp44.y, sp44.x, &sp44.z, &sp44.y, &sp44.x, gVoxmapCurrentObject);
    }
    var_a0 = sp44.z;
    var_a1 = sp44.y;
    var_a2 = sp44.x;
    if (sp44.z < 0.0f) {
        var_a0 -= SOME_FACTOR;
    }
    if (sp44.y < 0.0f) {
        var_a1 -= SOME_FACTOR;
    }
    if (sp44.x < 0.0f) {
        var_a2 -= SOME_FACTOR;
    }
    arg1->s[0] = var_a0 / SOME_FACTOR;
    arg1->s[1] = var_a1 / SOME_FACTOR;
    arg1->s[2] = var_a2 / SOME_FACTOR;
}

s32 func_80008048(Vec3s16 *arg0, Vec3s16 *arg1, Vec3s16 *arg2, u8* arg3, u8 arg4) {
    s32 spD4;
    s32 spD0;
    s32 spCC;
    s32 temp_a3;
    s32 temp_s0;
    s32 temp_t1;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 spB0;
    s32 spAC;
    s32 spA8;
    s32 spA4;
    VoxmapSlotData *var_s4;
    s32 sp9C;
    s32 var_s5;
    s32 var_s7;
    s32 var_v0;
    s32 var_v1;
    s32 sp88;
    Vec3s16 sp80 = arg0[0];
    Vec3s16 sp78;
    VoxmapSlotData *tempObj;
    s32 pad;
    s32 temp2;
    u8* sp68;
    u8 sp67;
    u8 temp;

    spD4 = 1;
    spB0 = arg1->s[0] - sp80.s[0];
    if (spB0 < 0) {
        spD4 = -1;
        spB0 = -spB0;
    }
    spD0 = 1;
    spAC = arg1->s[1] - sp80.s[1];
    if (spAC < 0) {
        spD0 = -1;
        spAC = -spAC;
    }
    spCC = 1;
    spA8 = arg1->s[2] - sp80.s[2];
    if (spA8 < 0) {
        spCC = -1;
        spA8 = -spA8;
    }
    var_s7 = spAC - spB0;
    var_s2 = spA8 - spB0;
    var_s3 = spAC - spA8;
    spA4 = spB0 + spAC + spA8;
    voxmap_find_closest_slot(&sp80);
    var_s5 = (sp80.s[0] - gVoxmapLastSearchedSlot.unk8);
    var_s5 &= 0x3F;
    var_s1 = (sp80.s[2] - gVoxmapLastSearchedSlot.unkC);
    var_s1 &= 0x3F;
    sp78 = sp80;
    var_s4 = NULL;
    sp67 = 1;
    while (spA4--) {
        if (arg4 && sp67 != 0) {
            sp67 = 0;
        } else {
            tempObj = gVoxmapLastSearchedSlot.data;
            if (tempObj != 0) {
                if ((tempObj != var_s4) || (sp80.s[1] != sp78.s[1])) {
                    sp88 = 1;
                    if (tempObj->unk4 > sp80.s[1]) {
                        sp9C = 0;
                        sp78.s[1] = sp80.s[1];
                    } else {
                        if (sp80.s[1] >= tempObj->unkC) {
                            sp9C = (tempObj->unkC - tempObj->unk4) - 1;
                            sp78.s[1] = sp80.s[1];
                        } else {
                            sp9C = sp80.s[1] - tempObj->unk4;
                            sp78.s[1] = sp80.s[1];
                        }
                    }
                    var_s4 = tempObj;
                }
                temp_a3 = var_s5 >> 2;
                temp_t1 = var_s1 >> 2;
                var_v0 = tempObj->unk24[((sp9C << 5) | ((temp_t1 * 2) + (temp_a3 >> 3)))];
                // @fake
                if (!var_s5) {}
                if ((var_v0 >> (temp_a3 & 7)) & 1) {
                    // var_s1 * 0 is @fake
                    if (sp88 != (var_s1 * 0)) {
                        sp68 = func_80007D08(tempObj->unk1C, tempObj->unk14, tempObj->unk24, temp_a3, sp9C, temp_t1);
                        sp88 = 0;
                    }
                    temp2 = sp68[var_s1 & 3];
                    // @fake
                    if (temp2>>((var_s5 & 3) << 1)) {}
                    temp = (temp2>>((var_s5 & 3) << 1)) & 3;
                    if (temp & 3) {
                        if (arg3 != NULL) {
                            arg3[0] = temp & 3;
                        }
                        if (arg2 != NULL) {
                            arg2[0] = sp78;
                        }
                        return 0;
                    }
                }
            }
        }
        if (var_s7 < 0) {
            if (var_s2 < 0) {
                var_s7 += spAC * 2;
                sp78.s[0] = sp80.s[0];
                sp80.s[0] += spD4;\
                var_s2 += spA8 * 2;
                temp_s0 = var_s5 >> 2;
                var_v1 = sp80.s[0] - gVoxmapLastSearchedSlot.unk8;
                if ((var_v1 >> 6) != 0) {
                    voxmap_find_closest_slot(&sp80);
                    var_s4 = NULL;
                }
                var_v1 = sp80.s[0] - gVoxmapLastSearchedSlot.unk8;
                var_s5 = var_v1 & 0x3F;
                if (temp_s0 != (var_s5 >> 2)) {
                    sp88 = 1;
                }
            } else {
                sp78.s[2] = sp80.s[2];
                sp80.s[2] += spCC;\
                var_s2 -= spB0 * 2;
                var_s3 += spAC * 2;
                temp_s0 = var_s1 >> 2;
                var_v1 = sp80.s[2] - gVoxmapLastSearchedSlot.unkC;
                if ((var_v1 >> 6) != 0) {
                    voxmap_find_closest_slot(&sp80);
                    var_s4 = NULL;
                }
                var_v1 = sp80.s[2] - gVoxmapLastSearchedSlot.unkC;
                var_s1 = var_v1 & 0x3F;
                if (temp_s0 != (var_s1 >> 2)) {
                    sp88 = 1;
                }
            }
        } else {
            if (var_s3 < 0) {
                sp78.s[2] = sp80.s[2];
                sp80.s[2] += spCC;\
                var_s2 -= spB0 * 2;\
                var_s3 += spAC * 2;
                var_v1 = sp80.s[2] - gVoxmapLastSearchedSlot.unkC;
                temp_s0 = var_s1 >> 2;
                if ((var_v1 >> 6) != 0) {
                    voxmap_find_closest_slot(&sp80);
                    var_s4 = NULL;
                }
                var_v1 = sp80.s[2] - gVoxmapLastSearchedSlot.unkC;
                var_s1 = var_v1 & 0x3F;
                if (temp_s0 != (var_s1 >> 2)) {
                    sp88 = 1;
                }
            } else {
                sp78.s[1] = sp80.s[1];
                var_s7 -= spB0 * 2;\
                sp80.s[1] += spD0;\
                var_s3 -= spA8 * 2;
            }
        }
    }
    if (arg2 != NULL) {
        arg2[0] = arg1[0];
    }
    return 1;
}

#ifndef NON_EQUIVALENT
void* func_80008524(Vec3s16 *arg0, Vec3s16 *arg1, s32* arg2, s32* arg3, s32* arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008524.s")
#else
// https://decomp.me/scratch/x1DTn
void* func_80008524(Vec3s16 *arg0, Vec3s16 *arg1, s32* arg2, s32* arg3, s32* arg4) {
    f32 temp_fv0;
    s32 temp_a2;
    s32 temp_t2;
    s32 temp_t3;
    u16 temp_v0_3;
    u16 temp_v1_3;
    s32 var_a0;
    s32 var_a1;
    s32 var_a3;
    s32 var_t0;
    s32 var_t1;
    s32 var_v0;
    s32 var_v1;
    s32 sp138;
    s32 spDC[0x14];
    s32 sp8C[0x14];

    temp_fv0 = sqrtf(SQ(arg0->s[0] - arg1->s[0]) + SQ(arg0->s[2] - arg1->s[2]));
    var_a3 = 0;
    var_v0 = temp_fv0;
    if (var_v0 < (temp_fv0 - 0.5f)) {
        var_v0 = var_v0 + 1;
    }
    var_a0 = (var_v0 * -2) + 5;
    var_t0 = var_v0;
    var_v1 = 1 - var_t0;
    var_a1 = 3;
    sp138 = 0;
    spDC[sp138] = var_a3;
    sp8C[sp138] = var_t0;
    while (var_t0 > var_a3) {
        if (var_v1 > 0) {
            var_v1 += var_a0;
            var_a0 += 4;
            var_t0 -= 1;
        } else {
            var_v1 += var_a1;
            var_a0 += 2;
        }
        var_a3++;
        spDC[sp138] = var_a3;
        sp8C[sp138] = var_t0;
        var_a1 += 2;
        sp138++;
        if (sp138 >= 0x14) {
            return NULL;
        }
    }
    var_t1 = 0;
    temp_a2 = sp138 * 2;
    while (var_t1 <= sp138) {
        s32 var_s2 = ((arg0->s[1] + sp8C[var_t1]) << 0x10);
        s32 var_s0 = ((arg0->s[1] + spDC[var_t1]) << 0x10);
        s32 temp = ((arg0->s[1] - spDC[var_t1]) << 0x10);
        s32 temp2 = ((arg0->s[1] - sp8C[var_t1]) << 0x10);
        temp_t3 = temp_a2 - var_t1;
        var_a0 = var_t1 + temp_a2;
        var_a1 = temp_t3 + temp_a2;
        temp_v0_3 = arg0->s[0] + spDC[var_t1];
        temp_v1_3 = arg0->s[0] + sp8C[var_t1];
        D_800A7C98[var_t1] = temp_v0_3 | var_s2;
        D_800A7C98[temp_t3] = temp_v1_3 | var_s0;
        D_800A7C98[var_a0] = temp_v1_3 | temp;
        D_800A7C98[var_a1] = temp_v0_3 | temp2;
        var_a0 += temp_a2;
        var_a1 += temp_a2;
        temp_v0_3 = arg0->s[0] - spDC[var_t1];
        temp_v1_3 = arg0->s[0] - sp8C[var_t1];
        D_800A7C98[var_a0] = temp_v0_3 | temp2;
        D_800A7C98[var_a1] = temp_v1_3 | temp;
        D_800A7C98[var_a0 + temp_a2] = temp_v1_3 | var_s0;
        D_800A7C98[var_a1 + temp_a2] = temp_v0_3 | var_s2;
        var_t1 += 1;
    }
    *arg2 = sp138 * 8;
    *arg3 = -2;
    *arg4 = -2;
    temp_a2 = (arg1->s[0] & 0xFFFF) | (arg1->s[2] << 0x10);
    var_t1 = 0;
    while (var_t1 < *arg2 && (temp_a2 != D_800A7C98[var_t1])) {
        var_t1 += 1;
    }
    if (var_t1 == *arg2) {
        temp_t2 = (arg1->s[0] & 0xFFFF) |  ((arg1->s[2] - 1) << 0x10);
        temp_a2 = (arg1->s[0] & 0xFFFF) |  ((arg1->s[2] + 1) << 0x10);
        var_t1 = 0;
        while (var_t1 < *arg2 && *arg3 == -2 && *arg4 == -2) {
            if (temp_t2 == D_800A7C98[var_t1]) {
                *arg3 = var_t1;
                *arg4 = var_t1 + 1;
            } else if (temp_a2 == D_800A7C98[var_t1]) {
                *arg3 = var_t1 - 1;
                *arg4 = var_t1;
            }
            var_t1 += 1;
        }
    } else {
        *arg3 = var_t1 - 1;
        *arg4 = var_t1 + 1;
    }
    if (*arg3 < 0) {
        *arg3 += *arg2;
    }
    if (*arg4 >= *arg2) {
        *arg4 = *arg4 - *arg2;
    }
    return D_800A7C98;
}
#endif

s32 func_8000896C(Vec3s16 *arg0, Vec3s16 *arg1, Vec3s16 *arg2) {
    s32 pad;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 var_s0;
    s32 var_s2;
    s32 sp6C;
    s32 var_s3;
    s32 var_s5;
    Vec3s16 sp5C;
    Vec3s16 sp54;
    s32* temp_v0;

    temp_v0 = func_80008524(arg0, arg1, &sp6C, &sp80, &sp7C);
    if (temp_v0 == NULL) {
        return 0;
    }
    sp78 = (sp6C >> 1) + sp7C;
    if (sp78 >= sp6C) {
        sp78 -= sp6C;
    }
    var_s0 = FALSE;
    var_s5 = 0;
    arg2[0] = arg1[0];
    var_s2 = FALSE;
    var_s3 = FALSE;
    sp5C.y = arg1->y;
    while (var_s0 == FALSE) {
        sp5C.x = temp_v0[sp7C] & 0xFFFF & 0xFFFF;
        sp5C.z = temp_v0[sp7C] >> 0x10;
        if ((var_s2 == FALSE) && func_80008048(&sp5C, arg0, &sp54, NULL, 0)) {
            var_s5 = -1;
            var_s0 = TRUE;
            arg2[0] = sp5C;
        } else {
            if (sp7C++ == sp78) {
                var_s0 = TRUE;
            } else if (sp7C >= sp6C) {
                sp7C = 0;
            }
            if ((sp54.x == sp5C.x) && (sp54.y == sp5C.y) && (sp54.z == sp5C.z)) {
                var_s2 = TRUE;
            }
        }
        if (var_s0 == FALSE) {
            sp5C.x = temp_v0[sp80] & 0xFFFF & 0xFFFF;
            sp5C.z = temp_v0[sp80] >> 0x10;
            if ((var_s3 == FALSE) && func_80008048(&sp5C, arg0, &sp54, NULL, 0)) {
                var_s0 = TRUE;
                var_s5 = 1;
                arg2[0] = sp5C;
            } else {
                sp80--;
                if (sp80 < 0) {
                    sp80 = sp6C - 1;
                }
                if ((sp54.x == sp5C.x) && (sp54.y == sp5C.y) && (sp54.z == sp5C.z)) {
                    var_s3 = TRUE;
                }
            }
        }
    }
    return var_s5;
}

s32 func_80008BE4(UnkCurvesStruct* arg0, s32 arg1) {
    s32 i;
    s32 j;
    s32 stepCount;
    f32 sp120[21];
    f32 spCC[21];
    f32 sp78[21];
    Vec3s16 sp70;
    Vec3s16 sp68;
    Vec3f sp5C;

    stepCount = 1;
    if ((arg0->unk94 == func_80004C5C) || (arg0->unk94 == func_80004D70)) {
        stepCount = 4;
    }

    for (i = 0; i <= (arg0->unk90 - 4); ) {
        func_8000598C(&arg0->unk84[i], &arg0->unk88[i], &arg0->unk8C[i], sp120, spCC, sp78, arg1, arg0->unk98);
        for (j = 0; j < arg1; j++) {
            sp5C.f[0] = sp120[j];
            sp5C.f[1] = spCC[j];
            sp5C.f[2] = sp78[j];
            func_80007EE0(&sp5C, &sp70);
            sp5C.f[0] = sp120[j + 1];
            sp5C.f[1] = spCC[j + 1];
            sp5C.f[2] = sp78[j + 1];
            func_80007EE0(&sp5C, &sp68);
            if (func_80008048(&sp70, &sp68, NULL, NULL, 0U) == 0) {
                return 1;
            }
        }
        i += stepCount;
    }
    return 0;
}

void func_80008D90(Object* arg0) {
    gVoxmapCurrentObject = arg0;
    if ((arg0 != NULL) && (arg0->def->unk7E == -1)) {
        gVoxmapCurrentObject = NULL;
    }
}

void func_80008DC0(SomeVoxmapAllocStruct* arg0) {
    arg0->unk0 = (SomeVoxmapAllocStructUnk0*) mmAlloc(sizeof(SomeVoxmapAllocStructUnk0) + sizeof(SomeVoxmapAllocStructUnk4) + sizeof(SomeVoxmapAllocStructUnk8), ALLOC_TAG_VOX_COL, NULL);
    arg0->unk4 = (SomeVoxmapAllocStructUnk4*) ((u8*)arg0->unk0 + sizeof(SomeVoxmapAllocStructUnk0));
    arg0->unk8 = (SomeVoxmapAllocStructUnk8*) ((u8*)arg0->unk4 + sizeof(SomeVoxmapAllocStructUnk4));
}

void func_80008E08(SomeVoxmapAllocStruct* arg0) {
    if (arg0->unk0 != NULL) {
        mmFree(arg0->unk0);
        arg0->unk0 = NULL;
    }
}

s32 func_80008E40(Unk80008E40* arg0, Vec3f* arg1, Vec3f* arg2) {
    Unk80008E40Unk0* temp_v0;
    Vec3s16 sp2C;

    func_8000A650(arg0);
    func_80007EE0(arg1, &arg0->unk12);
    func_80007EE0(arg2, &arg0->unkC);
    arg0->unk12.s[0] &= ~1;
    arg0->unk12.s[2] &= ~1;
    arg0->unkC.s[0] &= ~1;
    arg0->unkC.s[2] &= ~1;
    if (func_800099D0(&arg0->unk12, &arg0->unkC, &sp2C) != 0) {
        return 1;
    }
    arg0->unk24 = 10000;
    temp_v0 = func_8000A9AC(arg0, &sp2C, 0, 0xFF);
    func_8000A80C(arg0->unk4, &arg0->unk1E, arg0->unk1C - 1, temp_v0->unk6 + temp_v0->unk8);
    arg0->unk20 = 0;
    return 0;
}

s32 func_80008F34(Unk80008E40* arg0, s32 arg1) {
    s32 temp_v0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    Unk80008E40Unk0* temp_a1;

    var_s1 = 0;
    var_s3 = 0;
    while (var_s1 == 0 && arg1 != 0) {
        temp_v0 = func_8000A934(arg0->unk4, &arg0->unk1E);
        if (temp_v0 >= 0) {
            arg0->unk18 = temp_v0;
            temp_a1 = &arg0->unk0[temp_v0];
            if ((arg0->unkC.s[0] == temp_a1->unk0.s[0]) && (arg0->unkC.s[2] == temp_a1->unk0.s[2])) {
                var_s1 = 1;
                var_s3 = 1;
            } else {
                temp_a1->unkC = 1;
                func_80009F5C(arg0, temp_a1, temp_v0);
            }
        } else {
            var_s1 = 1;
            var_s3 = -1;
        }
        arg1 -= 1;
    }
    return var_s3;
}

s32 func_80009024(Unk80009024* arg0, Unk80008E40* arg1) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;

    sp2C = arg0->unk24;
    sp24 = 0;
    sp28 = 0;
    if (sp2C == 0) {
        sp24 = 0;
        if (func_80008E40(arg1, &arg0->unk0, &arg0->unkC) != 0) {
            arg0->unk18.x = arg0->unkC.x;
            arg0->unk18.y = arg0->unkC.y;
            arg0->unk18.z = arg0->unkC.z;
            sp28 = 1;
            sp24 = 1;
        } else {
            sp2C = 1;
        }
    }
    if (sp2C != 0) {
        sp28 = 1;
        switch (func_80008F34(arg1, arg0->unk27)) {
        case 0:
            if (sp2C++ >= arg0->unk26) {
                sp2C = 0;
                if (func_8000921C(arg1, 1) != 0) {
                    arg0->unk18.x = arg1->unk8->x;
                    arg0->unk18.y = arg1->unk8->y;
                    arg0->unk18.z = arg1->unk8->z;
                } else {
                    sp24 = 1;
                    arg0->unk18.x = arg0->unkC.x;
                    arg0->unk18.y = arg0->unkC.y;
                    arg0->unk18.z = arg0->unkC.z;
                }
            }
            break;
        case 1:
            sp2C = 0;
            if (func_8000921C(arg1, 1) != 0) {
                arg0->unk18.x = arg1->unk8->x;
                arg0->unk18.y = arg1->unk8->y;
                arg0->unk18.z = arg1->unk8->z;
            } else {
                sp24 = 1;
                arg0->unk18.x = arg0->unkC.x;
                arg0->unk18.y = arg0->unkC.y;
                arg0->unk18.z = arg0->unkC.z;
            }
            break;
        case -1:
            sp2C = 0;
            sp24 = 1;
            arg0->unk18.x = arg0->unk0.x;
            arg0->unk18.y = arg0->unk0.y;
            arg0->unk18.z = arg0->unk0.z;
        }
    }
    arg0->unk24 = sp2C;
    arg0->unk25 = sp24;
    return sp28;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000921C.s")
#else
// https://decomp.me/scratch/ZVLXX
s32 func_8000921C(Unk80008E40* arg0, s32 arg1) {
    Unk80008E40Unk0* var_s1;
    Unk80008E40Unk0* next;
    Unk80008E40Unk0* var_s5;
    Vec3f sp68;
    s32 var_s3;
    s32 pad[2];
    Unk80008E40Unk0 sp4C;
    s32 currentIdx;
    u8 nextIdx;

    if (arg1 < 0) {
        arg1 = 0xA;
    }
    currentIdx = arg0->unk18;
    next = &arg0->unk0[currentIdx];
    next->unkB = 0xFF;
    nextIdx = next->unkA;
    while (nextIdx != 0xFF) {
        next = &arg0->unk0[nextIdx];
        next->unkB = currentIdx;
        currentIdx = nextIdx;
        nextIdx = next->unkA;
    }
    sp4C.unk0.s[0] = arg0->unk12.s[0];
    sp4C.unk0.s[1] = arg0->unk12.s[1];
    sp4C.unk0.s[2] = arg0->unk12.s[2];
    sp4C.unkB = currentIdx;
    var_s1 = next->unkB == 0xFF ? NULL : &arg0->unk0[next->unkB];
    var_s5 = next;
    next = &sp4C;
    var_s3 = 0;
    while (arg1 > var_s3 && var_s1 != NULL) {
        if (((var_s1->unk0.s[0] != next->unk0.s[0]) || (var_s1->unk0.s[2] != next->unk0.s[2])) && (func_800099D0(&var_s1->unk0, &next->unk0, NULL) == 0)) {
            func_80007E2C(&sp68, &var_s5->unk0);
            arg0->unk8[var_s3].x = (s32) sp68.f[0] + 5;
            arg0->unk8[var_s3].y = (s32) sp68.f[1];
            arg0->unk8[var_s3].z = (s32) sp68.f[2] + 5;
            var_s3 += 1;
            next = var_s1;
        }
        var_s5 = var_s1;
        var_s1 = var_s1->unkB == 0xFF ? NULL : &arg0->unk0[var_s1->unkB];
    }
    if (var_s3 < arg1) {
        func_80007E2C(&sp68, &var_s5->unk0);
        arg0->unk8[var_s3].x = (s32) sp68.f[0] + 5;
        arg0->unk8[var_s3].y = (s32) sp68.f[1];
        arg0->unk8[var_s3].z = (s32) sp68.f[2] + 5;
        var_s3 += 1;
        if (var_s3 >= 0xA) {
            var_s3 = 0xA;
        }
    }
    arg0->unk20 = var_s3;
    arg0->unk22 = 0;
    return var_s3;
}
#endif

s32 func_80009528(Unk80008E40* arg0, Unk80009024* arg1) {
    if (arg0->unk22 < arg0->unk20) {
        arg1->unk18.x = arg0->unk8[arg0->unk22].x;
        arg1->unk18.y = arg0->unk8[arg0->unk22].y;
        arg1->unk18.z = arg0->unk8[arg0->unk22].z;
        arg0->unk22++;
        return 1;
    }

    return 0;
}

void func_800095B0(Gfx** gdl, Vtx_t** vertices, DLTri** tris, Mtx** matrics, Unk80008E40* arg4) {
    DLTri* tri;
    s32 temp_v0;
    s32 var_fp;
    s32 var_s4;
    Gfx* gfx;
    Vtx_t* vtx;

    vtx = *vertices;
    tri = *tris;
    gfx = *gdl;
    tex_gdl_set_textures(&gfx, NULL, NULL, 3, 0, 0, 1);
    dl_set_prim_color(&gfx, 0xFF, 0xFF, 0xFF, 0xFF);
    func_80003168(&gfx, matrics, 0, 0, 0, 1.0f);
    var_fp = 1;
    var_s4 = 0;
    temp_v0 = 0;
    while (temp_v0 < (arg4->unk20 - 1)) {
        gSPVertex(gfx++, OS_PHYSICAL_TO_K0(vtx), 4, 0);
        dl_triangles(&gfx, tri, 2);
        vtx[0].ob[0] = arg4->unk8[var_s4].x - D_80092A60;
        vtx[0].ob[1] = arg4->unk8[var_s4].y + 5.0f;
        vtx[0].ob[2] = arg4->unk8[var_s4].z - D_80092A64;
        vtx[0].cn[3] = 0xFF;
        vtx[0].cn[2] = 0;
        vtx[0].cn[1] = 0;
        vtx[0].cn[0] = 0xFF;
        vtx[0].flag = 0;
        vtx[0].tc[1] = 0;
        vtx[0].tc[0] = 0;

        vtx[1].ob[0] = arg4->unk8[var_s4].x - D_80092A60;
        vtx[1].ob[1] = arg4->unk8[var_s4].y;
        vtx[1].ob[2] = arg4->unk8[var_s4].z - D_80092A64;
        vtx[1].cn[3] = 0xFF;
        vtx[1].cn[2] = 0;
        vtx[1].cn[1] = 0;
        vtx[1].cn[0] = 0xFF;
        vtx[1].flag = 0;
        vtx[1].tc[1] = 0;
        vtx[1].tc[0] = 0;

        vtx[2].ob[0] = arg4->unk8[var_fp].x - D_80092A60;
        vtx[2].ob[1] = arg4->unk8[var_fp].y + 5.0f;
        vtx[2].ob[2] = arg4->unk8[var_fp].z - D_80092A64;
        vtx[2].cn[3] = 0xFF;
        vtx[2].cn[2] = 0;
        vtx[2].cn[1] = 0;
        vtx[2].cn[0] = 0xFF;
        vtx[2].flag = 0;
        vtx[2].tc[1] = 0;
        vtx[2].tc[0] = 0;

        vtx[3].ob[0] = arg4->unk8[var_fp].x - D_80092A60;
        vtx[3].ob[1] = arg4->unk8[var_fp].y;
        vtx[3].ob[2] = arg4->unk8[var_fp].z - D_80092A64;
        vtx[3].cn[3] = 0xFF;
        vtx[3].cn[2] = 0;
        vtx[3].cn[1] = 0;
        vtx[3].cn[0] = 0xFF;
        vtx[3].flag = 0;
        vtx[3].tc[1] = 0;
        vtx[3].tc[0] = 0;

        tri[0].unk0 = 0x40;
        tri[0].v0 = 2;\
        tri[0].v1 = 1;\
        tri[0].v2 = 0;
        tri[1].unk0 = 0x40;
        tri[1].v0 = 3;\
        tri[1].v1 = 2;\
        tri[1].v2 = 1;

        temp_v0 = var_fp;
        var_s4 += 1;
        var_fp += 1;
        vtx += 4;
        tri += 2;
    }
    *vertices = vtx;
    *tris = tri;
    *gdl = gfx;
}

s32 func_800099D0(Vec3s16* arg0, Vec3s16* arg1, Vec3s16* arg2) {
    s32 sp104;
    s32 sp100;
    s32 temp_a0;
    s32 pad_sp98;
    s32 var_s1;
    s32 temp_s4;
    s32 temp_t2;
    s32 var_a1;
    s32 var_a2;
    s32 spE0;
    s32 spD8[2];
    s32 spD4;
    s32 spCC[2];
    s32 spC4[2];
    s32 spC0;
    s32 var_a3;
    s32 spB8;
    s32 pad_spB4;
    s32 var_t0;
    s32 var_v0;
    u8 *temp2;
    s32 var_v1;
    Vec3s16 sp9C;
    Vec3s16 sp94;
    u8 *temp_v0_3;
    u8 *temp;
    VoxmapSlotData* temp_s2;
    u8 sp7C[12];
    s16 *new_var;
    s32 pad[2];

    sp104 = 2;
    sp9C = arg0[0];
    var_v1 = arg1->s[0] - sp9C.s[0];
    if (var_v1 < 0) {
        sp104 = -2;
        var_v1 = -var_v1;
    }
    sp100 = 2;
    var_v0 = arg1->s[2] - sp9C.s[2];
    if (var_v0 < 0) {
        sp100 = -2;
        var_v0 = -var_v0;
    }
    var_v1 = var_v1 >> 1;
    var_v0 = var_v0 >> 1;
    spE0 = var_v0 - var_v1;
    spD8[0] = var_v0;
    spD8[1] = var_v1;
    voxmap_find_closest_slot(&sp9C);
    spC0 = (sp9C.s[0] - gVoxmapLastSearchedSlot.unk8) & 0x3F;
    spB8 = (sp9C.s[2] - gVoxmapLastSearchedSlot.unkC) & 0x3F;
    spCC[0] = (spC0 & 3) << 1;
    spCC[1] = spCC[0] + 2;
    sp94 = sp9C;
    spD4 = (var_a1 = spD8[0] + spD8[1]);
    while (spD4--) {
        temp_s2 = gVoxmapLastSearchedSlot.data;
        new_var = sp9C.s;
        temp_t2 = spC0 >> 2;
        temp_s4 = spB8 >> 2;
        if (temp_s2 != NULL) {
            spC4[0] = spB8 & 3;
            spC4[1] = spC4[0] + 1;
            var_a2 = 0;
            for (; var_a2 < 3; var_a2++) {
                temp_a0 = new_var[1] + var_a2;
                var_v0 = temp_a0 - 1;
                if (var_v0) {}
                if (temp_s2->unk4 > var_v0) {
                    var_v0 = 0;
                } else {
                    if (var_v0 >= temp_s2->unkC) {
                        var_v0 = temp_s2->unkC - temp_s2->unk4;
                        var_v0--;
                    } else {
                        var_v0 = temp_a0 - temp_s2->unk4;
                        var_v0--;
                    }
                }
                var_t0 = temp_t2 & 7;
                temp_a0 = temp_s2->unk24[(var_v0 << 5) | ((temp_s4 * 2) + (temp_t2 >> 3))];
                if ((temp_a0 >> var_t0) & 1) {
                    temp_v0_3 = func_80007D08(temp_s2->unk1C, temp_s2->unk14, temp_s2->unk24, temp_t2, var_v0, temp_s4);
                    temp_a0 = temp_v0_3[spC4[0]];
                    sp7C[(var_a2 * 4) + 0] = (temp_a0 >> spCC[0]) & 3;
                    sp7C[(var_a2 * 4) + 1] = (temp_a0 >> spCC[1]) & 3;
                    temp_a0 = temp_v0_3[spC4[1]];
                    sp7C[(var_a2 * 4) + 2] = (temp_a0 >> spCC[0]) & 3;
                    sp7C[(var_a2 * 4) + 3] = (temp_a0 >> spCC[1]) & 3;
                } else {
                    sp7C[(var_a2 * 4) + 0] = 0;
                    sp7C[(var_a2 * 4) + 1] = 0;
                    sp7C[(var_a2 * 4) + 2] = 0;
                    sp7C[(var_a2 * 4) + 3] = 0;
                }
            }

            for (var_a2 = 1; var_a2 >= 0; var_a2--) {
                var_a3 = FALSE;
                var_t0 = var_a2;
                if ((sp7C[(var_a2 * 4) + 0] & 2) || (sp7C[(var_a2 * 4) + 1] & 2) || (sp7C[(var_a2 * 4) + 2] & 2) || (sp7C[(var_a2 * 4) + 3] & 2)) {
                    var_a3 = TRUE;
                }
                var_s1 = var_a2 + 1;
                if ((var_a3 == 0) && ((sp7C[(var_s1 * 4) + 0] & 2) || (sp7C[(var_s1 * 4) + 1] & 2) || (sp7C[(var_s1 * 4) + 2] & 2) || (sp7C[(var_s1 * 4) + 3] & 2))) {
                    var_a3 = TRUE;
                }
                if (var_a3 == FALSE) {
                    temp = &sp7C[(var_s1 << 2)];
                    do { temp2 = &sp7C[var_a2 << 2]; } while (0);
                    var_a1 = temp2[0]; temp_a0 = temp[0];
                    var_a1 += temp2[1]; temp_a0 += temp[1];
                    var_a1 += temp2[2]; temp_a0 += temp[2];
                    var_a1 += temp2[3]; temp_a0 += temp[3];

                    if ((var_s1 == 2) && (temp_a0 == 0)) {
                        var_a3 = TRUE;
                    } else {
                        if (var_s1 == 1) {
                            if (var_a1 >= temp_a0) {
                                var_t0 = var_a2 - 1;
                            } else {
                                var_a1 = temp_a0;
                            }
                        } else if (temp_a0 < var_a1) {
                            var_t0 = var_a2 - 1;
                        } else {
                            var_a1 = temp_a0;
                        }
                        if (var_a1 < 2) {
                            var_a3 = TRUE;
                        } else {
                            var_a2 = 0;
                        }
                    }
                }
            }
            if (var_a3) {
                if (arg2 != NULL) {
                    arg2[0] = sp94;
                }
                return 0;
            }
            new_var[1] = new_var[1] + var_t0;
            sp94.s[1] = new_var[1];
        }
        if (spE0 < 0) {
            sp94.s[0] = new_var[0];
            new_var[0] += sp104;\
            spE0 += spD8[0] * 2;
            if (((new_var[0] - gVoxmapLastSearchedSlot.unk8) >> 6) != 0) {
                voxmap_find_closest_slot(&sp9C);
            }
            spC0 = (new_var[0] - gVoxmapLastSearchedSlot.unkC) & 0x3F;
            spCC[0] = (spC0 & 3) << 1;
            var_v1 = spCC[0];
            spCC[1] = var_v1 + 2;
        } else {
            sp94.s[2] = new_var[2];
            new_var[2] += sp100;\
            spE0 -= spD8[1] * 2;
            var_v1 = new_var[2] - gVoxmapLastSearchedSlot.unkC;
            if ((var_v1 >> 6) != 0) {
                voxmap_find_closest_slot(&sp9C);
            }
            var_v1 = new_var[2] - gVoxmapLastSearchedSlot.unkC;
            spB8 = var_v1 & 0x3F;
        }
    }
    if (arg2 != NULL) {
        arg2[0] = arg1[0];
    }
    return 1;
}

void func_80009F5C(Unk80008E40* arg0, Unk80008E40Unk0* arg1, s32 arg2) {
    Vec3s16 sp30;
    u16 temp_t6;

    temp_t6 = (arg1->unk8 + 1);
    sp30.s[0] = arg1->unk0.s[0];
    sp30.s[1] = arg1->unk0.s[1];
    sp30.s[2] = arg1->unk0.s[2];
    sp30.s[0] += 2;
    func_8000A078(arg0, arg1, arg2, temp_t6, &sp30);
    sp30.s[0] -= 4;
    sp30.s[1] = arg1->unk0.s[1];
    func_8000A078(arg0, arg1, arg2, temp_t6, &sp30);
    sp30.s[0] += 2;
    sp30.s[2] += 2;
    sp30.s[1] = arg1->unk0.s[1];
    func_8000A078(arg0, arg1, arg2, temp_t6, &sp30);
    sp30.s[2] -= 4;
    sp30.s[1] = arg1->unk0.s[1];
    func_8000A078(arg0, arg1, arg2, temp_t6, &sp30);
}

#ifndef NON_EQUIVALENT
void func_8000A078(Unk80008E40* arg0, Unk80008E40Unk0* arg1, s32 arg2, u16 arg3, Vec3s16* arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A078.s")
#else
// https://decomp.me/scratch/jUNws
void func_8000A078(Unk80008E40* arg0, Unk80008E40Unk0* arg1, s32 arg2, u16 arg3, Vec3s16* arg4) {
    Unk80008E40Unk0* temp_v1_3;
    s32 var_v0;
    s32 sp9C;
    s32 sp98;
    s32 temp_a0;
    s32 temp_a2_2;
    s32 temp_t6;
    s32 sp84[2];
    s32 sp7C[2];
    s32 sp78;
    s32 temp_v0;
    s32 sp70;
    s32 sp6C;
    u8 *temp_v0_2;
    s32 new_var;
    s32 new_var2;
    s32 var_v1; // sp30
    s32 var_a0; // sp2C?
    VoxmapSlotData* sp54;
    Unk80008E40Unk0* sp50;
    s32 var_a1;
    u8 sp40;
    u8 sp3C[12];

    if ((arg0->unkC.s[0] == arg4->s[0]) && (arg0->unkC.s[2] == arg4->s[2])) {
        sp78 = arg0->unk1C;
        func_8000A9AC(arg0, arg4, arg3, arg2);
        func_8000A80C(arg0->unk4, &arg0->unk1E, sp78, 1U);
    }
    var_v1 = arg4->s[0] - gVoxmapLastSearchedSlot.unk8;
    var_a0 = arg4->s[2] - gVoxmapLastSearchedSlot.unkC;
    if ((var_v1 >> 6) || (var_a0 >> 6)) {
        voxmap_find_closest_slot(arg4);
    }
    var_v1 = arg4->s[0] - gVoxmapLastSearchedSlot.unk8;
    var_v1 &= 0x3F;
    var_a0 = arg4->s[2] - gVoxmapLastSearchedSlot.unkC;
    var_a0 &= 0x3F;
    sp54 = gVoxmapLastSearchedSlot.data;
    if (sp54 == NULL) {
        return;
    }
    sp84[0] = (var_v1 & 3) << 1;
    sp84[1] = sp84[0] + 2;
    sp7C[0] = var_a0 & 3;
    sp7C[1] = sp7C[0] + 1;
    sp6C = 0;
    for (; sp6C < 3; sp6C++) {
        temp_v0 = (arg4->s[1] + sp6C) - 1;
        if (temp_v0){}
        if (temp_v0 < sp54->unk4) {
            temp_v0 = 0;
        } else {
            if (temp_v0 >= sp54->unkC) {
                temp_v0 = (sp54->unkC - sp54->unk4);
                temp_v0--;
            } else {
                temp_v0 = temp_v0 - sp54->unk4;
            }
        }
        new_var2 = var_v1 >> 2;
        new_var = var_a0 >> 2;
        temp_a0 = sp54->unk24[(temp_v0 << 5) | ((new_var * 2) + (new_var2 >> 3))];
        if ((temp_a0 >> (new_var2 & 7)) & 1) {
            temp_v0_2 = func_80007D08(sp54->unk1C, sp54->unk14, sp54->unk24, new_var2, temp_v0, new_var);
            temp_a0 = temp_v0_2[sp7C[0]];
            sp3C[sp6C * 4 + 0] = (temp_a0 >> sp84[0]) & 3;
            sp3C[sp6C * 4 + 1] = (temp_a0 >> sp84[1]) & 3;
            temp_a0 = temp_v0_2[sp7C[1]];
            sp3C[sp6C * 4 + 2] = (temp_a0 >> sp84[0]) & 3;
            sp3C[sp6C * 4 + 3] = (temp_a0 >> sp84[1]) & 3;
        } else {
            sp3C[sp6C * 4 + 0] = 0;
            sp3C[sp6C * 4 + 1] = 0;
            sp3C[sp6C * 4 + 2] = 0;
            sp3C[sp6C * 4 + 3] = 0;
        }
    }
    if (arg0->unk26 != 0) {
        sp6C = -1;
        if ((sp3C[4 + 0] & 2) || (sp3C[4 + 1] & 2) || (sp3C[4 + 2] & 2) || (sp3C[4 + 3] & 2)) {
            sp98 = 1;
        }
    } else {
        sp6C = 1;
    }

    while (sp6C >= 0) {
        temp_a2_2 = sp6C + 1;
        sp98 = 0;
        sp70 = sp6C;
        if ((sp3C[sp6C * 4 + 0] & 2) || (sp3C[sp6C * 4 + 1] & 2) || (sp3C[sp6C * 4 + 2] & 2) || (sp3C[sp6C * 4 + 3] & 2)) {
            sp98 = 1;
            sp6C = 0;
        }
        if ((sp98 == 0) && (((sp3C[temp_a2_2 * 4 + 0] & 2)) || (sp3C[temp_a2_2 * 4 + 1] & 2) || (sp3C[temp_a2_2 * 4 + 2] & 2) || (sp3C[temp_a2_2 * 4 + 3] & 2))) {
            sp98 = 1;
            sp6C = 0;
        }
        if (sp98 == 0) {
            temp_a0 = sp3C[temp_a2_2 * 4 + 0];
            var_a1 = sp3C[sp6C * 4 + 0];

            temp_a0 += sp3C[temp_a2_2 * 4 + 1];
            var_a1 += sp3C[sp6C * 4 + 1];

            temp_a0 += sp3C[temp_a2_2 * 4 + 2];
            var_a1 += sp3C[sp6C * 4 + 2];

            temp_a0 += sp3C[temp_a2_2 * 4 + 3];
            var_a1 += sp3C[sp6C * 4 + 3];
            
            if (&sp3C[temp_a2_2 * 4] == &sp3C[2 * 4] && temp_a0 == 0) {
                sp98 = 1;
            } else {
                if (&sp3C[temp_a2_2 * 4] == &sp3C[1 * 4]) {
                    if (var_a1 >= temp_a0) {
                        sp70 -= 1;
                    } else {
                        var_a1 = temp_a0;
                    }
                } else if (temp_a0 < var_a1) {
                    sp70 -= 1;
                } else {
                    var_a1 = temp_a0;
                }
                if (var_a1 < 2) {
                    sp98 = 1;
                } else {
                    sp6C = 0;
                }
            }
        }

        sp6C--;
    }

    if (sp98 != 0) {
        return;
    }

    arg4->s[1] += sp70;
    temp_v0 = func_8000AB18(arg0, arg4->s[0], arg4->s[2], &sp9C);
    if ((temp_v0 >= 0) && (sp9C == 0)) {
        temp_v1_3 = &arg0->unk0[temp_v0];
        if (arg3 < temp_v1_3->unk8) {
            temp_v1_3->unkA = arg2;
            temp_v1_3->unk8 = arg3;
            func_8000A884(arg0->unk4, arg0->unk1E, temp_v0, temp_v1_3->unk6 + arg3);
        }
    } else if (temp_v0 < 0) {
        sp78 = arg0->unk1C;
        sp50 = func_8000A9AC(arg0, arg4, arg3, arg2);
        if (sp50 == NULL) {
            return;
        }

        if (arg0->unk24 < sp50->unk6) {
            if (func_8000A640(arg4) == 0) {
                arg0->unk1C -= 1;
            } else {
                func_8000A80C(arg0->unk4, &arg0->unk1E, sp78, sp50->unk6 + sp50->unk8);
            }
        } else {
            sp6C = sp50->unk6;
            if (sp6C < arg0->unk24) {
                arg0->unk24 = sp6C;
            }
            func_8000A80C(arg0->unk4, &arg0->unk1E, sp78, sp50->unk6 + sp50->unk8);
        }
    }
}
#endif

s32 func_8000A640(Vec3s16* arg0) {
    return 1;
}

void func_8000A650(Unk80008E40* arg0) {
    s32 i;

    arg0->unk1E = 0;
    arg0->unk1C = 0;
    for (i = 0; i < 200; i++) {
        arg0->unk4[i].unk0 = 0;
        arg0->unk0[i].unkC = 0;
    }
}

void func_8000A6D8(Unk80008E40Unk4* arg0, s32 arg1) {
    s32 var_a3;
    u16 temp_a2;
    u16 temp_v0;

    temp_v0 = arg0[arg1].unk0;
    temp_a2 = arg0[arg1].unk2;
    arg0[0].unk0 = 0xFFFF;
    var_a3 = arg1 >> 1;
    while (temp_v0 >= arg0[var_a3].unk0) {
        arg0[arg1].unk2 = arg0[var_a3].unk2;
        arg0[arg1].unk0 = arg0[var_a3].unk0;
        arg1 = var_a3;
        var_a3 >>= 1;
    }
    arg0[arg1].unk0 = temp_v0;
    arg0[arg1].unk2 = temp_a2;
}

void func_8000A76C(Unk80008E40Unk4* arg0, s32 arg1, s32 arg2) {
    Unk80008E40Unk4* var_v1;
    s32 var_t1;
    u16 temp_a3;
    u16 temp_v0;

    temp_v0 = arg0[arg2].unk0;
    temp_a3 = arg0[arg2].unk2;
    while ((arg1 >> 1) >= arg2) {
        var_t1 = arg2 + arg2;
        if (var_t1 < arg1) {
            if (arg0[var_t1].unk0 < arg0[var_t1 + 1].unk0) {
                var_t1 = var_t1 + 1;
            }
        }

        if (temp_v0 >= arg0[var_t1].unk0) {
            break;
        }

        arg0[arg2].unk0 = arg0[var_t1].unk0;
        arg0[arg2].unk2 = arg0[var_t1].unk2;
        arg2 = var_t1;
    }

    arg0[arg2].unk0 = temp_v0;
    arg0[arg2].unk2 = temp_a3;
}

void func_8000A80C(Unk80008E40Unk4 *arg0, s16* arg1, u16 arg2, u16 arg3) {
    arg1[0] = arg1[0] + 1;
    arg0[arg1[0]].unk2 = arg2;
    arg0[arg1[0]].unk0 = 0xFFFF - arg3;
    func_8000A6D8(arg0, arg1[0]);
}

void func_8000A884(Unk80008E40Unk4* arg0, s32 arg1, u16 arg2, u16 arg3) {
    s32 sp1C;
    s32 i;
    s32 temp_v1;

    for (i = 0; i <= arg1; i++) {
        if (arg2 == arg0[i].unk2) {
            sp1C = i;
            i = arg1 + 1;
        }
    }
    temp_v1 = arg0[sp1C].unk0;
    arg0[sp1C].unk0 = arg3;
    if (arg3 < temp_v1) {
        func_8000A76C(arg0, arg1, sp1C);
    } else if (temp_v1 < arg3) {
        func_8000A6D8(arg0, sp1C);
    }
}

s32 func_8000A934(Unk80008E40Unk4 *arg0, s16* arg1) {
    u16 sp1E;
    s16 temp_v0;

    if (arg1[0] == 0) {
        return -1;
    }

    sp1E = arg0[1].unk2;
    arg0[1].unk0 = arg0[arg1[0]].unk0;
    arg0[1].unk2 = arg0[arg1[0]].unk2;
    arg1[0]--;
    func_8000A76C(arg0, arg1[0], 1);

    return sp1E;
}

Unk80008E40Unk0* func_8000A9AC(Unk80008E40* arg0, Vec3s16* arg1, u16 arg2, u16 arg3) {
    Unk80008E40Unk0* sp24;
    s32 temp_a0;
    s32 temp_v0;

    if (arg0->unk1C == 0xC8) {
        return NULL;
    }
    sp24 = &arg0->unk0[arg0->unk1C++];
    sp24->unk0.s[0] = arg1->s[0];
    sp24->unk0.s[1] = arg1->s[1];
    sp24->unk0.s[2] = arg1->s[2];
    sp24->unk8 = arg2;
    sp24->unkA = arg3;
    temp_v0 = sp24->unk0.s[0] - arg0->unkC.s[0];
    temp_a0 = sp24->unk0.s[2] - arg0->unkC.s[2];
    sp24->unk6 = sqrtf((SQ(temp_v0) + SQ(temp_a0))) * 2;
    return sp24;
}

s32 func_8000AB18(Unk80008E40* arg0, s32 arg1, s32 arg2, s32* arg3) {
    s32 i;
    Unk80008E40Unk0 *temp;

    for (i = 0; i < arg0->unk1C; i++) {
        temp = &arg0->unk0[i];
        if (arg1 == temp->unk0.s[0] && arg2 == temp->unk0.s[2]) {
            arg3[0] = temp->unkC;
            return i;
        }
    }
    return -1;
}

s32 func_8000AB84(Object* arg0, Object* arg1, f32* arg2, s16 arg3, u8 arg4) {
    Vec3s16 sp48;
    Vec3s16 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    s32 var_v1;
    s32 pad;
    s32 sp28;

    sp3C = arg1->srt.transl.x - arg0->srt.transl.x;
    sp38 = arg1->srt.transl.y - arg0->srt.transl.y;
    sp34 = arg1->srt.transl.z - arg0->srt.transl.z;
    sp28 = 0;
    var_v1 = arg0->srt.yaw - (arctan2_f(-sp3C, -sp34) & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    if ((-arg3 < var_v1) && (var_v1 < arg3)) {
        sp28 = 1;
        if (arg4 != 0) {
            func_80007EE0(&arg0->srt.transl, &sp48);
            func_80007EE0(&arg1->srt.transl, &sp40);
            sp48.s[1] += 2;
            sp40.s[1] += 2;
            sp28 = func_80008048(&sp48, &sp40, NULL, NULL, 0U);
        }
        if (sp28 != 0) {
            *arg2 = SQ(sp3C) + SQ(sp38) + SQ(sp34);
        }
    }
    return sp28;
}

s32 func_8000ACD4(Object* arg0, Object* arg1, f32* arg2, s16 arg3, u8 arg4) {
    Vec3s16 sp50;
    Vec3s16 sp48;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp;
    s32 sp30;

    temp_fv0 = arg1->srt.transl.x - arg0->srt.transl.x;
    temp_fv1 = arg1->srt.transl.y - arg0->srt.transl.y;
    temp_fa0 = arg1->srt.transl.z - arg0->srt.transl.z;
    temp = SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa0);
    temp_fa1 = SQ(*arg2);
    sp30 = 0;
    if (temp < temp_fa1) {
        if (func_800456AC(arg1) != 0) {
            sp30 = 1;
            if (arg4 != 0) {
                func_80007EE0(&arg0->srt.transl, &sp50);
                func_80007EE0(&arg1->srt.transl, &sp48);
                sp50.s[1] += 2;
                sp48.s[1] += 2;
                sp30 = func_80008048(&sp50, &sp48, NULL, NULL, 0U);
            }
            if (sp30 != 0) {
                *arg2 = temp;
            }
        }
    }
    return sp30;
}
