#include "common.h"
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
        gUnknownVideoStructs[i].unk88 = 0;
    }

    for (i = 0; i < 4; i++)
    {
        D_800BCC18[i].unk10 = NULL;
        D_800BCC18[i].unk0 = 0;
        D_800BCC18[i].unk4 = 0;
        D_800BCC18[i].unk8 = 0;
        D_800BCC18[i].unkC = 0;
    }

    D_800BCC10->unk0 = -3.0f;
    D_800BCC10->unk4 = 0.0f;
    D_800BCC10->unk8 = -3.0f;
    D_800BCC10->unkC = -3.0f;
    D_800BCC10->unk10 = 0.0f;
    D_800BCC10->unk14 = 3.0f;
    
    D_800BCC10->unk18 = 3.0f;
    D_800BCC10->unk1C = 0.0f;
    D_800BCC10->unk20 = 3.0f;
    D_800BCC10->unk24 = 3.0f;
    D_800BCC10->unk28 = 0.0f;
    D_800BCC10->unk2C = -3.0f;
    
    D_800BCC10->unk30 = -3.0f;
    D_800BCC10->unk34 = 0.0f;
    D_800BCC10->unk38 = -3.0f;
    D_800BCC10->unk3C = -3.0f;
    D_800BCC10->unk40 = 0.0f;
    D_800BCC10->unk44 = 3.0f;
    
    D_800BCC10->unk48 = 3.0f;
    D_800BCC10->unk4C = 0.0f;
    D_800BCC10->unk50 = 3.0f;
    D_800BCC10->unk54 = 3.0f;
    D_800BCC10->unk58 = 0.0f;
    D_800BCC10->unk5C = -3.0f;

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
        gUnknownVideoStructs[i].unk88 = 0;
    }

    for (i = 0; i < 4; i++) {
        D_800BCC18[i].unk10 = 0;
        D_800BCC18[i].unk0 = 0.0f;
        D_800BCC18[i].unk4 = 0.0f;
        D_800BCC18[i].unk8 = 0.0f;
        D_800BCC18[i].unkC = 0;
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
        D_800BCC18[D_80092FF8].unk10 = a0;
        D_800BCC18[D_80092FF8].unk0 = a1[0];
        D_800BCC18[D_80092FF8].unk4 = a0[4] + a1[1];
        D_800BCC18[D_80092FF8].unk8 = a1[2];
        D_800BCC18[D_80092FF8].unkC = a2;

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
        if ((D_800BCC18[i].unk10 != 0) && (arg1 == D_800BCC18[i].unk10)) {
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
        ptr = &gUnknownVideoStructs[i].unk18[0];
        
        if (gUnknownVideoStructs[i].viMode != NULL) {
            gUnknownVideoStructs[i].unk88 -= 5;

            if (gUnknownVideoStructs[i].unk88 < 0) {
                gUnknownVideoStructs[i].unk88 = 0;
            }

            for (k = 0; k < 4; k++) {
                (ptr++)->cn[3] = gUnknownVideoStructs[i].unk88;
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
            ptr = &gUnknownVideoStructs[i].unk18[0];
            
            var1 = gWorldX - gUnknownVideoStructs[i].unk78;
            var2 = gWorldZ - gUnknownVideoStructs[i].unk7C;

            gUnknownVideoStructs[i].unk78 += var1;
            gUnknownVideoStructs[i].unk7C += var2;

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
    arg1->unk10 = NULL;
    sp66[1] = arg0->srt.yaw;

    if (D_80093000 == 0x27) {
        D_80093000 = 0;
    }
    if (func_80051D68(arg0, arg1->unk0 - ((Vec3s32 *)D_80092BE0)->x, arg1->unk8 - ((Vec3s32 *)D_80092BE0)->z, (UnkFunc80051D68Arg3* ) D_80092BDC, D_80092C1C, &sp68) == 0) {
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
    var_s0 = gUnknownVideoStructs[D_80093000].unk18;
    var_s5 = (DLTri *)gUnknownVideoStructs[D_80093000].unk58_pad;
    temp_v0 = &gUnknownVideoStructs[D_80093000];
    temp_v0->unk88 = 0xC8;
    temp_v0->viMode = 1;
    sp94.pitch = var_s3;
    sp94.roll = -var_s2;
    sp94.yaw = 0;
    sp7C.roll = 0;
    sp7C.pitch = 0;
    temp_v0->unk78 = gWorldX;
    temp_v0->unk7C = gWorldZ;
    sp7C.yaw = sp66[1];
    for (var_s1 = 0; var_s1 < 4; var_s1++, var_s0++) {
        spAC.x = D_800BCC10[var_s1].x;
        spAC.y = D_800BCC10[var_s1].y;
        spAC.z = D_800BCC10[var_s1].z;
        rotate_vec3(&sp7C, &spAC);
        rotate_vec3(&sp94, &spAC);
        var_s0->ob[0] = spAC.x + arg1->unk0 - gWorldX;
        var_s0->ob[1] = spAC.y + arg1->unk4 + 1;
        var_s0->ob[2] = spAC.z + arg1->unk8 - gWorldZ;
        var_s0->cn[0] = 0xFF;
        var_s0->cn[1] = 0xE1;
        var_s0->cn[2] = 0xE1;
        var_s0->cn[3] = temp_v0->unk88;
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
    if (arg1->unkC == 1) {
        var_s5->v0 = 0;
        var_s5->v1 = 1;
        var_s5->v2 = 2;
        var_s5 += 1;
        var_s5->v0 = 0;
        var_s5->v1 = 2;
        var_s5->v2 = 3;
    } else if (arg1->unkC == 2) {
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
