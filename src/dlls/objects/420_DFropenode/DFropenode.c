#include "common.h"
#include "dlls/objects/420_DFropenode.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
} DLL420_Setup;

typedef struct DLL420_Grandchild_TypeA {
    Vec3f unk0; //Position adjustment for the rope node
    Vec3f unkC; //Velocity for the rope node?
    Vec3f unk18;
    u8 unk24; //count of valid pointers in unk28 (number of connections?)
    u8 unk25;
    u8 unk26;
    u8 unk27;
    struct DLL420_Grandchild_TypeB* unk28[2]; //typeB for this node
    u8 unk30;
} DLL420_Grandchild_TypeA; //0x34 (confirmed from dll_420_func_152C)

typedef struct DLL420_Grandchild_TypeB { 
    f32 unk0; //distance?
    DLL420_Grandchild_TypeA* unk4; //typeA for this node
    DLL420_Grandchild_TypeA* unk8; //typeA for next node?
    f32 unkC;
    f32 unk10; //unsure
    f32 unk14; //unsure
    Vec3f unk18; 
} DLL420_Grandchild_TypeB; //0x24 (confirmed from dll_420_func_152C)

typedef struct {
    DLL420_Grandchild_TypeA* unk0; //pointer to unk44 in this struct (start of typeA data)
    DLL420_Grandchild_TypeB* unk4; //pointer to unk1E4 in this struct (start of typeB data)
    u8 unk8; //subdivision count?
    s8 unk9; //subdivision count plus 1?
    s8 unkA;
    u8 unkB;
    Vec3f unkC; 
    f32 unk18; 
    f32 unk1C; 
    f32 unk20; 
    f32 unk24; //distance
    s32 unk28; 
    f32 unk2C; 
    f32 unk30; 
    s8 unk34;
    s8 unk35;
    s8 unk36;
    s8 unk37;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    DLL420_Grandchild_TypeA unk44[8]; //count could vary, but in practice seems to always be 8
    DLL420_Grandchild_TypeB unk1E4[8]; //maybe 7? could be data for spans, and typeA is for points?
} DLL420_Child;  //Length: (count * 0x58) + 0x20     [(8 * (0x34 + 0x24)) + (0x44 - 0x24) = 0x2E0]

typedef struct DLL420_Data {
    Object* unk0; //Other paired DFropenode object
    Vec3f unk4;
    s32 unk10;
    f32 unk14; 
    s16 unk18; 
    Vec4f unk1C; //Rope spline (Vec4f)
    DLL420_Child* unk2C; //allocated by dll_420_func_152C, size `(count * 0x58) + 0x20` (count always seems to be 8)
    u32 unk30 : 1;
} DLL420_Data; //0x34

/*0x0*/ static Texture* data_0 = NULL; //Rope texture
/*0x4*/ static u8 data_4 = 0;
/*0x4*/ static u32 data_8[] = {
    0x00000000, 0x00000000
};

static void dll_420_func_18BC(DLL420_Child* rope);
static void dll_420_func_1A8C(DLL420_Child* rope);
static void dll_420_func_1C48(DLL420_Child* rope);
static void dll_420_func_1EF0(DLL420_Child* rope);

// offset: 0x0 | ctor
void dll_420_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_420_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_setup.s")
#else

static int dll_420_func_AD8(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

void dll_420_setup(Object* self, DLL420_Setup* arg1, s32 reset) {
    if (data_4 == 0) {
        data_0 = texLoadTexture(TEXTABLE_3CA);
    }
    data_4++;
    
    objAddObjectType(self, OBJTYPE_RopeNode);
    self->animCallback = dll_420_func_AD8;
}
#endif

// offset: 0xAC | func: 1 | export: 1
void dll_420_control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_control.s")

// offset: 0x578 | func: 2 | export: 2
void dll_420_update(Object* self) { }

// offset: 0x584 | func: 3 | export: 3
void dll_420_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_print.s")

// offset: 0x984 | func: 4 | export: 4
void dll_420_free(Object* self, s32 onlySelf) {
    DLL420_Data* objData;
    DLL420_Setup* objSetup;
    s32 i;
    s32 count; //38
    Object** objects;
    Object* otherObj;

    objSetup = (DLL420_Setup*)self->setup;
    objData = self->data;

    data_4--;
    if (data_4 == 0) {
        texFreeTexture(data_0);
    }
    
    objFreeObjectType(self, OBJTYPE_RopeNode);
    if (objSetup->unk18 & 1) {
        dll_420_func_18BC(objData->unk2C);
    }
    
    otherObj = objData->unk0;
    if (otherObj == NULL) {
        return;
    }
    
    objects = objGetAllOfType(OBJTYPE_RopeNode, &count);
    for (i = 0; i < count; i++) {
        if (otherObj == objects[i]) {
            ((DLL_420_DFRopeNode*)otherObj->dll)->vtbl->func16(otherObj);
        }
    }
}

// offset: 0xAB4 | func: 5 | export: 5
u32 dll_420_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0xAC4 | func: 6 | export: 6
u32 dll_420_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL420_Data);
}

