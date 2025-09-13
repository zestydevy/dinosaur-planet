#include "common.h"
#include "sys/controller.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/newshadows.h"
#include "PR/gbi.h"

// TODO: This is part of a larger data structure and is only here to allow the current .data section
// for this file to be aligned correctly!
u8 D_80092ff0[] = { 0x07, 0xb1, 0x04, 0x03, 0x07, 0xb1, 0x00, 0x00 };

UnkHeapVidStruct *D_800BCC10;
UnkVidStruct2 D_800BCC18[4]; // size:0x50

void func_8005C780() {
    s32 i;

    if (D_800BCC10 != NULL) {
        mmFree(D_800BCC10);
    }

    D_800BCC10 = mmAlloc(UNKNOWN_HEAP_VIDEO_STRUCT_SIZE, ALLOC_TAG_GFX_COL, 0);

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++)
    {
        gSPEndDisplayList(&gUnknownVideoStructs[i].dl);
        gUnknownVideoStructs[i].viMode = NULL;
        gUnknownVideoStructs[i].unk0x88 = 0;
    }

    for (i = 0; i < 4; i++)
    {
        D_800BCC18[i].unk0x10 = NULL;
        D_800BCC18[i].unk0x0 = 0;
        D_800BCC18[i].unk0x4 = 0;
        D_800BCC18[i].unk0x8 = 0;
        D_800BCC18[i].unk0xc = 0;
    }

    D_800BCC10->unk0x0 = -3.0f;
    D_800BCC10->unk0x4 = 0.0f;
    D_800BCC10->unk0x8 = -3.0f;
    D_800BCC10->unk0xc = -3.0f;
    D_800BCC10->unk0x10 = 0.0f;
    D_800BCC10->unk0x14 = 3.0f;
    
    D_800BCC10->unk0x18 = 3.0f;
    D_800BCC10->unk0x1c = 0.0f;
    D_800BCC10->unk0x20 = 3.0f;
    D_800BCC10->unk0x24 = 3.0f;
    D_800BCC10->unk0x28 = 0.0f;
    D_800BCC10->unk0x2c = -3.0f;
    
    D_800BCC10->unk0x30 = -3.0f;
    D_800BCC10->unk0x34 = 0.0f;
    D_800BCC10->unk0x38 = -3.0f;
    D_800BCC10->unk0x3c = -3.0f;
    D_800BCC10->unk0x40 = 0.0f;
    D_800BCC10->unk0x44 = 3.0f;
    
    D_800BCC10->unk0x48 = 3.0f;
    D_800BCC10->unk0x4c = 0.0f;
    D_800BCC10->unk0x50 = 3.0f;
    D_800BCC10->unk0x54 = 3.0f;
    D_800BCC10->unk0x58 = 0.0f;
    D_800BCC10->unk0x5c = -3.0f;

    D_800BCC68 = queue_load_texture_proxy(0x18);
    D_800BCC6C = queue_load_texture_proxy(0x19);
    D_800BCC70 = queue_load_texture_proxy(0x1a);

    D_800BCC78 = (u8)0;
}

void func_8005C998() {
    s32 i;

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++) {
        gSPEndDisplayList(&gUnknownVideoStructs[i].dl);
        gUnknownVideoStructs[i].viMode = NULL;
        gUnknownVideoStructs[i].unk0x88 = 0;
    }

    for (i = 0; i < 4; i++) {
        D_800BCC18[i].unk0x10 = 0;
        D_800BCC18[i].unk0x0 = 0.0f;
        D_800BCC18[i].unk0x4 = 0.0f;
        D_800BCC18[i].unk0x8 = 0.0f;
        D_800BCC18[i].unk0xc = 0;
    }
}

void func_8005CA5C(u32 param1) {
    D_800BCC78 = param1;

    if (param1 == 0) {
        func_8005C998();
    }
}

void func_8005CA88(f32 *a0, f32 *a1, u8 a2) {
    static s32 D_80092FF8 = 0;

    if (D_800BCC78 != 0) {
        D_800BCC18[D_80092FF8].unk0x10 = a0;
        D_800BCC18[D_80092FF8].unk0x0 = a1[0];
        D_800BCC18[D_80092FF8].unk0x4 = a0[4] + a1[1];
        D_800BCC18[D_80092FF8].unk0x8 = a1[2];
        D_800BCC18[D_80092FF8].unk0xc = a2;

        D_80092FF8 = D_80092FF8 + 1;

        if (D_80092FF8 == 4) {
            D_80092FF8 = 0;
        }
    }
}

