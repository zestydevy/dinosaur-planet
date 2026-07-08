#include "common.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/objects/251_weapons.h"
#include "game/objects/object_id.h"
#include "macros.h"
#include "sys/objhits.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    Baddie_Setup baddie;
} DLL228_Setup;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s8 unk18[8];
    Vec3f unk20;
    s8 unk2C[12];
    Vec3f unk38;
} DLL228_DataActual; //44

typedef struct {
    Baddie unk0;
    DLL228_DataActual unk3FC; //0x3FC
} DLL228_Data; //440

/*0x0*/ static s32 data_0[] = {
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002, 
    0x00000002, 0x00000002, 0x00000002, 0x00000002
};
/*0x70*/ static s8 data_70[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff
};
/*0x8C*/ static u32 data_8C[] = {
    0x02060167, 0x01650206, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 bss_0[0x8];
/*0x8*/ static u8 bss_8[0x4];
/*0xC*/ static u8 bss_C[0x4];
/*0x10*/ static u8 bss_10[0x4];
/*0x14*/ static u8 bss_14[0x4];
/*0x18*/ static ObjFSA_StateCallback bss_18[4];
/*0x28*/ static ObjFSA_StateCallback bss_28[6];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_func_0.s")

// offset: 0x9C | ctor
void dll_228_ctor(void* dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_ctor.s")

// offset: 0xDC | dtor
void dll_228_dtor(void* dll) { }

// offset: 0xE8 | func: 1 | export: 0
void dll_228_setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    DLL228_DataActual* objData;
    Baddie* baddie;
    u8 flags;

    baddie = self->data;
    
    flags = 2;
    if (reset != 0) {
        flags = 3;
    }
    if ((objSetup->unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 4, 6, 0x100, flags, 20.0f);
    
    self->animCallback = NULL;
    
    objData = baddie->objdata;
    bzero(objData, sizeof(DLL228_DataActual));
    
    objData->unk4 = objSetup->unk2C * 60.0f;
    objData->unk8 = rand_next(0xA, 0x12C);
    objData->unk10 = 0;
    objData->unk12 = 0;
    
    func_80023D30(self, 0, 0.0f, 0);
    
    baddie->fsa.animState = 0;
    baddie->fsa.logicState = 0;
    baddie->fsa.unk4.mode = 0;
    
    func_800267A4(self);
}

// offset: 0x244 | func: 2 | export: 1
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_control.s")
#else

static void dll_228_func_618(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_228_func_D08(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void dll_228_func_EC0(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

void dll_228_control(Object* self) {
    s32 pad;
    Baddie* objData;
    Baddie_Setup* objSetup;
    f32 time;

    objData = self->data;
    objSetup = (DLL228_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        return;
    }
    
    if (self->unkE0 == 0) {
        self->srt.transl.x = objSetup->base.x;
        self->srt.transl.y = objSetup->base.y;
        self->srt.transl.z = objSetup->base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, objData, 0) == FALSE) {
        objData->unk3B6 = 0;
        return;
    }
    
    if ((objData->unk3B0 & 0x10) && (gDLL_7_Newday->vtbl->func8(&time) == FALSE)) {
        objData->unk3B6 = 0;
        return;
    }
    
    dll_228_func_618(self, objData, &objData->fsa);
    
    if (objData->unk3B6 == 1) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, &objData->fsa, objData->unk3E2, 1)) {
            objData->unk3B6 = 0;
        }
    }
    
    if (objData->unk3B6 == 0) {
        dll_228_func_EC0(self, objData, &objData->fsa);
    } else {
        dll_228_func_D08(self, objData, &objData->fsa);
    }
    
    self->srt.transl.y = objSetup->base.y - 2.0f;
}
#endif

// offset: 0x43C | func: 3 | export: 2
void dll_228_update(Object* self) { }

// offset: 0x448 | func: 4 | export: 3
void dll_228_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    DLL228_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (visibility && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objData->unk20.x, &objData->unk20.y, &objData->unk20.z, 0);
        func_80031F6C(self, 1, &objData->unk38.x, &objData->unk38.y, &objData->unk38.z, 0);
    }
}

// offset: 0x50C | func: 5 | export: 4
void dll_228_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x5B0 | func: 6 | export: 5
u32 dll_228_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x5C0 | func: 7 | export: 6
u32 dll_228_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL228_Data);
}

