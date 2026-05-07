#include "common.h"
#include "dll.h"
#include "sys/map_enums.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/segment_1460.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/210_player.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 testStartRadius;
} DBSHshrine_Setup;

typedef struct {
    s16 testStartRadius;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    u8 state;
    u8 unkF;
    s8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
} DBSHshrine_Data;

/*0x0*/ static Texture* data_0 = NULL;

static int DBSHshrine_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static void DBSHshrine_handle_messages(Object* self);

// offset: 0x0 | ctor
void DBSHshrine_ctor(void *dll) { }

// offset: 0xC | dtor
void DBSHshrine_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBSHshrine_setup(Object* self, DBSHshrine_Setup* objSetup, s32 arg2) {
    DBSHshrine_Data* objData;
    DLL_IModgfx* modGfxDLL;

    objData = self->data;
    self->srt.yaw = 0;
    objData->testStartRadius = 10;
    if (objSetup->testStartRadius > 0) {
        objData->testStartRadius = objSetup->testStartRadius >> 8;
    }
    objData->state = 0;
    objData->unkF = 0;
    objData->unk2 = 0;

    self->animCallback = DBSHshrine_anim_callback;

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
    
    //Create glowing ring around test's startpoint
    modGfxDLL = dll_load_deferred(DLL_ID_122, 1);
    objData->unkC = modGfxDLL->vtbl->func0(self, 3, NULL, 0x402, -1, 0);
    dll_unload(modGfxDLL);
    
    if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) == 0) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 1);
    }
    
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
}

// offset: 0x224 | func: 1 | export: 1
void DBSHshrine_control(Object* self) {
    DBSHshrine_Data* objData;
    Object* player;
    DLL_IModgfx* modGfxDLL;
    s16 volume;
    f32 dz;
    f32 distance;
    Object* door;

    objData = self->data;
    player = get_player();
    distance = 1000.0f;
    
    if (player == NULL) {
        return;
    }

    DBSHshrine_handle_messages(self);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);

    if (objData->unk6 != 0) {
        objData->unk4 += objData->unk6;
        if (objData->unk4 <= 12) {
            objData->unk4 = 12;
            objData->unk6 = 0;
        } else if (objData->unk4 >= 70) {
            objData->unk4 = 70;
            objData->unk6 = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objData->unk4);
    }

    if (objData->unkA != 0) {
        objData->unk8 += objData->unkA;
        if ((objData->unk8 < 2) && (objData->unkA <= 0)) {
            objData->unk8 = 1;
            objData->unkA = 0;
        } else if ((objData->unk8 >= 70) && (objData->unkA >= 0)) {
            objData->unk8 = 70;
            objData->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objData->unk8);
    }

    if (objData->unk2 > 0) {
        objData->unk2 -= gUpdateRate;

        if (objData->unk2 <= 0) {
            objData->unk2 = 0;
            if (objData->unk11 == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objData->unk8, 0);
                objData->unk11 = 1;
            }
        }

        if ((objData->state == 2) && (objData->unk2 <= 40) && (objData->unk12 == 0)) {
            objData->unk12 = 1;
        }

        return;
    }

    door = obj_get_nearest_type_to(OBJTYPE_16, player, &distance);
    if ((door != NULL) && (distance < 300.0f) && (distance > 100.0f)) {
        dz = door->srt.transl.z - player->srt.transl.f[2];
        if (dz <= 0.0f) {
            if (dz < 0.0f) {
                dz *= -1.0f;
            }

            if (objData->unk8 != 30) {
                objData->unk8 = 30;
            }
            
            volume = objData->unk8 * ((dz - 100.0f) / 200.0f);
            if (volume <= 0) {
                volume = 1;
            }
            gDLL_5_AMSEQ->vtbl->set_volume(3, volume);
            
            volume = objData->unk4 * ((200.0f - (dz - 100.0f)) / 200.0f);
            if (volume <= 0) {
                volume = 1;
            }
            
            gDLL_5_AMSEQ->vtbl->set_volume(2, volume);
        }
    }

    //STATE MACHINE
    switch (objData->state) {
    case 0:
        if (vec3_distance(&self->globalPosition, &player->globalPosition) < objData->testStartRadius) {
            if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) != 0) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 0);
            }
            
            objData->state = 1;
            main_set_bits(BIT_DB_Entered_Shrine_3, 0);
            gDLL_3_Animation->vtbl->func17(0, self, -1);

            modGfxDLL = dll_load_deferred(DLL_ID_147, 1);
            modGfxDLL->vtbl->func0(self, 3, 0, 1, -1, 0);
            dll_unload(modGfxDLL);

            modGfxDLL = dll_load_deferred(DLL_ID_148, 1);
            modGfxDLL->vtbl->func0(self, 0, 0, 1, -1, 0);
            dll_unload(modGfxDLL);
            
            main_set_bits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_14_Modgfx->vtbl->func7(&objData->unkC);
        }
        break;
    case 1:
        if (objData->unkF == 1) {
            objData->state = 2;
            objData->unk2 = 80;
            main_set_bits(BIT_16A, 1);
        }
        break;
    case 2:
        if (main_get_bits(BIT_DB_Shrine_Test_of_Strength_Win)) {
            objData->state = 4;
            objData->unk2 = 0;
            objData->unk6 = 1;
            return;
        }

        if (main_get_bits(BIT_DB_Shrine_Test_of_Strength_Lose)) {
            objData->state = 8;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (u8)objData->unk8, 0);
            objData->unkA = 1;
            gDLL_3_Animation->vtbl->func17(2, self, -1);
            objData->unk2 = 0xA;
            return;
        }
        break;
    case 4:
        if (((DLL_210_Player*)player->dll)->vtbl->func38(player, 2) != 0) {
            objData->unk8 = 1;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8)objData->unk8, 0);
            objData->unkA = 1;
            main_set_bits(BIT_DB_Entered_Shrine_3, 1);
            objData->state = 6;
            return;
        }
        main_set_bits(BIT_DB_Entered_Shrine_1, 0);
        gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8)objData->unk8, 0);
        objData->unkA = 1;
        gDLL_3_Animation->vtbl->func17(1, self, -1);
        objData->state = 5;
        return;
    case 5:
        main_set_bits(BIT_15F, 0);
        objData->state = 6;
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 2, 1);
        gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 3);
        break;
    case 6:
        if (main_get_bits(BIT_Shrine_Do_Exit_Warp) == 0) {
            main_set_bits(BIT_Shrine_Do_Exit_Warp, 1);
        }
        main_set_bits(BIT_DB_Entered_Shrine_2, 0);
        main_set_bits(BIT_16A, 0);
        main_set_bits(BIT_DB_Shrine_Test_of_Strength_Win, 0);
        main_set_bits(BIT_DB_Shrine_Test_of_Strength_Lose, 0);
        main_set_bits(BIT_DB_Entered_Shrine_3, 0);
        objData->state = 7;
        return;
    case 8:
        objData->state = 0;
        objData->unkF = 0;
        objData->unk2 = 400;
        main_set_bits(BIT_DB_Entered_Shrine_3, 1);
        main_set_bits(BIT_15F, 0);
        main_set_bits(BIT_DB_Entered_Shrine_1, 1);
        main_set_bits(BIT_DB_Entered_Shrine_2, 1);
        main_set_bits(BIT_16A, 0);
        main_set_bits(BIT_DB_Shrine_Test_of_Strength_Win, 0);
        main_set_bits(BIT_DB_Shrine_Test_of_Strength_Lose, 0);
        
        modGfxDLL = dll_load_deferred(DLL_ID_122, 1);
        objData->unkC = modGfxDLL->vtbl->func0(self, 3, 0, 0x402, -1, 0);
        dll_unload(modGfxDLL);
        break;
    }
}

