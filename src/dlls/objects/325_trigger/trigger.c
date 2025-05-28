#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "PR/gu.h"
#include "dlls/objects/210_player.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/asset_thread.h"
#include "sys/camera.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/gfx/gx.h"
#include "dll.h"
#include "types.h"
#include "functions.h"
#include "prevent_bss_reordering.h"

static const char str_1[] = "triggers: unknown trigger object %d.\n";
static const char str_2[] = "TRIGGER: warning Script overflow\n";
static const char str_3[] = "Trigger [%d], Gamplay Vulnerable";
static const char str_4[] = "Trigger [%d], Music Action,       Action Num [%d] Free";
static const char str_5[] = "Trigger [%d], Music Action,       Action Num [%d] Set";
static const char str_6[] = "Trigger [%d], Sound FX,           Action Num [%d],Handle Num [%d]";
static const char str_7[] = "Trigger [%d], Camera,             Action [%d], Camera Num [%d], PassDir [%d]";
static const char str_8[] = "Trigger [%d], Track Sky On";
static const char str_9[] = "Trigger [%d], Track Sky Off";
static const char str_10[] = "Trigger [%d], Track AntiAlias On";
static const char str_11[] = "Trigger [%d], Track AntiAlias Off";
static const char str_12[] = "Trigger [%d], Track SkyObjects On";
static const char str_13[] = "Trigger [%d], Track SkyObjects Off";
static const char str_14[] = "Trigger [%d], Track Dome On";
static const char str_15[] = "Trigger [%d], Track Dome Off";
static const char str_16[] = "Trigger [%d], Track MrSheen On %d";
static const char str_17[] = "Trigger [%d], Track MrSheen Off";
static const char str_18[] = "Trigger [%d], Environment Effect, Action Num [%d], Range [%d]";
static const char str_19[] = "Trigger [%d], Lighting,           Action      [%d], Range [%d], PassDir [%d]";
static const char str_20[] = "Trigger [%d], Anim Sequence,      SequenceID [%d], Activate";
static const char str_21[] = "Trigger [%d], Anim Sequence,      SequenceID [%d], Flag = 1";
static const char str_22[] = "Trigger [%d], Anim Sequence,      SequenceID [%d], Flag = 0";
static const char str_23[] = "Trigger [%d], Trigger,            Local ID   [%d]";
static const char str_24[] = "Storyboard disabled, please remove trigger\n";
static const char str_25[] = "Trigger [%d], LOD Model [%d]";
static const char str_26[] = "Trigger [%d], Setup Point,        Level      [%d], SetupPoint [%d]";
static const char str_27[] = "Trigger [%d], Bits\n";
static const char str_28[] = "Trigger [%d], Object Load\n";
static const char str_29[] = "Trigger [%d], Object Free\n";
static const char str_30[] = "Trigger [%d], Object Toggle\n";
static const char str_31[] = "Trigger [%d], Tex Load\n";
static const char str_32[] = "Trigger [%d], Tex Free\n";
static const char str_33[] = "TRIGGER: warning DLL not loaded\n";
static const char str_34[] = "Script [%d], Subscript [%d]\n";
static const char str_35[] = "Trigger [%d], Object Load\n";
static const char str_36[] = "Trigger [%d], Object Free\n";
static const char str_37[] = "Restart Set [%d]\n";
static const char str_38[] = "Restart Clear [%d]\n";
static const char str_39[] = "Restart Goto [%d]\n";
static const char str_40[] = "killing sidekick\n";
static const char str_41[] = "findobj %i \n";
static const char str_42[] = "Trigger [%d], amSfxWaterFallsSetFlags,   Action [%d], PassDir [%d]";

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} TriggerCreateInfo_18;

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ TriggerCreateInfo_18 unk18[8];
/*38*/ s16 unk38;
/*3A*/ u8 unk3A;
/*3B*/ u8 unk3B;
/*3C*/ u8 unk3C;
/*3D*/ u8 unk3D;
/*3E*/ u8 unk3E;
/*3F*/ u8 _unk3F[4];
/*43*/ u8 unk43;
/*44*/ s16 bitstringID;
/*46*/ u16 unk46;
/*48*/ s16 unk48;
/*4A*/ s16 unk4A;
/*4C*/ s16 unk4C;
/*4E*/ s16 unk4E;
} TriggerCreateInfo;

typedef struct DLLInst_TriggerScript DLLInst_TriggerScript;
struct DLLInst_TriggerScript {
  struct {
    unsigned long _pad;
    /*0*/ void (*subscripts[1])(Object*,Object*,s8,s32);
    } *exports;
};

typedef struct {
/*00*/ u8 unk0;
/*01*/ u8 _unk1[3];
/*04*/ f32 unk4;
/*08*/ u8 _unk8[4];
/*0C*/ s32 unkC;
/*10*/ Vec3f unk10;
/*1C*/ f32 unk1C;
/*20*/ Vec3f unk20;
/*2C*/ Vec3f unk2C;
/*38*/ f32 unk38;
/*3C*/ f32 unk3C;
/*40*/ f32 unk40;
/*44*/ f32 unk44;
/*48*/ f32 unk48;
/*4C*/ f32 unk4C;
/*50*/ u8 _unk50[8];
/*58*/ s16 bitstringID;
/*5A*/ s16 bitstringIDList[4];
/*62*/ u8 _unk62[2];
/*64*/ s32 unk64[8];
/*84*/ DLLInst_TriggerScript *dlls[8];
} TriggerState;

static ModelInstance *sPointModel = NULL;
static ModelInstance *sPlaneModel = NULL;

static s16 sPointModelRefCount;
static s16 sPlaneModelRefCount;