// offset: 0x5D4 | func: 8 | export: 7
s16 dll_228_func_5D4(Object* self, s32 arg1) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x5E8 | func: 9 | export: 8
void dll_228_func_5E8(Object* self, u8 message, s32 unused) {
    Baddie* baddie = self->data;
    
    switch (message) {
    case 0x81:
        baddie->unk3B0 &= ~4;
        break;
    default:
        STUBBED_PRINTF("BADDIE:Caictua Unknown message [%d]\n", message);
        break;
    }
}

// offset: 0x618 | func: 10
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_func_618.s")
#else
void dll_228_func_618(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
/*0x0*/ static SRT bss_0;
    Object* player = get_player(); //6C
/*0x8C*/ s16 data_8C[] = { 0x0206, 0x0167, 0x0165, 0x0206 }; //64, 66, 68, 6A
    Vec3f sp58; //58, 5C, 60
    s32 count;
    s32 scaleIdx;

    if (fsa->target != NULL) {
        sp58.f[0] = fsa->target->globalPosition.f[0] - self->globalPosition.f[0];
        sp58.f[1] = fsa->target->globalPosition.f[1] - self->globalPosition.f[1];
        sp58.f[2] = fsa->target->globalPosition.f[2] - self->globalPosition.f[2];
        fsa->targetDist = sqrtf(SQ(sp58.f[0]) + SQ(sp58.f[1]) + SQ(sp58.f[2]));
    }
    
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 2, 3, baddie->unk3A6, baddie->unk3A4);
    }

    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
    if ((fsa->hitpoints > 0) && (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, data_0, data_70, 1, &baddie->unk3A8, &bss_0))) {
        scaleIdx = ((DLL_251_Weapons*)player->linkedObject->dll)->vtbl->func19(player->linkedObject);
        if (scaleIdx > 3) {
            scaleIdx = 3;
        }

        bss_0.scale = data_8C[scaleIdx];
        gDLL_17_partfx->vtbl->spawn(self, 0x323, &bss_0, 0x200001, -1, NULL);
        
        bss_0.transl.x -= self->srt.transl.f[0];
        bss_0.transl.y -= self->srt.transl.f[1];
        bss_0.transl.z -= self->srt.transl.f[2];
        bss_0.scale = data_8C[scaleIdx];
        
        count = 4;
        while (count--) {
            gDLL_17_partfx->vtbl->spawn(self, 0x324, &bss_0, 2, -1, NULL);
        }
    }
}
#endif

// offset: 0x938 | func: 11
f32 dll_228_func_938(s32 yawDiff, f32 yawSpeed, f32 maxAngle) {
    f32 temp_ft4;
    f32 temp_ft4_2;
    f32 temp_fv0;
    f32 var_fa0;
    s32 var_a0;
    f32 var_fa1;
    f32 var_fv0;
    f32 var_fv1;
    s32 isNegative;

    var_fv0 = 0.0f;
    var_fv1 = 0.0f;
    var_fa0 = 0.0f;
    
    isNegative = yawDiff < 0;
    if (isNegative != 0) {
        yawDiff = -yawDiff;
        yawSpeed = -yawSpeed;
    }
    
    if (yawSpeed < 0.0f) {
        return (isNegative) ? -maxAngle : maxAngle;
    }
    
    do {
        var_fv1 += maxAngle;
        var_fv0 += var_fv1;
    } while ((var_fv0 + var_fv1) < yawDiff);
    
    if ((yawDiff <= maxAngle) && (yawSpeed <= maxAngle) && 
        (((yawDiff >= 0) && (yawSpeed >= 0.0f)) || ((yawDiff <= 0) && (yawSpeed <= 0.0f)))
    ) {
        var_fa0 = 0.0f;
    } else {
        temp_fv0 = var_fv1 - maxAngle;
        temp_ft4 = yawSpeed + maxAngle;
        if (temp_ft4 <= temp_fv0) {
            var_fa0 = temp_ft4;
        } else {
            temp_ft4_2 = yawSpeed - maxAngle;
            if (temp_ft4_2 < var_fv1) {
                var_fa0 = temp_fv0;
                if (var_fv1 == maxAngle) {
                    var_fa0 = maxAngle;
                }
            } else {
                var_fa0 = temp_ft4_2;
                if (temp_ft4_2 == 0.0f) {
                    var_fa0 = maxAngle;
                }
            }
        }
    }
    
    if (isNegative) {
        var_fa0 = -var_fa0;
        yawSpeed = -yawSpeed;
    }
    
    return var_fa0 - yawSpeed;
}

