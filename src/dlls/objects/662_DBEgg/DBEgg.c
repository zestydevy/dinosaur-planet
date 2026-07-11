#include "common.h"
#include "dlls/objects/418_DFriverflow.h"
#include "game/objects/interaction_arrow.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"

typedef struct {
    ObjSetup base;
    u16 _unk18;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    u8 unk22;
    u8 unk23;
    s16 unk24;
    u8 unk26;
} DBEgg_Setup;

typedef struct {
    f32 unk0;
    u8 _unk4[0x10C - 0x4];
    u8 unk10C;
    u8 unk10D;
    s16 unk10E;
} DBEgg_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000001, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_662_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_662_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_662_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_setup.s")

// offset: 0x7C | func: 1 | export: 1
void dll_662_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_control.s")

// offset: 0xD28 | func: 2 | export: 2
void dll_662_update(Object* self) {
    s32 damageType;
    DBEgg_Data* objData;
    Object* player;
    Object* otherObj;

    damageType = func_80025F40(self, NULL, NULL, NULL);
    
    objData = self->data;
    
    if ((damageType == 0x11) && (objData->unk10C != 4)) {
        player = get_player();
        otherObj = (Object*) ((DLL_210_Player*)player->dll)->vtbl->func78(player);
        if (((DLL_Unknown*)otherObj->dll)->vtbl->func[10].withThreeArgsS32((s32)otherObj, 0x10, NULL) != 2) {
            self->objhitInfo->unk58 &= ~1;
            ((DLL_Unknown*)otherObj->dll)->vtbl->func[10].withThreeArgsS32((s32)otherObj, 2, (s32)self);
            objData->unk10C = 4;
        }
    }
    
    if ((objData->unk10C != 9) && (func_80059C40(&self->prevLocalPosition, &self->srt.transl, 8.0f, 1, NULL, self, 8, -1, 0xFFU, 0) != 0)) {
        self->velocity.f[0] -= (0.95f * self->velocity.f[0]);
        self->velocity.f[2] -= (0.95f * self->velocity.f[2]);
    }
    
    self->prevLocalPosition.f[0] = self->srt.transl.f[0];
    self->prevLocalPosition.f[1] = self->srt.transl.f[1];
    self->prevLocalPosition.f[2] = self->srt.transl.f[2];
}

// offset: 0xED0 | func: 3 | export: 3
void dll_662_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DBEgg_Data* objdata = self->data;

    if (visibility) {
        if ((objdata->unk10C != 0xC) && (objdata->unk10C != 4) && (objdata->unk10C != 0xB)) {
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0xF44 | func: 4 | export: 4
void dll_662_free(Object* self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_38);
}

// offset: 0xF84 | func: 5 | export: 5
u32 dll_662_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF94 | func: 6 | export: 6
u32 dll_662_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DBEgg_Data);
}

// offset: 0xFA8 | func: 7
// Get swept away by DFriverflow objects
void dll_662_func_FA8(Object* self, Vec3f* velocity) {
    DBEgg_Data* objData;
    Object* riverFlow;
    s32 count;
    Object** objects;
    s32 i;
    f32 delta;
    f32 dy;
    f32 dz;
    f32 range;
    f32 accelerationX;
    f32 accelerationZ;

    objData = self->data;

    accelerationX = 0.0f;
    accelerationZ = 0.0f;

    for (objects = obj_get_all_of_type(OBJTYPE_Riverflow, &count), i = 0; i < count; i++) {
        riverFlow = objects[i];
        dy = riverFlow->srt.transl.y - self->srt.transl.y;
        if ((dy <= 7.0f) && (dy >= -7.0f)) {
            delta = riverFlow->srt.transl.x - self->srt.transl.x;
            dz = riverFlow->srt.transl.z - self->srt.transl.z;
            delta = sqrtf(SQ(delta) + SQ(dz));
            
            range = ((DFriverflow_Setup*)riverFlow->setup)->range * 1.5f;
            if (delta < range) {
                delta = (range - delta) / range;
                delta *= riverFlow->srt.scale * 10.0f;
                
                accelerationX += fsin16_precise(riverFlow->srt.yaw) * delta;
                accelerationZ += fcos16_precise(riverFlow->srt.yaw) * delta;
            }
        }
    }
    
    if (count != 0) {
        accelerationX /= count;
        accelerationZ /= count;
        velocity->x -= (accelerationX * 0.05f);
        velocity->z -= (accelerationZ * 0.05f);
        velocity->x *= 0.99f;
        velocity->z *= 0.99f;
        
        delta = sqrtf(SQ(velocity->z) + SQ(velocity->x));
        if (delta > 1.85f) {
            velocity->x *= 1.85f / delta;
            velocity->z *= 1.85f / delta;
        }
    }
}