/*static*/ void trigger_update_effects(Object *self, Object *param2, s8 param3, s32 param4);
/*static*/ void trigger_func_1754(u8 param1, u8 param2);
/*static*/ void trigger_func_1764(u16 param1);
/*static*/ void trigger_func_17FC(u16 param1);
/*static*/ void trigger_func_1868(u16 param1);
/*static*/ void trigger_func_1920(u16 param1);

static void trigger_point_setup(Object *obj, TriggerCreateInfo *createInfo);
/*static*/ void trigger_test_point(Object *self, Object *param2);

static void trigger_cylinder_setup(Object *obj, TriggerCreateInfo *createInfo);
/*static*/ void trigger_test_cylinder(Object* self, Object* param2);

static void trigger_plane_setup(Object *obj, TriggerCreateInfo *createInfo);
/*static*/ void trigger_test_plane(Object* self, Object* param2);

static void trigger_area_setup(Object *obj, TriggerCreateInfo *createInfo);
/*static*/ void trigger_test_area(Object* arg0, Object* arg1);
/*static*/ s32 trigger_func_273C(Object *obj, Vec3f *vec);
static void trigger_func_2884(Object *obj, f32 *ox, f32 *oy, f32 *oz);
/*static*/ void trigger_func_29C0(u16 arg0, Object* arg1, s8 arg2, s32 arg3);

static void trigger_curve_setup(Object *obj, TriggerCreateInfo *createInfo);
/*static*/ void trigger_test_curve(Object* self, Object* param2);

void trigger_ctor(void *dll) { }

void trigger_dtor(void *dll) { }

void trigger_create(Object *self, TriggerCreateInfo *createInfo, s32 param3) {
    TriggerState *state;
    s32 s0;
    s32 i;
    TriggerCreateInfo_18 *temp;

    obj_add_object_type(self, OBJTYPE_2);
    obj_set_update_priority(self, 40);

    state = (TriggerState*)self->state;

    switch (createInfo->base.objId) {
        case OBJ_TriggerPoint:
            trigger_point_setup(self, createInfo);
            break;
        case OBJ_TriggerPlane:
            if ((createInfo && createInfo) && createInfo){} // fakematch

            state->bitstringIDList[0] = createInfo->unk48;
            trigger_plane_setup(self, createInfo);
            break;
        case OBJ_TriggerCylinder:
            trigger_cylinder_setup(self, createInfo);
            break;
        case OBJ_TriggerArea:
            trigger_area_setup(self, createInfo);
            break;
        case OBJ_TriggerBits:
            state->bitstringIDList[0] = createInfo->unk48;
            state->bitstringIDList[1] = createInfo->unk4A;
            state->bitstringIDList[2] = createInfo->unk4C;
            state->bitstringIDList[3] = createInfo->unk4E;
            break;
        case OBJ_TriggerCurve:
            trigger_curve_setup(self, createInfo);
            break;
        case OBJ_TriggerTime:
        case OBJ_TriggerButton:
        case OBJ_TriggerSetup:
            break;
        default:
            break;
    }

    state->bitstringID = createInfo->bitstringID;
    s0 = get_gplay_bitstring(state->bitstringID);

    temp = createInfo->unk18;

    for (i = 0; i < 8; i++, temp++) {
        state->unk64[i] = 0;
        state->dlls[i] = NULL;

        if (temp->unk1 == 25) {
            s0 = temp->unk2 + 81;
            if (s0 < 84 && param3 == 0) {
                state->dlls[i] = dll_load_deferred(s0, 0);
            }
        }
    }

    if (s0 == 1) {
        state->unk0 |= 4;
    }
    state->unk0 |= 0x40;
}

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_update.s")
#else
void trigger_update(Object* self) {
    TriggerState* state;
    TriggerCreateInfo* createInfo;
    Object* player;
    Object* temp_v0_2;
    Object* sidekick;
    Object* referenceObj;
    s32 i;
    s32 b_allBitsSet;
    s32 b_hasReferenceObj;
    f32 maxObjSearchDist;

    state = (TriggerState*)self->state;
    createInfo = (TriggerCreateInfo*)self->createInfo;
    
    maxObjSearchDist = 200.0f;
   
    player = get_player();
    if (player != NULL) {
        temp_v0_2 = ((DLLInst_210_Player*)player->dll)->exports->func7(player);
        if (temp_v0_2 != NULL) {
            player = temp_v0_2;
        }
    }
    
    sidekick = get_sidekick();
    
    if ((player != NULL) || (sidekick != NULL)) {
        if (state->unk0 & 4) {
            trigger_update_effects(self, player, 1, 0);
            state->unk0 &= ~4;
            state->unk0 |= 1;
            return;
        }
        
        b_hasReferenceObj = TRUE;
        if (createInfo->unk43 >= 3) {
            referenceObj = obj_get_nearest_type_to(createInfo->unk43, self, &maxObjSearchDist);
            if (referenceObj == NULL) {
                b_hasReferenceObj = FALSE;
            }
        } else {
            switch (createInfo->unk43) {
            case 0:
                referenceObj = player;
                if (player == NULL) {
                    b_hasReferenceObj = FALSE;
                }
                break;
            case 1:
                referenceObj = sidekick;
                if (sidekick == NULL) {
                    b_hasReferenceObj = FALSE;
                }
                break;
            case 2:
                referenceObj = gDLL_Camera->exports->func2();
                break;
            }
        }
        
        if (b_hasReferenceObj) {
            if (state->unk0 & 0x40) {
                switch (createInfo->unk43) {
                default:
                    state->unk20.x = referenceObj->positionMirror2.x;
                    state->unk20.y = referenceObj->positionMirror2.y;
                    state->unk20.z = referenceObj->positionMirror2.z;
                    break;
                case 2:
                    state->unk20.x = referenceObj->srt.transl.x;
                    state->unk20.y = referenceObj->srt.transl.y;
                    state->unk20.z = referenceObj->srt.transl.z;
                    break;
                case 0:
                case 1:
                    state->unk20.x = referenceObj->positionMirror3.x;
                    state->unk20.y = referenceObj->positionMirror3.y;
                    state->unk20.z = referenceObj->positionMirror3.z;
                    break;
                }
                
                state->unk0 &= ~0x40;
            } else {
                state->unk20.x = state->unk2C.x;
                state->unk20.y = state->unk2C.y;
                state->unk20.z = state->unk2C.z;
            }
            
            switch (createInfo->unk43) {
            case 0:
            case 1:
                state->unk2C.x = referenceObj->positionMirror.x;
                state->unk2C.y = referenceObj->positionMirror.y;
                state->unk2C.z = referenceObj->positionMirror.z;
                break;
            default:
            case 2:
                state->unk2C.x = referenceObj->srt.transl.x;
                state->unk2C.y = referenceObj->srt.transl.y;
                state->unk2C.z = referenceObj->srt.transl.z;
                break;
            }
        }
        
        switch (createInfo->base.objId) {
        case OBJ_TriggerPoint:
            if (b_hasReferenceObj) {
                trigger_test_point(self, referenceObj);
            }
            break;
        case OBJ_TriggerCylinder:
            if (b_hasReferenceObj) {
                trigger_test_cylinder(self, referenceObj);
            }
            break;
        case OBJ_TriggerPlane:
            b_allBitsSet = TRUE;
            if (state->bitstringIDList[0] >= 0) {
                if (get_gplay_bitstring(state->bitstringIDList[0]) == 0) {
                    b_allBitsSet = FALSE;
                }
            }
            if (b_allBitsSet && b_hasReferenceObj) {
                trigger_test_plane(self, referenceObj);
            }
            break;
        case OBJ_TriggerTime:
            state->unkC += delayByte;
            if (state->unkC >= createInfo->unk46) {
                trigger_update_effects(self, NULL, 1, 0);
            }
            break;
        case OBJ_TriggerArea:
            if (b_hasReferenceObj) {
                trigger_test_area(self, referenceObj);
            }
            break;
        case OBJ_TriggerSetup:
            trigger_update_effects(self, player, 1, 0);
            if (ret1_8001454c() != 0) {
                obj_destroy_object(self);
            }
            break;
        case OBJ_TriggerBits:
            b_allBitsSet = TRUE;
            for (i = 0; i < 4 && b_allBitsSet; i++) {
                if (state->bitstringIDList[i] >= 0) {
                    if (get_gplay_bitstring(state->bitstringIDList[i]) == 0) {
                        b_allBitsSet = FALSE;
                    }
                }
            }
            if (b_allBitsSet) {
                trigger_update_effects(self, player, 1, 0);
            }
            break;
        case OBJ_TriggerCurve:
            if (b_hasReferenceObj) {
                trigger_test_curve(self, referenceObj);
            }
            break;
        }
    }
}
#endif