// offset: 0xAA0 | func: 12
#ifdef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_func_AA0.s")
#else
s16 dll_228_func_AA0(f32 originX, f32 originY, f32 originZ, f32 targetX, f32 targetY, f32 targetZ, f32 speed, f32 arg7, s32 arg8) {
    f32 sp6C;
    f32 temp_fa0;
    f32 temp_fs3;
    f32 dy;
    f32 dx;
    f32 dz;
    f32 var_fa1;
    f32 temp;
    s32 iterations;
    s32 idx;
    s16 sp7C[2]; //7C

    idx = 0;
    
    dx = originX - targetX;
    dz = originZ - targetZ;
    dy = targetY - originY;
    dx = sqrtf(SQ(dx) + SQ(dz));
    
    sp6C = ((dy * arg7) + SQ(speed));
    dz = SQ(sp6C);
    temp_fa0 = SQ(arg7) * (SQ(dx) + SQ(dy));
    temp = dz - temp_fa0;
    
    if (temp_fa0 <= dz) {
        iterations = 2;
        temp_fs3 = sqrtf(temp);
        while (iterations--) {
            temp = (SQ(dy) / (SQ(dx))) + 1.0f;
            var_fa1 = (((iterations != 0) ? temp_fs3 : -temp_fs3) + sp6C) / (2.0f * temp);
            if (var_fa1 >= 0.0f) {
                var_fa1 = sqrtf(var_fa1);
                if (dx < 0.0f) {
                    var_fa1 = -var_fa1;
                }
                sp7C[idx] = arctan2_f(((dy / dx) * var_fa1) - ((arg7 * dx) / (2.0f * var_fa1)), var_fa1);
                idx++;
            }
        }
    }

    switch (idx) {
        case 2:
            if (sp7C[0] < sp7C[1]) {
                return (arg8 != 0) ? sp7C[1] : sp7C[0];
            } else {
                return (arg8 != 0) ? sp7C[0] : sp7C[1];
            }
            break;

        case 1:
            return sp7C[0];
            break;

        default:
            return M_45_DEGREES;
    }
}
#endif

// offset: 0xD08 | func: 13
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_func_D08.s")
#else

static f32 dll_228_func_938(s16 a0, f32 a1, f32 a2);

void dll_228_func_D08(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    s16 angle;
    s16 yawDiff;
    DLL228_DataActual* objData;
    Object* player;
    f32 dx;
    f32 dz;

    player = get_player();
    objData = baddie->objdata;

    angle = (u16)arctan2_f(self->srt.transl.x - player->srt.transl.x, self->srt.transl.z - player->srt.transl.z);
    yawDiff = angle - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(yawDiff);
    
    objData->unk10 += dll_228_func_938(yawDiff, objData->unk10, 45);
    self->srt.yaw += objData->unk10;
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_18, bss_28);
    self->animObj = baddie->unk3AC;
}
#endif

// offset: 0xEC0 | func: 14
void dll_228_func_EC0(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    DLL228_DataActual* objData;
    Object* target;
    Baddie_Setup* objSetup;

    objData = baddie->objdata;
    target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
    objSetup = (Baddie_Setup*)self->setup;
    
    if ((target != NULL) && !(baddie->unk3B0 & 4)) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0, -1);
        fsa->unk33D = 0;
        fsa->target = target;
        baddie->unk3B6 = 1;
        return;
    }
    
    if ((objData->unk0 > 0.0f) && ((fsa->logicState != 3) || (baddie->unk3B0 & 1))) {
        if (objData->unk4 <= objData->unk0) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0, -1);
            objData->unk0 = 0.0f;
            fsa->hitpoints = objSetup->quarterHitpoints * 4;
            self->srt.transl.x = objSetup->base.x;
            self->srt.transl.y = objSetup->base.y;
            self->srt.transl.z = objSetup->base.z;
            self->srt.yaw = objSetup->unk2A << 8;
            return;
        }
        
        objData->unk0 += gUpdateRateF;
    }
}

// offset: 0x10BC | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_func_10BC.s")

// offset: 0x1394 | func: 16
static void dll_228_func_1394(Object* self) {
    s32 index;
    s32 count;
    Object* obj;
    Object** objects;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        obj = objects[index];
        if ((self != obj) && (obj->id == OBJ_Caictua)) {
            ((DLL_Unknown*)obj->dll)->vtbl->func[8].withThreeArgsS32(obj, 0x81, 0);
        }
    }
}

