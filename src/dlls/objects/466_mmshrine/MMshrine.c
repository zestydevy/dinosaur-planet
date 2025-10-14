#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "dll.h"
#include "sys/gfx/texture.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "types.h"

s32 obj_recv_mesg(Object *obj, u32 *outMesgID, Object **outSender, void **outMesgArg);
Texture* func_8004A1E8(s32 match_value);
u16 get_masked_buttons(int port);

typedef struct {
/*00*/ s16 unk0;
/*02*/ s16 unk2;
/*04*/ s16 unk4;
/*06*/ s16 unk6;
/*08*/ s16 unk8;
/*0A*/ s16 unkA;
/*0C*/ s16 unkC;
/*0E*/ s8 unkE;
/*0F*/ u8 unkF;
/*10*/ s8 unk10;
/*11*/ s8 unk11;
/*12*/ s8 unk12;
/*13*/ s8 unk13;
} MMShrine_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
} Shrine_Setup;

typedef struct {
/*00*/ s8 unk0[0x62 - 0];
/*62*/ s8 unk62;
/*63*/ s8 unk63;
/*00*/ s8 unk64[0x7A - 0x64];
/*7A*/ s16 unk7A;
/*00*/ s8 unk7C[0x8E - 0x7C];
/*7A*/ u8 unk8E[0x98 - 0x8E];
/*7A*/ u8 unk98;
} UnkShrineStruct;

/*0x0*/ static Texture* _data_0 = 0;
/*0x4*/ static u32 _data_4[] = {
    0x01000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void MMshrine_ctor(void *dll) {
}

// offset: 0xC | dtor
void MMshrine_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/466_mmshrine/MMshrine_setup.s")

// offset: 0x1BC | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/466_mmshrine/MMshrine_control.s")

// offset: 0xB3C | func: 2 | export: 2
void MMshrine_update(Object *self) {
}

// offset: 0xB48 | func: 3 | export: 3
void MMshrine_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xB9C | func: 4 | export: 4
void MMshrine_free(Object *self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->func13(3, 0);
    gDLL_5_AMSEQ->vtbl->func13(2, 0);
}

// offset: 0xC2C | func: 5 | export: 5
u32 MMshrine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC3C | func: 6 | export: 6
u32 MMshrine_get_data_size(Object *self, s32 arg1) {
    return sizeof(MMShrine_Data);
}

// offset: 0xC50 | func: 7
int dll_466_func_C50(Object* self, s32 arg1, UnkShrineStruct* arg2, s32 arg3) {
    MMShrine_Data* objdata;
    Object* player;
    UnkShrineStruct* var_s2;
    s32 i;
    u8 temp;

    objdata = self->data;
    player = get_player();
    
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    
    if (objdata->unkA) {
        objdata->unk8 += objdata->unkA;
        if (objdata->unk8 < 2 && objdata->unkA <= 0) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if (objdata->unk8 >= 0x46 && objdata->unkA >= 0) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->func13(3, objdata->unk8);
    }

    for (i = 0; i < arg2->unk98; i++){
        temp = arg2->unk8E[i];
        if (temp != 0) {
            switch (temp) {
                case 1:
                    func_80000860(self, self, 0xC7, 0);
                    break;
                case 2:
                    if (D_80092A7C == -1) {
                        func_80000860(self, self, 0x14, 0);
                    } else {
                        func_80000860(self, self, D_80092A7C, 0);
                    }
                    break;
                case 3:
                    objdata->unk10 = 1;
                    break;
                case 4:
                    objdata->unk2 = 0;
                    break;
                case 5:
                    objdata->unkF = 5;
                    objdata->unk10 = 2;
                    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                    break;
                case 6:
                    objdata->unk10 = 3;
                    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                    break;
                case 7:
                    main_set_bits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                    break;
                case 8:
                    main_set_bits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
                    objdata->unkA = -3;
                    break;
                case 10:
                    main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                    if (_data_0 == NULL)
                        _data_0 = func_8004A1E8(1);
                    break;
                case 9:
                    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
                    break;
                case 11:
                    objdata->unk8 = 0x64;
                    gDLL_5_AMSEQ->vtbl->func5(3, 0x2f, 0x50, (u8)objdata->unk8, 0);
                    break;
                case 12:
                    func_80000860(self, self, 0xCE, 0);
                    main_set_bits(BIT_Test_of_Fear_Particles, 1);
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_342, MAX_VOLUME, 0, 0, 0, 0);
                    break;
                case 13:
                    if (D_80092A7C == -1) {
                        func_80000860(self, self, 0x14, 0);
                    } else {
                        func_80000860(self, self, D_80092A7C, 0);
                    }
                    main_set_bits(BIT_Test_of_Fear_Particles, 0);
                    break;
            }
        }
        arg2->unk8E[i] = 0;
    }
    
    if (objdata->unkF == 8) {
        if (vec3_distance(&self->positionMirror, &player->positionMirror) > 10.0f) {
            gDLL_3_Animation->vtbl->func18(arg2->unk63);
            objdata->unkF = 7;
        } else if (get_masked_buttons(0)) {
            gDLL_3_Animation->vtbl->func18(arg2->unk63);
            objdata->unkF = 7;
        }
    }
    
    return 0;
}

// offset: 0x1140 | func: 8
void MMshrine_func_1140(Object *arg0) {
    Object *sp54;
    u32 message;
    void *outMesgArg;
    MMShrine_Data *objdata;

    objdata = arg0->data;
    outMesgArg = 0;

    while (obj_recv_mesg(arg0, &message, &sp54, &outMesgArg)){
        switch (message) {
            case 0x30005:
                objdata->unk6 = -3;
                break;
            case 0x30006:
                objdata->unk6 = 0x10;
                break;
        }
    }
}