void func_8005CB10(Gfx **gdl, Object *obj) {
    s32 i;
    Texture *tex;

    switch (obj->id) {
        case OBJ_Sabre:
            tex = D_800BCC68;
            break;
        case OBJ_Krystal:
            tex = D_800BCC6C;
            break;
        default:
            tex = D_800BCC68;
            break;
    }

    set_textures_on_gdl(gdl, tex, NULL, 0x1a, 0, 0, 1);

    i = 0;

    while (i < UNKNOWN_VIDEO_STRUCTS_COUNT) {
        if (obj == gUnknownVideoStructs[i].obj) {
            gSPDisplayList((*gdl)++, &gUnknownVideoStructs[i].dl);
        }

        i++;
    }
}

static s32 D_80092FFC = 1;
static s32 D_80093000 = 0;
void func_8005CF4C(Object *, UnkVidStruct2*);
void func_8005CDFC(s32 _);
void func_8005CC74(Gfx **gdl, Object *arg1) {
    s32 i;

    if (D_800BCC78 == 0) {
        return;
    }

    if (D_80092FFC != 0) {
        D_800BCC80[0] = 0.0f;
        D_800BCC80[1] = 0.0f;
    }

    if ((gWorldX != D_800BCC80[0]) || (gWorldZ != D_800BCC80[1])) {
        D_800BCC80[0] = gWorldX;
        D_800BCC80[1] = gWorldZ;
        func_8005CDFC(0);
    }

    for (i = 0; i < 4; i++) {
        if ((D_800BCC18[i].unk0x10 != 0) && (arg1 == D_800BCC18[i].unk0x10)) {
            func_8005CF4C(arg1, &D_800BCC18[i]);
        }
    }
    func_8005CB10(gdl, arg1);
    D_80092FFC = 0;
}

void func_8005CD80() {
    s32 i;
    s32 k;
    Vtx_t *ptr;

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++) {
        ptr = &gUnknownVideoStructs[i].unk0x18[0];
        
        if (gUnknownVideoStructs[i].viMode != NULL) {
            gUnknownVideoStructs[i].unk0x88 -= 5;

            if (gUnknownVideoStructs[i].unk0x88 < 0) {
                gUnknownVideoStructs[i].unk0x88 = 0;
            }

            for (k = 0; k < 4; k++) {
                (ptr++)->cn[3] = gUnknownVideoStructs[i].unk0x88;
            }
        }
    }
}