void trigger_func_7D4(Object *self) { }

void trigger_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

void trigger_destroy(Object *self, s32 param2) {
    TriggerCreateInfo *createInfo;
    TriggerState *state;
    u8 i;

    createInfo = (TriggerCreateInfo*)self->createInfo;
    state = (TriggerState*)self->state;

    for (i = 0; i < 8; i++) {
        if (state->unk64[i] != 0) {
            gDLL_AMSFX->exports->func6(state->unk64[i]);
        }
        if (state->dlls[i] != NULL) {
            dll_unload(state->dlls[i]);
        }

        state->unk64[i] = 0;
        state->dlls[i] = NULL;
    }

    switch (createInfo->base.objId) {
        case OBJ_TriggerPoint:
        case OBJ_TriggerButton: // ??? is this a bug?
            if (sPointModel != NULL) {
                sPointModelRefCount -= 1;
                if (sPointModelRefCount == 0) {
                    destroy_model_instance(sPointModel);
                    sPointModel = NULL;
                }
            }
            break;
        case OBJ_TriggerPlane:
            if (sPlaneModel != NULL) {
                sPlaneModelRefCount -= 1;
                if (sPlaneModelRefCount == 0) {
                    destroy_model_instance(sPlaneModel);
                    sPlaneModel = NULL;
                }
            }
            break;
    }

    obj_free_object_type(self, OBJTYPE_2);
}

u32 trigger_func_990(Object *self) {
    return 0;
}