// offset: 0x1240 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_func_1240.s")

// offset: 0x1670 | func: 9
void dll_662_func_1670(Object* self) {
    Object* player;
    DBEgg_Data* objdata;
    DBEgg_Setup* setup;

    player = get_player();
    objdata = self->data;
    setup = (DBEgg_Setup*)self->setup;

    obj_free_object_type(self, OBJTYPE_38);
    objdata->unk10C = 3;
    main_set_bits(BIT_DB_Picked_Up_Egg, 1);
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    main_set_bits(setup->unk1C, 1);

    obj_send_mesg(player, 0x7000A, self, (void* )0x10000);
}

// offset: 0x1744 | func: 10
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_func_1744.s")
#else

s32 dll_662_func_1240(Object* self, f32* arg1, f32 arg2, f32 arg3, s32 arg4);

void dll_662_func_1744(Object* self, DBEgg_Data* objData) {
    f32 sp3C; //3C
    DBEgg_Setup* objSetup; //38

    objSetup = (DBEgg_Setup*)self->setup;
    
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    
    objData->unk10D = 0;
    self->srt.yaw = objSetup->unk1B << 8;
    self->srt.pitch = objSetup->unk22 << 8;
    self->srt.roll = objSetup->unk23 << 8;

    self->srt.scale = objSetup->unk1A * 0.015625f;
    self->srt.scale *= self->def->scale;

    if (main_get_bits(objSetup->unk1C)) {
        objData->unk10C = 3;
    } else {
        objData->unk10C = 1;
    }
    
    if (objData->unk10C == 1) {
        if (dll_662_func_1240(self, &sp3C, 0, 0, 1) == 0) {
            objData->unk10C = 2;
        }
    }
    
    if (objSetup->unk26) {
        objData->unk10D |= 1;
        if (objSetup->unk26 == 2) {
            objData->unk10D |= 2;
        }
        
        if (objSetup->unk26 == 3) {
            objData->unk10C = 0xA;
        }
        
        if (objSetup->unk26 == 4) {
            objData->unk10D |= 4;
            objData->unk10D &= ~1;
        }
    }

    obj_add_object_type(self, 0x26);
    
    if (main_get_bits(objSetup->unk24)) {
        objData->unk10C = 5;
    } else {
        objData->unk10C = 0xC;
    }
    
    self->unkE0 = 0;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    
    objData->unk0 = 0.0f;
}
#endif

// offset: 0x1954 | func: 11
void dll_662_func_1954(Object* self) {
    u32 outMessageID;
    u32 outMesgArg;
    DBEgg_Data* objData;
    Object* sender;
    f32 temp;

    outMessageID = 0;
    outMesgArg = 0;
    objData = self->data;
    
    while (obj_recv_mesg(self, &outMessageID, &sender, (void*)&outMesgArg)) {
        if (outMessageID == 0x11) {
            switch (outMesgArg) {
            case 0x12:  
                objData->unk10C = 0xB;
                break;
            case 0x11:   
                temp = fsin16_precise(sender->srt.yaw);
                self->velocity.f[0] = rand_next(0x11, 0x1B) * -temp * 0.3f;
                temp = fcos16_precise(sender->srt.yaw);
                self->velocity.f[2] = rand_next(0x11, 0x1B) * -temp * 0.3f;
                self->velocity.f[1] = rand_next(0x11, 0x1B) * 0.3f;
                /* fallthrough */
            case 0x10: 
                objData->unk10C = 5;
                self->unkAF &= ~8;
                break;
            default:
                break;
            }
        }
    }
}
