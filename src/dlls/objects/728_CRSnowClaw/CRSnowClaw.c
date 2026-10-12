#include "dlls/engine/33_BaddieControl.h"
#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "dlls/objects/common/vehicle.h"
#include "dlls/objects/730_CRSpellstone.h"
#include "sys/rand.h"
#include "dll.h"

/** @file official name: CRsnowclaw.c */

typedef struct {
    Object* vehicle;
    s16* bits;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 lowerTorsoX;
    f32 lowerTorsoY;
    f32 lowerTorsoZ;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 opacity;
    s8 unk31;
    s8 unk32;
    u8 unk33;
    s8 health;
    s8 impactDebounce;
} CRSnowClaw_Data;

/*0x0*/ static s16 data_0[][2] = {
    {BIT_CF_Race_Started, BIT_Played_Seq_0205_CF_Race_Scales_Intro}, 
    {BIT_875,             BIT_Played_Seq_0205_CF_Race_Scales_Intro}
};

static int CRSnowClaw_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);
static void CRSnowClaw_printVehicle(Object* self, Object* vehicle, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility, s32 opacity, s32 doPrint);
static void CRSnowClaw_vehicleRiderAnim(Object* self, Object* vehicle);
static void CRSnowClaw_func_EC8(Object* self, CRSnowClaw_Data* objdata);

// offset: 0x0 | ctor
void CRSnowClaw_ctor(void* dll) { }

// offset: 0xC | dtor
void CRSnowClaw_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CRSnowClaw_obj_Setup(Object* self, Baddie_Setup* setup, s32 reset) {
    CRSnowClaw_Data* objdata;

    self->animCallback = CRSnowClaw_animCallback;
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_4000;
        self->shadow->distFadeMaxOpacity = 100;
        self->shadow->distFadeMinOpacity = 150;
    }
    objdata = self->data;
    objdata->vehicle = NULL;
    objdata->unk32 = setup->initialWeaponID;
    objdata->health = 4;
    objdata->impactDebounce = -1;
    objdata->bits = data_0[setup->unk26];
}

// offset: 0xA0 | func: 1 | export: 1
void CRSnowClaw_obj_Control(Object* self) {
    CRSnowClaw_Data* objdata = self->data;
    Object* vehicle;
    Object** vehicles;
    s32 numVehicles;
    f32 var_fv1;
    s32 targetVehicleID;
    s32 i;
    
    objdata->unk31 = 0;
    objdata->opacity = 255;
    if (objdata->health < 0) {
        objdata->health--;
        if (objdata->health < -2) {
            objDisable(self);
            self->srt.flags |= OBJFLAG_INVISIBLE;
            objDisable(objdata->vehicle);
            objdata->vehicle->srt.flags |= OBJFLAG_INVISIBLE;
            return;
        }
        objdata->health--;
        return;
    }
    CRSnowClaw_func_EC8(self, objdata);
    if (objdata->vehicle == NULL) {
        vehicles = objGetAllOfType(OBJTYPE_Vehicle, &numVehicles);
        if (vehicles == NULL) {
            STUBBED_PRINTF(" Objlist is NULL");
        }
        switch (self->id) {
        case OBJ_CRSnowClaw:
            targetVehicleID = OBJ_CRSnowClawBike;
            break;
        case OBJ_CRSnowClaw2:
            targetVehicleID = OBJ_CRSnowClawBike2;
            break;
        case OBJ_CRSnowClaw3:
            targetVehicleID = OBJ_CRSnowClawBike3;
            break;
        default:
            STUBBED_PRINTF(" Not A Bike Riding Object ");
            break;
        }
        for (i = 0; i < numVehicles; i++) {
            vehicle = vehicles[i];
            if (targetVehicleID == vehicle->id) {
                objdata->vehicle = vehicle;
                i = numVehicles;
            }
        }
    }
    if (mainGetBits(objdata->bits[0]) != 0) {
        if (objdata->vehicle != NULL) {
            CRSnowClaw_vehicleRiderAnim(self, objdata->vehicle);
        }
        if (objdata->vehicle != NULL) {
            var_fv1 = (vec3Distance(&objdata->vehicle->globalPosition, &objGetPlayer()->globalPosition) - 600.0f) / 50.0f;
            if (var_fv1 < 0.0f) {
                var_fv1 = 0.0f;
            } else if (var_fv1 > 1.0f) {
                var_fv1 = 1.0f;
            }
            var_fv1 = 1.0f - var_fv1;
            objdata->opacity = var_fv1 * 255.0f;
            if (self->shadow != NULL) {
                self->shadow->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
            }
        } else {
            objdata->opacity = 255;
            if (self->shadow != NULL) {
                self->shadow->flags &= ~OBJ_SHADOW_FLAG_FADE_OUT;
            }
        }
        objdata->opacity = 255;
    }
}