u32 trigger_get_state_size(Object *self, u32 param2) {
    return sizeof(TriggerState);
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_update_effects.s")
#else
void trigger_update_effects(Object* self, Object* param2, s8 param3, s32 param4) {
    TriggerCreateInfo* createInfo; // sp+74
    TriggerState* state; // sp+70
    Object* player;
    u8 i;
    Object* var_v0_2;
    TriggerCreateInfo_18 *unk18Ptr;
    Object* sidekick;
    s32 temp_a1;

    state = (TriggerState*)self->state;
    createInfo = (TriggerCreateInfo*)self->createInfo;
    
    for (i = 0, unk18Ptr = createInfo->unk18; i < 8; i++, unk18Ptr++) {
        if (unk18Ptr->unk1 != 0) {
            if ((state->unk0 & 4) && !(unk18Ptr->unk0 & 0x20)) {
                continue;
            }
            
            if (!(unk18Ptr->unk0 & 0x10)) {
                if (param3 == 1) {
                    if (unk18Ptr->unk0 & 1) {
                        if ((state->unk0 & 1) && !(unk18Ptr->unk0 & 4)) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } else if ((param3 == -1) && (unk18Ptr->unk0 & 2)) {
                    if ((state->unk0 & 2) && !(unk18Ptr->unk0 & 8)) {
                        continue;
                    }
                } else {
                    continue;
                }
            } else if (unk18Ptr->unk0 & 1) {
                if (param3 < 0) {
                    continue;
                }
            } else if ((unk18Ptr->unk0 & 2) && (param3 > 0)) {
                continue;
            }
            
            switch (unk18Ptr->unk1) {                  /* switch 1 */
            case 1:                             /* switch 1 */
                switch (unk18Ptr->unk2) {              /* switch 2 */
                case 0:                         /* switch 2 */
                case 1:                         /* switch 2 */
                case 2:                         /* switch 2 */
                case 3:                         /* switch 2 */
                case 4:                         /* switch 2 */
                case 5:                         /* switch 2 */
                case 6:                         /* switch 2 */
                    break;
                case 7: {                         /* switch 2 */
                        s32 var_v0; // sp+50
                        if (param3 == 1) {
                            var_v0 = 0x80;
                        } else {
                            var_v0 = 0x81;
                        }
                        func_80031864(0x365, 6000.0f, 0, self, var_v0, 0);
                        func_80031864(0x4CA, 6000.0f, 0, self, var_v0, 0);
                    }
                    break;
                case 8: {                        /* switch 2 */
                        Object *player = get_player();
                        if (player != NULL) {
                            ((DLLInst_210_Player*)player->dll)->exports->func67(player, 9, 0.0f);
                        }
                    }
                    break;
                case 9: {                        /* switch 2 */
                        Object *player = get_player();
                        if (player != NULL) {
                            ((DLLInst_210_Player*)player->dll)->exports->func67(player, 10, 0.0f);
                        }
                    }
                    break;
                case 10: {                       /* switch 2 */
                        Object *player = get_player();
                        if (player != NULL) {
                            ((DLLInst_210_Player*)player->dll)->exports->func67(player, 11, 0.0f);
                        }
                    }
                    break;
                }
                break;
            case 3:                             /* switch 1 */
                if ((param3 < 0) && (gDLL_AMSEQ->exports->func2(self, (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8))) != 0)) {
                    gDLL_AMSEQ2->exports->func1(self, (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)), 0, 0, 0);
                } else {
                    gDLL_AMSEQ2->exports->func0(self, (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)), 0, 0, 0);
                }
                break;
            case 4:                             /* switch 1 */
                if (param3 >= 0) {
                    gDLL_AMSFX->exports->func14(self, (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)), &state->unk64[i]);
                } else {
                    if (state->unk64[i] != 0) {
                        gDLL_AMSFX->exports->func6(state->unk64[i]);
                        state->unk64[i] = 0;
                    }
                }
                break;
            case 6:                             /* switch 1 */
                gDLL_Camera->exports->func8(unk18Ptr->unk2, unk18Ptr->unk3);
                break;
            case 8:                             /* switch 1 */
                switch (unk18Ptr->unk2) {              /* switch 3 */
                case 0:                         /* switch 3 */
                    if ((s32) unk18Ptr->unk3 >= 2) {
                        unk18Ptr->unk3 = 1;
                    }
                    func_80041C6C(unk18Ptr->unk3);
                    if (unk18Ptr->unk3 != 0) {

                    }
                    break;
                case 1:                         /* switch 3 */
                    if ((s32) unk18Ptr->unk3 >= 2) {
                        unk18Ptr->unk3 = 1;
                    }
                    func_80041CA8((s32) unk18Ptr->unk3);
                    if (unk18Ptr->unk3 != 0) {

                    }
                    break;
                case 2:                         /* switch 3 */
                    if ((s32) unk18Ptr->unk3 >= 2) {
                        unk18Ptr->unk3 = 1;
                    }
                    func_80041CE4((s32) unk18Ptr->unk3);
                    if (unk18Ptr->unk3 != 0) {

                    }
                    break;
                case 3:                         /* switch 3 */
                    if ((s32) unk18Ptr->unk3 >= 2) {
                        unk18Ptr->unk3 = 1;
                    }
                    gDLL_minic->exports->func6(unk18Ptr->unk3);
                    if (unk18Ptr->unk3 != 0) {

                    }
                    break;
                case 4:                         /* switch 3 */
                    gDLL_16->exports->func2(unk18Ptr->unk3);
                    if (unk18Ptr->unk3 != 0) {

                    }
                    break;
                case 5:                         /* switch 3 */
                    func_8005CA5C((u32) unk18Ptr->unk3);
                    break;
                case 6:                         /* switch 3 */
                    if ((s32) unk18Ptr->unk3 > 0) {
                        func_8001EBD0(1);
                    } else {
                        func_8001EBD0(0);
                    }
                    break;
                case 7:                         /* switch 3 */
                    if ((s32) unk18Ptr->unk3 > 0) {
                        func_80041E24(1);
                    } else {
                        func_80041E24(0);
                    }
                    break;
                }
                break;
            case 5:                             /* switch 1 */
                if ((state->unk4 != 0.0f) && (param4 != 0) && (createInfo->base.objId == OBJ_TriggerPlane)) {

                }
                break;
            case 10:                            /* switch 1 */
                func_80000860(self, param2, (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)), param4);
                break;
            case 13:                            /* switch 1 */
                func_80000450(self, param2, (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)), param3, param4, 0);
                break;
            case 11:                            /* switch 1 */
                switch (unk18Ptr->unk2) {
                    case 0:
                    case 3:
                        gDLL_ANIM->exports->func1(unk18Ptr->unk3, 0); // should be 3 params?
                        break;
                    case 1:
                        gDLL_ANIM->exports->func2(unk18Ptr->unk3, 1);
                        break;
                    case 2:
                        gDLL_ANIM->exports->func2(unk18Ptr->unk3, 0);
                        break;
                }
                break;
            case 12:                            /* switch 1 */
                trigger_func_29C0((unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)), param2, param3, param4);
                break;
            case 16:                            /* switch 1 */
                func_80023A18(get_player(), (s32) unk18Ptr->unk2);
                break;
            case 15:                            /* switch 1 */
                trigger_func_1754(unk18Ptr->unk2, unk18Ptr->unk3);
                break;
            case 18:                            /* switch 1 */
                trigger_func_1764((unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)));
                break;
            case 33:                            /* switch 1 */
                trigger_func_17FC((unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)));
                break;
            case 19:                            /* switch 1 */
                gDLL_29_gplay->exports->func_16C4((s32) self->mapID, unk18Ptr->unk3 | (unk18Ptr->unk2 << 8), 1);
                break;
            case 20:                            /* switch 1 */
                gDLL_29_gplay->exports->func_16C4((s32) self->mapID, unk18Ptr->unk3 | (unk18Ptr->unk2 << 8), 0);
                break;
            case 34:                            /* switch 1 */
                temp_a1 = (unk18Ptr->unk3 | (unk18Ptr->unk2 << 8));
                gDLL_29_gplay->exports->func_16C4((s32) self->mapID, temp_a1, gDLL_29_gplay->exports->func_14F0((s32) self->mapID, temp_a1) ^ 1);
                break;
            case 21:                            /* switch 1 */
                trigger_func_1868((unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)));
                break;
            case 22:                            /* switch 1 */
                trigger_func_1920((unk18Ptr->unk3 | (unk18Ptr->unk2 << 8)));
                break;
            case 24:                            /* switch 1 */
                gDLL_29_gplay->exports->func_139C((s32) self->mapID, unk18Ptr->unk3 | (unk18Ptr->unk2 << 8));
                break;
            case 25:                            /* switch 1 */
                if (state->dlls[i] != NULL) {
                    state->dlls[i]->exports->subscripts[unk18Ptr->unk3](self, param2, param3, param4);
                }
                break;
            case 26:                            /* switch 1 */
                gDLL_29_gplay->exports->func_16C4((s32) unk18Ptr->unk3, (s32) unk18Ptr->unk2, 1);
                break;
            case 27:                            /* switch 1 */
                gDLL_29_gplay->exports->func_16C4((s32) unk18Ptr->unk3, (s32) unk18Ptr->unk2, 0);
                break;
            case 28:                            /* switch 1 */
                set_gplay_bitstring(0x46E, unk18Ptr->unk3 | (unk18Ptr->unk2 << 8));
                break;
            case 29:                            /* switch 1 */
                set_gplay_bitstring(0x488, unk18Ptr->unk3 | (unk18Ptr->unk2 << 8));
                break;
            case 30:                            /* switch 1 */
                gDLL_29_gplay->exports->func_139C((s32) unk18Ptr->unk3, (s32) unk18Ptr->unk2);
                break;
            case 17:                            /* switch 1 */
                set_gplay_bitstring(0x4E2, unk18Ptr->unk3 | (unk18Ptr->unk2 << 8));
                break;
            case 31:                            /* switch 1 */
                gDLL_29_gplay->exports->func_958(&self->srt.transl, (s16) ((s16) self->srt.yaw >> 8), (s32) unk18Ptr->unk3, func_80048498());
                break;
            case 32:                            /* switch 1 */
                if (unk18Ptr->unk2 == 0) {
                    func_80047374();
                } else {
                    func_800473BC();
                }
                break;
            case 35:                            /* switch 1 */
                switch (unk18Ptr->unk2) {            /* switch 4; irregular */
                case 0:                         /* switch 4 */
                    gDLL_29_gplay->exports->func_B3C(&self->srt.transl, self->srt.yaw, func_80048498());
                    break;
                case 1:                         /* switch 4 */
                    gDLL_29_gplay->exports->func_D20();
                    break;
                case 2:                         /* switch 4 */
                    gDLL_29_gplay->exports->func_CBC();
                    break;
                }
                break;
            case 38:                            /* switch 1 */
                sidekick = get_sidekick();
                if (sidekick != NULL) {
                    switch (unk18Ptr->unk2) {       /* switch 5; irregular */
                    case 0:                     /* switch 5 */
                        ((DLLInst_Unknown*)sidekick->dll)->exports->func[23].withOneArg(sidekick);
                        break;
                    case 1:                     /* switch 5 */
                        obj_destroy_object(get_sidekick());
                        break;
                    case 2:                     /* switch 5 */
                        var_v0_2 = obj_get_nearest_type_to(0x34, sidekick, NULL);
                        if (var_v0_2 == NULL) {
                            var_v0_2 = obj_get_nearest_type_to(0x33, sidekick, NULL);
                        }
                        if (var_v0_2 != NULL) {
                            ((DLLInst_Unknown*)sidekick->dll)->exports->func[22].withTwoArgs(sidekick, var_v0_2);
                        }
                        break;
                    }
                }
                break;
            case 36:                            /* switch 1 */
            case 37:                            /* switch 1 */
                gDLL_AMSFX->exports->func19(unk18Ptr->unk2);
                break;
            }
        }
    }

    if (param3 > 0) {
        state->unk0 |= 1;
        set_gplay_bitstring(state->bitstringID, 1);
    } else if (param3 < 0) {
        state->unk0 |= 2;
    }
}
#endif

