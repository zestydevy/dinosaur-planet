#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/541_DIMexplosion.h"

/*0x0*/ static DLL_IModgfx* _data_0 = NULL;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
} DLL537_Setup; //used by DIMCannon

typedef struct {
    Object* unk0;
    Vec3f unk4;
    f32 unk10;
    Vec3f unk14;
    s16 unk20;
    s16 unk22;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    s8 unk27;
} DLL537_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} DIMCannonBall_Setup;

typedef struct {
    s8 unk0;
} DIMCannonBall_Data;

typedef struct {
    u8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} Unk_Data;

static int dll_537_func_A94(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);
static void dll_537_func_1150(Object* self);
static void dll_537_func_1314(Object* self, DIMCannonBall_Setup* objSetup);
static void dll_537_func_1640(Object* self);
static void dll_537_func_16AC(Object* self);

// offset: 0x0 | ctor
void dll_537_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_537_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_537_setup(Object* self, DLL537_Setup* objSetup, s32 arg2) {
    DLL537_Data* objData;

    obj_init_mesg_queue(self, 4);

    if (self->id == OBJ_DIMCannonBall) {
        dll_537_func_1314(self, (DIMCannonBall_Setup*)objSetup);
    } else {
        obj_set_update_priority(self, OBJPRIORITY_MOBILE_MAP);
        objData = self->data;
        self->unkAF |= 8;
        self->animCallback = dll_537_func_A94;
        self->srt.yaw = objSetup->unk28 << 8;
        _data_0 = dllLoadDeferred(DLL_ID_137, 1);
        
        if (mainGetBits(objSetup->unk1A)) {
            objData->unk27 = 60;
            objData->unk24 = 5;
        } else if (mainGetBits(objSetup->unk1C)) {
            objData->unk24 = 4;
        }
    }

    obj_add_object_type(self, OBJTYPE_Baddie);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x178 | func: 1 | export: 1
void dll_537_control(Object *self);
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/537_DIMCannon/dll_537_control.s")
#else

static void dll_537_func_1430(Object* self);
static void dll_537_func_DAC(Object* self, f32 arg1, f32 arg2, f32 arg3, f32 arg4);

void dll_537_control(Object* self) {
    DLL537_Data* objData;
    DLL537_Setup* objSetup; //50
    Object* player; //4C
    Object* sidekick; //48
    f32 var_fv0;
    u8 temp_v0;
    Object* sp3C; //3C
    u32 var_v0;

    objSetup = (DLL537_Setup*)self->setup;
    sidekick = get_sidekick();
    
    if (self->id == OBJ_DIMCannonBall) {
        dll_537_func_1430(self);
        return;
    }
    
    if (self->unkAF & ARROW_FLAG_8_No_Targetting) {
        if (mainGetBits(objSetup->unk1A)) {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    }
    
    var_v0 = 0;
    objData = self->data;
    if (sidekick != NULL) {

        objData->unk26 = ((DLL_Unknown*)sidekick->dll)->vtbl->func[24].withOneArgS32(sidekick);
        if (objData->unk24 != 6) {
            ((DLL_Unknown*)sidekick->dll)->vtbl->func[14].withTwoArgs(sidekick, 2);
        } else {
            var_v0 = objData->unk26;
            if (var_v0 != 0) {
                ((DLL_Unknown*)sidekick->dll)->vtbl->func[21].withThreeArgs(sidekick, 0, 0);
                var_v0 = objData->unk26 = 0;
            }
        }
    } else {
        objData->unk26 = var_v0;
    }
    
    if ((var_v0 = objData->unk26)) {
        objData->unk0 = sidekick;
    } else {
        player = get_player();
        if (((DLL_Unknown*)player->dll)->vtbl->func[7].withOneArgS32(player)) {
            objData->unk0 = NULL;
        } else {
            objData->unk0 = player;
        }
    }
    
    if ((self->curModAnimId == 1) && (self->animProgress >= 1.0f)) {
        func_80023D30(self, 0, 0, 0);
    }
    
    self->srt.flags &= ~OBJFLAG_INVISIBLE;
    
    switch (objData->unk24) {
    case 5:
        if (objData->unk27 > 0) {
            objData->unk27 -= gUpdateRate;
        } else if (self->unkAF & 1) {
            sp3C = self;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMCANNON, TRUE, 0, sizeof(&sp3C), &sp3C, 0x32, Cam_Ease_All);
            objData->unk24 = 3;
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
            objData->unk27 = 0x3C;
            self->unkAF |= 8;
        }
        objData->unk25 = 0;
        objData->unk20 = 0;
        objData->unk22 = 0;
        break;
    case 4:
        dll_537_func_DAC(self, objData->unk4.x, objData->unk4.y, objData->unk4.z, objData->unk10);
        if (mainGetBits(objSetup->unk1A)) {
            objData->unk24 = 5;
        } else if ((objData->unk0 != NULL) && (mainGetBits(objSetup->unk1E) == 0) && (vec3_distance_xz_squared(&self->globalPosition, &objData->unk0->globalPosition) < (((f32) objSetup->unk26 * 250000.0f) / 100.0f))) {
            objData->unk24 = 1;
        }
        objData->unk25 = 0;
        objData->unk20 = 0;
        objData->unk22 = 0;
        break;
    case 1:
        if (mainGetBits(objSetup->unk1A)) {
            objData->unk24 = 5;
        } else if (mainGetBits(objSetup->unk1E)) {
            objData->unk24 = 4;
        } else {
            if (objData->unk0 != NULL) {
                objData->unk4.x = objData->unk0->srt.transl.x;
                objData->unk4.y = objData->unk0->srt.transl.y;
                objData->unk4.z = objData->unk0->srt.transl.z;
                
                if (objData->unk20 > 0) {
                    objData->unk20 -= gUpdateRate;
                }
                
                if (objData->unk22 > 0) {
                    objData->unk22 -= gUpdateRate;
                }
                
                objData->unk10 = vec3_distance_xz_squared(&self->globalPosition, &objData->unk0->globalPosition);
                if ((objData->unk10 < (f32) SQ(objSetup->unk2B)) && (objData->unk26 == 0)) {
                    sidekick = get_sidekick();
                    if (sidekick != NULL) {
                        ((DLL_Unknown*)sidekick->dll)->vtbl->func[21].withThreeArgs(sidekick, 0, 0);
                    }
                    mainSetBits(objSetup->unk20, 1);
                    objData->unk24 = 6;
                } else {
                    dll_537_func_DAC(self, objData->unk4.x, objData->unk4.y, objData->unk4.z, objData->unk10);
                    dll_537_func_1150(self);
                    if (((objSetup->unk26 * 260100.0f) / 100.0f) < objData->unk10) {
                        objData->unk24 = 4;
                    }
                }
            } else {
                objData->unk24 = 4;
            }
        }
        break;
    case 6:
        if (objData->unk0 != NULL) {
            objData->unk4.x = objData->unk0->srt.transl.x;
            objData->unk4.y = objData->unk0->srt.transl.y;
            objData->unk4.z = objData->unk0->srt.transl.z;
            objData->unk10 = vec3_distance_xz_squared(&self->globalPosition, &objData->unk0->globalPosition);
            if (((objSetup->unk26 * 90000.0f) / 100.0f) < objData->unk10) {
                mainSetBits(objSetup->unk22, 1);
                objData->unk24 = 1;
            }
        }
        break;
    case 2:
        if ((objData->unk10 < SQ(objSetup->unk2B)) && (objData->unk26 == 0)) {
            sidekick = get_sidekick();
            if (sidekick != NULL) {
                ((DLL_Unknown*)sidekick->dll)->vtbl->func[21].withThreeArgs(sidekick, 0, 0);
            }
            mainSetBits(objSetup->unk20, 1);
            objData->unk24 = 6;
        }
        break;
    }

    if ((self->curModAnimId == 0) || (self->curModAnimId != 1)) {
        var_fv0 = 0.01f;
    } else {
        var_fv0 = 0.025f;
    }
    func_80024108(self, var_fv0, gUpdateRateF, 0);
}

#endif

// offset: 0x8F0 | func: 2 | export: 2
void dll_537_update(Object *self) { }

// offset: 0x8FC | func: 3 | export: 3
void dll_537_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL537_Data* objData;

    if (self->id != OBJ_DIMCannonBall) {
        objData = self->data;
        if (visibility) {
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
            func_80031F6C(self, 1, &objData->unk14.x, &objData->unk14.y, &objData->unk14.z, 0);
        }
    } else if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x9BC | func: 4 | export: 4
void dll_537_free(Object* self, s32 arg1) {
    if (self->id == OBJ_DIMCannonBall) {
        dll_537_func_1640(self);
    } else {
        dllFree(_data_0);
    }
    
    obj_free_object_type(self, OBJTYPE_Baddie);
}

// offset: 0xA48 | func: 5 | export: 5
u32 dll_537_get_model_flags(Object* self) {
    if (self->id == OBJ_DIMCannonBall) {
        return MODFLAGS_NONE;
    } else {
        return MODFLAGS_NONE;
    }
}

// offset: 0xA6C | func: 6 | export: 6
s32 dll_537_get_data_size(Object* self, s32 arg1) {
    if (self->id == OBJ_DIMCannonBall) {
        // return sizeof(DIMCannonBall_Data);
        return 1; //TODO: figure out why the size is 1 here, but dll_537_func_16AC needs a longer struct
    } else {
        return sizeof(DLL537_Data);
    }
}

// offset: 0xA94 | func: 7
int dll_537_func_A94(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    DLL537_Data* objData;
    DLL537_Setup* objSetup;
    s32 pad;

    animData->unk62 = 0;
    animData->unk7A &= ~(0x400 | 0x200 | 8);
    
    objData = self->data;

    pad = objData->unk24 & 0xFF;
    switch (pad) {
    case 3:
        if (objData->unk27 > 0) {
            objData->unk27 -= gUpdateRate;
        } else {      
            s16* jointAngle;
            s32 angle;
            
            jointAngle = func_80034804(self, 0);
            angle = -*jointAngle;
            self->srt.yaw -= joyGetStickX(0) * 4;

            if (objData->unk20 > 0) {
                objData->unk20 -= gUpdateRate;
            }
    
            if (objData->unk22 > 0) {
                objData->unk22 -= gUpdateRate;
            }
            
            if ((joyGetButtons(0) & A_BUTTON) && (objData->unk20 <= 0)) {
                angle += 800;
            } else {
                angle -= 1200;
            }
            
            if (angle > M_45_DEGREES) {
                angle = M_45_DEGREES;
            }
            if (angle < 0) {
                angle = 0;
            }
            
            if ((joyGetReleased(0) & A_BUTTON) && (objData->unk20 <= 0)) {
                objData->unk25 = 1;
            }
            dll_537_func_1150(self);
            
            if (joyGetPressed(0) & Z_TRIG) {
                gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
                objData->unk24 = 5;
                objData->unk27 = 60;
                animData->unk9D |= 4;
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            }
            
            angle = -angle;
            angle -= (u16)(*jointAngle);
            CIRCLE_WRAP(angle);            
            *jointAngle += angle >> 2;
        }
        break;
    default:
        self->srt.flags &= ~OBJFLAG_INVISIBLE;
        if (animData->lastMessage == 1) {
            objSetup = (DLL537_Setup*)self->setup;
            mainSetBits(objSetup->unk18, 1);
        }
        
        animData->lastMessage = 0;
        if (objData->unk24 != 6) {
            objData->unk24 = 4;
        }
        break;
    } 

    return 0;  
}

// offset: 0xDAC | func: 8
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/537_DIMCannon/dll_537_func_DAC.s")
#else
void dll_537_func_DAC(Object* self, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 pad2;
    f32 temp_fv1;
    f32 var_fs1;
    s32 var_s1;
    // f32 pad;
    s32 var_s2;
    f32 temp_fv0_2; //temp_fv0_2
    f32 temp_fs0; //sp 80
    s16* sp78; //78
    s32 dYaw;
    f32 sp70; //70
    s32 var_s3;
    f32 temp_fa0;
    f32 temp_fs1;
    f32 temp_fs2;
    s32 pad = 0;
    DLL537_Data* objData; //58

    
    objData = self->data;
    if (objData->unk22 > 0) {
        return;
    }

    sp78 = func_80034804(self, 0);
    
    temp_fs2 = 2500.0f;
    arg4 = sqrtf(arg4);
    temp_fs1 = arg4 * 2.2f;

    temp_fs0 = (temp_fs2 = SQ(temp_fs2) - SQ(temp_fs1));
    
    var_s2 = 0;
    if (temp_fs2 >= 0) {
        var_s1 = arctan2_f(temp_fs1, sqrtf(temp_fs2)) >> 1;
    } else {
        var_s1 = 0x2000;
        var_s2 = 1;
    }
    
    temp_fv0_2 = (objData->unk14.y - arg2) - 10.0f;
    if (temp_fv0_2 > 0.0f) {
        var_s3 = -0xB6;
    } else {
        var_s3 = 0xB6;
    }
    
    while (var_s2 == 0) {
        f32 temp = 4.0f;
        temp_fs0 = (temp * 1.1f) * temp_fv0_2;

        sp70 = fsin16_precise(var_s1) * 50.0f;
        temp_fv1 = SQ(sp70) - temp_fs0;
        
        
        if (temp_fs0 <= SQ(sp70)) {
            f32 temp1 = -1.1f;
            f32 temp2 = -1.1f;

            temp_fv1 = sqrtf(temp_fv1);
            if (temp1 + temp2) {  
            }

                //fake
                sp70++;
                sp70--;
        }

        temp_fs1 = fcos16_precise(var_s1) * 50.0f;
        temp_fs0 = temp_fs1 * temp_fv1;
        
        var_s1 += var_s3;
        
        if ((arg4 < temp_fs0) && (var_s3 > 0)) {
            var_s2 = 1;
        }
        if ((temp_fs0 < arg4) && (var_s3 < 0)) {
            var_s2 = 1;
        }
        if (var_s1 > 0x2000) {
            var_s1 = 0x2000;
            var_s2 = 1;
        } else if (var_s1 < 0) {
            var_s1 = 0;
            var_s2 = 1;
        }
    }

    var_s1 = -var_s1;
    var_s1 -= (*sp78 & 0xFFFF);
    CIRCLE_WRAP(var_s1);
    
    *sp78 += (var_s1 >> 2);

    arg1 -= self->srt.transl.x;
    arg3 -= self->srt.transl.z;
    dYaw = arctan2_f(arg1, arg3) - (self->srt.yaw & 0xFFFF);

    CIRCLE_WRAP(dYaw);
    if (dYaw > 0x1000) {
        dYaw = 0x1000;
    }
    if (dYaw < -0x1000) {
        dYaw = -0x1000;
    }
    if ((dYaw < 0x800) && (dYaw >= -0x7FF)) {
        objData->unk25 = 1;
    }
    if (objData->unk10 < 10000.0f) {
        objData->unk25 = pad;
    }
    self->srt.yaw += dYaw >> 2;
}
#endif

// offset: 0x1150 | func: 9
void dll_537_func_1150(Object* self) {
    DIMCannonBall_Setup* shotSetup;
    Object* shot;
    s16* angle;
    DLL537_Data* objData;
    DLL537_Setup* objSetup;

    objData = self->data;
    objSetup = (DLL537_Setup*)self->setup;
    
    if (objData->unk25 && (objData->unk20 <= 0)) {
        angle = func_80034804(self, 0);
        
        shotSetup = (DIMCannonBall_Setup*)obj_alloc_setup(sizeof(DIMCannonBall_Setup), OBJ_DIMCannonBall);
        shotSetup->base.loadFlags = objSetup->base.loadFlags;
        shotSetup->base.byte6 = objSetup->base.byte6;
        shotSetup->base.byte5 = objSetup->base.byte5;
        shotSetup->base.fadeDistance = objSetup->base.fadeDistance;
        shotSetup->base.x = objData->unk14.x;
        shotSetup->base.y = objData->unk14.y;
        shotSetup->base.z = objData->unk14.z;
        shotSetup->unk18 = self->srt.yaw >> 8;
        shotSetup->unk1A = fsin16_precise(*angle) * 50.0f;
        shotSetup->unk1C = fcos16_precise(*angle) * 50.0f;
        
        shot = obj_create((ObjSetup*)shotSetup, 5, self->mapID, -1, NULL);
        shot->unkC4 = self;
        
        objData->unk25 = 0;
        objData->unk22 = 50;
        
        if (objData->unk24 == 3) {
            objData->unk20 = 100;
        } else {
            objData->unk20 = rand_next(objSetup->unk29, objSetup->unk2A);
        }
        
        func_80023D30(self, 1, 0, 0);
    }
}

// offset: 0x1314 | func: 10
void dll_537_func_1314(Object* self, DIMCannonBall_Setup* objSetup) {
    f32 verticalSpeed;
    f32 lateralSpeed;
    DIMCannonBall_Data* objData;
    ObjectHitInfo* objHits;
    s32 hitsValue = 1;

    self->srt.yaw = objSetup->unk18 << 8;
    verticalSpeed = objSetup->unk1A * 0.1f;
    lateralSpeed = objSetup->unk1C * 0.1f;
    self->velocity.x = fsin16_precise(self->srt.yaw) * lateralSpeed;
    self->velocity.y = -verticalSpeed;
    self->velocity.z = fcos16_precise(self->srt.yaw) * lateralSpeed;
    
    self->unkDC = 0;

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_OPACITY | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->flags |= OBJ_SHADOW_FLAG_8000;
        self->shadow->maxDistScale = 2.0f * self->shadow->scale;
    }

    objData = self->data;
    objData->unk0 = 1;

    objHits = self->objhitInfo;
    if (objHits != NULL) {
        objHits->unkA1 = hitsValue;
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
}

// offset: 0x1430 | func: 11
void dll_537_func_1430(Object* self) {
    ObjectHitInfo* objHits;
    s32 pad;
    Unk_Data* objData;
    
    self->velocity.y += -0.022f * gUpdateRateF;
    obj_move(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);

    objHits = self->objhitInfo;
    if (objHits != NULL) {
        func_80026128(self, 5, 1, 0);
        if ((objHits->unk48 != NULL) && (objHits->unk48 != self->unkC4)) {
            dll_537_func_16AC(self);
            obj_destroy_object(self);
        }
    }
    
    if (self->objhitInfo->unk9D != 0) {
        dll_537_func_16AC(self);
        obj_destroy_object(self);
    }
    
    self->unkDC += gUpdateRate;
    if (self->unkDC > 1200) {
        obj_destroy_object(self);
    }
    
    objData = self->data;
    
    self->srt.pitch = arctan2_f(self->velocity.y, sqrtf(SQ(self->velocity.x) + SQ(self->velocity.z)));
    
    if (objData->unk0 != 0) {
        _data_0->vtbl->func0(self, 2, 0, 0x10002, -1, 0);
        objData->unk0 = 0;
    }
}

// offset: 0x1640 | func: 12
void dll_537_func_1640(Object* self) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x16AC | func: 13
void dll_537_func_16AC(Object* self) {
    Unk_Data* objData; //This should be the cannonball's data, but `dll_537_get_data_size` says `DIMCannonBall_Data` has a size of 1?
    DIMExplosion_Setup* explosion;

    objData = self->data;
    
    explosion = (DIMExplosion_Setup*)obj_alloc_setup(sizeof(DIMExplosion_Setup), OBJ_DIMExplosion);
    explosion->base.loadFlags = objData->unk4; //Is objData storing an objSetup here? The offsets match as it's setting up the explosion's objSetup
    explosion->base.byte6 = objData->unk6;
    explosion->base.byte5 = objData->unk5;
    explosion->base.fadeDistance = objData->unk7;
    explosion->base.x = self->srt.transl.x;
    explosion->base.y = self->srt.transl.y;
    explosion->base.z = self->srt.transl.z;
    obj_create((ObjSetup*)explosion, 5, self->mapID, -1, self->parent);
}
