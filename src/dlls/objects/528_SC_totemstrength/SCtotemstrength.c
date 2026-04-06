#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/fonts.h"
#include "sys/map_enums.h"

#include "dlls/objects/210_player.h"
#include "dlls/objects/519_SC_levelcontrol.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
} SCTotemStrength_Setup;

typedef struct {
    Object* lightfoot; //Muscle LightFoot
    f32 unk4;
    f32 unk8;
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    u32 soundHandle;
    u32 soundHandleLightFoot;
    u8 unk20;
    Vec3f home; //initial position
    f32 soundTimerKrystal;
    f32 soundTimerLF;
} SCTotemStrength_Data;

/*0x0*/ static u16 dSoundsLightFoot[3] = {
    SOUND_90F, SOUND_910, SOUND_911
};
/*0x8*/ static u16 dSoundsKrystal[3] = {
    SOUND_D9_Krystal_Hurt_Agh, SOUND_E1, SOUND_E7
};

// offset: 0x0 | ctor
void SCTotemStrength_ctor(void *dll) { }

// offset: 0xC | dtor
void SCTotemStrength_dtor(void *dll) { }

static int SCTotemStrength_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 a3);
static void SCTotemStrength_func_F44(Object* self, u8 arg1);

// offset: 0x18 | func: 0 | export: 0
void SCTotemStrength_setup(Object* self, SCTotemStrength_Setup* objSetup, s32 arg2) {
    SCTotemStrength_Data* objData;

    objData = self->data;
    self->animCallback = SCTotemStrength_anim_callback;
    self->unkB0 |= 0x6000;
    self->srt.yaw = -0x2900;
    objData->unkC = -0x2900;
    objData->unk16 = 0;
    objData->lightfoot = NULL;

    objData->home.x = self->srt.transl.x;
    objData->home.y = self->srt.transl.y;
    objData->home.z = self->srt.transl.z;

    main_set_bits(BIT_780, 1);
}

// offset: 0xB0 | func: 1 | export: 1
void SCTotemStrength_control(Object* self) {
    SCTotemStrength_Data* objData;

    objData = self->data;
    
    if ((gDLL_29_Gplay)->vtbl->get_map_setup(MAP_SWAPSTONE_CIRCLE) == 6) {
        if (objData->unk20 & 4) {
            objData->unk20 &= ~4;
            self->srt.transl.x = objData->home.x;
            self->srt.transl.y = objData->home.y;
            self->srt.transl.z = objData->home.z;
            self->srt.yaw = -0x2900;
            objData->unkC = self->srt.yaw;
            objData->lightfoot = NULL;
            main_set_bits(BIT_780, 1);
        }
        
        if (objData->unk20 & 2) {
            if (objData->unk16 == 0) {
                self->srt.yaw = -0x2900;
                objData->unkC = self->srt.yaw;
                objData->unk16 = 1;
                objData->unk20 &= ~1;
                objData->unk4 = 0.0f;
                objData->unk8 = 0.0f;
                return;
            }
            
            if (objData->unk16 == 1) {
                objData->unk10 = gDLL_3_Animation->vtbl->func17(0, self, -1);
                return;
            }
            
            if (objData->unk16 == 2) {
                objData->unk20 &= ~3;
                objData->unk16 = 0;
                objData->unkC = -0x46DC;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_3_Animation->vtbl->func28(objData->unk10, objData->unkC);
                main_set_bits(BIT_784, 1);
                gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                objData->soundHandle = 0;
                return;
            }
            
            if (objData->unk16 == 3) {
                objData->unk20 &= ~3;
                objData->unk16 = 0;
                objData->unkC = -0xB24;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_912_Object_Refused, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_2_Camera->vtbl->change_mode(3, 0);
                gDLL_3_Animation->vtbl->func28(objData->unk10, objData->unkC);
                main_set_bits(BIT_786, 1);
                gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                objData->soundHandle = 0;
                return;
            }
        }
    }
}

// offset: 0x38C | func: 2 | export: 2
void SCTotemStrength_update(Object *self) { }