// offset: 0xAD8 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_AD8.s")

// offset: 0xDFC | func: 8 | export: 7
void dll_420_func_DFC(Object* self, Vec4f* spline) {
    DLL420_Data* objData = self->data;
    
    spline->x = objData->unk1C.x;
    spline->y = objData->unk1C.y;
    spline->z = objData->unk1C.z;
    spline->w = objData->unk1C.w;
}

// offset: 0xE28 | func: 9 | export: 8
void dll_420_func_E28(Object* self, f32 arg1, f32* ox, f32* oy, f32* oz) {
    DLL420_Data* objData;
    s8 idx;
    Vec3f d;

    objData = self->data;
    
    idx = (s8) arg1;
    arg1 -= idx;
    
    d.x = objData->unk2C->unk0[idx + 1].unk0.f[0] - objData->unk2C->unk0[idx].unk0.f[0];
    d.y = objData->unk2C->unk0[idx + 1].unk0.f[1] - objData->unk2C->unk0[idx].unk0.f[1];
    d.z = objData->unk2C->unk0[idx + 1].unk0.f[2] - objData->unk2C->unk0[idx].unk0.f[2];
    
    *ox = objData->unk2C->unk0[idx].unk0.x + self->srt.transl.x + d.x * arg1;
    *oy = objData->unk2C->unk0[idx].unk0.y + self->srt.transl.y + d.y * arg1;
    *oz = objData->unk2C->unk0[idx].unk0.z + self->srt.transl.z + d.z * arg1;
}

// offset: 0xF04 | func: 10 | export: 9
void dll_420_func_F04(Object* self, f32* arg1, f32 arg2) {
    DLL420_Data* objData = self->data;
    DLL420_Grandchild_TypeA* node;
    s8 idx;
    
    idx = (s8)*arg1;
    
    *arg1 -= idx;
    
    node = &objData->unk2C->unk0[idx];

    arg2 /= sqrtf(SQ(node[0].unk0.f[2] - node[1].unk0.f[2]) + SQ(node[0].unk0.f[0] - node[1].unk0.f[0]));
    *arg1 += arg2;
    *arg1 +=  idx;
}

// offset: 0xFDC | func: 11 | export: 10
void dll_420_func_FDC(Object* self, f32 arg1, f32 arg2) {
    s8 idx;
    DLL420_Data* objData;
    DLL420_Child* rope;
    DLL420_Grandchild_TypeA* node;

    objData = self->data;

    
    arg1 -= (s8)arg1;
    idx = arg1;
    arg1 -= idx;
    
    objData->unk2C->unk0[idx].unk18.f[1] += arg2 * arg1;
    objData->unk2C->unk0[idx].unk18.f[1] += arg2 * (1.0f - arg1);
}

// offset: 0x1098 | func: 12 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_1098.s")

// offset: 0x135C | func: 13
f32 dll_420_func_135C(f32* arg0, f32* arg1, f32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 var_fv1;
    
    dx = arg6 - arg3;
    dy = arg7 - arg4;
    dz = arg8 - arg5;
    
    if ((arg6 == arg3) && (dz == 0.0f)) {
        var_fv1 = 0.0f;
    } else {
        var_fv1 = ((((*arg0) - arg3) * dx) + (((*arg2) - arg5) * dz)) / (SQ(dx) + SQ(dz));
    }
    
    if (var_fv1 < 0.0f) {
        *arg0 = arg3;
        *arg1 = arg4;
        *arg2 = arg5;
    } else if (var_fv1 >= 1.0f) {
        *arg0 = arg6;
        *arg1 = arg7;
        *arg2 = arg8;
    } else {
        *arg0 = (var_fv1 * dx) + arg3;
        *arg1 = (var_fv1 * dy) + arg4;
        *arg2 = (var_fv1 * dz) + arg5;
    }
    
    return var_fv1;
}

