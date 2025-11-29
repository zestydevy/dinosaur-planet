#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/779_WCLevelControl.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "functions.h"
#include "types.h"
#include "dll.h"

typedef struct {
/*000*/    s8 unk0[0x260 - 0]; // probably DLL27_Data
/*260*/    Object* levelCtrl; //WCLevelControl
/*264*/    f32 unk264;
/*268*/    f32 unk268;
/*26C*/    u32 unk26C;
/*270*/    s16 unk270;
/*272*/    s16 unk272;
/*274*/    u8 unk274;
/*275*/    u8 unk275;
/*276*/    u8 unk276;
/*277*/    u8 unk277;
} WCPushBlock_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18;
/*19*/ s8 modelIndex;
/*1A*/ s16 unk1A;
} WCPushBlock_Setup;

static s32 dll_782_func_FF0(Object* self, WCPushBlock_Data* objdata, Object* player);

// offset: 0x0 | ctor
void dll_782_ctor(void* dll){ }

// offset: 0xC | dtor
void dll_782_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void dll_782_setup(Object* self, WCPushBlock_Setup* setup, s32 arg2) {
    WCPushBlock_Data* objdata = self->data;

    self->opacity = 0;
    self->modelInstIdx = setup->modelIndex;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    objdata->unk276 = setup->unk1A;
}

