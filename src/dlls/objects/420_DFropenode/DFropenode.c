#include "common.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"

typedef struct {
    Object* unk0;
    struct DLL420_Func18FCUnk* unk4;
    struct DLL420_Func18FCUnk* unk8;
    u8 _unkC[0x14 - 0xC];
    f32 unk14; 
    s16 unk18; 
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    s32 unk28[3];
} DLL420_Func18FCUnk;

typedef struct {
    Vec3f unk0;
    u8 _unkC[0x14 - 0xC];
    f32 unk14; 
    s16 unk18; 
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    s32 unk28[3];
} DLL420_FuncF04Unk;

typedef struct {
    ObjSetup base;
    u8 unk18;
} DLL420_Setup;

typedef struct DLL420_Data {
    Object* unk0;
    u8 _unk4[0x14 - 0x4];
    f32 unk14; 
    s16 unk18; 
    f32 unk1C[4];
    DLL420_FuncF04Unk** unk2C;
    u32 unk30 : 1;
} DLL420_Data;

/*0x0*/ static Texture* data_0 = NULL;
/*0x4*/ static u8 data_4 = 0;
/*0x4*/ static u32 data_8[] = {
    0x00000000, 0x00000000
};

static void dll_420_func_18BC(DLL420_FuncF04Unk** nodeData);

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
        data_0 = tex_load_deferred(TEXTABLE_3CA);
    }
    data_4++;
    
    obj_add_object_type(self, OBJTYPE_RopeNode);
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
    Object* otherNode;

    objSetup = (DLL420_Setup*)self->setup;
    objData = self->data;

    data_4--;
    if (data_4 == 0) {
        tex_free(data_0);
    }
    
    obj_free_object_type(self, OBJTYPE_RopeNode);
    if (objSetup->unk18 & 1) {
        dll_420_func_18BC(objData->unk2C);
    }
    
    otherNode = objData->unk0;
    if (otherNode == NULL) {
        return;
    }
    
    objects = obj_get_all_of_type(0x19, &count);
    for (i = 0; i < count; i++) {
        if (otherNode == objects[i]) {
            ((DLL_Unknown*)otherNode->dll)->vtbl->func[16].withOneArg(otherNode);
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
void dll_420_func_DFC(Object* self, f32* arg1) {
    DLL420_Data* objData = self->data;
    
    arg1[0] = objData->unk1C[0];
    arg1[1] = objData->unk1C[1];
    arg1[2] = objData->unk1C[2];
    arg1[3] = objData->unk1C[3];
}

// offset: 0xE28 | func: 9 | export: 8
void dll_420_func_E28(Object* self, f32 arg1, f32* ox, f32* oy, f32* oz) {
    DLL420_Data* objData;
    s8 index;
    Vec3f d;

    objData = self->data;
    
    index = (s8) arg1;
    arg1 -= index;
    
    d.x = (objData->unk2C[0] + index + 1)->unk0.f[0] - (objData->unk2C[0] + index)->unk0.f[0];
    d.y = (objData->unk2C[0] + index + 1)->unk0.f[1] - (objData->unk2C[0] + index)->unk0.f[1];
    d.z = (objData->unk2C[0] + index + 1)->unk0.f[2] - (objData->unk2C[0] + index)->unk0.f[2];
    
    *ox = (objData->unk2C[0] + index)->unk0.x + self->srt.transl.x + d.x * arg1;
    *oy = (objData->unk2C[0] + index)->unk0.y + self->srt.transl.y + d.y * arg1;
    *oz = (objData->unk2C[0] + index)->unk0.z + self->srt.transl.z + d.z * arg1;
}

// offset: 0xF04 | func: 10 | export: 9
void dll_420_func_F04(Object* self, f32* arg1, f32 arg2) {
    DLL420_Data* objData = self->data;
    DLL420_FuncF04Unk* temp_v0;
    s8 index;
    
    index = (s8)*arg1;
    
    *arg1 -= index;
    
    temp_v0 = objData->unk2C[0] + index;

    arg2 /= sqrtf(SQ(temp_v0[0].unk0.z - temp_v0[1].unk0.z) + SQ(temp_v0[0].unk0.x - temp_v0[1].unk0.x));
    *arg1 += arg2;
    *arg1 +=  index;
}

// offset: 0xFDC | func: 11 | export: 10
void dll_420_func_FDC(Object* self, f32 arg1, f32 arg2) {
    DLL420_Data* objData;
    s8 idx;

    objData = self->data;
    
    arg1 -= (s8)arg1;
    idx = arg1;
    arg1 -= idx;
    
    objData->unk2C[0][idx].unk1C += arg2 * arg1;
    objData->unk2C[0][idx].unk1C += arg2 * (1.0f - arg1);
}

// offset: 0x1098 | func: 12 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_1098.s")

// offset: 0x135C | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_135C.s")

// offset: 0x148C | func: 14 | export: 16
void dll_420_func_148C(Object* self) {
    DLL420_Data* objData = self->data;
    objData->unk0 = 0;
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
void dll_420_func_18BC(DLL420_FuncF04Unk** nodeData) {
    if (nodeData != NULL) {
        mmFree(nodeData);
    }
}

// offset: 0x18FC | func: 21
void dll_420_func_18FC(DLL420_Func18FCUnk* arg0, DLL420_Func18FCUnk* arg1, DLL420_Func18FCUnk* arg2) {
    s32 idx1;
    s32 idx2;

    idx1 = 0;
    idx2 = 0;
    
    while(arg1->unk28[idx1]) {
        idx1++;
    }
    
    while(arg2->unk28[idx2]) {
        idx2++;
    }
    
    if ((arg1->unk24 >= idx1) && (arg2->unk24 >= idx2)) {
        arg1->unk28[idx1] = arg0;
        arg2->unk28[idx2] = arg0;
        arg0->unk4 = arg1;
        arg0->unk8 = arg2;
    }
}

// offset: 0x1994 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_1994.s")

// offset: 0x1A8C | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_1A8C.s")

// offset: 0x1C48 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_1C48.s")

// offset: 0x1EF0 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/420_DFropenode/dll_420_func_1EF0.s")

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
