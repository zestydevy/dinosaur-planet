#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
} DLL274_Setup;

typedef struct {
    u8 _unk0[0xC - 0x0];
    f32 unkC;
    u8 _unk10[0x14 - 0x10];
    f32 unk14;
    Vec3f unk18[4];
    Vec3f unk48[4];
    u32 unk78;
    u32 unk7C;
    s16 unk80;
    s16 unk82;
    u8 _unk84[0x88 - 0x84];
    s8 unk88;
    Object* unk8C;
    Object* unk90;
    Vec3f unk94;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    Vec3f unkAC;
    // f32 unkB0;
    // f32 unkB4;
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    u8 _unkD0[0xD4 - 0xD0];
    u16 unkD4;
    u8 _unkD6[0xDC - 0xD6];
} DLL274_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003
};
/*0x10*/ static u32 _data_10[] = {
    0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x30*/ static u32 _data_30[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static void dll_274_func_2020(Object* self, MtxF* oMtx);

// offset: 0x0 | ctor
void dll_274_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_274_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_274_setup(Object *self, ObjSetup* setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_setup.s")

// offset: 0x638 | func: 1 | export: 1
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_control.s")
#else
void dll_274_control(Object* self) {
    DLL274_Data* objData;
    s32 damageType;
    Object* hitBy;

    objData = self->data;
    
    objData->unkD4 &= ~2;
    if (self->velocity.y != 0.0f) {
        objData->unkD4 |= 2;
    }
    
    if ((self->unkAF & 4) && (mainGetBits(BIT_913) == 0)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        mainSetBits(BIT_913, 1);
        return;
    }
    
    switch (self->id) {
    case OBJ_NWSH_colpush:
        if (dll_274_func_225C(self, objData) != 0) {
            return;
        }
        break;
    case OBJ_NWSH_colpushped:
        if (dll_274_func_225C(self, objData) != 0) {
            return;
        }
        break;
    case OBJ_VFP_Block2:
        damageType = func_80025F40(self, &hitBy, NULL, NULL);
        if (damageType && (hitBy != NULL) && (hitBy->id == OBJ_VFPDragBreath)) {
            objData->unk14 = 100.0f;
        } else if ((objData->unkD4 & 0x80) && damageType && (hitBy != NULL) && (hitBy->id == 0x14B)) {
            mainSetBits(BIT_4FA, 1);
        }
        
        dll_274_func_27C8(self, objData);
        break;
    case OBJ_DIM2IceBlock:
        dll_274_func_2A74(self, objData);
        break;
    default:
        break;
    }

    mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
}
#endif

// offset: 0x87C | func: 2 | export: 2
void dll_274_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_update.s")

// offset: 0xEE4 | func: 3 | export: 3
void dll_274_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL274_Data* objData;
    ModelInstance* modelInst;
    Model* model;

    if (visibility == FALSE) {
        return;
    }
    
    objData = self->data;
    
    switch (self->id) {
    case OBJ_NWSH_colpush:
        if (mainGetBits(objData->unk80)) {
            return;
        }
        break;
    case OBJ_NWSH_colpushped:
        if (mainGetBits(objData->unk80)) {
            return;
        }
        break;
    case OBJ_VFP_Block2:
        if (objData->unk14 > 0) {
            objData->unk14 -= gUpdateRateF;
            if (objData->unk14 <= 0.0f) {
                objData->unk14 = 0.0f;
            } else {
                objprintSetBlendColor(0xC8, 0, 0, 0xFF);
            }
        }
        break;
    default:
        break;
    }

    modelInst = self->modelInsts[self->modelInstIdx];
    model = modelInst->model;
    model->unk71 |= 2;
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x1058 | func: 4 | export: 4
void dll_274_free(Object* self, s32 onlySelf) {
    DLL274_Data* objData;
    DLL274_Setup* objSetup;

    objSetup = (DLL274_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    case OBJ_NWSH_colpush:
        mainSetBits(objData->unk80, 0);
        break;
    case OBJ_NWSH_colpushped:
        mainSetBits(objData->unk80, 0);
        break;
    default:
        if (objSetup->unk18 >= 0) {
            mapSaveObject(&objSetup->base, self->mapID, objSetup->base.x, objSetup->base.y, objSetup->base.z);
        }
        break;
    }

    if (objData->unk78) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk78);
        objData->unk78 = 0;
    }
    
    objFreeObjectType(self, OBJTYPE_PushBlock);
}

// offset: 0x116C | func: 5 | export: 5
u32 dll_274_get_model_flags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8;
}

