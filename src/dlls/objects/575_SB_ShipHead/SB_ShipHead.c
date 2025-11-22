#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "dlls/objects/572_SB_Galleon.h"
#include "dlls/projgfx/194.h"

typedef struct {
/*00*/ Object *cloudrunner;
/*04*/ s16 counter;
} SB_ShipHead_Data;

/*0x0*/ static s32 _data_0[] = { 0 };

// offset: 0x0 | ctor
void SB_ShipHead_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_ShipHead_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_ShipHead_setup(Object *self, ObjSetup *setup, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_4);
    obj_init_mesg_queue(self, 10);
}

// offset: 0x7C | func: 1 | export: 1
void SB_ShipHead_control(Object *self) {
    ObjSetup *minifireSetup;
    Object *fireballObj;
    Object *parent;
    Object *player;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 dx;
    f32 dy;
    f32 dz;
    ObjSetup *fireballSetup;
    s32 i;
    Object **objects;
    s32 objIndex;
    s32 objCount;
    s32 parentDC;
    Object *outSender;
    f32 temp_fv1;
    Object *sp4C;
    SB_ShipHead_Data *objdata;
    u32 outMesgID;
    void *outMesgArg;
    void **dll;
    u8 sp3B;

    sp3B = 0;
    parent = self->parent;
    if (!parent)
        return;
    parentDC = parent->unkDC;
    if (parent->id == OBJ_WL_Galleon) {
        if (self->modelInstIdx != 1) {
            func_80023A18(self, 1);
        }
        self->objhitInfo->unk58 &= ~1;
        if (parentDC != 10) {
            func_80024108(self, 0.005f, gUpdateRateF, NULL);
        } else {
            func_800240BC(self, 0);
        }
        return;
    }
    objdata = self->data;
    if (!objdata->cloudrunner) {
        objects = get_world_objects(&objIndex, &objCount);
        for (i = objIndex; i < objCount; i++) {
            if (objects[i]->id == OBJ_SB_Cloudrunner) {
                objdata->cloudrunner = objects[i];
                i = objCount;
            }
        }
    }
    if (obj_recv_mesg(self, &outMesgID, &outSender, &outMesgArg)) {
        switch (outMesgID) {
        case 0x130001:
            dll = dll_load_deferred(DLL_ID_194, 1);
            ((DLL_194*)dll)->vtbl->base.func0(self, 0, NULL, 1, -1, 8, 0);
            if (dll != NULL) {
                dll_unload(dll);
            }
            break;
        case 0x130002:
            sp3B = 1;
            break;
        case 0x130003:
            sp3B = 2;
            break;
        }
    }
    if ((self->unkE0 <= 0) && ((parentDC == 3) || (parentDC == 4) || (parentDC == 5)) && (func_80025F40(self, &sp4C, NULL, NULL) != 0) && (sp4C->id != OBJ_SB_FireBall)) {
        objdata->counter = 255;
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_179, MAX_VOLUME, NULL, NULL, 0, NULL);
        ((DLL_572_SB_Galleon*)parent->dll)->vtbl->func7(parent);
        self->unkE0 = 300;
    }
    if (self->unkE0 > 0) {
        self->unkE0 -= gUpdateRate;
    }
    if (parentDC >= 8) {
        func_80023A18(self, 1);
    }
    if (parentDC == 8) {
        self->unkDC++;
        if (self->unkDC > 10) {
            self->unkDC = 0;
        }
    }
    if ((parentDC == 5) && (_data_0[0] != 5)) {
        self->unkDC += gUpdateRate;
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_17A, MAX_VOLUME, NULL, NULL, 0, NULL);
        self->srt.transl.y += 50.0f;
        self->srt.transl.z -= 300.0f;
        get_object_child_position(self, &ox, &oy, &oz);
        self->srt.transl.y -= 50.0f;
        self->srt.transl.z += 300.0f;
        fireballSetup = obj_alloc_create_info(sizeof(ObjSetup), OBJ_SB_FireBall);
        fireballSetup->loadDistance = 0xFF;
        fireballSetup->fadeDistance = 0xFF;
        fireballSetup->loadFlags = 2;
        fireballSetup->fadeFlags = 1;
        fireballSetup->x = ox;
        fireballSetup->y = oy;
        fireballSetup->z = oz;
        fireballObj = obj_create(fireballSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
        player = get_player();
        dx = player->positionMirror.x - fireballObj->srt.transl.x;
        dy = (player->positionMirror.y - 30.0f) - fireballObj->srt.transl.y;
        dz = player->positionMirror.z - fireballObj->srt.transl.z;
        temp_fv1 = 30.0f / sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        fireballObj->unkDC = 120;
        fireballObj->speed.x = dx * temp_fv1;
        fireballObj->speed.y = dy * temp_fv1;
        fireballObj->speed.z = dz * temp_fv1;
        fireballObj->unkE0 = (s32)objdata->cloudrunner;
    }
    if (sp3B == 1) {
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_17A, 43, NULL, NULL, 0, NULL);
        player = get_player();
        minifireSetup = obj_alloc_create_info(sizeof(ObjSetup), OBJ_SB_MiniFire);
        minifireSetup->x = player->positionMirror.x + 100.0f;
        minifireSetup->y = rand_next(-6, 6) + player->positionMirror.y + 50.0f;
        minifireSetup->z = rand_next(-6, 6) + player->positionMirror.z + 45.0f;
        minifireSetup->loadFlags = 2;
        minifireSetup->fadeFlags = 1;
        minifireSetup->loadDistance = 0xFF;
        minifireSetup->fadeDistance = 0xFF;
        obj_create(minifireSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
    }
    func_80024108(self, 0.005f, gUpdateRateF, NULL);
    _data_0[0] = parentDC;
}

