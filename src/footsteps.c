#include "PR/gbi.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object_id.h"
#include "sys/vi.h"
#include "sys/gfx/textable.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/footsteps.h"
#include "macros.h"

/* -------- .data start 80092ec0 -------- */
s32 D_80092EC0 = 0; // unused
u16 gFootstepSfxBank1[36] = {
    SOUND_40, SOUND_41, SOUND_42, SOUND_43, 
    SOUND_40, SOUND_41, SOUND_42, SOUND_43, 
    SOUND_40, SOUND_41, SOUND_42, SOUND_43, 
    SOUND_40, SOUND_41, SOUND_42, SOUND_43, 
    SOUND_40, SOUND_41, SOUND_42, SOUND_43, 
    SOUND_44, SOUND_45, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0
};
u16 gFootstepSfxBank2[36] = {
    SOUND_86, SOUND_87, 0, 0, 
    SOUND_88, SOUND_89, 0, 0, 
    SOUND_8A, SOUND_89, 0, 0, 
    SOUND_8A, SOUND_89, 0, 0, 
    SOUND_8A, SOUND_8B, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0
};
u16 gFootstepSfxBank3[36] = {
    SOUND_61_Footstep, SOUND_62_Footstep, SOUND_61_Footstep, SOUND_62_Footstep, 
    SOUND_61_Footstep, SOUND_62_Footstep, SOUND_61_Footstep, SOUND_62_Footstep, 
    SOUND_61_Footstep, SOUND_62_Footstep, SOUND_61_Footstep, SOUND_62_Footstep, 
    SOUND_65_Footstep, SOUND_66_Footstep, SOUND_368, SOUND_369, 
    SOUND_27C, SOUND_27D, SOUND_27C, SOUND_27D, 
    SOUND_6D_Water_Footstep, SOUND_6E_Water_Footstep, SOUND_6D_Water_Footstep, SOUND_6E_Water_Footstep, 
    SOUND_280, SOUND_281, SOUND_280, SOUND_281, 
    SOUND_406, SOUND_404, SOUND_406, SOUND_404, 
    0, SOUND_88D, 0, SOUND_88D
};
u16 gFootstepSfxBank4[36] = {
    SOUND_63_Footstep, SOUND_64_Footstep, SOUND_63_Footstep, SOUND_64_Footstep, 
    SOUND_63_Footstep, SOUND_64_Footstep, SOUND_63_Footstep, SOUND_64_Footstep, 
    SOUND_63_Footstep, SOUND_64_Footstep, SOUND_63_Footstep, SOUND_64_Footstep, 
    SOUND_67_Footstep, SOUND_68_Footstep, SOUND_36A, SOUND_36B, 
    SOUND_27E, SOUND_27F, SOUND_27E, SOUND_27F, 
    SOUND_6F_Water_Footstep, SOUND_70_Water_Footstep, SOUND_6F_Water_Footstep, SOUND_70_Water_Footstep, 
    SOUND_282, SOUND_283, SOUND_282, SOUND_283, 
    SOUND_407, SOUND_402, SOUND_407, SOUND_402, 
    0, SOUND_88D, 0, SOUND_88D
};
u16 gFootstepSfxBank5[10] = {
    SOUND_7B1, SOUND_7B1, SOUND_7B1, SOUND_7B1, 
    SOUND_7B0, SOUND_7B1, SOUND_7B1, SOUND_403, 
    SOUND_7B1, 0
};
// data continued below in function statics, etc
/* -------- .data end 80093010 -------- */

/* -------- .bss start 800bb590 -------- */
UnkFootstepsStruct gUnkFootstepStructs[40];
Vec3f *D_800BCC10;
UnkFootstepsStruct2 D_800BCC18[4]; // size:0x50
Texture *D_800BCC68;
Texture *D_800BCC6C;
Texture *D_800BCC70;
u32 _bss_800BCC74; // unused
u8 D_800BCC78;
f32 D_800BCC80[2];
/* -------- .bss end 800bcc90 -------- */

void footsteps_func_8005CF4C(Object *, UnkFootstepsStruct2*);
void footsteps_func_8005CDFC(s32 _);