// offset: 0x1460 | func: 17
s32 dll_228_func_1460(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s16 angle;
    DLL228_DataActual* objData;
    s16 yawDiff;
    Object* player;
    Baddie* baddie;
    Vec3f v;
    Vec3s16 vCactus16;
    Vec3s16 vPlayer16;
    u8 sp37;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 1;
    
    if (fsa->enteredAnimState) {
        func_8002674C(self);
    }
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    
    if (objData->unk12 >= gUpdateRate) {
        objData->unk12 -= gUpdateRate;
    } else {
        //Check if the caictua has line-of-sight to the player
        player = get_player();
        
        //Get yaw diff
        angle = (u16)arctan2_f(self->srt.transl.x - player->srt.transl.x, self->srt.transl.f[2] - player->srt.transl.f[2]);
        yawDiff = angle - (self->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(yawDiff);
        
        //If the player is within a 90 viewing wedge
        if ((-M_45_DEGREES < yawDiff) && (yawDiff < M_45_DEGREES)) {
            v.f[0] = self->srt.transl.x; 
            v.f[1] = self->srt.transl.y + 55.0f; 
            v.f[2] = self->srt.transl.z; 
            func_80007EE0(&v, &vCactus16);

            v.f[0] = player->srt.transl.x;
            v.f[1] = player->srt.transl.y + 25.0f; 
            v.f[2] = player->srt.transl.z; 
            func_80007EE0(&v, &vPlayer16);

            if (func_80008048(&vPlayer16, &vCactus16, NULL, &sp37, 0) || (sp37 == 1)) {
                objData->unk12 = rand_next(120, 240);
                return 2;
            }
        }
    }

    return 0;
}

// offset: 0x16B0 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/228_Caictua/dll_228_func_16B0.s")

// offset: 0x17B4 | func: 19
s32 dll_228_func_17B4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->unk341 = 3;
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->animTickDelta = 0.015f;
    
    if (fsa->enteredAnimState) {
        dll_228_func_1394(self);
        gDLL_6_AMSFX->vtbl->play(self, 0x720, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    return 0;
}

// offset: 0x1888 | func: 20
s32 dll_228_func_1888(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL228_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    fsa->unk341 = 3;
    
    if (fsa->enteredAnimState) {
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        self->unkAF |= 8;
        gDLL_18_objfsa->vtbl->func21(self, fsa, 0x3C, 0xA, 0);
        gDLL_6_AMSFX->vtbl->play(self, 0x723, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_800267A4(self);
    }
    
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->animTickDelta = 0.0f;

    if (self->opacity >= (gUpdateRate * 2)) {
        self->opacity -= (gUpdateRate * 2);
    } else {
        self->opacity = 0;
    }
    
    if (fsa->unk33A && (self->opacity == 0) && (objData->unk0 == 0.0f)) {
        objData->unk0 = 1.0f;
        main_set_bits(baddie->unk39E, 0);
        main_set_bits(baddie->unk39C, 1);
        func_80023D30(self, 0, 0.0f, 0);
        baddie->unk3B6 = 0;
        fsa->animState = 0;
    }
    
    return 0;
}

// offset: 0x1A74 | func: 21
s32 dll_228_func_1A74(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        fsa->unk27C = 0.0f;
        fsa->unk278 = 0.0f;
        return 6;
    }
    
    return 0;
}

// offset: 0x1AB4 | func: 22
s32 dll_228_func_1AB4(s32 self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return 3;
    }
    
    if (fsa->unk33A) {
        return 6;
    }
    
    return 0;
}

// offset: 0x1AF0 | func: 23
s32 dll_228_func_1AF0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    DLL228_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredLogicState) {
        objData->unk0 = 0.0f;
        objData->unkC = 0.0f;
        objData->unk8 = 0.0f;
        fsa->animState = 3;
    } else if (fsa->unk33A) {
        obj_send_mesg_many(0, 3, self, 0xE0000, self);
        
        if (self->setup == NULL) {
            obj_destroy_object(self);
        }

        return 4;
    }
    
    return 0;
}

// offset: 0x1BAC | func: 24
s32 dll_228_func_1BAC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x1C14 | func: 25
s32 dll_228_func_1C14(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL228_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (fsa->enteredLogicState != 0) {
        objData->unk0 = 0.0f;
        objData->unkC = 0.0f;
        objData->unk8 = 0.0f;
        fsa->animState = 0;
    }
    return 0;
}

// offset: 0x1C58 | func: 26
s32 dll_228_func_1C58(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->unk33A || fsa->enteredLogicState) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1)) {
            return 5;
        }
        
        fsa->animState = 0;
    }
    
    return 0;
}
