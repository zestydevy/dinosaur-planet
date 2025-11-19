#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/main.h"
#include "game/objects/object_id.h"
#include "sys/objects.h"
#include "types.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"

void func_8005B5B8(Object*, Object*, s32);

typedef struct {
    u8 unk0;
    Vec3f translate;
    u8 unk10;
    s16 yaw;
} WLGalleon_Data;

typedef struct {
    ObjSetup base;
    s8 yaw;
} WLGalleon_Setup;

/*0x0*/ static s8 dataShowKrystalsAdventureScreen[] = {
    0, 0, 0, 0, 0, 0, 0, 0
};
/*0x8*/ static void* dataDLLUnused = NULL;
/*0xC*/ static u32 sUpdateRateCopy = 3;

static int WLgalleon_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void WLgalleon_ctor(void *dll) { }

// offset: 0xC | dtor
void WLgalleon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WLgalleon_setup(Object* self, WLGalleon_Setup* setup, s32 arg2) {
    s32 index;
    WLGalleon_Data* objData;

    objData = self->data;

    if (main_get_bits(BIT_Play_Seq_00EF_Scales_Escapes_With_Kyte) || 
            self->id == OBJ_SB_ShipShadow) {
        return;
    }

    obj_set_update_priority(self, 0x5A);
    self->animCallback = WLgalleon_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    self->unkDC = 9;

    objData->translate.x = self->srt.transl.x;
    objData->translate.y = self->srt.transl.y;
    objData->translate.z = self->srt.transl.z;
    objData->yaw = self->srt.yaw;

    self->unkB0 |= 8;

    func_80059038(0, self, 0);

    for (index = 0; index < 5; index++){
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, index, 0);
    }
}

// offset: 0x14C | func: 1 | export: 1
void WLgalleon_control(Object* self) {
    u32 arrivedAtWM;
    Object* player;
    WLGalleon_Data* objData;
    u8 colourRGBA[4] = {0xe4, 0x9c, 0x44, 0xff}; //unused orange colour?

    if (!main_get_bits(BIT_Play_Seq_00EF_Scales_Escapes_With_Kyte)) {
        if (self->id == OBJ_SB_ShipShadow) {
            self->opacity = 128;
            return;
        }

        player = get_player();
        objData = self->data;

        if (main_get_bits(BIT_429)) {
            if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->unk34, 2)) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 1, 0);
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 2, 0);
            }
        } else if (!main_get_bits(BIT_WM_Played_Randorn_First_Meeting) && 
                !gDLL_29_Gplay->vtbl->get_obj_group_status(self->unk34, 2)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 1, 1);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 2, 1);
        }

        if (!main_get_bits(BIT_WM_Played_Randorn_First_Meeting)) {
            if (!objData->unk10 && !main_get_bits(BIT_429)) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 1, 1);
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 2, 1);
                objData->unk10 = TRUE;
            }
        } else {
            if (!gDLL_29_Gplay->vtbl->get_obj_group_status(self->unk34, 4)) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 4, 1);
            }
            if (objData->unk10) {
                objData->unk10 = FALSE;
            }
        }

        arrivedAtWM = main_get_bits(BIT_Galleon_Arrived_at_Warlock_Mountain);

        if (arrivedAtWM) {
            self->unkDC = 0xA;
        }

        if (!arrivedAtWM) {
            player->srt.transl.x = -121.0f;
            player->srt.transl.y = 116.0f; //@bug: slightly too high above Galleon, causes fall sound
            player->srt.transl.z = 5.0f;
            func_8005B5B8(player, self, 0);
            ((DLL_Unknown*)player->dll)->vtbl->func[68].withOneArg((s32)player);
            self->unkE0 = 1;
            return;
        }

        if (self->unkE0 == 1) {
            self->srt.transl.x = objData->translate.x;
            self->srt.transl.y = objData->translate.y;
            self->srt.transl.z = objData->translate.z;
            self->srt.yaw = objData->yaw;
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            self->unkE0 = 2;
        }
    }
}

// offset: 0x4F0 | func: 2 | export: 2
void WLgalleon_update(Object *self) { }

// offset: 0x4FC | func: 3 | export: 3
void WLgalleon_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (main_get_bits(BIT_Play_Seq_00EF_Scales_Escapes_With_Kyte) || !visibility){
        return;
    }

    if (self->id != OBJ_SB_ShipShadow || self->parent->unkDC < 7) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (dataShowKrystalsAdventureScreen[0]) {
            gDLL_20_Screens->vtbl->show_screen(1);
        }
    }
}

// offset: 0x5D8 | func: 4 | export: 4
void WLgalleon_free(Object* self, s32 arg1) {
    WLGalleon_Data* objData;

    if (self->id == OBJ_SB_ShipShadow) {
        return;
    }

    objData = self->data;
    if (objData->unk10 && !arg1) {
        objData->unk10 = FALSE;
    }

    if (dataDLLUnused) {
        dll_unload(dataDLLUnused);
        dataDLLUnused = NULL;
    }
}

// offset: 0x654 | func: 5 | export: 5
u32 WLgalleon_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x664 | func: 6 | export: 6
u32 WLgalleon_get_data_size(Object *self, u32 a1) {
    return sizeof(WLGalleon_Data);
}

// offset: 0x678 | func: 7
static int WLgalleon_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    s32 index;

    sUpdateRateCopy = gUpdateRate; //unused?
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;

    for (index = 0; index < animObjData->unk98; index++){
        switch (animObjData->unk8E[index]) {
            case 1:
                self->unkDC = 0xA;
                break;
            case 9:
                self->unkDC = 0xB;
                break;
            case 4:
                self->unkDC = 0xC;
                break;
            case 5:
                self->unkDC = 0xD;
                break;
            case 6:
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 1, 0);
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 2, 0);
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 4, 0);
                main_set_bits(BIT_WL_Load_Unload_Galleon, 0);
                break;
            case 2:
                //Setting envFxActions
                func_80000450(self, self, 0x77, 0, 0, 0);
                func_80000450(self, self, 0x78, 0, 0, 0);
                func_80000450(self, self, 0x80, 0, 0, 0);
                break;
            case 3:
                gDLL_23->vtbl->func_4C(0, 0x1e, 0x50);
                break;
            case 7:
                dataShowKrystalsAdventureScreen[0] = TRUE;
                break;
            case 8:
                dataShowKrystalsAdventureScreen[0] = FALSE;
                break;
        }
    }
    if (main_get_bits(BIT_429) && gDLL_29_Gplay->vtbl->get_obj_group_status(self->unk34, 2)) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 1, 0);
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 2, 0);
    }
    return 0;
}