void trigger_func_1754(u8 param1, u8 param2) { }

void trigger_func_1764(u16 param1) {
    s32 _stack_pad[2];
    s32 entry;
    u32 value;

    entry = param1 & 0x3FFF;
    param1 >>= 14;
    value = get_gplay_bitstring(entry);

    if (param1 == 0) {
        value = 0;
    } else if (param1 == 1) {
        value = -1;
    } else if (param1 == 2) {
        value = ~value;
    }

    set_gplay_bitstring(entry, value);
}

void trigger_func_17FC(u16 param1) {
    s32 _stack_pad[2];
    s32 entry;
    u32 value;

    entry = param1 & 0x1FFF;
    param1 >>= 13;

    value = get_gplay_bitstring(entry);
    value ^= (1 << param1);
    set_gplay_bitstring(entry, value);
}

void trigger_func_1868(u16 param1) {
    s32 *ptr;
    s32 *ptr2;
    Texture *tex;

    ptr = func_800213A0(param1 + 2);
    if (ptr != NULL) {
        for (ptr2 = ptr; *ptr2 != -1; ptr2++) {
            tex = func_8003E960(*ptr2);
            if (tex == NULL) {
                func_80012584(50, 3, NULL, (UnkStructAssetThreadSingle_0x8*)*ptr2, 0, 0, 0, 0);
            }
        }
    }
}

