#include "common.h"
#include "game/gamebits.h"
#include "macros.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    ObjSetup base;
} DLL461_Setup;

typedef struct {
    u8 state;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    Object* player; // krystal
    Object* sidekick; // kyte
    Object *unkC;
    Object *unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} CCsandwormBoss_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000003, 0x00000006, 0x00000007
};
/*0xC*/ static u32 _data_C[] = {
    0x0000005a, 0x0000005b, 0x0000032b, 0x00000335
};

static void dll_461_func_330(Object* self, CCsandwormBoss_Data* objData);
static void dll_461_func_564(Object* self, CCsandwormBoss_Data* objData);
static void dll_461_func_5E0(Object* self, CCsandwormBoss_Data *objData);
static void dll_461_func_1030(Object* self, CCsandwormBoss_Data* objData);
static void dll_461_func_1090(Object* self, Object* arg1, CCsandwormBoss_Data* objData, s32 arg3);
static void dll_461_func_1250(Object* self, CCsandwormBoss_Data* objData);
static void dll_461_func_12B0(Object* self, Object* a1);
static void dll_461_move_towards_point(Object* self, Vec3f* a1, f32 a2);
static void dll_461_func_14B0(Object* aself0, CCsandwormBoss_Data* objData);
static void dll_461_func_1540(Object* self, CCsandwormBoss_Data* objData);
static int dll_461_func_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void dll_461_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_461_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_461_setup(Object* self, DLL461_Setup* objSetup, s32 reset) {
    CCsandwormBoss_Data* objData;

    objData = self->data;
    
    self->srt.flags |= OBJFLAG_INVISIBLE;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    func_800267A4(self);
    self->animCallback = dll_461_func_anim_callback;
    obj_add_object_type(self, OBJTYPE_Baddie);
    
    if (main_get_bits(BIT_3FB)) {
        if (main_get_bits(BIT_3D4) == 0) {
            objData->state = 3;
        } else {
            objData->state = 15;
        }
        return;
    }
    
    if (main_get_bits(BIT_1EB)) {
        
        if (main_get_bits(BIT_1EC)) {
            if (main_get_bits(BIT_22A)) {
                objData->state = 2;
            } else {
                main_set_bits(BIT_1EB, 0);
                main_set_bits(BIT_22A, 1);
                objData->state = 0;
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xC, 1);
            }
        } else if (main_get_bits(BIT_22A)) {
            main_set_bits(BIT_1EB, 0);
            main_set_bits(BIT_1EC, 1);
            objData->state = 0;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xC, 1);
        } else {
            main_set_bits(BIT_1EB, 0);
            main_set_bits(BIT_22A, 1);
            objData->state = 0;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xC, 1);
        }

        return;
    }
    
    objData->state = 0;
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xC, 1);
}

// offset: 0x2C8 | func: 1 | export: 1
void dll_461_control(Object* self) {
    CCsandwormBoss_Data* objData = self->data;
    
    if (objData->state < 3) {
        dll_461_func_330(self, objData);
    } else {
        dll_461_func_5E0(self, objData);
    }
}

// offset: 0x330 | func: 2
void dll_461_func_330(Object* self, CCsandwormBoss_Data* objData) {
    if (objData->player == NULL) {
        objData->player = get_player();
    }
    
    if (objData->sidekick == NULL) {
        objData->sidekick = get_sidekick();
    }
    
    if (objData->unkC == NULL) {
        objData->unkC = obj_get_nearest_type_to(OBJTYPE_UseObj, self, NULL);
    }

    switch (objData->state) {
    case 0:
        if (main_get_bits(BIT_1EB)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xB, 1);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xC, 0);
            objData->state = 1;
            return;
        }
    case 3:
        return;
    case 1:
        if (main_get_bits(BIT_1EC) && main_get_bits(BIT_22A)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xB, 0);
            dll_461_func_564(self, objData);
            gDLL_3_Animation->vtbl->start_obj_sequence(4, objData->unkC, -1);
        }
        break;
    case 2:
        dll_461_func_564(self, objData);
        gDLL_3_Animation->vtbl->start_obj_sequence(4, objData->unkC, 2);
        break;
    }
}

// offset: 0x564 | func: 3
void dll_461_func_564(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = 4;
    objData->unk14 = 0.005f;
    self->srt.flags &= ~0x4000;
    func_8002674C(self);
    func_80026128(self, 0xA, 1, -1);
}

/*0x0*/ static const char str_0[] = "need to prempt fire crystal into correct position\n";