// offset: 0x3CC | func: 2 | export: 2
void CRSnowClaw_obj_Update(Object* self) {
    CRSnowClaw_Data* objdata = self->data;
    s32 _pad;
    s32 _pad2;
    f32 spellstoneDist;
    Object* hitBy;
    Object* spellstone;

    spellstoneDist = 500.0f;
    if ((func_80025F40(self, &hitBy, NULL, NULL) == Damage_Type_Speeding_Vehicle) && (objdata->impactDebounce < 0)) {
        objdata->health--;
        dll_amSfx->Play(self, SOUND_292_Impact, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->Play(self, SOUND_23A_SharpClaw_Ugh_Snort, MAX_VOLUME, NULL, NULL, 0, NULL);
        objdata->impactDebounce = 20;
        if (objdata->health < 0) {
            func_800267A4(self);
            if (objdata->vehicle != NULL) {
                ((DLL_IVehicle*)objdata->vehicle->dll)->vtbl->SetMountState(objdata->vehicle, VEHICLE_NoRider);
            }
            if (self->id == OBJ_CRSnowClaw) {
                spellstone = objGetNearestTypeTo(OBJTYPE_32, self, &spellstoneDist);
                if (spellstone != NULL) {
                    ((DLL_730_CRSpellstone*)spellstone->dll)->vtbl->SetState(spellstone, 2);
                }
            }
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, 3);
            objdata->unk24 = mathSinfInterp(self->srt.yaw) * 0.1f;
            objdata->unk28 = mathRnd(40, 100) * 0.01f;
            objdata->unk2C = mathCosfInterp(self->srt.yaw) * 0.1f;
        }
    }
    if ((objdata->vehicle != NULL) 
            && (((DLL_IVehicle*)objdata->vehicle->dll)->vtbl->GetMountState(objdata->vehicle) == VEHICLE_Mounted)) {
        CRSnowClaw_printVehicle(self, objdata->vehicle, NULL, NULL, NULL, NULL, 0, 0, FALSE);
    }
    if (objdata->impactDebounce >= 0) {
        objdata->impactDebounce -= gUpdateRate;
    }
}

// offset: 0x674 | func: 3 | export: 3
void CRSnowClaw_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CRSnowClaw_Data* objdata;
    Object* vehicle;
    Object* spellstone;
    s32 ridingVehicle;
    s32 prevOpacity;
    f32 spellstoneDist;

    ridingVehicle = FALSE;
    spellstoneDist = 5000.0f;
    objdata = self->data;
    vehicle = objdata->vehicle;
    if ((objdata->health >= 0) && (vehicle != NULL)) {
        if (((DLL_IVehicle*)vehicle->dll)->vtbl->GetMountState(vehicle) == VEHICLE_Mounted) {
            ridingVehicle = TRUE;
        }
    }
    if (ridingVehicle) {
        self->srt.flags |= OBJFLAG_MANUAL_PREV_POSITIONS;
        visibility = trackObjVisCheck(vehicle);
        CRSnowClaw_printVehicle(self, vehicle, gdl, mtxs, vtxs, pols, visibility, objdata->opacity, TRUE);
    } else {
        self->srt.flags &= ~OBJFLAG_MANUAL_PREV_POSITIONS;
    }
    self->linkedObject = NULL;
    if (visibility != 0 && objdata->opacity > 0) {
        prevOpacity = self->opacityWithFade;
        if (ridingVehicle) {
            self->opacityWithFade = objdata->opacity;
        }
        if ((self->id == OBJ_CRSnowClaw) && (mainGetBits(objdata->bits[0]) != 0)) {
            spellstone = objGetNearestTypeTo(OBJTYPE_32, self, &spellstoneDist);
            if (spellstone != NULL) {
                if ((((DLL_730_CRSpellstone*)spellstone->dll)->vtbl->IsNotRescued(spellstone) != 0) 
                        && (((DLL_730_CRSpellstone*)spellstone->dll)->vtbl->SetState(spellstone, 0) != 0)) {
                    self->linkedObject = spellstone;
                }
            }
        }
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        objGetAttachPointWorldSpace(self, 1, &objdata->lowerTorsoX, &objdata->lowerTorsoY, &objdata->lowerTorsoZ, 0);
        self->opacityWithFade = prevOpacity;
    }
}

// offset: 0x8C4 | func: 4 | export: 4
void CRSnowClaw_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x8D4 | func: 5 | export: 5
u32 CRSnowClaw_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0x8E4 | func: 6 | export: 6
u32 CRSnowClaw_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CRSnowClaw_Data);
}

