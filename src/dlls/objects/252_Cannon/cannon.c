#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
} Cannon_Setup;

typedef struct {
    f32 unk0;
    Vec3f unk4;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 unk18;
    u8 unk19;
} Cannon_Data; //0x1C

/*0x0*/ static s16 _data_0 = 0x0046;
/*0x4*/ static Object* _data_4 = NULL;

// offset: 0x0 | ctor
void dll_252_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_252_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_252_setup(Object* self, Cannon_Setup* objSetup, s32 arg2) {
    Cannon_Data* objData = self->data;
    
    self->srt.yaw = (objSetup->unk18 << 4) << 4;
    self->srt.transl.y -= 100.0f;
    self->unkDC = objSetup->unk19;
    
    objData->unk18 = 0;

    if (objData->unk18 == 0) { //NOTE: always true
        objData->unk12 = _data_0;
        _data_0 = 70 - _data_0;
        
        objData->unk19 = objSetup->unk19;
        objData->unk16 = 0;
        objData->unk0 = 0.0f;
        objData->unk4.x = self->srt.transl.x;
        objData->unk4.y = self->srt.transl.y;
        objData->unk4.z = self->srt.transl.z;
    }
}

// offset: 0xB4 | func: 1 | export: 1
void dll_252_control(Object* self) {
    Object* cannonball; //5C
    s32 temp;
    ObjSetup* cannonballSetup;
    f32 sp50; //50
    f32 sp4C; //4C
    f32 sp48; //48
    s16 yaw; //46
    s16 temp2;
    Cannon_Data* objData; //40

    objData = self->data;
    
    diPrintf("CANNON\n");

    switch (objData->unk16) {
    case 0:
        if (objData->unk12 >= 0) {
            objData->unk12--;
            return;
        }
        objData->unk12 = 30;
        objData->unk16 = 1;
        objData->unk0 = 0.0f;
        self->srt.transl.x = objData->unk4.x;
        self->srt.transl.y = objData->unk4.y;
        self->srt.transl.z = objData->unk4.z;
        return;
    case 1:
        if (objData->unk12 >= 0) {
            self->srt.transl.x = self->srt.transl.x;
            self->srt.transl.y += 2.2f;
            self->srt.transl.z = self->srt.transl.z;
            objData->unk12--;
            return;
        }
        objData->unk16 = 2;
        objData->unk12 = 20;
        objData->unk0 = 0.0f;
        return;
    default:
        return;
    case 2:
        if (objData->unk12 >= 0) {
            objData->unk12--;
            if (self != NULL) {
                if ((self->objhitInfo != NULL) && (self->objhitInfo->unk50 < 0x46)) {
                    cannonball = self->objhitInfo->unk0;
                    if ((cannonball != _data_4) && (cannonball->group == 0x29)) {

                        if (self->srt.yaw & 0x8000) {
                            gDLL_3_Animation->vtbl->func25(gDLL_3_Animation->vtbl->func24() - 1);
dummy_label1: ;
                        } else {
                            gDLL_3_Animation->vtbl->func27(gDLL_3_Animation->vtbl->func26() - 1);
                        }
                        
                        gDLL_17_partfx->vtbl->spawn(self, 9, NULL, 4, -1, NULL);
                        gDLL_17_partfx->vtbl->spawn(self, 5, NULL, 4, -1, NULL);
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x18, MAX_VOLUME, NULL, NULL, 0, NULL);
                        obj_send_mesg_many(0x36, 0, 0, 0xE0000, self);
                        obj_destroy_object(cannonball);
                        obj_destroy_object(self);
                        return;
                    }
                }
            }
        } else {
            objData->unk12 = 0;
            objData->unk16 = 3;
            return;
        }
        break;
    case 3:
    
        get_object_child_position(self, &sp50, &sp4C, &sp48);
        cannonballSetup = obj_alloc_setup(0x18, OBJ_CannonBall);
        cannonballSetup->x = sp50;
        cannonballSetup->y = sp4C + 30.0f;
        cannonballSetup->z = sp48;
        cannonball = obj_create(cannonballSetup, 5, -1, -1, NULL);

        yaw = self->srt.yaw;
        if (self->parent != NULL) {
            yaw += self->parent->srt.yaw;
        }
        yaw += 0x4000;
        cannonball->speed.f[0] = fsin16_precise(yaw) * 7.0f;
        cannonball->speed.f[2] = fcos16_precise(yaw) * 7.0f;
        cannonball->srt.transl.f[0] += fsin16_precise(yaw) * 80.0f;
        cannonball->srt.transl.f[2] += fcos16_precise(yaw) * 80.0f;
        cannonball->unkDC = 300;
        camera_enable_y_offset();
        camera_set_shake_offset(0.9f);
        gDLL_17_partfx->vtbl->spawn(self, 0x69, NULL, 4, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, 2, NULL, 4, -1, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x96, MAX_VOLUME, NULL, NULL, 0, NULL);
    
        objData->unk16 = 4;
        objData->unk12 = 20;
        objData->unk14 = 30;
        
        objData->unk0 = 0.0f;
        return;
    case 4:
        if (objData->unk12 >= 0) {
            objData->unk12--;
            if (self != NULL) {
                if ((self->objhitInfo != NULL) && (self->objhitInfo->unk50 < 0x46)) {
                    cannonball = self->objhitInfo->unk0;
                    if ((cannonball != _data_4) && (cannonball->group == 0x29)) {
                        if (self->srt.yaw & 0x8000) {
                            gDLL_3_Animation->vtbl->func25(gDLL_3_Animation->vtbl->func24() - 1);
dummy_label3: ;
                        } else {
                            gDLL_3_Animation->vtbl->func27(gDLL_3_Animation->vtbl->func26() - 1);
                        }
                        
                        gDLL_17_partfx->vtbl->spawn(self, 9, NULL, 4, -1, NULL);
                        gDLL_17_partfx->vtbl->spawn(self, 5, NULL, 4, -1, NULL);
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x18, MAX_VOLUME, NULL, NULL, 0, NULL);
                        _data_4 = cannonball;
                        obj_send_mesg_many(0x36, 0, 0, 0xE0000, self);
                        obj_destroy_object(cannonball);
                        obj_destroy_object(self);
                        return;
                    }
                }
            }
        } else {
            if (objData->unk14 >= 0) {
                self->srt.transl.x = self->srt.transl.x;
                self->srt.transl.y -= 2.2f;
                self->srt.transl.z = self->srt.transl.z;
                objData->unk14--;
                return;
            }
            objData->unk16 = 0;
            objData->unk12 = 20;
            objData->unk14 = 0;
            objData->unk0 = 0.0f;
            return;
        }
        break;
    }
}

// offset: 0x888 | func: 2 | export: 2
void dll_252_update(Object *self) { }

// offset: 0x894 | func: 3 | export: 3
void dll_252_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8E8 | func: 4 | export: 4
void dll_252_free(Object* arg0, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(arg0);
}

// offset: 0x930 | func: 5 | export: 5
u32 dll_252_get_model_flags(Object *self){
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x940 | func: 6 | export: 6
u32 dll_252_get_data_size(Object *self, u32 a1){
    return sizeof(Cannon_Data);
}