// offset: 0x5E0 | func: 4
void dll_461_func_5E0(Object *self, CCsandwormBoss_Data *objData) {
    ObjSetup* setup;
    f32 dist;
  
    setup = self->setup;
    objData->unk18 += gUpdateRateF;
    dist = M_INFINITY_F;
    obj_get_nearest_type_to(OBJTYPE_Pickup, self, &dist);

    diPrintf("worm %d, barrel %d\n", (s32) vec3_distance_xz(&self->globalPosition, &objData->player->globalPosition), (s32) dist);
    
    switch (objData->state) {
    case 4:
        dll_461_func_12B0(self, objData->player);
        if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < 32400.0f) {
            dll_461_func_1090(self, objData->player, objData, 5);
        } else {
            if (((DLL_ISidekick*)objData->sidekick->dll)->vtbl->func24(objData->sidekick) != 0) {
                diPrintf("kyte dist %d interest range 50.0F\n", (s32) vec3_distance_xz(&self->globalPosition, &objData->sidekick->globalPosition));
                if (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < 3600.0f) {
                    dll_461_func_1250(self, objData);
                    objData->unk18 = 0.0f;
                }
            }
        }
        break;
    case 5:
        dll_461_func_12B0(self, objData->player);
        if (self->animProgress > 0.95f) {
            dll_461_func_1030(self, objData);
        }
        break;
    case 6:
        dll_461_func_12B0(self, objData->player);
        if (self->animProgress > 0.95f) {
            dll_461_func_1250(self, objData);
        }
        break;
    case 7:
        dll_461_func_12B0(self, objData->sidekick);
        if (self->animProgress > 0.95f) {
            dll_461_func_1250(self, objData);
        }
        break;
    case 8:
        dll_461_func_12B0(self, objData->player);
        if (self->animProgress > 0.95f) {
            objData->state = 13;
            objData->unk14 = 0.005f;
            func_80023D30(self, 5, 0, 0);
        }
        dll_461_func_14B0(self, objData);
        break;
    case 9:
        dll_461_func_12B0(self, objData->sidekick);

        if (objData->unk18 > 300.0f) {
            STUBBED_PRINTF("setting flight group to %d\n", 0x65);
            main_set_bits(BIT_Kyte_Flight_Curve, 0x65);
        } else {
            STUBBED_PRINTF("setting flight group to %d\n", 0xC3);
            main_set_bits(BIT_Kyte_Flight_Curve, 0xC3);
        }

        if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < 32400.0f) {
            dll_461_func_1090(self, objData->player, objData, 6);
        } else if (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < 32400.0f) {
            dll_461_func_1090(self, objData->sidekick, objData, 7);
        } else {
            if ((((DLL_ISidekick*)objData->sidekick->dll)->vtbl->func24(objData->sidekick) != 0) || (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < 90000.0f)) {
                func_8002493C(self, 1.5f, &objData->unk14);
                dll_461_move_towards_point(self, &objData->sidekick->srt.transl, 1.5f);
            } else if (vec3_distance_xz_squared(&self->globalPosition, (Vec3f* ) &setup->x) < 10000.0f) {
                dll_461_func_1030(self, objData);
            } else {
                objData->state = 10;
                objData->unk14 = 0.01f;
                func_80023D30(self, 2, 0, 0);
            }
        }
        break;
    case 10:
        if (self->animProgress > 0.95f) {
            objData->state = 11;
        }
        break;
    case 11:
        if (((s32) setup->x == (s32) self->srt.transl.f[0]) && ((s32) setup->z == (s32) self->srt.transl.f[2])) {
            dist = vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition);
            if (dist < 2500.0f) {
                objData->unk3 = 0;
                gDLL_3_Animation->vtbl->start_obj_sequence(8, objData->unkC, -1);
            } else if (dist < 32400.0f) {
                objData->state = 12;
                objData->unk14 = 0.005f;
                func_80023D30(self, 8, 0, 0);
                gDLL_6_AMSFX->vtbl->play(self, _data_C[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                objData->unk2 = 0;
                objData->unk1 = 3;
            } else {
                dist = 50.0f;
                objData->unk10 = obj_get_nearest_type_to(OBJTYPE_Pickup, self, &dist);
                if ((objData->unk10 != NULL) && (gDLL_54_pickup->vtbl->get_state(objData->unk10->data) == PICKUP_NotHeld)) {
                    objData->state = 13;
                    objData->unk3 = 0;
                    objData->unk18 = 0.0f;
                    gDLL_3_Animation->vtbl->start_obj_sequence(5, objData->unkC, -1);
                } else {
                    objData->state = 12;
                    objData->unk14 = 0.01f;
                    func_80023D30(self, 0xB, 0, 0);
                    objData->unk2 = 0;
                    objData->unk1 = 3;
                }
            }
        } else {
            dll_461_move_towards_point(self, (Vec3f* ) &setup->x, 3.0f);
            if (vec3_distance_xz_squared(&self->globalPosition, (Vec3f* ) &setup->x) < 10000.0f) {
                objData->unk2 = 1;
            }
        }
        break;
    case 12:
        dll_461_func_12B0(self, objData->player);
        if (self->animProgress > 0.95f) {
            dll_461_func_1030(self, objData);
        }
        break;
    case 13:
        if (objData->unk10 != NULL) {
            setup = objData->unk10->setup;
            objData->unk10->srt.transl.f[0] = setup->x;
            objData->unk10->srt.transl.f[1] = setup->y;
            objData->unk10->srt.transl.f[2] = setup->z;
            objData->unk10 = NULL;
        }
        if (objData->unk18 > 3000.0f) {
            objData->state = 4;
        } else if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < 32400.0f) {
            dll_461_func_1090(self, objData->player, objData, 8);
        }
        dll_461_func_14B0(self, objData);
        break;
    case 14:
        objData->state = 15;
        self->srt.flags |= OBJFLAG_INVISIBLE;
        func_800267A4(self);
        func_80026160(self);
        main_set_bits(BIT_3FB, 1);
        break;
    case 15:
        return;
    }

    func_80024108(self, objData->unk14, gUpdateRateF, 0);
    dll_461_func_1540(self, objData);
}