// offset: 0x770 | func: 2 | export: 2
void SB_ShipHead_update(Object *self) {
    SB_ShipHead_Data *objdata;

    objdata = self->data;
    if (objdata->counter > 0) {
        objdata->counter -= gUpdateRate * 3;
        if (objdata->counter < 0) {
            objdata->counter = 0;
        }
    }
}

// offset: 0x7BC | func: 3 | export: 3
void SB_ShipHead_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SRT transform;
    s16 temp_v1;
    s32 temp_v0;
    SB_ShipHead_Data *objdata;

    if (visibility) {
        objdata = self->data;
        temp_v1 = objdata->counter;
        if (temp_v1 != 0) {
            func_80036F6C(0xFF, 0xFF - temp_v1, 0xFF - temp_v1);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if ((self->parent) && (self->parent->id == OBJ_SB_Galleon)) {
            temp_v0 = ((DLL_572_SB_Galleon*)self->parent->dll)->vtbl->func10(self->parent);
            if ((temp_v0 != 0) && (temp_v0 != 2)) {
                transform.yaw = 0;
                if (1) {} if (1) {}
                transform.roll = 0;
                transform.pitch = 0;
                transform.scale = 1.0f;
                if (func_80031F6C(self, 2, &transform.transl.x, &transform.transl.y, &transform.transl.z, 0) == 0) {
                    if (rand_next(0, 3) == 0) {
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A3, &transform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                    }
                    if (rand_next(0, 3) == 0) {
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A6, &transform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                    }
                    if ((func_80031F6C(self, 8, &transform.transl.x, &transform.transl.y, &transform.transl.z, 0) == 0) && (rand_next(0, 6) == 0)) {
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A4, &transform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                    }
                }
            }
        }
    }
}

// offset: 0xA28 | func: 4 | export: 4
void SB_ShipHead_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_4);
}

// offset: 0xA68 | func: 5 | export: 5
u32 SB_ShipHead_get_model_flags(Object *self) {
    return MODFLAGS_1;
}

// offset: 0xA78 | func: 6 | export: 6
u32 SB_ShipHead_get_data_size(Object *self, u32 a1) {
    return 8;
}

/*0x0*/ static const char str_0[] = "CASE3\n";
/*0x8*/ static const char str_8[] = "CASE4\n";
/*0x10*/ static const char str_10[] = "CASE5\n";
/*0x18*/ static const char str_18[] = "CASE8\n";