void trigger_func_1920(u16 param1) {
    s32 *ptr;
    s32 *ptr2;
    Texture *tex;

    ptr = func_800213A0(param1 + 2);
    if (ptr != NULL) {
        for (ptr2 = ptr; *ptr2 != -1; ptr2++) {
            tex = func_8003E960(*ptr2);
            if (tex != NULL) {
                texture_destroy(tex);
            }
        }
    }
}

static void trigger_point_setup(Object *self, TriggerCreateInfo *createInfo) {
    TriggerState *state;
    ModelInstance *modelInstance;
    Model *model;
    Vtx *vertex;
    u8 _stack_pad[4];
    f32 x,y,z;
    f32 fvar3;
    f32 fvar2;

    if (sPointModel == NULL) {
        sPointModel = func_80017D2C(85, 0);
        sPointModelRefCount = 1;
    } else {
        sPointModelRefCount += 1;
    }

    state = (TriggerState*)self->state;
    fvar3 = createInfo->unk3A << 1;
    state->unk4 = fvar3 * fvar3;

    self->srt.roll = 0;
    self->srt.pitch = 0;
    self->srt.yaw = createInfo->unk3D << 8;

    modelInstance = sPointModel;
    model = modelInstance->model;
    vertex = &model->vertices[1];
    x = (f32)vertex->v.ob[0];
    y = (f32)vertex->v.ob[1];
    z = (f32)vertex->v.ob[2];
    fvar2 = sqrtf((x * x) + (y * y) + (z * z));
    
    self->srt.scale = fvar3 / fvar2;
}

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_test_point.s")
#else
void trigger_test_point(Object *self, Object *param2) {
    TriggerCreateInfo *createInfo;
    TriggerState *state;
    f32 lengthSquared;
    f32 lengthSquared2;
    f32 diffX;
    f32 diffY;
    f32 diffZ;
    s32 var2;

    state = (TriggerState*)self->state;
    createInfo = (TriggerCreateInfo*)self->createInfo;

    diffX = state->unk20.x - self->positionMirror.x;
    diffY = state->unk20.y - self->positionMirror.y;
    diffZ = state->unk20.z - self->positionMirror.z;

    lengthSquared = diffX * diffX + diffY * diffY + diffZ * diffZ;

    if (createInfo->unk38 > 0) {
        lengthSquared2 = gDLL_CURVES->exports->curves_func_14f4(7, createInfo->unk38, 
            state->unk2C.x, state->unk2C.y, state->unk2C.z, 
            &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z);
        
            if (lengthSquared2 < 0.0f) {
                lengthSquared2 = -lengthSquared2;
            }

            if (self->parent != NULL) {
                inverse_transform_point_by_object(self->positionMirror.x, self->positionMirror.y, self->positionMirror.z,
                    &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z, 
                    self->parent);
            }
    } else {
        diffX = state->unk2C.x - self->positionMirror.x;
        diffY = state->unk2C.y - self->positionMirror.y;
        diffZ = state->unk2C.z - self->positionMirror.z;
        lengthSquared2 = diffX * diffX + diffY * diffY + diffZ * diffZ;
    }

    if (lengthSquared2 < state->unk4) {
        if (lengthSquared < state->unk4) {
            var2 = 2;
        } else {
            var2 = 1;
        }
    } else {
        if (lengthSquared < state->unk4) {
            var2 = -1;
        } else {
            var2 = -2;
        }
    }

    trigger_update_effects(self, param2, var2, lengthSquared2);
}
#endif

static void trigger_cylinder_setup(Object *self, TriggerCreateInfo *createInfo) {
    TriggerState *state;

    state = (TriggerState*)self->state;
    state->unk4 = createInfo->unk3A << 1;
    state->unk4 *= state->unk4;
}

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_test_cylinder.s")
#else
void trigger_test_cylinder(Object* self, Object* param2) {
    TriggerState* state;
    TriggerCreateInfo *createInfo;
    f32 lengthSquared;
    f32 lengthSquared2;
    f32 diffX;
    f32 diffY2;
    f32 diffY;
    f32 diffZ;
    f32 unk3BTimes2;
    s32 var_a2;

    state = self->state;
    createInfo = (TriggerCreateInfo*)self->createInfo;
    unk3BTimes2 = createInfo->unk3B << 1;
    
    diffX = state->unk20.x - self->positionMirror.x;
    diffY = state->unk20.y - self->positionMirror.y;
    diffZ = state->unk20.z - self->positionMirror.z;
    
    lengthSquared = (diffX * diffX) + (diffZ * diffZ);

    diffX = state->unk2C.x - self->positionMirror.x;
    diffY2 = state->unk2C.y - self->positionMirror.y;
    diffZ = state->unk2C.z - self->positionMirror.z;
    
    lengthSquared2 = (diffX * diffX) + (diffZ * diffZ);
    
    if (lengthSquared2 == lengthSquared) {
        if (((diffY == diffY2) && (!state)) && (!state)) {} // partial fakematch
    }

    if (lengthSquared2 < state->unk4 && (diffY2 < 0.0f ? -diffY2 : diffY2) < unk3BTimes2) {
        if (lengthSquared < state->unk4 && (diffY < 0.0f ? -diffY : diffY) < unk3BTimes2) {
            var_a2 = 2;
        } else {
            var_a2 = 1;
        }
    } else {
        if (lengthSquared < state->unk4 && (diffY < 0.0f ? -diffY : diffY) < unk3BTimes2) {
            var_a2 = -1;
        } else {
            var_a2 = -2;
        }
    }
    trigger_update_effects(self, param2, var_a2, lengthSquared2);
}
#endif