void footsteps_init(void) {
    s32 i;

    if (D_800BCC10 != NULL) {
        mmFree(D_800BCC10);
    }

    D_800BCC10 = mmAlloc(sizeof(Vec3f) * 8, ALLOC_TAG_GFX_COL, ALLOC_NAME("foot:vol"));

    for (i = 0; i < ARRAYCOUNT_S(gUnkFootstepStructs); i++) {
        gSPEndDisplayList(&gUnkFootstepStructs[i].dl);
        gUnkFootstepStructs[i].unk84 = 0;
        gUnkFootstepStructs[i].unk88 = 0;
    }

    for (i = 0; i < 4; i++) {
        D_800BCC18[i].unk10 = NULL;
        D_800BCC18[i].unk0.x = 0;
        D_800BCC18[i].unk0.y = 0;
        D_800BCC18[i].unk0.z = 0;
        D_800BCC18[i].unkC = 0;
    }

    D_800BCC10[0].x = -3.0f;
    D_800BCC10[0].y = 0.0f;
    D_800BCC10[0].z = -3.0f;
    D_800BCC10[1].x = -3.0f;
    D_800BCC10[1].y = 0.0f;
    D_800BCC10[1].z = 3.0f;
    
    D_800BCC10[2].x = 3.0f;
    D_800BCC10[2].y = 0.0f;
    D_800BCC10[2].z = 3.0f;
    D_800BCC10[3].x = 3.0f;
    D_800BCC10[3].y = 0.0f;
    D_800BCC10[3].z = -3.0f;
    
    D_800BCC10[4].x = -3.0f;
    D_800BCC10[4].y = 0.0f;
    D_800BCC10[4].z = -3.0f;
    D_800BCC10[5].x = -3.0f;
    D_800BCC10[5].y = 0.0f;
    D_800BCC10[5].z = 3.0f;
    
    D_800BCC10[6].x = 3.0f;
    D_800BCC10[6].y = 0.0f;
    D_800BCC10[6].z = 3.0f;
    D_800BCC10[7].x = 3.0f;
    D_800BCC10[7].y = 0.0f;
    D_800BCC10[7].z = -3.0f;

    D_800BCC68 = tex_load_deferred(TEXTABLE_18_Footprint1);
    D_800BCC6C = tex_load_deferred(TEXTABLE_19_Footprint2);
    D_800BCC70 = tex_load_deferred(TEXTABLE_1A_Footprint3);

    D_800BCC78 = (u8)0;
}

void footsteps_clear(void) {
    s32 i;

    for (i = 0; i < ARRAYCOUNT_S(gUnkFootstepStructs); i++) {
        gSPEndDisplayList(&gUnkFootstepStructs[i].dl);
        gUnkFootstepStructs[i].unk84 = 0;
        gUnkFootstepStructs[i].unk88 = 0;
    }

    for (i = 0; i < 4; i++) {
        D_800BCC18[i].unk10 = NULL;
        D_800BCC18[i].unk0.x = 0.0f;
        D_800BCC18[i].unk0.y = 0.0f;
        D_800BCC18[i].unk0.z = 0.0f;
        D_800BCC18[i].unkC = 0;
    }
}

// officialName: footstepsTurnOn
void footsteps_toggle(u32 enabled) {
    D_800BCC78 = enabled;

    if (enabled == 0) {
        footsteps_clear();
    }
}

// unused
void footsteps_func_8005CA88(Object *obj, Vec3f *a1, u8 a2) {
    static s32 D_80092FF8 = 0;

    if (D_800BCC78 != 0) {
        D_800BCC18[D_80092FF8].unk10 = obj;
        D_800BCC18[D_80092FF8].unk0.x = a1->x;
        D_800BCC18[D_80092FF8].unk0.y = a1->y + obj->srt.transl.y;
        D_800BCC18[D_80092FF8].unk0.z = a1->z;
        D_800BCC18[D_80092FF8].unkC = a2;

        D_80092FF8 = D_80092FF8 + 1;

        if (D_80092FF8 == 4) {
            D_80092FF8 = 0;
        }
    }
}

void footsteps_func_8005CB10(Gfx **gdl, Object *obj) {
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

    tex_gdl_set_textures(gdl, tex, NULL, 0x1a, 0, 0, 1);

    i = 0;

    while (i < ARRAYCOUNT_S(gUnkFootstepStructs)) {
        if (obj == gUnkFootstepStructs[i].obj) {
            gSPDisplayList((*gdl)++, &gUnkFootstepStructs[i].dl);
        }

        i++;
    }
}

static s32 D_80092FFC = 1;

// unused
void footsteps_func_8005CC74(Gfx **gdl, Object *arg1) {
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
        footsteps_func_8005CDFC(0);
    }

    for (i = 0; i < 4; i++) {
        if ((D_800BCC18[i].unk10 != 0) && (arg1 == D_800BCC18[i].unk10)) {
            footsteps_func_8005CF4C(arg1, &D_800BCC18[i]);
        }
    }
    footsteps_func_8005CB10(gdl, arg1);
    D_80092FFC = 0;
}

void footsteps_func_8005CD80(void) {
    s32 i;
    s32 k;
    Vtx_t *ptr;

    for (i = 0; i < ARRAYCOUNT_S(gUnkFootstepStructs); i++) {
        ptr = &gUnkFootstepStructs[i].unk18[0];
        
        if (gUnkFootstepStructs[i].unk84 != 0) {
            gUnkFootstepStructs[i].unk88 -= 5;

            if (gUnkFootstepStructs[i].unk88 < 0) {
                gUnkFootstepStructs[i].unk88 = 0;
            }

            for (k = 0; k < 4; k++) {
                (ptr++)->cn[3] = gUnkFootstepStructs[i].unk88;
            }
        }
    }
}