// offset: 0x8F8 | func: 7
static int CRSnowClaw_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    CRSnowClaw_Data* objdata;
    Object* spellstone;
    Object* vehicle;
    s32 i;
    f32 spellstoneDist;

    spellstoneDist = 5000.0f;
    objdata = self->data;
    objdata->unk31 = 1;
    vehicle = objdata->vehicle;
    if (animObjData->state == ANIMOBJ_STATE_Ready) {
        animObjData->unk9D |= AnimData_FLAG_8; // clear event flag
    }
    animObjData->unk7A = animObjData->unk7C;
    
    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
        case 3:
            objdata->unk32 = -1;
            break;
        case 4:
            if (mainGetBits(objdata->bits[1]) != 0) {
                animObjData->unk9D |= AnimData_FLAG_4; // set event flag
            }
            break;
        case 5:
            if (mainGetBits(objdata->bits[0]) != 0) {
                animObjData->unk9D |= AnimData_FLAG_4; // set event flag
            }
            break;
        case 2:
            STUBBED_PRINTF(" GETON GETON GETON \n ");
            if (vehicle != NULL) {
                objdata->unk8 = 1.0f;
                objdata->unkC = objdata->lowerTorsoX;
                objdata->unk10 = objdata->lowerTorsoY;
                objdata->unk14 = objdata->lowerTorsoZ;
                ((DLL_IVehicle*)vehicle->dll)->vtbl->SetMountState(vehicle, VEHICLE_Mounted);
                objAnimSet(self, 0x1A, 0.0f, 1);
                if (self->shadow != NULL) {
                    self->shadow->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
                }
                animObjData->unk7A &= ~ANIM7AFLAG_OVERRIDE_MODEL;
            }
            break;
        case 1:
            STUBBED_PRINTF(" GETOFF LET A WOMAN \n ");
            vehicle = objdata->vehicle;
            if (vehicle != NULL) {
                ((DLL_IVehicle*)vehicle->dll)->vtbl->SetMountState(vehicle, VEHICLE_NoRider);
            }
            break;
        case 6:
            spellstone = objGetNearestTypeTo(OBJTYPE_32, self, &spellstoneDist);
            if (spellstone != NULL) {
                ((DLL_730_CRSpellstone*)spellstone->dll)->vtbl->SetState(spellstone, 2);
            }
            break;
        case 7:
            spellstone = objGetNearestTypeTo(OBJTYPE_32, self, &spellstoneDist);
            if (spellstone != NULL) {
                ((DLL_730_CRSpellstone*)spellstone->dll)->vtbl->SetState(spellstone, 0);
            }
            break;
        }
        animObjData->messages[i] = 0;
    }
    CRSnowClaw_func_EC8(self, objdata);
    if (vehicle != NULL) {
        if (((DLL_IVehicle*)vehicle->dll)->vtbl->GetMountState(vehicle) == VEHICLE_Mounted) {
            animObjData->unk7A &= ~(ANIM7AFLAG_OVERRIDE_POS | ANIM7AFLAG_OVERRIDE_ROT);
        }
    }
    return 0;
}

// offset: 0xBF0 | func: 8
static void CRSnowClaw_printVehicle(Object* self, Object* vehicle, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility, s32 opacity, s32 doPrint) {
    s32 prevOpacity;
    f32 x;
    f32 y;
    f32 z;

    if ((doPrint != 0) && (visibility != 0) && (opacity > 0)) {
        prevOpacity = vehicle->opacityWithFade;
        vehicle->opacityWithFade = opacity;
        vehicle->dll->vtbl->Print(vehicle, gdl, mtxs, vtxs, pols, -1);
        vehicle->opacityWithFade = prevOpacity;
    }
    self->prevGlobalPosition.x = self->globalPosition.x;
    self->prevGlobalPosition.y = self->globalPosition.y;
    self->prevGlobalPosition.z = self->globalPosition.z;
    self->prevLocalPosition.x = self->srt.transl.x;
    self->prevLocalPosition.y = self->srt.transl.y;
    self->prevLocalPosition.z = self->srt.transl.z;
    ((DLL_IVehicle*)vehicle->dll)->vtbl->GetRiderPosition(vehicle, &x, &y, &z);
    self->srt.transl.x = x;
    self->srt.transl.y = y;
    self->srt.transl.z = z;
    self->srt.yaw = vehicle->srt.yaw;
    self->srt.pitch = vehicle->srt.pitch;
    self->srt.roll = vehicle->srt.roll;
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
    self->velocity.x = vehicle->velocity.x;
    self->velocity.y = vehicle->velocity.y;
    self->velocity.z = vehicle->velocity.z;
}

// offset: 0xD58 | func: 9
static void CRSnowClaw_vehicleRiderAnim(Object* self, Object* vehicle) {
    f32 tValue;
    s32 blend;
    f32 animDelta;
    s32 _pad;
    s32 animIndex;

    if (self->curModAnimId != 0x1A) {
        objAnimSet(self, 0x1A, 0.0f, 0);
    }
    animDelta = (((DLL_IVehicle*)vehicle->dll)->vtbl->Func16(vehicle, &animDelta) * 0.05f) + 0.01f;
    ((DLL_IVehicle*)vehicle->dll)->vtbl->GetPlayerAnim(vehicle, &tValue, &animIndex);
    blend = (s32) (tValue * 1023.0f);
    if (blend < 0) {
        blend = -blend;
    }
    if (animIndex != 0) {
        objAnimSetBlend(self, 0x1B, blend);
    } else {
        objAnimSetBlend(self, 0x1C, blend);
    }
    objAnimAdvance(self, animDelta, gUpdateRate, NULL);
}

// offset: 0xEC8 | func: 10
static void CRSnowClaw_func_EC8(Object* arg0, CRSnowClaw_Data* arg1) {
    s16 sp4[] = {0x0023, 0x0069, 0x0033, 0x0064, 0x001d};

    // yea
    if (1) {
        return;
    }
}