static void trigger_plane_setup(Object *self, TriggerCreateInfo *createInfo) {
    TriggerState *state;
    ModelInstance *modelInstance;
    Model *model;
    Vtx *vertex;
    SRT srt; // sp+a8
    MtxF mtx; // sp+68
    f32 ox; // sp+64
    f32 oy; // sp+60
    f32 oz; // sp+5c
    f32 temp;
    s32 i;
    f32 minX; // sp+50
    f32 minZ; // sp+4c
    f32 minY; // sp+48
    f32 maxY; // sp+44
    f32 maxX; // sp+40
    f32 maxZ; // sp+3c
    f32 swapTemp;
    
    if (sPlaneModel == NULL) {
        sPlaneModel = func_80017D2C(78, 0);
        sPlaneModelRefCount = 1;
    } else {
        sPlaneModelRefCount += 1;
    }

    state = (TriggerState*)self->state;

    self->srt.yaw = createInfo->unk3D << 8 << 2;
    self->srt.pitch = createInfo->unk3E << 8;
    temp = createInfo->unk3A * 0.0625f;
    self->srt.scale = self->def->scale * temp;

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    matrix_from_srt(&mtx, &srt);

    vec3_transform(&mtx, 0.0f, 0.0f, 1.0f, &ox, &oy, &oz);
    state->unk10.x = ox;
    state->unk10.y = oy;
    state->unk10.z = oz;
    state->unk1C = -((self->positionMirror.x * ox) + (self->positionMirror.y * oy) + (self->positionMirror.z * oz));

    modelInstance = sPlaneModel;
    model = modelInstance->model;
    vertex = &model->vertices[1];
    minX = maxX = (f32)vertex->v.ob[0];
    minY = maxY = (f32)vertex->v.ob[1];
    minZ = maxZ = (f32)vertex->v.ob[2];

    for (i = 1; i < model->vertexCount; i++, vertex++) {
        if (vertex->v.ob[0] < minX) {
            minX = vertex->v.ob[0];
        }
        if (vertex->v.ob[0] > maxX) {
            maxX = vertex->v.ob[0];
        }

        if (vertex->v.ob[1] < minY) {
            minY = vertex->v.ob[1];
        }
        if (vertex->v.ob[1] > maxY) {
            maxY = vertex->v.ob[1];
        }

        if (vertex->v.ob[2] < minZ) {
            minZ = vertex->v.ob[2];
        }
        if (vertex->v.ob[2] > maxZ) {
            maxZ = vertex->v.ob[2];
        }
    }

    vec3_transform(&mtx, minX, minY, minZ, &minX, &minY, &minZ);
    vec3_transform(&mtx, maxX, maxY, maxZ, &maxX, &maxY, &maxZ);

    if (maxX < minX) {
        swapTemp = minX;
        minX = maxX;
        maxX = swapTemp;
    }
    if (maxY < minY) {
        swapTemp = maxY;
        maxY = minY;
        minY = swapTemp;
    }
    if (maxZ < minZ) {
        swapTemp = maxZ;
        maxZ = minZ;
        minZ = swapTemp;
    }

    state->unk38 = self->positionMirror.x + self->srt.scale * minX;
    state->unk3C = self->positionMirror.y + self->srt.scale * minY;
    state->unk40 = self->positionMirror.z + self->srt.scale * minZ;
    state->unk44 = self->positionMirror.x + self->srt.scale * maxX;
    state->unk48 = self->positionMirror.y + self->srt.scale * maxY;
    state->unk4C = self->positionMirror.z + self->srt.scale * maxZ;
    state->unk4 = (self->srt.scale * 145.0f) * (self->srt.scale * 145.0f);
}

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_test_plane.s")
#else
void trigger_test_plane(Object* self, Object* param2) {
    TriggerState* state;
    f32 temp_f0;
    f32 temp_f6_2;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12_2;
    f32 temp_f14_2;
    f32 temp_f16_2;
    f32 temp_f18_2;
    f32 temp_f2;
    s32 var_a2;

    state = (TriggerState*)self->state;
    
    temp_f0 = (state->unk10.x * state->unk20.x) + (state->unk10.y * state->unk20.y) + (state->unk10.z * state->unk20.z) + state->unk1C;
    temp_f6_2 = (state->unk10.x * state->unk2C.x) + (state->unk10.y * state->unk2C.y) + (state->unk10.z * state->unk2C.z) + state->unk1C;
    
    if (temp_f6_2 < 0.0f) {
        if (temp_f0 < 0.0f) {
            var_a2 = 2;
        } else {
            var_a2 = 1;
        }
    } else {
        if (temp_f0 < 0.0f) {
            var_a2 = -1;
        } else {
            var_a2 = -2;
        }
    }
    if (var_a2 == 1 || var_a2 == -1) {
        temp_f0_2 = state->unk2C.x - state->unk20.x;
        temp_f12_2 = state->unk2C.y - state->unk20.y;
        temp_f14_2 = state->unk2C.z - state->unk20.z;
        
        temp_f2 = ((((-state->unk10.x * state->unk20.x) - (state->unk10.y * state->unk20.y)) - (state->unk10.z * state->unk20.z)) - state->unk1C) / 
            ((state->unk10.x * temp_f0_2) + (state->unk10.y * temp_f12_2) + (state->unk10.z * temp_f14_2));
        
        temp_f16_2 = state->unk20.x + (temp_f2 * temp_f0_2);
        temp_f0_3 = state->unk20.y + (temp_f2 * temp_f12_2);
        temp_f18_2 = state->unk20.z + (temp_f2 * temp_f14_2);
        
        if ((state->unk38 <= temp_f16_2) && 
            (temp_f16_2 <= state->unk44) && 
            (state->unk3C <= temp_f0_3) && 
            (temp_f0_3 <= state->unk48) && 
            (state->unk40 <= temp_f18_2) && 
            (temp_f18_2 <= state->unk4C)) {
            trigger_update_effects(self, param2, var_a2, temp_f6_2);
        }
    }
}
#endif