void footsteps_func_8005CDFC(s32 _) {
    f32 var1;
    f32 var2;
    s32 i;
    s32 k;
    Vtx_t *ptr;

    var1 = 0;
    var2 = 0;

    for (i = 0; i < ARRAYCOUNT_S(gUnkFootstepStructs); i++) {
        if (gUnkFootstepStructs[i].unk84 != 0) {
            ptr = &gUnkFootstepStructs[i].unk18[0];
            
            var1 = gWorldX - gUnkFootstepStructs[i].unk78;
            var2 = gWorldZ - gUnkFootstepStructs[i].unk7C;

            gUnkFootstepStructs[i].unk78 += var1;
            gUnkFootstepStructs[i].unk7C += var2;

            for (k = 0; k < 4; k++) {
                ptr->ob[0] -= var1;
                ptr->ob[2] -= var2;
                ptr++;
            }
        }
    }
}

void footsteps_func_8005CF4C(Object* arg0, UnkFootstepsStruct2* arg1) {
    static s32 D_80093000 = 0;
    s16 var_s2;
    s16 var_s3;
    Vtx_t* var_s0;
    s32 var_s1;
    Gfx *dl;
    Vec3f spAC;
    SRT sp94;
    SRT sp7C;
    DLTri* var_s5;
    Vec4f sp68;
    s16 sp66;

    sp68.x = 0.0f;
    sp68.y = 0.0f;
    sp68.z = 0.0f;
    var_s2 = 0;
    var_s3 = 0;
    sp66 = arg0->srt.yaw;
    arg1->unk10 = NULL;

    if (D_80093000 == 39) {
        D_80093000 = 0;
    }
    if (shadows_func_80051D68(
            arg0, 
            arg1->unk0.x - D_80092BE0->x,
            arg1->unk0.z - D_80092BE0->z,
            D_80092BDC, 
            *((s32*)&D_80092C1C), 
            &sp68) == 0) {
        return;
    }

    if (sp68.y != 0.0f) {
        if (sp68.x != 0.0f) {
            var_s2 = arctan2_f(sp68.x, sp68.y);
        }
        if (sp68.z != 0.0f) {
            var_s3 = arctan2_f(sp68.z, sp68.y);
        }
    }

    gUnkFootstepStructs[D_80093000].obj = arg0;
    var_s0 = gUnkFootstepStructs[D_80093000].unk18;
    var_s5 = gUnkFootstepStructs[D_80093000].unk58;
    dl = gUnkFootstepStructs[D_80093000].dl;
    gUnkFootstepStructs[D_80093000].unk88 = 200;
    gUnkFootstepStructs[D_80093000].unk84 = 1;
    gUnkFootstepStructs[D_80093000].unk78 = gWorldX;
    gUnkFootstepStructs[D_80093000].unk7C = gWorldZ;

    sp94.transl.x = 0.0f;
    sp94.transl.y = 0.0f;
    sp94.transl.z = 0.0f;
    sp94.scale = 1.0f;
    sp94.pitch = var_s3;
    sp94.roll = -var_s2;
    sp94.yaw = 0;

    sp7C.transl.x = 0.0f;
    sp7C.transl.y = 0.0f;
    sp7C.transl.z = 0.0f;
    sp7C.scale = 1.0f;
    sp7C.roll = 0;
    sp7C.pitch = 0;
    sp7C.yaw = sp66;
    for (var_s1 = 0; var_s1 < 4; var_s1++, var_s0++) {
        spAC.x = D_800BCC10[var_s1].x;
        spAC.y = D_800BCC10[var_s1].y;
        spAC.z = D_800BCC10[var_s1].z;
        rotate_vec3(&sp7C, spAC.f);
        rotate_vec3(&sp94, spAC.f);
        var_s0->ob[0] = arg1->unk0.f[0] + spAC.f[0] - gWorldX;
        var_s0->ob[1] = arg1->unk0.f[1] + spAC.f[1] + 1;
        var_s0->ob[2] = arg1->unk0.f[2] + spAC.f[2] - gWorldZ;
        var_s0->cn[0] = 0xFF;
        var_s0->cn[1] = 0xE1;
        var_s0->cn[2] = 0xE1;
        var_s0->cn[3] = gUnkFootstepStructs[D_80093000].unk88;
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
    gSPVertex(dl++, OS_PHYSICAL_TO_K0(var_s0), 4, 0);
    dl_triangles(&dl, var_s5, 2);
    gSPEndDisplayList(dl++);
    D_80093000 = D_80093000 + 1;
    if (D_80093000 == 39) {
        gUnkFootstepStructs->unk84 = 0;
    } else {
        gUnkFootstepStructs[D_80093000].unk84 = 0;
    }
    footsteps_func_8005CD80();
}

u16 *footsteps_get_sfx_bank(s32 bank) {
    switch (bank) {
        case 1:
            return gFootstepSfxBank1;
        case 2:
            return gFootstepSfxBank2;
        case 3:
            return gFootstepSfxBank3;
        case 4:
            return gFootstepSfxBank4;
        case 5:
            return gFootstepSfxBank5;  
        case 0:
        default:
            return gFootstepSfxBank3;
    } 
}