// offset: 0x58 | func: 1 | export: 1
void dll_782_control(Object* self) {
    TextureAnimator* temp_v0;
    Object* player;
    f32 var_fv1;
    s32 var_v0;
    s32 isNighttime;
    f32 distance;
    WCPushBlock_Data* objdata;
    f32 time;
    s32 var_a0;

    objdata = self->data;
    player = get_player();
    distance = 100000.0f;
    if (objdata->levelCtrl == NULL) {
        objdata->levelCtrl = obj_get_nearest_type_to(OBJTYPE_10, self, &distance); //getting WCLevelControl
        self->opacity = 0;
        return;
    }
    
    temp_v0 = func_800348A0(self, 0, 0);
    if (temp_v0 != NULL) {
        temp_v0->frame = 0;
    }
    if (objdata->unk274 != 6) {
        isNighttime = gDLL_7_Newday->vtbl->func8(&time);
        
        if (self->modelInstIdx == 1) {
            if (main_get_bits(BIT_812) != 0) {
                objdata->unk274 = 6;
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func12(objdata->unk276, &objdata->unk270, &objdata->unk272);
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func7(&self->srt, objdata->unk270, objdata->unk272, &self->srt.transl.x, &self->srt.transl.z);
            } else if (isNighttime != 0) {
                objdata->unk274 = 3;
            }
        } else {
            if (main_get_bits(BIT_813) != 0) {
                objdata->unk274 = 6;
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func19(objdata->unk276, &objdata->unk270, &objdata->unk272);
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func14(&self->srt, objdata->unk270, objdata->unk272, &self->srt.transl.x, &self->srt.transl.z);
            } else if (isNighttime == 0) {
                objdata->unk274 = 3;
            }
        }
    }
    switch (objdata->unk274) {
    case 0:
        if (self->modelInstIdx == 1) {
            ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func11(objdata->unk276, &objdata->unk270, &objdata->unk272);
            ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func7(&self->srt, objdata->unk270, objdata->unk272, &self->srt.transl.x, &self->srt.transl.z);
        } else {
            ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func18(objdata->unk276, &objdata->unk270, &objdata->unk272);
            ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func14(&self->srt, objdata->unk270, objdata->unk272, &self->srt.transl.x, &self->srt.transl.z);
        }
        objdata->unk274 = 1;
        break;
    case 1:
        var_v0 = self->opacity + (gUpdateRate * 8);
        if (var_v0 > 0xFF) {
            var_v0 = 0xFF;
        }
        self->opacity = (u8) var_v0;
        self->speed.x = 0.0f;
        self->speed.z = 0.0f;
        if (((DLL_210_Player*)player->dll)->vtbl->func47(player, self, &objdata->unk275) != 0) {
            if (self->modelInstIdx == 1) {
                if (objdata->unk275 == 0) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func13(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, -1, 0);
                }
                else if (objdata->unk275 == 1) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func13(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, 1, 0);
                }
                else if  (objdata->unk275 == 2) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func13(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, 0, -1);
                }
                else if  (objdata->unk275 == 3) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func13(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, 0, 1);
                }
            } else {
                if (objdata->unk275 == 0) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func20(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, -1, 0);
                }
                else if  (objdata->unk275 == 1) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func20(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, 1, 0);
                }
                else if  (objdata->unk275 == 2) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func20(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, 0, -1);
                }
                else if  (objdata->unk275 == 3) {
                    objdata->unk277 = ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func20(
                        &self->srt, objdata->unk270, objdata->unk272, &objdata->unk264, &objdata->unk268, 0, 1);
                }
            }
            if ((objdata->unk264 != self->srt.transl.x) || (objdata->unk268 != self->srt.transl.y)) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9B9_Block_Shifting, 1, &objdata->unk26C, 0, 0, 0);
                objdata->unk274 = 2;
            }
        }
        break;
    case 2:
        var_fv1 = sqrtf((self->speed.x * self->speed.x) + (self->speed.z * self->speed.z));
        var_fv1 -= 0.25f;
        if (var_fv1 < 0.0f) {
            var_fv1 = 0.0f;
        }
        var_fv1 = ((126.0f * var_fv1) / 1.25f) + 1.0f;
        if (var_fv1 > 127.0f) {
            var_fv1 = 127.0f;
        }
        
        gDLL_6_AMSFX->vtbl->func_860(objdata->unk26C, var_fv1);
        obj_integrate_speed(self, self->speed.x * gUpdateRateF, 0.0f, self->speed.z * gUpdateRateF);
        var_a0 = 0;
        if (objdata->unk275 == 0) {
            if (self->speed.x < 1.5f) {
                self->speed.x += (gUpdateRateF * 0.05f);
            }
            if (objdata->unk264 <= self->srt.transl.x) {
                self->srt.transl.x = objdata->unk264;
                var_a0 = 1;
            }
        } else if (objdata->unk275 == 1) {
            if (self->speed.x > -1.5f) {
                self->speed.x -= (gUpdateRateF * 0.05f);
            }
            if (self->srt.transl.x <= objdata->unk264) {
                self->srt.transl.x = objdata->unk264;
                var_a0 = 1;
            }
        } else if (objdata->unk275 == 2) {
            if (self->speed.z < 1.5f) {
                self->speed.z += (gUpdateRateF * 0.05f);
            }
            if (objdata->unk268 <= self->srt.transl.z) {
                self->srt.transl.z = objdata->unk268;
                var_a0 = 1;
            }
        } else if (objdata->unk275 == 3) {
            if (self->speed.z > -1.5f) {
                self->speed.z -= (gUpdateRateF * 0.05f);
            }
            if (self->srt.transl.z <= objdata->unk268) {
                self->srt.transl.z = objdata->unk268;
                var_a0 = 1;
            }
        }
        if (self->speed.x > 1.5f) {
            self->speed.x = 1.5f;
        }
        if (self->speed.x < -1.5f) {
            self->speed.x = -1.5f;
        }
        if (self->speed.z > 1.5f) {
            self->speed.z = 1.5f;
        }
        if (self->speed.z < -1.5f) {
            self->speed.z = -1.5f;
        }
        if (var_a0 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk26C);
            self->speed.x = 0.0f;
            self->speed.z = 0.0f;
            if (objdata->unk277 == 2) {
                objdata->unk274 = 4;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9BB_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, 0, 0, 0);
                if (self->modelInstIdx == 1) {
                    main_increment_bits(BIT_810);
                } else {
                    main_increment_bits(BIT_811);
                }
            } else if (objdata->unk277 == 1) {
                objdata->unk274 = 1;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9BA_Thud, MAX_VOLUME, NULL, 0, 0, 0);
            } else {
                objdata->unk274 = 3;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9BA_Thud, MAX_VOLUME, NULL, 0, 0, 0);
            }
            if (objdata->unk274 != 3) {
                if (self->modelInstIdx == 1) {
                    ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func9(0, objdata->unk270, objdata->unk272);
                    ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func8(&self->srt, self->srt.transl.x, self->srt.transl.z, &objdata->unk270, &objdata->unk272);
                    ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func9(objdata->unk276, objdata->unk270, objdata->unk272);
                } else {
                    ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func16(0, objdata->unk270, objdata->unk272);
                    ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func15(&self->srt, self->srt.transl.x, self->srt.transl.z, &objdata->unk270, &objdata->unk272);
                    ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func16(objdata->unk276, objdata->unk270, objdata->unk272);
                }
            }
        }
        break;
    case 3:
        func_800267A4(self);
        if (self->opacity == 0xFF) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9C5_Vanish, MAX_VOLUME, NULL, 0, 0, 0);
        }
        var_v0 = self->opacity - (gUpdateRate * 8);
        if (var_v0 < 0) {
            var_v0 = 0;
        }
        self->opacity = (u8) var_v0;
        if (self->opacity == 0 && (dll_782_func_FF0(self, objdata, get_player()) != 0)) {
            if (self->modelInstIdx == 1) {
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func11(objdata->unk276, &objdata->unk270, &objdata->unk272);
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func7(&self->srt, objdata->unk270, objdata->unk272, &self->srt.transl.x, &self->srt.transl.z);
            } else {
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func18(objdata->unk276, &objdata->unk270, &objdata->unk272);
                ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func14(&self->srt, objdata->unk270, objdata->unk272, &self->srt.transl.x, &self->srt.transl.z);
            }
            objdata->unk274 = 5;
        }
        break;
    case 5:
        if (self->opacity == 0) {
            func_8002674C(self);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9C6_Appear, MAX_VOLUME, NULL, 0, 0, 0);
        }
        var_v0 = self->opacity + (gUpdateRate * 8);
        if (var_v0 > 0xFF) {
            var_v0 = 0xFF;
        }
        self->opacity = (u8) var_v0;
        if (self->opacity >= 0xFF) {
            objdata->unk274 = 1;
        }
        break;
    case 6:
        self->opacity = 0xFF;
        /* fallthrough */
    case 4:
        temp_v0 = func_800348A0(self, 0, 0);
        if (temp_v0 != NULL) {
            temp_v0->frame = 0x100;
        }
        break;
    }
}