static void trigger_area_setup(Object *self, TriggerCreateInfo *createInfo) {
    self->srt.yaw = createInfo->unk3D << 8;
    self->srt.pitch = createInfo->unk3E << 8;
    self->srt.roll = 0;
}

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_test_area.s")
#else
void trigger_test_area(Object* arg0, Object* arg1) {
    TriggerState* state;
    s32 sp20;
    s32 temp_v0;

    state = (TriggerState*)arg0->state;
    
    sp20 = trigger_func_273C(arg0, &state->unk2C);
    temp_v0 = trigger_func_273C(arg0, &state->unk20);
    
    if (sp20 != 0) {
        if (temp_v0 == 0) {
            trigger_update_effects(arg0, arg1, 1, 0);
        } else {
            trigger_update_effects(arg0, arg1, 2, 0);
        }
    } else {
        if (temp_v0 != 0) {
            trigger_update_effects(arg0, arg1, -1, 0);
        } else {
            trigger_update_effects(arg0, arg1, -2, 0);
        }
    }
}
#endif

s32 trigger_func_273C(Object *obj, Vec3f *vec) {
    f32 x;
    f32 y;
    f32 z;
    TriggerCreateInfo *createInfo;

    createInfo = (TriggerCreateInfo*)obj->createInfo;
    x = vec->x;
    y = vec->y;
    z = vec->z;
    
    trigger_func_2884(obj, &x, &y, &z);
    
    if (x < 0.0f) {
        x = -x;
    }
    if (y < 0.0f) {
        y = -y;
    }
    if (z < 0.0f) {
        z = -z;
    }
    
    if (x <= (createInfo->unk3A * 2) && y <= (createInfo->unk3B * 2) && z <= (createInfo->unk3C * 2)) {
        return 1;
    }
    
    return 0;
}

static void trigger_func_2884(Object *obj, f32 *ox, f32 *oy, f32 *oz) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f16;
    f32 temp_f2;
    
    sp2C = fsin16_precise(-obj->srt.yaw);
    sp28 = fcos16_precise(-obj->srt.yaw);
    sp24 = fsin16_precise(-obj->srt.pitch);
    temp_f0 = fcos16_precise(-obj->srt.pitch);
    
    temp_f2 = (*ox) - obj->positionMirror.x;
    temp_f16 = (*oy) - obj->positionMirror.y;
    temp_f12 = (*oz) - obj->positionMirror.z;
    
    *ox = (temp_f2 * sp28) - (temp_f12 * sp2C);
    temp_f12 = (temp_f2 * sp2C) + (temp_f12 * sp28);
    *oy = (temp_f16 * temp_f0) - (temp_f12 * sp24);
    *oz = (temp_f16 * sp24) + (temp_f12 * temp_f0);
}

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_func_29C0.s")
#else
void trigger_func_29C0(u16 arg0, Object* arg1, s8 arg2, s32 arg3) {
    Object** objects;
    Object* obj;
    TriggerCreateInfo* objCreateInfo;
    s32 i;
    s32 numObjs;
    
    objects = get_world_objects(&i, &numObjs);
    while (i < numObjs) {
        obj = objects[i];
        objCreateInfo = (TriggerCreateInfo*)obj->createInfo;
        
        if (objCreateInfo != NULL) {
            switch (objCreateInfo->base.objId) {
            case OBJ_TriggerPoint:
            case OBJ_TriggerPlane:
            case OBJ_TriggerArea:
            case OBJ_TriggerTime:
            case OBJ_TriggerButton:
            case OBJ_TriggerSetup:
            case OBJ_TriggerBits:
                if (0) { } // fakematch
            case OBJ_TriggerCylinder:
                if (objCreateInfo->unk38 == arg0) {
                    trigger_update_effects(obj, arg1, arg2, arg3);
                }
                break;
            }
        }
        
        i++;
    }
}
#endif

static void trigger_curve_setup(Object *self, TriggerCreateInfo *createInfo) { }

// needs trigger_update_effects to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_test_curve.s")
#else
void trigger_test_curve(Object* self, Object* param2) {
    TriggerState* state;
    TriggerCreateInfo *createInfo;
    s32 sp3C;
    f32 sp38;
    s32 sp34;
    s32 temp_v0;
    s32 temp_v0_2;

    sp34 = 0x17;
    state = (TriggerState*)self->state;
    createInfo = (TriggerCreateInfo*)self->createInfo;
    
    temp_v0 = gDLL_CURVES->exports->curves_func_1e4(state->unk2C.x, state->unk2C.y, state->unk2C.z, &sp34, 1, createInfo->unk38);
    sp3C = gDLL_CURVES->exports->curves_func_291c(temp_v0, state->unk2C.x, state->unk2C.y, state->unk2C.z, &sp38);
    temp_v0_2 = gDLL_CURVES->exports->curves_func_291c(temp_v0, state->unk20.x, state->unk20.y, state->unk20.z, &sp38);
    
    if (sp3C != 0) {
        if (temp_v0_2 == 0) {
            trigger_update_effects(self, param2, 1, sp38);
        } else {
            trigger_update_effects(self, param2, 2, sp38);
        }
    } else {
        if (temp_v0_2 != 0) {
            trigger_update_effects(self, param2, -1, sp38);
        } else {
            trigger_update_effects(self, param2, -2, sp38);
        }
    }
}
#endif