void func_8005CDFC(s32 _) {
    float var1;
    float var2;
    s32 i;
    s32 k;
    Vtx_t *ptr;

    var1 = 0;
    var2 = 0;

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++) {
        if (gUnknownVideoStructs[i].viMode != NULL) {
            ptr = &gUnknownVideoStructs[i].unk0x18[0];
            
            var1 = gWorldX - gUnknownVideoStructs[i].unk0x78;
            var2 = gWorldZ - gUnknownVideoStructs[i].unk0x7c;

            gUnknownVideoStructs[i].unk0x78 += var1;
            gUnknownVideoStructs[i].unk0x7c += var2;

            for (k = 0; k < 4; k++) {
                ptr->ob[0] -= var1;
                ptr->ob[2] -= var2;
                ptr++;
            }
        }
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CF4C.s")
#else
extern s32 D_80092BDC;
s32 func_80051D68(Object* arg0, s16 arg1, s16 arg2, UnkFunc80051D68Arg3* arg3, s32 arg4, Vec4f* arg5); /* extern */
void func_8005CF4C(Object* arg0, UnkVidStruct2* arg1) {
    s16 var_s2;
    s16 var_s3;
    Vtx_t* var_s0;
    s32 var_s1;
    UnkVidStruct* temp_v0;
    Vec3f spAC;
    SRT sp94;
    SRT sp7C;
    DLTri* var_s5;
    s32 temp_a2;
    Vec4f sp68;
    s16 sp66[2];

    sp68.x = 0.0f;
    sp68.y = 0.0f;
    sp68.z = 0.0f;
    var_s2 = 0;
    var_s3 = 0;
    arg1->unk0x10 = NULL;
    sp66[1] = arg0->srt.yaw;

    if (D_80093000 == 0x27) {
        D_80093000 = 0;
    }
    if (func_80051D68(arg0, arg1->unk0x0 - ((Vec3s32 *)D_80092BE0)->x, arg1->unk0x8 - ((Vec3s32 *)D_80092BE0)->z, (UnkFunc80051D68Arg3* ) D_80092BDC, D_80092C1C, &sp68) == 0) {
        return;
    }

    if (sp68.y != 0.0f) {
        if (sp68.x != 0.0f) {
            var_s2 = arctan2_f(sp68.x, sp68.y);
        }
        if (sp68.z != 0.0f) {
            var_s3 = arctan2_f(sp68.z, sp68.y);
            if (((!var_s5) && (!var_s5)) && (!var_s5)) {}
        }
    }

    gUnknownVideoStructs[D_80093000].obj = arg0;
    sp94.transl.x = 0.0f;
    sp94.transl.y = 0.0f;
    sp94.transl.z = 0.0f;
    sp94.scale = 1.0f;
    sp7C.transl.x = 0.0f;
    sp7C.transl.y = 0.0f;
    sp7C.transl.z = 0.0f;
    sp7C.scale = 1.0f;
    var_s0 = gUnknownVideoStructs[D_80093000].unk0x18;
    var_s5 = (DLTri *)gUnknownVideoStructs[D_80093000].unk0x58_pad;
    temp_v0 = &gUnknownVideoStructs[D_80093000];
    temp_v0->unk0x88 = 0xC8;
    temp_v0->viMode = 1;
    sp94.pitch = var_s3;
    sp94.roll = -var_s2;
    sp94.yaw = 0;
    sp7C.roll = 0;
    sp7C.pitch = 0;
    temp_v0->unk0x78 = gWorldX;
    temp_v0->unk0x7c = gWorldZ;
    sp7C.yaw = sp66[1];
    for (var_s1 = 0; var_s1 < 4; var_s1++, var_s0++) {
        spAC.x = D_800BCC10[var_s1].x;
        spAC.y = D_800BCC10[var_s1].y;
        spAC.z = D_800BCC10[var_s1].z;
        rotate_vec3(&sp7C, &spAC);
        rotate_vec3(&sp94, &spAC);
        var_s0->ob[0] = spAC.x + arg1->unk0x0 - gWorldX;
        var_s0->ob[1] = spAC.y + arg1->unk0x4 + 1;
        var_s0->ob[2] = spAC.z + arg1->unk0x8 - gWorldZ;
        var_s0->cn[0] = 0xFF;
        var_s0->cn[1] = 0xE1;
        var_s0->cn[2] = 0xE1;
        var_s0->cn[3] = temp_v0->unk0x88;
    }
    var_s0 -= 4;
    var_s0[0].tc[0] = 0;
    var_s0[0].tc[1] = 0x400;
    var_s0[1].tc[0] = 0;
    var_s0[1].tc[1] = 0;
    var_s0[2].tc[0] = 0x400;
    var_s0[2].tc[1] = 0;
    var_s0[3].tc[0] = 0x400;
    var_s0[3].tc[1] = 0x400;
    if (arg1->unk0xc == 1) {
        var_s5->v0 = 0;
        var_s5->v1 = 1;
        var_s5->v2 = 2;
        var_s5 += 1;
        var_s5->v0 = 0;
        var_s5->v1 = 2;
        var_s5->v2 = 3;
    } else if (arg1->unk0xc == 2) {
        var_s5->v0 = 0;
        var_s5->v1 = 1;
        var_s5->v2 = 2;
        var_s5 += 1;
        var_s5->v0 = 0;
        var_s5->v1 = 2;
        var_s5->v2 = 3;
    }
    var_s5--;
    gSPVertex(&temp_v0->dl, OS_PHYSICAL_TO_K0(var_s0), 4, 0)
    dl_triangles(&temp_v0->dl2, var_s5, 2);
    gSPEndDisplayList(temp_v0->dl2++)
    temp_a2 = D_80093000 + 1;
    if (temp_a2 == 39) {
        D_80093000 = temp_a2;
        gUnknownVideoStructs->viMode = NULL;
    } else {
        gUnknownVideoStructs[temp_a2].viMode = 0;
        D_80093000 = temp_a2;
    }
    func_8005CD80();
}
#endif


// Note: Return type is void* because the real type is unclear
void *func_8005D3A4(int param) {
    switch (param) {
        case 1:
            return &D_80092EC4;
        case 2:
            return &D_80092F0C;
        case 3:
            return &D_80092F54;
        case 4:
            return &D_80092F9C;
        case 5:
            return &D_80092FE4;  
        case 0:
        default:
            return &D_80092F54;
    } 
}
