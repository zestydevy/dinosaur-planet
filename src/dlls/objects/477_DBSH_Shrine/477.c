#include "common.h"
#include "sys/objmsg.h"
#include "sys/segment_1460.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
} DLL477_Setup;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    u8 unkE;
    u8 unkF;
    s8 unk10;
    s8 unk11;
    s8 unk12;
    u8 unk13;
} DLL477_Data; //0x14

/*0x0*/ static Texture* data_0 = NULL;

static int dll_477_func_D1C(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);

// offset: 0x0 | ctor
void dll_477_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_477_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_477_setup(Object* self, DLL477_Setup* objSetup, s32 arg2) {
    DLL477_Data* objData;
    DLL_IModgfx* sp30;

    objData = self->data;
    
    self->srt.yaw = 0;
    objData->unk0 = 10;
    if (objSetup->unk1A > 0) {
        objData->unk0 = objSetup->unk1A >> 8;
    }
    objData->unkE = 0;
    objData->unkF = 0;
    objData->unk2 = 0;

    self->animCallback = dll_477_func_D1C;

    obj_init_mesg_queue(self, 4);

    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
    main_set_bits(BIT_15F, 0);
    main_set_bits(BIT_DB_Entered_Shrine_1, 1);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);

    objData->unk13 = 0;
    objData->unk4 = 12;
    objData->unk8 = 30;
    objData->unk2 = 1000;

    gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    
    objData->unk6 = 0;
    objData->unkA = 0;
    objData->unk11 = 0;
    objData->unk12 = 0;
    
    sp30 = dll_load_deferred(DLL_ID_122, 1);
    objData->unkC = sp30->vtbl->func0(self, 3, 0, 0x402, -1, 0);
    dll_unload(sp30);
    
    if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) == 0) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 1);
    }
    
    self->positionMirror.x = self->srt.transl.x;
    self->positionMirror.y = self->srt.transl.y;
    self->positionMirror.z = self->srt.transl.z;
}

// offset: 0x224 | func: 1 | export: 1
void dll_477_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/477_DBSH_Shrine/dll_477_control.s")

// offset: 0xC0C | func: 2 | export: 2
void dll_477_update(Object *self) { }

// offset: 0xC18 | func: 3 | export: 3
void dll_477_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    //@bug? Doesn't check visibility
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0xC60 | func: 4 | export: 4
void dll_477_free(Object* self, s32 arg1) {
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
    gDLL_14_Modgfx->vtbl->func5(self);
}

// offset: 0xCF8 | func: 5 | export: 5
u32 dll_477_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xD08 | func: 6 | export: 6
u32 dll_477_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL477_Data);
}

// offset: 0xD1C | func: 7
int dll_477_func_D1C(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    DLL477_Data* objData;
    s32 i;

    objData = self->data;
    animData->unk7A = -1;
    animData->unk62 = 0;
    
    if (objData->unkA != 0) {
        objData->unk8 += objData->unkA;
        if ((objData->unk8 < 2) && (objData->unkA <= 0)) {
            objData->unk8 = 1;
            objData->unkA = 0;
        } else if ((objData->unk8 >= 0x46) && (objData->unkA >= 0)) {
            objData->unk8 = 0x46;
            objData->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objData->unk8);
    }
    
    for (i = 0; i < animData->unk98; i++) {
        if (animData->unk8E[i] != 0) {
            switch (animData->unk8E[i]) {
            case 1:
                func_80000860(self, self, 0xD5, 0);
                break;
            case 2:
                if (D_80092A7C[0] == -1) {
                    func_80000860(self, self, 0x14, 0);
                } else {
                    func_80000860(self, self, D_80092A7C[0], 0);
                }
                break;
            case 3:
                objData->unkF = 1;
                break;
            case 4:
                objData->unkF = 2;
                main_set_bits(0x129, 1);
                main_set_bits(0x15F, 0);
                main_set_bits(0x126, 1);
                objData->unkA = -3;
                break;
            case 5:
                objData->unkF = 3;
                objData->unkA = -3;
                main_set_bits(0x129, 1);
                break;
            case 7:
                main_set_bits(0x15F, 1);
                break;
            case 8:
                main_set_bits(0x15F, 0);
                objData->unkA = -3;
                break;
            case 10:
                main_set_bits(0x128, 1);
                if (data_0 == NULL) {
                    data_0 = func_8004A1E8(1);
                }
                break;
            case 9:
                main_set_bits(0x127, 1);
                break;
            case 11:
                objData->unk8 = 0x50;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2E, 0x50, (u8)objData->unk8, 0);
                break;
            case 13:
                gDLL_3_Animation->vtbl->func19(0x59, 1, 0, 0x50);
                break;
            case 14:
                objData->unk8 = 0x64;
                break;
            }
        }
        
        animData->unk8E[i] = 0;
    }
    
    return 0;
}

// offset: 0x10A0 | func: 8
void dll_477_func_10A0(Object* self) {
    Object* sender;
    u32 message;
    s32 messageArg;
    DLL477_Data* objData;

    objData = self->data;
    messageArg = 0;
    
    while (obj_recv_mesg(self, &message, &sender, (void*)&messageArg) != 0) {
        switch (message) {
        case 0x30005:
            if (vec3_distance(&self->positionMirror, &sender->positionMirror) < 200.0f) {
                objData->unk6 = -3;
            }
            break;
        case 0x30006:
            if (vec3_distance(&self->positionMirror, &sender->positionMirror) < 200.0f) {
                objData->unk6 = 0x10;
            }
            break;
        }
    }
}