// static const char str_1[] = "setting flight group to %d\n";
// static const char str_2[] = "setting flight group to %d\n";
static const char str_3[] = "eat player sequence\n";
static const char str_4[] = "attack player from under ground\n";
static const char str_5[] = "eat barrel\n";
static const char str_5b[] = "get up without attack\n";
static const char str_6[] = "barrel %x put to %f %f %f\n";
static const char str_7[] = "worm at %f %f %f\n";
static const char str_8[] = "boss dead\n";
static const char str_9[] = "turning on parts\n";
static const char str_10[] = "Parts all over\n";
static const char str_11[] = "yaw to player\n";

// offset: 0x1030 | func: 5
void dll_461_func_1030(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = 4;
    objData->unk14 = 0.005f;
    func_80023D30(self, 5, 0.0f, 0);
}

// offset: 0x1090 | func: 6
void dll_461_func_1090(Object* self, Object* arg1, CCsandwormBoss_Data* objData, s32 arg3) {
    if (vec3_distance_xz_squared(&self->globalPosition, &arg1->globalPosition) < 8100.0f) {
        objData->state = arg3;
        objData->unk14 = 0.02f;
        func_80023D30(self, 0x100, 0.0f, 0U);
        gDLL_6_AMSFX->vtbl->play(self, _data_C[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
        return;
    }
    objData->state = arg3;
    objData->unk14 = 0.009f;
    func_80023D30(self, _data_0[rand_next(0, 2)], 0.0f, 0);
    gDLL_6_AMSFX->vtbl->play(self, _data_C[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0x1250 | func: 7
void dll_461_func_1250(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = 9;
    objData->unk14 = 0.005f;
    func_80023D30(self, 0xF, 0.0f, 0);
}

// offset: 0x12B0 | func: 8
static void dll_461_func_12B0(Object* self, Object* obj) {
    s16 angle;
    s32 yawDiff;
    s16 yaw;

    angle = (u16)arctan2_f(self->srt.transl.x - obj->srt.transl.x, self->srt.transl.z - obj->srt.transl.z);
    yaw = self->srt.yaw;
    yawDiff = yaw - ((u16)angle);
    CIRCLE_WRAP(yawDiff);
    
    if ((yawDiff >> 8) > 0x10) {
        self->srt.yaw = yaw - 0x1000;
    } else if ((yawDiff >> 8) < -0x10) {
        self->srt.yaw = yaw + 0x1000;
    } else {
        self->srt.yaw = angle;
    }
}

// offset: 0x1384 | func: 9
void dll_461_move_towards_point(Object* self, Vec3f* point, f32 speed) {
    f32 d[2];
    f32 magnitude;
    f32 initialDistance;
    s32 pad;

    initialDistance = vec3_distance_xz_squared(&self->srt.transl, point);
 
    //Get 2D unit vector towards the point 
    d[0] = point->f[0] - self->srt.transl.x;
    d[1] = point->f[2] - self->srt.transl.z;
    magnitude = sqrtf(SQ(d[0]) + SQ(d[1]));
    
    //Move along the unit vector
    self->srt.transl.f[0] += (d[0] / magnitude) * speed * gUpdateRateF;
    self->srt.transl.f[2] += (d[1] / magnitude) * speed * gUpdateRateF;
    
    //If the worm was closer to the destination beforehand, restore that position
    if (initialDistance < vec3_distance_xz_squared(&self->srt.transl, point)) {
        self->srt.transl.x = point->f[0];
        self->srt.transl.z = point->f[2];
    }
}

// offset: 0x14B0 | func: 10
void dll_461_func_14B0(Object* self, CCsandwormBoss_Data* objData) {
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Projectile) {
        objData->unk3 = 0;
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objData->state = 14;
    }
}

// offset: 0x1540 | func: 11
static void dll_461_func_1540(Object* self, CCsandwormBoss_Data* objData) {
    SRT fxTransform;

    if (objData->unk2 == 0) {
        fxTransform.transl.y = 15.0f;
        fxTransform.transl.x = rand_next(-40, 40);
        fxTransform.transl.z = rand_next(-40, 40);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3DE, &fxTransform, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3DE, &fxTransform, 0, -1, NULL);
    }
}

// offset: 0x1634 | func: 12 | export: 2
void dll_461_update(Object *self) { }

// offset: 0x1640 | func: 13 | export: 3
void dll_461_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CCsandwormBoss_Data* objData = self->data;
    u8 var_a3;
/*0x1C*/ u8 _data_1C[] = {0xff, 0xc0, 0x96};
    SRT fxTransform; //38

    if (!visibility) {
        return;
    }
    
    if (objData->state == 0xD) {
        objData->unk1C += gUpdateRateF * 5.0f;

        if (objData->unk1C > 382.0f) {
            objData->unk1C -= 382.0f;
        }
        
        if (objData->unk1C >= 191.5f) {
            var_a3 = 382 - objData->unk1C;
        } else {
            var_a3 = (s32) objData->unk1C;
        }
        func_80036FBC(0xC8, 0, 0, var_a3);
    }
    
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    if (objData->unk2 == 0) {
        if (objData->state == 0xB) {
            func_80031F6C(self, 2, &fxTransform.transl.x, &fxTransform.transl.y, &fxTransform.transl.z, 0);
        } else {
            func_80031F6C(self, 0, &fxTransform.transl.x, &fxTransform.transl.y, &fxTransform.transl.z, 0);
        }
        
        fxTransform.yaw = 0;
        fxTransform.pitch = 0;
        fxTransform.roll = 0;
        fxTransform.flags = 0;
        fxTransform.transl.y = self->srt.transl.y + 15.0f;
        fxTransform.transl.x -= fsin16_precise(self->srt.yaw) * 10.0f;
        fxTransform.transl.z -= fcos16_precise(self->srt.yaw) * 10.0f;
        fxTransform.scale = 1.7f;
        
        gDLL_17_partfx->vtbl->spawn(self, 0x56, &fxTransform, 0x200001, -1, &_data_1C);
        gDLL_17_partfx->vtbl->spawn(self, 0x56, &fxTransform, 0x200001, -1, &_data_1C);
        gDLL_17_partfx->vtbl->spawn(self, 0x57, &fxTransform, 0x200001, -1, &_data_1C);
        
        if (objData->unk1) {
            gDLL_17_partfx->vtbl->spawn(self, 0x58, &fxTransform, 0x200001, -1, &_data_1C);
            objData->unk1--;
        }
    }
}

// offset: 0x1A20 | func: 14 | export: 4
void dll_461_free(Object* self, s32 onlySelf) {
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xB, 0);
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0xC, 0);
    obj_free_object_type(self, 4);
}

// offset: 0x1AC0 | func: 15 | export: 5
u32 dll_461_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1AD0 | func: 16 | export: 6
u32 dll_461_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(CCsandwormBoss_Data);
}

// offset: 0x1AE4 | func: 17
int dll_461_func_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    CCsandwormBoss_Data *objData;
    CCsandwormBoss_Data *objData2;
    s32 i;
    
    objData = self->data;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 1:
            objData->unk2 = 0;
            /* fallthrough */
        case 2:
            objData->unk1 = 3;
            break;
        case 3:
            objData->unk3 = 1;
            break;
        }
    }
    
    dll_461_func_1540(self, self->data);
    
    objData2 = objData;
    if (objData2->unk3 != 0) {
        dll_461_func_12B0(self, objData2->player);
    }
    
    return 0;
}