// offset: 0x148C | func: 14 | export: 16
void dll_420_func_148C(Object* self) {
    DLL420_Data* objData = self->data;
    objData->unk0 = NULL;
}

// offset: 0x149C | func: 15 | export: 12
s16 dll_420_func_149C(Object* self) {
    DLL420_Data* objData = self->data;
    return objData->unk18;
}

// offset: 0x14AC | func: 16 | export: 13
void dll_420_func_14AC(Object* self, u32 arg1) {
    DLL420_Data* objData = self->data;

    arg1 = !arg1;
    objData->unk30 = arg1;

    if (objData->unk0 != NULL) {
        objData = objData->unk0->data;
        objData->unk30 = arg1;
    }
}

// offset: 0x14F0 | func: 17 | export: 14
s16 dll_420_func_14F0(Object* self) {
    DLL420_Data* objData = self->data;
    s32 value = (objData->unk30 == 0);
    return value;
}

// offset: 0x1514 | func: 18 | export: 15
void dll_420_func_1514(Object* self, f32 arg1) {
    DLL420_Data* objData = self->data;
    objData->unk14 = arg1;
}

// offset: 0x152C | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_152C.s")

// offset: 0x18BC | func: 20
void dll_420_func_18BC(DLL420_Child* rope) {
    if (rope != NULL) {
        mmFree(rope);
    }
}

// offset: 0x18FC | func: 21
void dll_420_func_18FC(DLL420_Grandchild_TypeB* typeB, DLL420_Grandchild_TypeA* typeA1, DLL420_Grandchild_TypeA* typeA2) {
    s32 idx1 = 0;
    s32 idx2 = 0;
    
    while (typeA1->unk28[idx1]) {
        idx1++;
    }
    
    while (typeA2->unk28[idx2]) {
        idx2++;
    }
    
    if ((idx1 <= typeA1->unk24) && (idx2 <= typeA2->unk24)) {
        typeA1->unk28[idx1] = typeB;
        typeA2->unk28[idx2] = typeB;
        typeB->unk4 = typeA1;
        typeB->unk8 = typeA2;
    }
}

// offset: 0x1994 | func: 22
void dll_420_func_1994(DLL420_Child* rope) {
    DLL420_Grandchild_TypeA* node;
    s32 i;
    s32 j;

    node = rope->unk0;
    dll_420_func_1EF0(rope);
    
    for (i = 0; i < rope->unk28; i++) {
        dll_420_func_1A8C(rope);
        dll_420_func_1C48(rope);
    }
    
    for (i = 0; i < rope->unk8; i++) {
        j = 0;
        while (j < 3) {
            node->unk18.f[j++] = 0.0f;
        }
        node++;
    }
}

// offset: 0x1A8C | func: 23
void dll_420_func_1A8C(DLL420_Child* rope) {
    DLL420_Grandchild_TypeB* node;
    Vec3f delta;
    Vec3f deltaV;
    f32 factor;
    s32 i;

    node = rope->unk4;
    
    for (i = 0; i < (rope->unk8 - 1); i++, node++) {
        delta.f[0] = node->unk4->unk0.f[0] - node->unk8->unk0.f[0];
        delta.f[1] = node->unk4->unk0.f[1] - node->unk8->unk0.f[1];
        delta.f[2] = node->unk4->unk0.f[2] - node->unk8->unk0.f[2];
        
        deltaV.f[0] = node->unk4->unkC.f[0] - node->unk8->unkC.f[0];
        deltaV.f[1] = node->unk4->unkC.f[1] - node->unk8->unkC.f[1];
        deltaV.f[2] = node->unk4->unkC.f[2] - node->unk8->unkC.f[2];
        
        node->unk0 = sqrtf(SQ(delta.f[0]) + SQ(delta.f[1]) + SQ(delta.f[2]));
        if (node->unk0 > node->unk14) {
            node->unkC = 0.0f;
        }
        
        if (node->unkC == 0.0f) {
            node->unk18.f[0] = node->unk18.f[1] = node->unk18.f[2] = 0.0f;
        } else {
            factor = -node->unk10 * (node->unk0 - node->unkC);
            node->unk18.f[0] = delta.f[0] * factor;
            node->unk18.f[1] = delta.f[1] * factor;
            node->unk18.f[2] = delta.f[2] * factor;
        }
    }
}