// offset: 0x117C | func: 6 | export: 6
u32 dll_274_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL274_Data);
}

// offset: 0x1190 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1190.s")

// offset: 0x176C | func: 8 | export: 8
s32 dll_274_func_176C(Object* self, Object* obj) {
    Vec3f d;
    s32 pad2;
    DLL274_Data* objData;
    s32 pad1;

    objData = self->data;
    
    d.f[0] = obj->srt.transl.x - self->srt.transl.x;
    d.f[1] = obj->srt.transl.y - self->srt.transl.y;
    d.f[2] = obj->srt.transl.z - self->srt.transl.z;

    return (sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2])) < objData->unkC);
}

// offset: 0x1814 | func: 9 | export: 9
void dll_274_func_1814(Object* self, s32 bitIndex) {
    DLL274_Data* objData = self->data;
    objData->unk7C |= (1 << bitIndex);
}

// offset: 0x1834 | func: 10 | export: 10
s32 dll_274_func_1834(Object* self) {
    DLL274_Data* objData = self->data;
    return objData->unkD4 & 1;
}

// offset: 0x184C | func: 11
s32 dll_274_func_184C(Object* self, s32 arg1, f32 arg2, f32* arg3) {
    UnkFunc_80024108Struct sp24;
    s32 temp_v1;

    sp24.unk12 = 0;
    temp_v1 = objAnimAdvance(self, arg2, 1.0f, &sp24);
    if (sp24.unk12 != 0) {
        *arg3 = sp24.unk0[2];
    } else {
        *arg3 = 0.0f;
    }
    
    return temp_v1;
}

// offset: 0x18C8 | func: 12
static void dll_274_func_18C8(Object* self, s32 arg1) {
    DLL274_Data* objData;
    Object* outSender;
    u32 outMessageID;
    f32* outMesgArg;

    objData = self->data;
    outMesgArg = NULL;
    while (objRecvMesg(self, &outMessageID, &outSender, (void*)&outMesgArg)) {
        switch (outMessageID) {
        case 0xF0003:
            objData->unk8C = outSender;
            break;
        case 0xE:
            if ((self->id != OBJ_NWSH_colpush) && (self->id != OBJ_NWSH_colpushped)) {
                objFreeObject(self);
            }
            break;
        case 0x40001:
            if (self->id == OBJ_NWSH_colpush) {
                objData->unkC4 = *outMesgArg;
            }
            if (self->id == OBJ_NWSH_colpushped) {
                objData->unkC4 = *outMesgArg;
            }
            break;
        }
    }
}