// offset: 0x398 | func: 3 | export: 3
void SCTotemStrength_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    //@bug?: doesn't check visibility argument
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x3E0 | func: 4 | export: 4
void SCTotemStrength_free(Object* self, s32 arg1) {
    SCTotemStrength_Data* objData;

    objData = self->data;

    if (objData->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x448 | func: 5 | export: 5
u32 SCTotemStrength_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x458 | func: 6 | export: 6
u32 SCTotemStrength_get_data_size(Object *self, u32 a1) {
    return sizeof(SCTotemStrength_Data);
}

// offset: 0x46C | func: 7
static int SCTotemStrength_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    static s32 bss_0;

    s32 i;
    f32 temp_fs0;
    f32 var_fv1;
    Object** objects;
    s32 index;
    s32 count;
    SCTotemStrength_Data* objData;
    f32 sp78;
    Object* player;

    objData = self->data;
    player = get_player();
    objData->unk20 |= 4;
    main_set_bits(BIT_780, 0);
    
    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case 1:
            objData->unk20 |= 1;
            break;
        case 2:
            objData->unk16 = 0;
            objData->unk20 |= 2;

            objects = get_world_objects(&index, &count);
            while (index < count) {
                objData->lightfoot = objects[index++];
                if (objData->lightfoot->id == OBJ_SC_musclelightf) {
                    index = count;
                }
            }
            
            func_80023D30(player, 0x401, 0.0f, 0);
            func_80023D30(objData->lightfoot, 0, 1.0f, 0);
            gDLL_3_Animation->vtbl->func19(0x5A, 3, 0, 0);
            break;
        case 3:
            SCTotemStrength_func_F44(self, 3);
            break;
        case 4:
            SCTotemStrength_func_F44(self, 4);
            break;
        case 5:
            ((DLL_210_Player*)player->dll)->vtbl->set_health(player, 0x7F);
            ((DLL_210_Player*)player->dll)->vtbl->set_magic(player, 0xFF);
            //break;
        default:
            break;
        }
    }
    
    if (!(objData->unk20 & 1)) {
        return 0;
    }
    
    animData->unk7A = -1;
    animData->unk62 = 0;

    objects = get_world_objects(&index, &count);
    
    //Find the Muscle LightFoot
    while (index < count) {
        objData->lightfoot = objects[index++];
        if (objData->lightfoot->id == OBJ_SC_musclelightf) {
            index = count;
        }
    }
    
    if (objData->soundHandle == 0) {
        objData->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_776, MAX_VOLUME, 0, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->func_954(objData->soundHandle, 0.8f);
    }

    //Loop for each frame skipped
    for (i = 0; i < gUpdateRate; i++) {
        if (objData->lightfoot == NULL) {
            return 0;
        }
        
        temp_fs0 = (objData->unkC + 0xB24) / -15288.0f;
        sp78 = (2.0f * temp_fs0) + -1.0f;
        if (sp78 < 0/*.0f*/) {
            var_fv1 = -sp78;
        } else {
            var_fv1 = sp78;
        }

        sp78 = ((temp_fs0 * 1.7f) + 0.2f);
        temp_fs0 = var_fv1 * sp78 + 1.0f;
        
        if (joy_get_released_buffered(0, i) & A_BUTTON) {
            objData->unk8 -= 2.5f; //@bug: makes things harder at smoother FPS, since it won't decrease as much per press
        }
        if (objData->unk8 < -40.0f) {
            objData->unk8 = -40.0f;
        }
        
        if ((objData->unkC >= -0x46DC) && (objData->unkC < -0xB23)) {
            objData->unkC += objData->unk8;
        }

        sp78 = ((f32) bss_0 - (f32) objData->unkC) / 40.0f;
        
        if (objData->unkC < -0x46DC) {
            bss_0 = -0x2900;
            objData->unk16 = 2;
            func_80023D30(player, 0, 0.0f, 0);
            gDLL_3_Animation->vtbl->func18(objData->unk10);
            return 4;
        }
    
        gDLL_3_Animation->vtbl->func28(objData->unk10, objData->unkC);
        
        if (objData->unkC >= -0xB23) {
            bss_0 = -0x2900;
            objData->unk16 = 3;
            func_80023D30(player, 0, 0.0f, 0);
            gDLL_3_Animation->vtbl->func18(objData->unk10);
            return 4;
        }
        
        if (objData->unk8 < 40.0f) {
            objData->unk8 += (0.2f * temp_fs0);
        }
        
        if (func_80024108(player, ((f32) bss_0 - (f32) objData->unkC) / 9500.0f, gUpdateRateF, 0) != 0) {
            if ((((f32) bss_0 - (f32) objData->unkC) / 9500.0f) < 0.0f) {
                func_80023D30(player, 0x401, 1.0f, 0);
            } else {
                func_80023D30(player, 0x401, 0.0f, 0);
            }
        }
        if (func_80024108(objData->lightfoot, -(((f32) bss_0 - (f32) objData->unkC) / 9500.0f), gUpdateRateF, 0) != 0) {
            if (-(((f32) bss_0 - (f32) objData->unkC) / 9500.0f) < 0.0f) {
                func_80023D30(objData->lightfoot, 0, 1.0f, 0);
            } else {
                func_80023D30(objData->lightfoot, 0, 0.0f, 0);
            }
        }

        bss_0 = objData->unkC;
    }

    objData->soundTimerKrystal -= gUpdateRateF;
    if (objData->soundTimerKrystal < 0.0f) {
        if (sp78 < 0.0f) {
            objData->soundTimerKrystal = rand_next(40, 100);
        } else {
            objData->soundTimerKrystal = rand_next(120, 240);
        }
        gDLL_6_AMSFX->vtbl->play_sound(self, dSoundsKrystal[rand_next(0, 2)], MAX_VOLUME, 0, 0, 0, 0);
    }
    
    objData->soundTimerLF -= gUpdateRateF;
    if (objData->soundTimerLF < 0.0f) {
        if (sp78 > 0.0f) {
            objData->soundTimerLF = rand_next(40, 100);
        } else {
            objData->soundTimerLF = rand_next(120, 240);
        }
        gDLL_6_AMSFX->vtbl->play_sound(self, dSoundsLightFoot[rand_next(0, 2)], MAX_VOLUME, &objData->soundHandleLightFoot, 0, 0, 0);
    }
    
    if (sp78 < 0/*.0f*/) {
        sp78 = -sp78;
    }
    
    gDLL_6_AMSFX->vtbl->func_954(objData->soundHandle, (sp78 * 0.3f) + 0.85f);
    gDLL_6_AMSFX->vtbl->func_860(objData->soundHandle, (u8)(sp78 * 64) + 32);

    return 0;
}

// offset: 0xF44 | func: 8
void SCTotemStrength_func_F44(Object* self, u8 arg1) {
    Object** objects;
    s32 index;
    s32 count;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        if ((self != objects[index]) && (objects[index]->id == OBJ_SC_levelcontrol)) {
            ((DLL_519_SC_Levelcontrol*)objects[index]->dll)->vtbl->func7(objects[index], arg1);
            return;
        }
    }
}

/*0x0*/ static const char str_0[] = "Enable music change\n";