// offset: 0x1C48 | func: 24
void dll_420_func_1C48(DLL420_Child* rope) {
    DLL420_Grandchild_TypeA* node;
    f32 sp68[3];
    DLL420_Grandchild_TypeB* temp_a0;
    f32 distance;
    f32 scaleFactor;
    s32 i;
    s32 j;
    s32 k;

    node = rope->unk0;
    for (i = 0; i < rope->unk8; i++, node++) {
        for (j = 0; j < 3; j++) {
            sp68[j] = 0.0f;
        }

        if (node->unk30 != 0) {
            continue;
        }

        for (k = 0; k < node->unk24; k++) {
            for (j = 0; j < 3; j++) {
                temp_a0 = node->unk28[k];
                if (node == temp_a0->unk4) {
                    sp68[j] += temp_a0->unk18.f[j];
                } else {
                    sp68[j] -= temp_a0->unk18.f[j];
                }
            }
        }

        distance = sqrtf(SQ(sp68[0]) + SQ(sp68[1]) + SQ(sp68[2]));
        if (rope->unk2C < distance) {
            scaleFactor = rope->unk2C / distance;
            sp68[0] *= scaleFactor;
            sp68[1] *= scaleFactor;
            sp68[2] *= scaleFactor;
        }

        for (j = 0; j < 3; j++) {
            node->unkC.f[j] += (sp68[j] * rope->unk40) + node->unk18.f[j];
            node->unkC.f[j] -= (node->unkC.f[j] * rope->unk38);
        }

        node->unkC.f[1] += rope->unk30 * rope->unk3C;
        for (j = 0; j < 3; j++) {
            node->unk0.f[j] += (node->unkC.f[j] * rope->unk30);
        }
    }
}

// offset: 0x1EF0 | func: 25
void dll_420_func_1EF0(DLL420_Child* rope) {
    DLL420_Grandchild_TypeA* nodes;
    s32 i;
    
    nodes = rope->unk0;

    if (rope->unk34 < -50) {
        rope->unk35 = 1;
    }

    if (rope->unk34 > 50) {
        rope->unk35 = 2;
    }

    if (rope->unk35 == 2) {
        rope->unk34--;
    } else {
        rope->unk34++;
    }

    for (i = 1; i < (rope->unk8 - 1); i++) {
        nodes[i].unk18.f[0] += 0.01f * rope->unk34;
    }
}

/*0x0*/ static const u32 rodata_0[] = {
    0x00000103, 0x00000000, 0x00000000, 0x00000000, 0x00010403, 0x00000000, 0x00000000, 0x00000000, 
    0x00000302, 0x00000000, 0x00000000, 0x00000000, 0x00020305, 0x00000000, 0x00000000, 0x00000000, 
    0x00010204, 0x00000000, 0x00000000, 0x00000000, 0x00020504, 0x00000000, 0x00000000, 0x00000000
};
/*0x60*/ static const u32 rodata_60[] = {
    0x00000001, 0x00000000, 0x01000000, 0xffffffff, 0xfffeffff, 0x00000000, 0x00000000, 0xffffffff, 
    0x0002ffff, 0x00000000, 0x02000000, 0xffffffff, 0x00000001, 0x00000000, 0x01001fe0, 0xffffffff, 
    0xfffeffff, 0x00000000, 0x00001fe0, 0xffffffff, 0x0002ffff, 0x00000000, 0x02001fe0, 0xffffffff, 
    0x20574152, 0x4e494e47, 0x3a20526f, 0x70652043, 0x6f756c64, 0x204e6f74, 0x2046696e, 0x64205061, 
    0x72746e65, 0x72000000, 0x4572726f, 0x72206174, 0x74616368, 0x696e6720, 0x746f6f20, 0x6d616e79, 
    0x20737072, 0x696e6773, 0x20746f20, 0x61207665, 0x72746578, 0x0a000000
};