// offset: 0x1A24 | func: 13
int dll_274_func_1A24(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DLL274_Data* objData;
    s32 pad[2];
    f32 dx;
    f32 dz;
    f32 magnitude;
    Object* player;
    Object* obj;

    objData = self->data;

    animData->unk7A = -1;
    
    if (animData->unk62 != 0) {
        if (animData->unk62 != 2) {
            animData->unk58 = 1.0f;
            animData->unk4C.f[0] = self->srt.transl.x - animObj->srt.transl.x;
            animData->unk4C.f[1] = self->srt.transl.y - animObj->srt.transl.y;
            animData->unk4C.f[2] = self->srt.transl.z - animObj->srt.transl.z;
            
            animData->yawDiff = self->srt.yaw - (animObj->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(animData->yawDiff);
            
            animData->pitchDiff = self->srt.pitch - (animObj->srt.pitch & 0xFFFF);
            CIRCLE_WRAP(animData->pitchDiff);
 
            animData->rollDiff = ((animObj->srt.roll & 0xFFFF) - (self->srt.roll & 0xFFFF));
            CIRCLE_WRAP(animData->rollDiff);
            
            animData->unk62 = 2;
        }
        
        animData->unk58 -= animData->unk24 * gUpdateRateF;
        if (animData->unk58 <= 0.0f) {
            animData->unk62 = 0;
        }
    }
    
    if (animData->unk62 == 0) {
        self->objhitInfo->unk5A = 8;
    }
    
    if (self->unkE0 == 0) {
        self->unkE0 = 2;
    }
    
    if ((self->id == OBJ_NWSH_colpush) || (self->id == OBJ_NWSH_colpushped)) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        
        if (self->polyhits->unk10F > 0){
            obj = self->polyhits->unk100[0];

            if ((obj->controlNo == OBJCONTROL_Projectile) && (mainGetBits(BIT_103) == 0)) {
                mainSetBits(BIT_103, 1);
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                player = objGetPlayer();
                
                dx = self->srt.transl.f[0] - player->srt.transl.f[0];
                dz = self->srt.transl.f[2] - player->srt.transl.f[2];
                magnitude = sqrtf(SQ(dx) + SQ(dz));
                if (magnitude != 0.0f) {
                    dx /= magnitude;
                    dz /= magnitude;
                }
                
                objData->unk94.f[0] = dx * 4.0f;
                objData->unk94.f[1] = 0;
                objData->unk94.f[2] = dz * 4.0f;
            
                return 4;
            }
        }
    }
    
    return 0;
}

// offset: 0x1D18 | func: 14
static void dll_274_func_1D18(Object* self, DLL274_Data* objData) {
    DLL274_Setup* objSetup;
    Vec3f* var_s1;
    MtxF mtx;
    Vec3f* var_s0;
    s32 pad1;
    s32 pad2;
    Vec3f spEC[6];
    s8 spEB;
    s8 spEA;
    s32 i;
    Func_80059C40_Struct sp90;
    s32 pad;

    objSetup = (DLL274_Setup*)self->setup;
    
    spEA = (-0x80 >> (7 - objData->unk88));
    
    spEB = ~spEA;
    spEA = ~spEA;
    if (spEA) {
        do {
            spEB = spEA;
            var_s1 = objData->unk18;
            for (i = 0; i < objData->unk88; i++) {
                dll_274_func_2020(self, &mtx);
                vec3_transform(&mtx, var_s1[i].x, var_s1[i].y, var_s1[i].z, &spEC[i].x, &spEC[i].y, &spEC[i].z);
                if (func_80059C40(&objData->unk48[i], &spEC[i], 0.5f, 1, &sp90, self, 8, 0xD, 3 + i, 0xA) == 0) {
                    spEB &= ~(1 << i);
                } else {
                    if ((sp90.unk51 != -1) && !(objData->unkD4 & 1)) {
                        objData->unkD4 |= 1;
                        if (objSetup->unk18 >= 0) {
                            if ((self->id != 0x21E) && (self->id != 0x411)) {
                                mainSetBits(objSetup->unk18, 1);
                                gDLL_6_AMSFX->vtbl->play(self, 0xB01, MAX_VOLUME, NULL, NULL, 0, NULL);
                            }
                        }
                    }
                    
                    bcopy(&spEC[i], &objData->unk48[i], sizeof(Vec3f));
                    mtx.m[3][0] = spEC[i].f[0];
                    mtx.m[3][1] = spEC[i].f[1];
                    mtx.m[3][2] = spEC[i].f[2];
                    vec3_transform(&mtx, -var_s1[i].f[0], -var_s1[i].f[1], -var_s1[i].f[2], &self->srt.transl.x, &self->srt.transl.f[1], &self->srt.transl.f[2]);
                }
            }
        } while (spEB);
    }
    
    bcopy(spEC, objData->unk48, objData->unk88 * sizeof(Vec3f));
}

// offset: 0x2020 | func: 15
void dll_274_func_2020(Object* self, MtxF* oMtx) {
    SRT srt;

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    
    matrix_from_srt(oMtx, &srt);
}