// offset: 0xF38 | func: 2 | export: 2
void dll_782_update(Object* self) { }

// offset: 0xF44 | func: 3 | export: 3
void dll_782_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF98 | func: 4 | export: 4
void dll_782_free(Object* self, s32 arg1) { }

// offset: 0xFA8 | func: 5 | export: 5
u32 dll_782_get_model_flags(Object* self) {
    WCPushBlock_Setup* setup;
    s8 modelIndex;

    setup = (WCPushBlock_Setup*)self->setup;
    modelIndex = setup->modelIndex;
    if (modelIndex >= self->def->numModels) {
        modelIndex = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0xFDC | func: 6 | export: 6
u32 dll_782_get_data_size(Object* self, s32 arg1) {
    return sizeof(WCPushBlock_Data);
}

// offset: 0xFF0 | func: 7
static s32 dll_782_func_FF0(Object* self, WCPushBlock_Data* objdata, Object* player) {
    f32 positionX;
    f32 positionZ;
    f32 min;
    f32 max;
    f32* time;
    u8 isNighttime;

    isNighttime = gDLL_7_Newday->vtbl->func8((f32*)&time);

    if (self->modelInstIdx == 1) {
        //Sun block?
        if (isNighttime) {
            return 0;
        }
        ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func11(objdata->unk276, &objdata->unk270, &objdata->unk272);
        ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func7(&self->srt, objdata->unk270, objdata->unk272, &positionX, &positionZ);
    } else {
        //Moon block?
        if (!isNighttime) {
            return 0;
        }

        ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func18(objdata->unk276, &objdata->unk270, &objdata->unk272);
        ((DLL_779_WCLevelControl*)(objdata->levelCtrl)->dll)->vtbl->func14(&self->srt, objdata->unk270, objdata->unk272, &positionX, &positionZ);
    }

    //Check if player out of range
    max = positionX + 56.0f;
    min = positionX - 56.0f;
    if (max < player->srt.transl.x || player->srt.transl.x < min) {
        return 1;
    }
    max = positionZ + 56.0f;
    min = positionZ - 56.0f;
    if (positionZ + 56.0f < player->srt.transl.z || player->srt.transl.z < positionZ - 56.0f) {
        return 1;
    }

    return 0;
}