// offset: 0xC0C | func: 2 | export: 2
void DBSHshrine_update(Object *self) { }

// offset: 0xC18 | func: 3 | export: 3
void DBSHshrine_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    //@bug? Doesn't check visibility
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0xC60 | func: 4 | export: 4
void DBSHshrine_free(Object* self, s32 arg1) {
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
    gDLL_14_Modgfx->vtbl->func5(self);
}

// offset: 0xCF8 | func: 5 | export: 5
u32 DBSHshrine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xD08 | func: 6 | export: 6
u32 DBSHshrine_get_data_size(Object *self, u32 a1) {
    return sizeof(DBSHshrine_Data);
}

// offset: 0xD1C | func: 7
int DBSHshrine_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    DBSHshrine_Data* objData;
    s32 i;

    objData = self->data;
    animData->unk7A = -1;
    animData->unk62 = 0;
    
    if (objData->unkA != 0) {
        objData->unk8 += objData->unkA;
        if ((objData->unk8 < 2) && (objData->unkA <= 0)) {
            objData->unk8 = 1;
            objData->unkA = 0;
        } else if ((objData->unk8 >= 70) && (objData->unkA >= 0)) {
            objData->unk8 = 70;
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
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                main_set_bits(BIT_15F, 0);
                main_set_bits(BIT_DB_Entered_Shrine_1, 1);
                objData->unkA = -3;
                break;
            case 5:
                objData->unkF = 3;
                objData->unkA = -3;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 7:
                main_set_bits(BIT_15F, 1);
                break;
            case 8:
                main_set_bits(BIT_15F, 0);
                objData->unkA = -3;
                break;
            case 10:
                main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (data_0 == NULL) {
                    data_0 = block_texanim_get_tex(1);
                }
                break;
            case 9:
                main_set_bits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                objData->unk8 = 80;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2E, 0x50, (u8)objData->unk8, 0);
                break;
            case 13:
                gDLL_3_Animation->vtbl->func19(0x59, 1, 0, 0x50);
                break;
            case 14:
                objData->unk8 = 100;
                break;
            }
        }
        
        animData->unk8E[i] = 0;
    }
    
    return 0;
}

// offset: 0x10A0 | func: 8
void DBSHshrine_handle_messages(Object* self) {
    Object* sender;
    u32 message;
    s32 messageArg;
    DBSHshrine_Data* objData;

    objData = self->data;
    messageArg = 0;
    
    while (obj_recv_mesg(self, &message, &sender, (void*)&messageArg) != 0) {
        switch (message) {
        case 0x30005:
            if (vec3_distance(&self->globalPosition, &sender->globalPosition) < 200.0f) {
                objData->unk6 = -3;
            }
            break;
        case 0x30006:
            if (vec3_distance(&self->globalPosition, &sender->globalPosition) < 200.0f) {
                objData->unk6 = 0x10;
            }
            break;
        }
    }
}