// offset: 0x20A0 | func: 16
void dll_274_func_20A0(Object* self, DLL274_Data* objData) {
    DLL274_Setup* objSetup;
    TextureAnimator* texAnim;

    objSetup = (DLL274_Setup*)self->setup;
    objData->unkA0 = 1.2f;
    objData->unkA4 = 0.6f;
    objData->unkA8 = 0.6f;
    objData->unkB8 = rand_next(25, 75) * 0.01f;
    objData->unkBC = objData->unkB8 / rand_next(40, 70);
    objData->unkC0 = 0.0f;
    objData->unk80 = objSetup->unk18;
    objData->unk82 = objSetup->unk1A;
    objData->unkC4 = 0.0f;
    objData->unk90 = NULL;
    
    mainSetBits(objData->unk80, 0);
    texAnim = objExprGetTexAnimator(self, 0, 0);

    objData->unkAC.f[1] += objData->unkA4;
    if (objData->unkAC.f[1] > 255.0f) {
        objData->unkAC.f[1] = 255.0f;
    } else if (objData->unkAC.f[1] < 0.0f) {
        objData->unkAC.f[1] = 255.0f;
    }
    
    objData->unkAC.f[2] += objData->unkA8;
    if (objData->unkAC.f[2] > 255.0f) {
        objData->unkAC.f[2] = 255.0f;
    } else if (objData->unkAC.f[2] < 0.0f) {
        objData->unkAC.f[2] = 255.0f;
    }
    
    texAnim->multiplyR = 0xA;
    texAnim->multiplyG = 0xA;
    texAnim->multiplyB = 0xA;
}

// offset: 0x225C | func: 17
s32 dll_274_func_225C(Object* self, DLL274_Data* objData) {
    TextureAnimator* texAnim; //5C
    DLL_IModgfx* sp58; //58
    u8 sp57; //57
    f32 dx;
    f32 dz;
    f32 distance; //48
    f32 temp1;
    f32 temp2;
    s8 temp3;

    sp57 = FALSE;
    distance = 10000.0f;
    
    dll_274_func_18C8(self, 0);

    if (mainGetBits(objData->unk80)) {
        if (self->srt.scale > 0.001f) {
            self->srt.scale -= 0.02f * gUpdateRateF;
            if (self->srt.scale <= 0.001f) {
                self->unkAF |= ARROW_FLAG_8_No_Targetting;
                self->srt.scale = 0.0f;
                self->srt.transl.f[1] -= 300.0f;
            }
        }
        return 1;
    }
    
    if (objData->unk90 == NULL) {
        objData->unk90 = objGetNearestTypeTo(0x13, self, &distance);
    }
    
    if (objData->unk90 == NULL) {
        return 0;
    }
    
    if (objData->unkAC.f[0] < 150.0f) {
        objData->unkAC.f[0] = 150.0f;
    }
    
    dz = objData->unk90->srt.transl.f[2] - self->srt.transl.f[2];
    if (dz < 0.0f) {
        dz *= -1.0f;
    }
    
    if (objData->unkC4 < dz + 10.0f) {
        return 0;
    }
        
    dx = objData->unk90->srt.transl.x - self->srt.transl.x;
    if (dx < 0.0f) {
        dx *= -1.0f;
    }
    if (dx > 30.0f) {
        return 0;
    }
    
    if ((dz + 10.0f <= objData->unkC4) && (objData->unkC4 <= (dz + 40.0f))) {
        sp57 = TRUE;
        mainSetBits(BIT_1C9, 1);
    }
    
    texAnim = objExprGetTexAnimator(self, 0, 0);

    objData->unkC0 += objData->unkBC * gUpdateRateF;
    if (objData->unkB8 <= objData->unkC0) {
        objData->unkBC *= -1.0f;
    } else if (objData->unkC0 < 0.0f) {
        objData->unkB8 = rand_next(25, 75) * 0.01f;
        objData->unkBC = objData->unkB8 / rand_next(40, 70);
        objData->unkC0 = 0.0f;
    }
    
    if (texAnim != NULL) {
        objData->unkAC.f[0] += objData->unkA0;
        if (objData->unkAC.f[0] >= 225.0f) {
            mainSetBits(objData->unk80, 1);
            if (sp57 != 0) {
                mainSetBits(BIT_1C9, 0);
            }
            
            sp58 = dllLoad(0x1003, 1);
            sp58->vtbl->func0(self, 0x14, 0, 2, -1, 0);
            sp58->vtbl->func0(self, 0x14, 0, 2, -1, 0);
            dllFree(sp58);
            
            gDLL_6_AMSFX->vtbl->play(NULL, 0x778, 0x64, NULL, NULL, 0, NULL);
        } else {
            objData->unkAC.f[1] += objData->unkA4;

            
            if (objData->unkAC.f[1] > 255.0f) {
                objData->unkAC.f[1] = 255.0f;
            } else if (objData->unkAC.f[1] < 0.0f) {
                objData->unkAC.f[1] = 255.0f;
            }
            
            objData->unkAC.f[2] += objData->unkA8;
            if (objData->unkAC.f[2] > 255.0f) {
                objData->unkAC.f[2] = 255.0f;
            } else if (objData->unkAC.f[2] < 0.0f) {
                objData->unkAC.f[2] = 255.0f;
            }

            temp1 = objData->unkAC.f[1] * (0.25f + objData->unkC0);
            temp2 = objData->unkAC.f[2] * (0.25f + objData->unkC0);
            temp3 = objData->unkAC.f[0];
            
            texAnim->multiplyR = (s8)temp3 & 0xFF;
            texAnim->multiplyG = (s16)temp1;
            texAnim->multiplyB = (s16)temp2;
        }
    }
    
    return 0;
}

// offset: 0x27C8 | func: 18
s32 dll_274_func_27C8(Object* self, DLL274_Data* objData) {
    SRT sp50;
/*0x40*/ Vec3f _data_40 = VEC3F(0, 0, 0.1);
    Object* player = objGetPlayer();
    
    if (objData->unkD4 & 0x80) {
        if (objData->unk78) {
            gDLL_6_AMSFX->vtbl->stop(objData->unk78);
            objData->unk78 = 0;
        }
        return 0;
    } 
    
    if (((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xA)) {
        if (objData->unk78) {
            gDLL_6_AMSFX->vtbl->stop(objData->unk78);
            objData->unk78 = 0;
        }
        return 0;
    }
    
    if (objData->unk78 == 0) {
        gDLL_6_AMSFX->vtbl->play(self, 0xA87, MAX_VOLUME, &objData->unk78, NULL, 0, NULL);
    }
    
    objData->unkD4 |= 2;
    
    sp50.yaw = self->srt.yaw;
    sp50.pitch = 0;
    sp50.roll = 0;
    sp50.transl.x = 0.0f;
    sp50.transl.y = 0.0f;
    sp50.transl.z = 0.0f;
    sp50.scale = 1.0f;
    rotate_vec3(&sp50, _data_40.f);
    
    objData->unk94.x = _data_40.f[0] * gUpdateRateF;
    objData->unk94.z = _data_40.f[2] * gUpdateRateF;
    objMove(self, objData->unk94.x, objData->unk94.y, objData->unk94.z);
    
    dll_274_func_1D18(self, objData);
    
    if (self->srt.transl.x < -15870.0f) {
        self->srt.transl.x = -15876.0f;
        self->srt.transl.y = -1218.0f;
        self->srt.transl.z = -766.0f;
        self->srt.roll = 0;
        self->srt.pitch = 0;
        objData->unkD4 &= ~4;
        objData->unkD4 |= 0x80;
        mainSetBits(objData->unk80, 1);
        gDLL_6_AMSFX->vtbl->play(self, 0xA88, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return 0;
}

// offset: 0x2A74 | func: 19
s32 dll_274_func_2A74(Object* self, DLL274_Data* objData) {
    if ((objData->unkCC == 0.0f) && (objData->unkC8 > 0.0f)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_3D8_Water_Splash, MAX_VOLUME, NULL, NULL, 0, NULL);
        mainSetBits(BIT_DIM_Pushed_Ice_Block_Into_Lake, TRUE);
    }
    return 0;
}

/*0x0*/ static const char str_0[] = "PUSHPULL OBJECT: hitpoint overflow\n";
/*0x24*/ static const char str_24[] = "handle is %d\n";
