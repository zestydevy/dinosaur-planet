#include "common.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/math.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/267_checkpoint4.h"

typedef struct {
    s8 unk0[0xE];
    s8 unkE;
    s8 unkF;
    s8 unk10;
} DLL732_Unk_2E0; //Controller/joystick-related

typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} DLL732_Data2AC;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    u8 unk20;
} DLL732_Setup;

typedef struct {
    SRT unk0;
    RaceStruct unk18;
    s8 _unk3C[0x48 - 0x3C];
    u8 unk48;
    u8 unk49;
    DLL27_Data unk4C;
    DLL732_Data2AC unk2AC;
    DLL732_Unk_2E0 unk2E0;
    DLL_IModgfx* unk2F4; //DLL
    DLL_IModgfx* unk2F8; //DLL
    s16* unk2FC; //soundIDs?
    s8 _unk300[0x330 - 0x300];
    Vec3f unk330[6];
    s8 _unk378[0x384 - 0x378];
    f32 unk384;
    Vec3f unk388;
    f32 unk394;
    f32 unk398;
    f32 unk39C;
    Vec3f unk3A0; //previous position?
    Vec3f unk3AC;
    u32 unk3B8; //soundHandle
    u32 unk3BC; //soundHandle
    u32 unk3C0; //soundHandle
    u32 unk3C4; //soundHandle
    s32 unk3C8; //total damage taken?
    f32 unk3CC;
    f32 unk3D0;
    f32 unk3D4;
    s8 _unk3D8[0x3DC - 0x3D8];
    s16 unk3DC;
    s16 unk3DE;
    s16 unk3E0; //yaw
    s16 unk3E2; //pitch
    s16 unk3E4; //roll
    s16 unk3E6;
    s8 _unk3E8[0x3EA - 0x3E8];
    s16 unk3EA;
    s8 unk3EC;
    u8 unk3ED;
    u8 unk3EE;
    u8 unk3EF; //flags
    s8 unk3F0;
    u8 unk3F1;
    s8 unk3F2;
    s8 unk3F3;
    u8 unk3F4_0 : 1; //flags
} DLL732_Data; //0x3F8

/*0x0*/ static s16 data_0[][3] = {
    { 0x0497, 0x0498, 0x049c },
    { 0x0479, 0x077e, 0x077f }
};
/*0xC*/ static u32 data_C[][3] = {
    {0x00030c60, 0x00030c60, 0x00030c60}, 
    {0x00034dd3, 0x00034dc7, 0x00034dc9}
};
/*0x24*/ static Vec3f data_24[] = {
    VEC3F(-6.5, 0, -7),
    VEC3F(6.5, 0, -7),
    VEC3F(6.5, 0, 7),
    VEC3F(-6.5, 0, 7)
};
/*0x54*/ static f32 data_54[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x64*/ static Vec3f data_64[] = {
    VEC3F(0, 1, -18), 
    VEC3F(-6.5, 1, -7), 
    VEC3F(6.5, 1, -7), 
    VEC3F(-6, 1, 11), 
    VEC3F(6, 1, 11)
};
/*0xA0*/ static f32 data_A0[] = {
    5, 4, 4, 4.5, 4.5, -5, 0, 0, 
    5, 0, 0, 0, 2.0001223, 0, 0, 0, 
    2.0157478, 0, 0, 0
};
/*0xF0*/ static f32 data_F0[] = {
    14.5, 0, 9, -14.5, 0, 9
};

/*0x0*/ static Texture* bss_0;
/*0x4*/ static Texture* bss_4;
/*0x8*/ static Texture* bss_8;
/*0x10*/ static SRT bss_10;
// /*0x28*/ static u32 bss_28;

static s32 dll_732_func_0(Object* self, DLL732_Data* objData, f32 arg2);
void dll_732_func_1C38(Object* self);
static void dll_732_func_1DC8(Object* self, DLL732_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols);
static void dll_732_func_2340(Object* self, DLL732_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4);
static void dll_732_func_22BC(Object* self, DLL732_Data2AC* arg1);
static void dll_732_func_2E64(Object* self, DLL732_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4);
static void dll_732_func_3618(Object* self, DLL732_Unk_2E0* arg1, u8 controllerPort, s32 buffer);
static void dll_732_func_3694(Object* self, DLL732_Data* objData, MtxF* arg2, s32 addToYaw, s32 useRoll, s32 usePitch);
static void dll_732_func_3748(Object* self, DLL732_Data* objData);
static int dll_732_func_3860(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void dll_732_func_3AF8(Object* self, DLL732_Data* objData, DLL27_Data* collision);
static s32 dll_732_func_3DAC(Object* self, s32 arg1, DLL732_Data* objData, DLL732_Unk_2E0* arg3);
static void dll_732_func_3FE0(Object* self, DLL732_Data* objData);
static void dll_732_func_40FC(Object* self, DLL732_Data* objData, f32 arg2, s32 arg3, s8* arg6, u8 arg5);

// offset: 0x0 | func: 0
static s32 dll_732_func_0(Object* self, DLL732_Data* objData, f32 arg2) {
    s32 pad;
    s32 sp30;
    DLL732_Data* objData2;

    objData2 = self->data;
    if (gDLL_4_Race->vtbl->func8(objData->unk18.unk10, &sp30)->unk20[1] == -1) {
        objData2->unk3F4_0 = rand_next(0, 1);
    }
    return gDLL_4_Race->vtbl->func5(&objData->unk0, &objData->unk18, arg2, 1, 0, objData2->unk3F4_0);
}

// offset: 0xE4 | ctor
void dll_732_ctor(void* dll) { }

// offset: 0xF0 | dtor
void dll_732_dtor(void* dll) { }

// offset: 0xFC | func: 1 | export: 0
void dll_732_setup(Object* self, DLL732_Setup* setup, s32 reset) {
    s32 pad;
    s32 pad2;
    s32 var_v0;
    DLL732_Setup* objSetup; //40
    u8 data_108[] = {5, 5, 5, 5};
    DLL732_Data* objData;

    objSetup = (DLL732_Setup*)self->setup;
    objData = self->data;
    bzero(objData, sizeof(DLL732_Data));
    
    objData->unk3E0 = (setup->unk18 & 0xFF) << 8;
    objData->unk2F4 = 0;
    objData->unk2F8 = 0;
    self->srt.yaw = objData->unk3E0;

    self->animCallback = dll_732_func_3860;
    
    gDLL_27->vtbl->init(&objData->unk4C, 0, 0x01040007, 1);
    gDLL_27->vtbl->setup_terrain_collider(&objData->unk4C, 4, data_24, data_54, data_108);
    
    if (setup->unk19 != 0) {
        objData->unk3EE = 3;
    } else {
        objData->unk3EE = 5;
    }

    dll_732_func_3FE0(self, objData);
    dll_732_func_22BC(self, &objData->unk2AC);

    func_80023D30(self, 0, 0.0f, 0);

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_4000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }

    if (setup->unk19 != 0) {
        objData->unk3EF |= 0x20;
    }

    objData->unk18.unk10 = -1;
    objData->unk18.unk14 = -1;
    objData->unk18.unk18 = -1;
    objData->unk48 = setup->unk1C;
    objData->unk49 = setup->unk1D;
    
    objData->unk0.transl.x = self->srt.transl.x;
    objData->unk0.transl.y = self->srt.transl.y;
    objData->unk0.transl.z = self->srt.transl.z;
    
    obj_add_object_type(self, OBJTYPE_Vehicle);
    bss_0 = tex_load_deferred(TEXTABLE_186);
    bss_4 = tex_load_deferred(TEXTABLE_89);
    bss_8 = tex_load_deferred(TEXTABLE_3C);
    
    if (main_get_bits(objSetup->unk1E) != 0) {
        var_v0 = 1;
    } else {
        var_v0 = 0;
    }
    objData->unk3EF |= var_v0;
    
    objData->unk2FC = &data_0[setup->unk1C][0];
}

/*0x0*/ static const char str_0[] = " FInished Is SEt for Some Reason \n";
/*0x24*/ static const char str_24[] = " FInished Is SEt for Some Reason \n";

// offset: 0x398 | func: 2 | export: 1
void dll_732_control(Object* self) {
    DLL732_Setup* objSetup;
    Object* player;
    s32 pad;
    f32 spC0;
    MtxF sp80;
    Vec3f sp74;
    DLL732_Data2AC* innerData;
    DLL732_Data* objData;
    DLL732_Unk_2E0* new_var;
    f32 dx;
    f32 dz;
    s32 sp5C;
    f32 var_fv1;
    s32 updateIdx;
    s32 flagValue;
    DLL732_Data2AC* sp44;

    objData = self->data;
    objSetup = (DLL732_Setup*)self->setup;
    
    innerData = &objData->unk2AC;
    new_var = &objData->unk2E0;
    
    spC0 = 0.0f;
    
    if ((objData->unk3EF & 1)) {
        return;
    }
        
    if (main_get_bits(objSetup->unk1E)) {
        objData->unk3EF |= 1;
        return;
    }
    
    player = get_player();
    self->unkAF |= 8;
    objData->unk3A0.f[0] = self->srt.transl.f[0];
    objData->unk3A0.f[1] = self->srt.transl.f[1];
    objData->unk3A0.f[2] = self->srt.transl.f[2];
    
    if (objData->unk3EF & 0x10) {
        objData->unk3EF &= 0xFFEF;
        if (!(objData->unk3EF & 0x20)) {
            innerData->unkC.f[0] = 0.0f;
            innerData->unkC.f[1] = 0.0f;
            innerData->unkC.f[2] = -2.0f;
            objData->unk3DC = 0;
            objData->unk3DE = 0;
            objData->unk3F2 = 0;
            objData->unk3E6 = 0;
            objData->unk3E0 = self->srt.yaw;
            objData->unk3E2 = self->srt.pitch;
            objData->unk3E4 = self->srt.roll;
            dll_732_func_3FE0(self, objData);
        }
    }

    switch (objData->unk3F0) {
    case 0:
        if (!(objData->unk3EF & 0x20)) {
            self->objhitInfo->unk5B = 0;
            self->objhitInfo->unk5C = 0;
            objData->unk3ED = 0;
            self->unkAF &= 0xFFF7;

            if ((objSetup->unk1A == NO_GAMEBIT) || (main_get_bits(objSetup->unk1A) != 0)) {
                self->unkAF &= ~0x10;
            } else {
                self->unkAF |= 0x10;
            }
            
            if ((player != NULL) && (vec3_distance(&player->globalPosition, &self->globalPosition) < 100.0f)) {
                objData->unk3ED = 1;
                dll_732_func_3694(self, objData, &sp80, 0, 1, 1);
                vec3_transform(&sp80, data_F0[0], data_F0[1], data_F0[2], &sp74.z, &sp74.y, &sp74.x);
                dx = player->srt.transl.x - sp74.z;
                dz = player->srt.transl.z - sp74.x;
                if ((SQ(dx) + SQ(dz)) < 100.0f) {
                    self->unkAF &= ~8;
                    objData->unk3ED = 2;
                } else {
                    vec3_transform(&sp80, data_F0[3], data_F0[4], data_F0[5], &sp74.z, &sp74.y, &sp74.x);
                    dx = player->srt.transl.x - sp74.z;
                    dz = player->srt.transl.z - sp74.x;
                    if ((SQ(dx) + SQ(dz)) < 100.0f) {
                        self->unkAF &= 0xFFF7;
                        objData->unk3ED = 1;
                    }
                }
            }
        }

        if (objData->unk3BC != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->unk3BC);
            objData->unk3BC = 0;
        }

        if (objData->unk3C0 != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->unk3C0);
            objData->unk3C0 = 0;
        }

        if (objData->unk3C4 != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->unk3C4);
            objData->unk3C4 = 0;
        }

        if (objData->unk2F4 != NULL) {
            dll_unload(objData->unk2F4);
            objData->unk2F4 = NULL;
        }

        if (objData->unk2F8 != NULL) {
            dll_unload(objData->unk2F8);
            objData->unk2F8 = NULL;
        }

        objData->unk3EF = objData->unk3EF & 0xFFF7;
        break;
    case 2:
        if (!(objData->unk3EF & 8)) {
            objData->unk18.unk10 = -1;
            objData->unk18.unk14 = -1;
            objData->unk18.unk18 = -1;
            objData->unk18.unk1C = 0;
            if (main_get_bits(objData->unk2FC[0]) != 0) {
                flagValue = 8;
            } else {
                flagValue = 0;
            }
            objData->unk3EF |= flagValue;
            if (objData->unk3EF & 8) {
                if (objData->unk3EF & 0x20) {
                    dll_732_func_1C38(self);
                } else {
                    gDLL_4_Race->vtbl->func3(self, &objData->unk18, objSetup->unk1C);
                }
                gDLL_4_Race->vtbl->func9(&objData->unk18);
            }
        } else if (main_get_bits(objData->unk2FC[1]) != 0) {
            objData->unk3EF &= 0xFFF7;
        }
        dll_732_func_3748(self, objData);

        if (objData->unk3EF & 0x20) {
            if (!(objData->unk3EF & 8)) {
                return;
            }
            
            if (map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2]) >= 0) {
                if (objData->unk3EF & 2) {
                    sp5C = dll_732_func_0(self, objData, 2.8f * gUpdateRateF);
                    gDLL_4_Race->vtbl->func4(self, &objData->unk18);
                    gDLL_4_Race->vtbl->func10(&objData->unk18);
                    if (sp5C == 0) {
                        sp44 = &objData->unk2AC;
                        self->srt.yaw = arctan2_f(self->srt.transl.x - objData->unk0.transl.x, self->srt.transl.f[2] - objData->unk0.transl.f[2]);
                        self->srt.transl.x = objData->unk0.transl.x;
                        self->srt.transl.y = objData->unk0.transl.y;
                        self->srt.transl.z = objData->unk0.transl.z;
                        sp44->unkC.f[0] = 0.0f;
                        sp44->unkC.f[1] = 0.0f;
                        sp44->unkC.f[2] = -2.0f;
                        objData->unk3DC = 0;
                        objData->unk3DE = 0;
                        objData->unk3F2 = 0;
                        objData->unk3E6 = 0;
                        objData->unk3E0 = self->srt.yaw;
                        objData->unk3E2 = self->srt.pitch;
                        objData->unk3E4 = self->srt.roll;
                        dll_732_func_3FE0(self, objData);
                        func_80058680(self, self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], &spC0, 0);
                        self->srt.transl.f[1] -= spC0;
                        objData->unk3EF &= ~2;
                    }
                    return;
                } else if (dll_732_func_3DAC(self, objData, objData, &objData->unk2E0) != 0) {
                    return;
                }
            } else {
                sp5C = dll_732_func_0(self, objData, 2.8f * gUpdateRateF);
                gDLL_4_Race->vtbl->func4(self, &objData->unk18);
                gDLL_4_Race->vtbl->func10(&objData->unk18);
                if (sp5C == 0) {
                    self->srt.yaw = arctan2_f(self->srt.transl.x - objData->unk0.transl.x, self->srt.transl.f[2] - objData->unk0.transl.f[2]);
                    self->srt.transl.x = objData->unk0.transl.x;
                    self->srt.transl.f[1] = objData->unk0.transl.f[1];
                    self->srt.transl.f[2] = objData->unk0.transl.f[2];
                    objData->unk3EF |= 2;
                }
                return;
            }
        } else {
            self->objhitInfo->unk5B = 0xA;
            self->objhitInfo->unk5C = 0xA;
        }

        if ((objData->unk3EF & 0x20) == 0) {
            if (objData->unk3EF & 8) {
                gDLL_4_Race->vtbl->func4(self, &objData->unk18);
                gDLL_4_Race->vtbl->func10(&objData->unk18);
                objData->unk3F1 = gDLL_4_Race->vtbl->func12(&objData->unk18);
            }

            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++){
                dll_732_func_3618(self, &objData->unk2E0, 0, updateIdx);
                dll_732_func_2340(self, objData, &objData->unk2AC, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->unk3DC += (s16) ((-objData->unk2E0.unkE * 60.0f) - objData->unk3DC) >> 4;
                objData->unk3DE += (s16) ((-objData->unk2E0.unkE * 105.0f) - objData->unk3DE) >> 4;
                self->srt.yaw = objData->unk3E0 + objData->unk3DC;
                self->srt.roll = objData->unk3E4 + objData->unk3DE;
            }
            
            sp44 = &objData->unk2AC;
            if (objData->unk3C8 >= 0) {
                objData->unk3C8 = objData->unk3C8 - (s32) (VECTOR_MAGNITUDE(sp44->unkC) * gUpdateRateF * 1.5f) - gUpdateRate;
                diPrintf(" FUEL AMT %i \n", objData->unk3C8 / 10);
                gDLL_1_cmdmenu->vtbl->energy_bar_set(objData->unk3C8);
            } else if (objData->unk3CC > 0.1f) {
                diPrintf(" \tRAN OUT OF FUEL \t");
                
                if (rand_next(0, 0xA) == 0) {
                    gDLL_6_AMSFX->vtbl->play(self, 0xB38, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                objData->unk3CC *= 0.95f;
                objData->unk3D0 *= 0.95f;
                objData->unk3D4 *= 0.95f;
                if (objData->unk3CC < 0.1f) {
                    gDLL_1_cmdmenu->vtbl->energy_bar_free();
                    gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                    objData->unk3CC = 0.01f;
                    objData->unk3D0 = 0.01f;
                    objData->unk3D4 = 0.01f;
                }
            }
        } else {
            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++) {
                dll_732_func_2E64(self, objData, &objData->unk2AC, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->unk3DC += (s16) ((-objData->unk2E0.unkE * 60.0f) - objData->unk3DC) >> 4;
                objData->unk3DE += (s16) ((-objData->unk2E0.unkE * 105.0f) - objData->unk3DE) >> 4;
                self->srt.yaw = objData->unk3E0 + objData->unk3DC;
                self->srt.roll = objData->unk3E4 + objData->unk3DE;
            }
        }
        
        if (!(objData->unk3EF & 0x20)) {
            dll_732_func_40FC(self, objData, objData->unk2AC.unkC.f[2], new_var->unkF, &new_var->unk10, 7);
        } else {
            if (objData->unk3B8 != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->unk3B8);
                objData->unk3B8 = 0;
            }
            if (objData->unk3BC != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->unk3BC);
                objData->unk3BC = 0;
            }
            if (objData->unk3C0 != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->unk3C0);
                objData->unk3C0 = 0;
            }
            if (objData->unk3C4 != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->unk3C4);
                objData->unk3C4 = 0;
            }
        }
        break;
    case 1:
        break;
    }

    if (((objData->unk3EF & 0x20) == 0) && (objData->unk3F0 == 2)) {
        if (objData->unk39C >= 0.0f) {
            var_fv1 = objData->unk39C;
        } else {
            var_fv1 = -objData->unk39C;
        }
        if (var_fv1 > 2.0f) {
            self->objhitInfo->unk5F = 0x14;
            self->objhitInfo->unk60 = 1;
            self->objhitInfo->unk58 |= 1;
            self->objhitInfo->unk40 = 0x10;
            self->objhitInfo->unk58 |= 4;
            self->srt.flags &= 0xBFFF;
        }
    }
}

static const char str_extra0[] = " HIT OBJECT %i \n";
static const char str_extra1[] = " Bike Can Mount %i ";
static const char str_extra2[] = "tracks %d\n";
static const char str_extra3[] = "ident %d\n";

// offset: 0x123C | func: 3 | export: 2
void dll_732_update(Object* self) {
    DLL732_Data* objData;
    ObjectHitInfo* objHitInfo;
    Vec3f spDC;
    MtxF sp9C;
    MtxF sp5C;
    DLL732_Data2AC* temp_v0;
    Object* obj;
    s32 objID;
    s32 i;

    objHitInfo = self->objhitInfo;
    objData = self->data;
    temp_v0 = &objData->unk2AC;

    if (objHitInfo->unk48) {
        obj = objHitInfo->unk48;
        objID = obj->id;
        
        switch (objID){
        case OBJ_CRSnowClaw:
        case OBJ_CRSnowClaw2:
            camera_enable_y_offset();
            camera_set_shake_offset(1.0f);
            gDLL_17_partfx->vtbl->spawn(self, 0x551, NULL, 4, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, 0x552, NULL, 4, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, 0x554, NULL, 4, -1, NULL);

            i = 10;
            while (i--) {
                gDLL_17_partfx->vtbl->spawn(self, 0x553, NULL, 2, -1, NULL);
            }
            
            objData->unk3EC = 5;
            objData->unk384 = 0.2f;
            break;
        }
    }
    
    if (!(objData->unk3EF & 0x20)) {
        bss_10.yaw = -objData->unk3E0;
        bss_10.pitch = -objData->unk3E2;
        bss_10.roll = -objData->unk3E4;
        matrix_from_srt_reversed(&sp9C, &bss_10);
        self->velocity.f[0] = (self->srt.transl.x - self->prevLocalPosition.x) * gUpdateRateInverseF;
        self->velocity.f[1] = (self->srt.transl.y - self->prevLocalPosition.y) * gUpdateRateInverseF;
        self->velocity.f[2] = (self->srt.transl.z - self->prevLocalPosition.z) * gUpdateRateInverseF;
        spDC.x = self->velocity.f[0] * 0.93749994f;
        spDC.y = self->velocity.f[1] * 0.93749994f;
        spDC.z = self->velocity.f[2] * 0.93749994f;
        
        vec3_transform(&sp9C, spDC.f[0], spDC.f[1], spDC.f[2], 
                       &temp_v0->unkC.f[0], &temp_v0->unkC.f[1], &temp_v0->unkC.f[2]);
        dll_732_func_3694(self, objData, &sp5C, 0, 0, 0);
        vec3_transform(&sp5C, 0.0f, 0.0f, -10.0f, &objData->unk3AC.x, &objData->unk3AC.y, &objData->unk3AC.z);
    }
}

// offset: 0x1534 | func: 4 | export: 3
void dll_732_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL732_Data* objData;
    s32 sp38;

    objData = self->data;
    if (objData->unk3EF & 1) {
        return;
    }
    
    if (visibility == -1) {
        sp38 = gDLL_13_Expgfx->vtbl->func10(self);
        if (!(objData->unk3EF & 0x20)) {
            dll_732_func_1DC8(self, objData, gdl, mtxs, vtxs, pols);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objData->unk388.x, &objData->unk388.y, &objData->unk388.z, 0);
        if (sp38 != 0) {
            gDLL_13_Expgfx->vtbl->func6(self, gdl, mtxs, NULL, 0, 0, 0);
        }
        return;
    } else if (objData->unk3F0 == 2) {
        objData->unk388.x = self->srt.transl.x;
        objData->unk388.y = self->srt.transl.y;
        objData->unk388.z = self->srt.transl.z;
        return;
    }
    
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objData->unk388.x, &objData->unk388.y, &objData->unk388.z, 0);
    }
}

// offset: 0x1724 | func: 5 | export: 4
void dll_732_free(Object* self, s32 onlySelf) {
    DLL732_Data* objData;

    objData = self->data;

    if (objData->unk3B8 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3B8);
        objData->unk3B8 = 0;
    }

    if (objData->unk3BC != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3BC);
        objData->unk3BC = 0;
    }

    if (objData->unk3C0 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3C0);
        objData->unk3C0 = 0;
    }

    if (objData->unk3C4 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3C4);
        objData->unk3C4 = 0;
    }

    if (objData->unk2F4 != NULL) {
        dll_unload(objData->unk2F4);
    }
    objData->unk2F4 = NULL;
    
    if (objData->unk2F8 != NULL) {
        dll_unload(objData->unk2F8);
    }
    objData->unk2F8 = NULL;
    
    obj_free_object_type(self, OBJTYPE_Vehicle);
    
    if (!(objData->unk3EF & 0x20)) {
        gDLL_1_cmdmenu->vtbl->energy_bar_free();
    }
    
    tex_free(bss_0);
    tex_free(bss_4);
    tex_free(bss_8);
}

// offset: 0x18D8 | func: 6 | export: 5
u32 dll_732_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0x18E8 | func: 7 | export: 6
u32 dll_732_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL732_Data);
}

// offset: 0x18FC | func: 8 | export: 7
u8 dll_732_func_18FC(Object* self, s32 arg1) {
    DLL732_Data* objData;
    DLL732_Setup* objSetup;

    objData = self->data;
    objSetup = (DLL732_Setup*)self->setup;
    
    if (objData->unk3EF & 0x20) {
        return 0;
    }

    if (objSetup->unk1A != NO_GAMEBIT) {
        if (main_get_bits(objSetup->unk1A) == FALSE) {
            return 0;
        }
    }
    
    return objData->unk3ED;
}

// offset: 0x1980 | func: 9 | export: 8
u8 dll_732_func_1980(Object* self) {
    DLL732_Data* objData = self->data;
    return objData->unk3ED;
}

// offset: 0x1990 | func: 10 | export: 9
void dll_732_func_1990(Object* self, f32* ox, f32* oy, f32* oz) {
    DLL732_Data* objData = self->data;
    
    *ox = objData->unk388.x;
    *oy = objData->unk388.y;
    *oz = objData->unk388.z;
}

// offset: 0x19B4 | func: 11 | export: 10
s32 dll_732_func_19B4(Object* self, s32 arg1) {
    DLL732_Data* objData = self->data;
    
    if (objData->unk3EF & 8) {
        return 0;
    }
    
    if (!(joy_get_pressed(0) & B_BUTTON)) {
        return 0;
    }
    
    return 2;
}

// offset: 0x1A24 | func: 12 | export: 11
s32 dll_732_func_1A24(s32 arg0) {
    return 2;
}

// offset: 0x1A34 | func: 13 | export: 12
void dll_732_func_1A34(Object* self, f32* ox, f32* oy, f32* oz) {
    DLL732_Data* objData = self->data;
    
    *ox = objData->unk3AC.x;
    *oy = objData->unk3AC.y;
    *oz = objData->unk3AC.z;
}

// offset: 0x1A58 | func: 14 | export: 13
s8 dll_732_func_1A58(Object* self) {
    DLL732_Data* objData = self->data;
    return objData->unk3F0;
}

// offset: 0x1A68 | func: 15 | export: 14
void dll_732_func_1A68(Object* self, s32 arg1) {
    DLL732_Data* objData = self->data;
    
    objData->unk3F0 = arg1;
    if ((arg1 == 2) && !(objData->unk3EF & 0x20)) {
        objData->unk3C8 = 0x2710;
        objData->unk3CC = 2.0f;
        objData->unk3D0 = 4.0f;
        objData->unk3D4 = 4.6f;
        gDLL_1_cmdmenu->vtbl->energy_bar_create(0, 13000, TEXTABLE_569, TEXTABLE_56A, 1);
    }
}

// offset: 0x1B10 | func: 16 | export: 15
void dll_732_func_1B10(Object* self, f32* arg1, s32* arg2) {
    DLL732_Data* objData = self->data;
    
    *arg1 = objData->unk3DC / 2500.0f;
    
    if (*arg1 > 1.0f) {
        *arg1 = 1.0f;
    } else if (*arg1 < -1.0f) {
        *arg1 = -1.0f;
    }
    
    *arg2 = objData->unk3DC < 0;
}

// offset: 0x1B9C | func: 17 | export: 16
f32 dll_732_func_1B9C(Object* self, f32* arg1) {
    DLL732_Data* objData;
    f32 magnitude;

    objData = self->data;
    
    *arg1 = 5.0f;

    magnitude = VECTOR_MAGNITUDE(objData->unk2AC.unkC) * 0.2f;
    if (magnitude > 1.0f) {
        magnitude = 1.0f;
    }
    return magnitude;
}

// offset: 0x1C28 | func: 18 | export: 17
s8 dll_732_func_1C28(Object* self) {
    DLL732_Data* objData = self->data;
    return objData->unk3F1;
}

// offset: 0x1C38 | func: 19 | export: 18
void dll_732_func_1C38(Object* self) {
    s32 checkpointUID;
    checkpoint4_Setup* checkpointSetup;
    DLL732_Data* objData;
    DLL732_Setup* objSetup;

    objSetup = (DLL732_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    default:
    case 0x38D:
        checkpointUID = data_C[objSetup->unk1C][0];
        break;
    case 0x38E:
        checkpointUID = data_C[objSetup->unk1C][1];
        break;
    case 0x4D4:
        checkpointUID = data_C[objSetup->unk1C][2];
        break;
    }
    
    checkpointSetup = (checkpoint4_Setup*)map_find_obj_setup(checkpointUID, NULL, NULL, NULL, NULL);
    if (checkpointSetup == NULL) {
        return;
    }
    
    self->srt.transl.x = checkpointSetup->base.x;
    self->srt.transl.y = checkpointSetup->base.y;
    self->srt.transl.z = checkpointSetup->base.z;
    self->srt.yaw = checkpointSetup->yaw << 8;

    dll_732_func_3FE0(self, objData);
    gDLL_4_Race->vtbl->func3(self, &objData->unk18, objSetup->unk1C);
    
    objData->unk0.transl.x = self->srt.transl.x;
    objData->unk0.transl.y = self->srt.transl.y;
    objData->unk0.transl.z = self->srt.transl.z;
    objData->unk0.yaw = self->srt.yaw;
}

// offset: 0x1DB8 | func: 20 | export: 19
void dll_732_func_1DB8(s32 arg0, s32 arg1) {

}

#include "prevent_bss_reordering.h"

// offset: 0x1DC8 | func: 21
void dll_732_func_1DC8(Object* self, DLL732_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols) {
/*0x28*/ static u32 bss_28;
    Vertex* spA4; //A4
    Gfx* spA0; //A0
    Triangle* sp9C; //9C
    s32 pad1[6];
    SRT sp6C; //6C
    s32 i;
    f32 var_fv0;
    s32 pad2;
    DLL732_Data2AC* temp_v1; //3C
    u32 data_10C[] = { 0x00000006, 0x00000069, 0x00000069, 0x000000ff};
    s32 volume; //48
    s32 pad3;

    temp_v1 = &objData->unk2AC;
    spA0 = *gdl;
    spA4 = *vtxs;
    sp9C = *pols;
    
    var_fv0 = 0.0f;
    if (temp_v1->unkC.z < 0.0f) {
        var_fv0 = temp_v1->unkC.z;
    }
    sp6C.transl.z = var_fv0;

    if (temp_v1->unkC.z < 0.0f) {
        var_fv0 = temp_v1->unkC.x;
    }
    sp6C.transl.x = var_fv0;
    
    dl_set_prim_color(&spA0, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if (temp_v1->unkC.z < -0.5f) {
        if (1) { }
        gDLL_17_partfx->vtbl->spawn(self, 0x12E, &sp6C, 4, -1, NULL);
    }
    if (temp_v1->unkC.z < -1.5f) {
        if (1) { }
        gDLL_17_partfx->vtbl->spawn(self, 0x12F, &sp6C, 4, -1, NULL);
    }
    if (temp_v1->unkC.z < -2.1f) {
        if (1) { }
        gDLL_17_partfx->vtbl->spawn(self, 0x130, &sp6C, 4, -1, NULL);
    }
    
    i = 0;
    if (objData->unk2E0.unkF > 0) {
        i = 2;
    }

    while (i) {
        gDLL_17_partfx->vtbl->spawn(self, 0x131, NULL, 4, -1, NULL);
        i--;
    }
    
    sp6C.yaw = 0;
    sp6C.pitch = 0;
    sp6C.roll = 0;
    sp6C.scale = 1.0f;
    if (objData->unk39C < -1.2f) {
        data_10C[1] += rand_next(0, 0x9B);
        data_10C[2] += rand_next(0, 0x9B);
        volume = ((0.0f - objData->unk39C) * 21.0f);
        if (objData->unk3F3 & 1) {
            gDLL_6_AMSFX->vtbl->play(self, 0x292, volume, &bss_28, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(bss_28, (volume / 127.0f) + 0.5f);
        } else if (objData->unk3F3 & 2) {
            gDLL_6_AMSFX->vtbl->play(self, 0x292, volume, &bss_28, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(bss_28, (volume / 127.0f) + 0.5f);
        } else if (objData->unk3F3 & 4) {
            gDLL_6_AMSFX->vtbl->play(self, 0x292, volume, &bss_28, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(bss_28, (volume / 127.0f) + 0.5f);
        }
    }
    
    *gdl = spA0;
    *vtxs = spA4;
    *pols = sp9C;
}

// offset: 0x22BC | func: 22
void dll_732_func_22BC(Object* self, DLL732_Data2AC* arg1) {
    arg1->unk1C = 90;
    arg1->unk20 = 1/arg1->unk1C;
    arg1->unk28 = -11.0f;
    arg1->unk2C = 0.1f;
    arg1->unk30 = 13.0f;
    arg1->unk18 = (1/arg1->unk1C) * 0.01666666f;
    
    bss_10.transl.x = 0;
    bss_10.transl.y = 0;
    bss_10.transl.z = 0;
    bss_10.scale = 1;
}

// offset: 0x2340 | func: 23
void dll_732_func_2340(Object* self, DLL732_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4) {
    MtxF sp130;
    MtxF spF0;
    MtxF spB0;
    s32 pad1;
    f32 temp3;
    f32 temp2;
    f32 temp_fv0;
    f32 temp_fv1_7;
    f32 temp_fa0_4;
    f32 sp94;
    f32 var_fv1;
    Vec3f sp84;
    f32 var_fv0_4;
    f32 var_fa0;
    s32 var_a0;
    Vec3f sp6C;
    Vec3f sp60;
    s32 pad2;
    DLL27_Data* collision; //44
    s32 angle1;
    s32 angle2;
    s32 i;

    collision = &objData->unk4C;
    
    if (objData->unk3EC != 0) {
        VECTOR_SCALE(arg2->unkC, objData->unk384);
        objData->unk3EC--;
        if (objData->unk3EC < 0) {
            objData->unk3EC = 0;
        }
    }

    temp3 = -objData->unk3CC;
    arg2->unkC.f[0] = (arg2->unkC.x < temp3) ? temp3 : (
                      (objData->unk3CC < arg2->unkC.x) ? objData->unk3CC : arg2->unkC.x);
    
    temp3 = -objData->unk3D0;
    arg2->unkC.f[1] = (arg2->unkC.f[1] < temp3) ? temp3 : (
                      (objData->unk3D0 < arg2->unkC.f[1]) ? objData->unk3D0 : arg2->unkC.f[1]);

    temp3 = -objData->unk3D4;
    arg2->unkC.f[2] = (arg2->unkC.f[2] < temp3) ? temp3 : (
                      (objData->unk3D4 < arg2->unkC.f[2]) ? objData->unk3D4 : arg2->unkC.f[2]);
    
    
    bss_10.yaw = objData->unk3E0;
    bss_10.pitch = objData->unk3E2;
    bss_10.roll = objData->unk3E4;
    matrix_from_srt(&sp130, &bss_10);
    bss_10.yaw = -objData->unk3E0;
    bss_10.pitch = -objData->unk3E2;
    bss_10.roll = -objData->unk3E4;
    matrix_from_srt_reversed(&spF0, &bss_10);
    
    if (!(objData->unk3EF & 4)) {
        var_fv0_4 = (f32) -objData->unk2E0.unkF / 60.0f;
        if (var_fv0_4 > 1.0f) {
            var_fv0_4 = 1.0f;
        } else if (var_fv0_4 < -1.0f) {
            var_fv0_4 = -1.0f;
        }
        var_fv0_4 *= 6144.0f;
        objData->unk3E6 += ((s32) (var_fv0_4) - objData->unk3E6) >> 5;
    } else {
        if (objData->unk3E6 != 0) {
            objData->unk3E6 -= (-objData->unk3E6 >> 5);
        }
    }
    
    vec3_transform(&spF0, 0.0f, arg2->unk28 * arg2->unk1C, 0.0f, &sp84.f[0], &sp84.f[1], &sp84.f[2]);

    if (objData->unk2E0.unkF < 0) {
        var_fv1 = -(f32) objData->unk2E0.unkF * 4.0f;
    } else {
        var_fv1 = -(f32) objData->unk2E0.unkF * 10.0f;
    }
    
    temp_fv0 = arg2->unk20 * var_fv1;
    if (temp_fv0 < 0.0f) {
        arg2->unkC.f[2] += (temp_fv0 * 0.01666666f);
    } else {
        if (arg2->unkC.f[2] <= 0.0f) {
            arg2->unkC.f[2] += (temp_fv0 * 0.01666666f);
            if (arg2->unkC.f[2] > 0.0f) {
                arg2->unkC.f[2] = 0.0f;
            }
        }
    }
    
    arg2->unk0.x = sp84.f[0] * arg2->unk18;
    arg2->unk0.y = sp84.f[1] * arg2->unk18;
    arg2->unk0.z = sp84.f[2] * arg2->unk18;
    arg2->unkC.x = arg2->unk0.x + arg2->unkC.x;
    arg2->unkC.y = arg2->unk0.y + arg2->unkC.y;
    arg2->unkC.z = arg2->unk0.z + arg2->unkC.z;
    
    if (collision->unk25D != 0) {
        temp_fv0 = arg2->unk2C * sp84.y;
        if (arg2->unkC.f[2] < 0.0f) {
            if (temp_fv0 < 0.0f) {
                temp_fv0 = -temp_fv0;
            }
        } else if (temp_fv0 > 0.0f) {
            temp_fv0 = -temp_fv0;
        }
        temp_fv0 *= arg2->unk18;
        var_fa0 = arg2->unkC.f[2] + temp_fv0;
        if (arg2->unkC.f[2] < 0.0f) {
            if (var_fa0 > 0.0f) {
                arg2->unkC.f[2] = 0.0f;
            } else {
                arg2->unkC.f[2] = var_fa0;
            }
        } else if (var_fa0 < 0.0f) {
            arg2->unkC.f[2] = 0.0f;
        } else {
            arg2->unkC.f[2] = var_fa0;
        }
        
        if (arg2->unkC.f[2] < 0.0f) {
            var_fa0 = -arg2->unkC.f[2];
        } else {
            var_fa0 = arg2->unkC.f[2];
        }
        
        temp_fv0 = arg2->unk2C * sp84.y * (4.0f + SQ(var_fa0)); //SQ should be on temp?
        if (arg2->unkC.f[0] < 0.0f) {
            if (temp_fv0 < 0.0f) {
                temp_fv0 = -temp_fv0;
            }
        } else if (temp_fv0 > 0.0f) {
            temp_fv0 = -temp_fv0;
        }
        temp_fv0 *= arg2->unk18;
        
        temp_fv1_7 = arg2->unkC.f[0] + temp_fv0;
        if (arg2->unkC.f[0] < 0.0f) {
            if (temp_fv1_7 > 0.0f) {
                arg2->unkC.f[0] = 0.0f;
            } else {
                arg2->unkC.f[0] = temp_fv1_7;
            }
        } else if (temp_fv1_7 < 0.0f) {
            arg2->unkC.f[0] = 0.0f;
        } else {
            arg2->unkC.f[0] = temp_fv1_7;
        }
        
        objData->unk3F2 = 0;
        objData->unk3E6 = 0;
    } else {
        objData->unk3F2++;
        if (objData->unk3F2 > 0x64) {
            objData->unk3F2 = 0x64;
        }
    }

    temp2 = SQ(arg2->unkC.f[2]);
    temp_fv0 = arg2->unk30 * temp2; //SQ on temp?
    if (arg2->unkC.f[2] > 0.0f) {
        temp_fv0 = -temp_fv0;
    }
    temp_fv0 *= arg2->unk18;
    temp_fa0_4 = arg2->unkC.f[2] + temp_fv0;
    if (arg2->unkC.f[2] < 0.0f) {
        if (temp_fa0_4 > 0.0f) {
            arg2->unkC.f[2] = 0.0f;
        } else {
            arg2->unkC.f[2] = temp_fa0_4;
        }
    } else if (temp_fa0_4 < 0.0f) {
        arg2->unkC.f[2] = 0.0f;
    } else {
        arg2->unkC.f[2] = temp_fa0_4;
    }
    
    vec3_transform(&sp130, arg2->unkC.x, arg2->unkC.y, arg2->unkC.z, 
                   &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]);
    VECTOR_SCALE(self->velocity, 1.0666667f);
    obj_move(self, self->velocity.x, self->velocity.f[1], self->velocity.f[2]);
    if (arg4 != 0) {
        sp94 = 1.0f / updateRate;
        dll_732_func_3AF8(self, objData, collision);
        gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, collision);
        gDLL_27->vtbl->func_624(self, collision, updateRate);
        
        self->velocity.x = (self->srt.transl.x - self->prevLocalPosition.x) * sp94;
        self->velocity.y = (self->srt.transl.y - self->prevLocalPosition.y) * sp94;
        self->velocity.z = (self->srt.transl.z - self->prevLocalPosition.z) * sp94;
        
        sp60.f[0] = self->velocity.x * 0.93749994f;
        sp60.f[1] = self->velocity.y * 0.93749994f;
        sp60.f[2] = self->velocity.z * 0.93749994f;
        vec3_transform(&spF0, sp60.f[0], sp60.f[1], sp60.f[2], 
                       &arg2->unkC.x, &arg2->unkC.y, &arg2->unkC.z);
        sp6C.f[0] = 0.0f;
        sp6C.f[1] = 1.0f;
        sp6C.f[2] = 0.0f;
        
        if (collision->unk25C & 0xF) {
            objData->unk3EF |= 4;
        } else {
            objData->unk3EF &= ~4;
        }
        
        for (var_a0 = 0, i = 0; i < 4; i++) {
            if (collision->unk25C & (1 << i)) {
                sp6C.x = collision->unk68.unk0[i].x + sp6C.x;
                sp6C.y = collision->unk68.unk0[i].y + sp6C.y;
                sp6C.z = collision->unk68.unk0[i].z + sp6C.z;
                var_a0++;
            }
        }
        
        VECTOR_SCALE(sp6C, 0.25f);
        if (var_a0 != 0) {
            temp_fv0 = 1.0f / var_a0;
            VECTOR_SCALE(sp6C, temp_fv0);
        } else {
            sp6C.f[0] = 0.0f;
            sp6C.f[1] = 1.0f;
            sp6C.f[2] = 0.0f;
        }
        
        bss_10.yaw = -objData->unk3E0;
        bss_10.pitch = 0;
        bss_10.roll = 0;
        matrix_from_srt_reversed(&spB0, &bss_10);
        vec3_transform(&spB0, sp6C.f[0], sp6C.f[1], sp6C.f[2], &sp6C.f[0], &sp6C.f[1], &sp6C.f[2]);
        
        angle1 = M_90_DEGREES - arctan2_f(sp6C.f[1], sp6C.f[2]);
        angle2 = -(M_90_DEGREES - arctan2_f(sp6C.f[1], sp6C.f[0]));
        angle1 -= (objData->unk3E2 & 0xFFFF);
        CIRCLE_WRAP(angle1);
        objData->unk3E2 += (((angle1 >> 2) / 3) * (s32) updateRate);
        self->srt.pitch = objData->unk3E2 + objData->unk3E6;
        
        angle2 -= (objData->unk3E4 & 0xFFFF);
        CIRCLE_WRAP(angle2);
        objData->unk3E4 += (((angle2 >> 2) / 3) * (s32) updateRate);
    }
    
    objData->unk3E0 -= (s16) (objData->unk2E0.unkE * (70.0f - (objData->unk2E0.unkF * 0.05f)) * 0.0666f);
}

// offset: 0x2E64 | func: 24
void dll_732_func_2E64(Object* self, DLL732_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4) {
    MtxF sp120;
    MtxF spE0;
    MtxF spA0;
    DLL27_Data* collision;
    f32 temp_fv0;
    f32 var_fv1;
    f32 sp90;
    s32 temp_a1;
    Vec3f sp80;
    Vec3f sp74;
    Vec3f sp68;
    s32 pad;
    s32 var_a0;
    s32 var_s0;
    s32 var_v1;
    s32 i;
    f32 temp;

    collision = &objData->unk4C;
    
    if (arg2->unkC.f[0] > 4.0f) {
        arg2->unkC.f[0] = 4.0f;
    }
    if (arg2->unkC.f[0] < -4.0f) {
        arg2->unkC.f[0] = -4.0f;
    }
    if (arg2->unkC.f[1] > 4.0f) {
        arg2->unkC.f[1] = 4.0f;
    }
    if (arg2->unkC.f[1] < -4.0f) {
        arg2->unkC.f[1] = -4.0f;
    }
    if (arg2->unkC.f[2] > 5.0f) {
        arg2->unkC.f[2] = 5.0f;
    }
    if (arg2->unkC.f[2] < -5.8f) {
        arg2->unkC.f[2] = -5.8f;
    }
    
    bss_10.yaw = objData->unk3E0;
    bss_10.pitch = objData->unk3E2;
    bss_10.roll = objData->unk3E4;
    matrix_from_srt(&sp120, &bss_10);
    
    bss_10.yaw = -objData->unk3E0;
    bss_10.pitch = -objData->unk3E2;
    bss_10.roll = -objData->unk3E4;
    matrix_from_srt_reversed(&spE0, &bss_10);
    
    vec3_transform(&spE0, 0.0f, arg2->unk28 * arg2->unk1C, 0.0f, &sp80.f[0], &sp80.f[1], &sp80.f[2]);

    temp_fv0 = -(f32) objData->unk2E0.unkF * 50.0f;
    temp_fv0 *= arg2->unk18;
    arg2->unkC.f[2] += temp_fv0;
    arg2->unk0.f[0] = sp80.f[0] * arg2->unk18;
    arg2->unk0.f[1] = sp80.f[1] * arg2->unk18;
    arg2->unk0.f[2] = sp80.f[2] * arg2->unk18;
    arg2->unkC.f[0] = arg2->unkC.f[0] + arg2->unk0.f[0];
    arg2->unkC.f[1] = arg2->unkC.f[1] + arg2->unk0.f[1];
    arg2->unkC.f[2] = arg2->unkC.f[2] + arg2->unk0.f[2];
    
    if (collision->unk25D != 0) {
        arg2->unkC.f[0] = 0.0f;
        var_fv1 = arg2->unk2C * sp80.y;
        
        if (arg2->unkC.f[2] < 0.0f) {
            if (var_fv1 < 0.0f) {
                var_fv1 = -var_fv1;
            }
        } else if (var_fv1 > 0.0f) {
            var_fv1 = -var_fv1;
        }

        var_fv1 *= arg2->unk18;
        temp_fv0 = arg2->unkC.f[2] + var_fv1;
        if (temp_fv0 > 0.0f) {
            arg2->unkC.f[2] = 0.0f;
        } else {
            arg2->unkC.f[2] = temp_fv0;
        }
    }

    temp = SQ(arg2->unkC.f[2]);
    var_fv1 = arg2->unk30 * temp;
    var_fv1 *= arg2->unk18;
    
    temp_fv0 = arg2->unkC.f[2] + var_fv1;
    if (temp_fv0 > 0.0f) {
        arg2->unkC.f[2] = 0.0f;
    } else {
        arg2->unkC.f[2] = temp_fv0;
    }
    
    vec3_transform(&sp120, arg2->unkC.f[0], arg2->unkC.f[1], arg2->unkC.f[2], &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]);
    VECTOR_SCALE(self->velocity, 1.0666667f);
    obj_move(self, self->velocity.x, self->velocity.f[1], self->velocity.f[2]);
    
    if (arg4 != 0) {
        sp90 = 1.0f / updateRate;
        gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, collision);
        gDLL_27->vtbl->func_624(self, collision, updateRate);
        self->velocity.x = (self->srt.transl.x - self->prevLocalPosition.x) * sp90;
        self->velocity.y = (self->srt.transl.y - self->prevLocalPosition.y) * sp90;
        self->velocity.z = (self->srt.transl.z - self->prevLocalPosition.z) * sp90;
        sp68.f[0] = self->velocity.x * 0.93749994f;
        sp68.f[1] = self->velocity.y * 0.93749994f;
        sp68.f[2] = self->velocity.z * 0.93749994f;
        vec3_transform(&spE0, sp68.f[0], sp68.f[1], sp68.f[2], &arg2->unkC.f[0], &arg2->unkC.f[1], &arg2->unkC.f[2]);
        
        sp74.f[0] = 0.0f;
        sp74.f[1] = 1.0f;
        sp74.f[2] = 0.0f;
        
        if (collision->unk25C & 0xF) {
            objData->unk3EF |= 4;
        } else {
            objData->unk3EF &= ~4;
        }

        for (var_a0 = 0, i = 0; i < 4; i++) {
            if (collision->unk25C & (1 << i)) {
                sp74.f[0] = sp74.f[0] + collision->unk68.unk0[i].f[0];
                sp74.f[1] = sp74.f[1] + collision->unk68.unk0[i].f[1];
                sp74.f[2] = sp74.f[2] + collision->unk68.unk0[i].f[2];
                var_a0++;
            }
        }
        
        if (var_a0 != 0) {
            VECTOR_SCALE(sp74, 1.0f / var_a0);
        } else {
            sp74.f[0] = 0.0f;
            sp74.f[1] = 1.0f;
            sp74.f[2] = 0.0f;
        }
        
        bss_10.yaw = -objData->unk3E0;
        bss_10.pitch = 0;
        bss_10.roll = 0;
        matrix_from_srt_reversed(&spA0, &bss_10);
        vec3_transform(&spA0, sp74.f[0], sp74.f[1], sp74.f[2], &sp74.f[0], &sp74.f[1], &sp74.f[2]);
        
        var_s0 = M_90_DEGREES - arctan2_f(sp74.f[1], sp74.f[2]);
        temp_a1 = -(M_90_DEGREES - arctan2_f(sp74.f[1], sp74.f[0]));
        var_s0 -= (objData->unk3E2 & 0xFFFF);
        CIRCLE_WRAP(var_s0);
        objData->unk3E2 += ((var_s0 >> 2) / 3) * (s32)updateRate;
        self->srt.pitch = objData->unk3E2 + objData->unk3E6;
        temp_a1 -= (objData->unk3E4 & 0xFFFF);
        CIRCLE_WRAP(temp_a1);
        objData->unk3E4 += ((temp_a1 >> 2) / 3) * (s32)updateRate;
    }
    
    objData->unk3E0 -= (s16) (objData->unk2E0.unkE * (70.0f - (objData->unk2E0.unkF * 0.05f)) * 0.0666f);
}

// offset: 0x3618 | func: 25
void dll_732_func_3618(Object* self, DLL732_Unk_2E0* arg1, u8 controllerPort, s32 buffer) {
    arg1->unkE = joy_get_stick_x_buffered(controllerPort, buffer);
    arg1->unkF = joy_get_stick_y_buffered(controllerPort, buffer);
}

// offset: 0x3694 | func: 26
void dll_732_func_3694(Object* self, DLL732_Data* objData, MtxF* arg2, s32 addToYaw, s32 useRoll, s32 usePitch) {
    SRT sp20;

    sp20.yaw = objData->unk3E0;
    sp20.pitch = usePitch ? self->srt.pitch : 0;
    sp20.roll = useRoll ? self->srt.roll : 0;
    sp20.scale = 1.0f;
    sp20.transl.x = self->srt.transl.x;
    sp20.transl.y = self->srt.transl.y;
    sp20.transl.z = self->srt.transl.z;
    
    if (addToYaw) {
        sp20.yaw += objData->unk3DC;
    }
    
    matrix_from_srt(arg2, &sp20);
}

// offset: 0x3748 | func: 27
void dll_732_func_3748(Object* self, DLL732_Data* objData) {
    s32 damageType;
    s32 hitSphereID;
    s32 hitDamage;
    Object* hitBy;

    damageType = func_80025F40(self, &hitBy, &hitSphereID, &hitDamage);
    if (!(self->objhitInfo->unk58 & 1) || (damageType == 0)) {
        return;
    }
    
    switch (damageType) {
    case Damage_Type_D:
        objData->unk3EC = 20;
        objData->unk384 = 0.8f;
        return;
    case Damage_Type_14:
        objData->unk3EC = 5;
        objData->unk384 = 0.75f;
        return;
    case Damage_Type_1D:
        objData->unk3C8 += (hitDamage * 100);
        objData->unk3CC = 2.0f;
        objData->unk3D0 = 4.0f;
        objData->unk3D4 = 4.6f;
        if (objData->unk3C8 > 13000) {
            objData->unk3C8 = 13000;
        }
        break;
    }
}

// offset: 0x3860 | func: 28
int dll_732_func_3860(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DLL732_Data* objData;
    DLL732_Unk_2E0* unkSubstruct;
    DLL732_Data2AC* v;
    Vec3f spB8;
    SRT spA0;
    s32 i;
    MtxF sp5C;

    objData = self->data;
    unkSubstruct = &objData->unk2E0;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 2:
            main_set_bits(0x499, 1);
            break;
        case 3:
            gDLL_1_cmdmenu->vtbl->energy_bar_free();
            break;
        }
    }
    
    if (objData->unk3F0 == 2) {
        spB8.f[0] = (self->srt.transl.x - objData->unk3A0.f[0]) * gUpdateRateInverseF;
        spB8.f[1] = (self->srt.transl.y - objData->unk3A0.f[1]) * gUpdateRateInverseF;
        spB8.f[2] = (self->srt.transl.z - objData->unk3A0.f[2]) * gUpdateRateInverseF;

        spB8.f[0] *= 0.93749994f;
        spB8.f[1] *= 0.93749994f;
        spB8.f[2] *= 0.93749994f;
        
        spA0.transl.x = 0.0f;
        spA0.transl.y = 0.0f;
        spA0.transl.z = 0.0f;
        spA0.scale = 1.0f;
        spA0.yaw = -self->srt.yaw;
        spA0.pitch = -self->srt.pitch;
        spA0.roll = -self->srt.roll;
        v = &objData->unk2AC;
        matrix_from_srt_reversed(&sp5C, &spA0);
        
        vec3_transform(&sp5C, spB8.f[0], spB8.f[1], spB8.f[2], &v->unkC.f[0], &v->unkC.f[1], &v->unkC.f[2]);

        unkSubstruct->unkF += gUpdateRate * 8;
        if (unkSubstruct->unkF > 70) {
            unkSubstruct->unkF = 70;
        }
        dll_732_func_40FC(self, objData, v->unkC.f[2], unkSubstruct->unkF, &unkSubstruct->unk10, 4);
    }
    
    objData->unk3A0.x = self->srt.transl.x;
    objData->unk3A0.y = self->srt.transl.y;
    objData->unk3A0.z = self->srt.transl.z;
    objData->unk3EF |= 0x10;
    objData->unk3EF &= ~8;
    return 0;
}

// offset: 0x3AF8 | func: 29
void dll_732_func_3AF8(Object* self, DLL732_Data* objData, DLL27_Data* collision) {
    s32 pad;
    MtxF spDC;
    Vec3f sp88[7];
    s8 sp87;
    s8 sp86;
    s32 i;

    sp86 = -0x80 >> (7 - objData->unk3EE);
    sp86 = ~sp86;
    objData->unk3F3 = -1;
    objData->unk39C = objData->unk2AC.unkC.f[2];
    
    if (sp86) {
        do {
            sp87 = sp86;
            for (i = 0; i < objData->unk3EE; i++){
                dll_732_func_3694(self, objData, &spDC, 1, 1, 1);
                vec3_transform(&spDC, data_64[i].x, data_64[i].y, data_64[i].z, 
                               &sp88[i].x, &sp88[i].y, &sp88[i].z);
                if (func_80059C40(&objData->unk330[i], 
                        &sp88[i], data_A0[i], 0, NULL, self, 8, 1, 0xFF, 0) == 0) {
                    sp87 &= ~(1 << i);
                } else {
                    bcopy(&sp88[i], &objData->unk330[i], sizeof(Vec3f));
                    spDC.m[3][0] = sp88[i].x;
                    spDC.m[3][1] = sp88[i].y;
                    spDC.m[3][2] = sp88[i].z;
                    vec3_transform(&spDC, -data_64[i].x, -data_64[i].y, -data_64[i].z, 
                        &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z
                    );
                }
            }
            
            if (objData->unk3F3 == -1) {
                objData->unk3F3 = sp87;
            }
        } while (sp87);
    }
    
    bcopy(&sp88, objData->unk330, objData->unk3EE * sizeof(Vec3f));
}

// offset: 0x3DAC | func: 30
s32 dll_732_func_3DAC(Object* self, s32 arg1, DLL732_Data* objData, DLL732_Unk_2E0* arg3) {
    f32 dx;
    f32 dz;
    s32 sp3C;
    f32 var_fv1;
    s32 dYaw;
    s32 angle;
    DLL732_Setup* objSetup;

    objSetup = (DLL732_Setup*)self->setup;
    
    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dx = sqrtf(SQ(dx) + SQ(dz));
    
    var_fv1 = 100.0f - dx;
    if (dx > 100.0f) {
        var_fv1 = 0.0f;
    }
    
    sp3C = dll_732_func_0(self, objData, var_fv1);
    
    gDLL_4_Race->vtbl->func4(self, &objData->unk18);
    gDLL_4_Race->vtbl->func10(&objData->unk18);
    if (sp3C != 0) {
        arg3->unkE = 0;
        arg3->unkF = 0;
        return 1;
    }

    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dYaw = arctan2_f(dx, dz) - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(dYaw);
    
    angle = dYaw >> 5;
    if (angle > 65) {
        angle = 65;
    } else if (angle < -65) {
        angle = -65;
    }
    
    arg3->unkE = -angle;
    if (angle < 0) {
        angle = -angle;
    }

    arg3->unkF = (objSetup->unk20 - 8.0f) - (angle * (objSetup->unk20 * 0.02f));
    diPrintf(" YJOY %i ", arg3->unkF);
    return 0;
}

// offset: 0x3FE0 | func: 31
void dll_732_func_3FE0(Object* self, DLL732_Data* objData) {
    MtxF sp60;
    s32 i;

    gDLL_27->vtbl->reset(self, &objData->unk4C);
    dll_732_func_3694(self, objData, &sp60, 0, 0, 0);
    
    for (i = 0; i < objData->unk3EE; i++) {
        vec3_transform(&sp60, 
            data_64[i].x, data_64[i].y, data_64[i].z, 
            &objData->unk330[i].x, &objData->unk330[i].y, &objData->unk330[i].z
        );
    }
}

// offset: 0x40FC | func: 32
void dll_732_func_40FC(Object* self, DLL732_Data* objData, f32 arg2, s32 arg3, s8* arg6, u8 arg5) {
    /*0x2C*/ static f32 bss_2C;
    f32 sp54 = 1.0f;
    SRT sp3C;
    s32 var_v0;
    
    if (arg5 & 1) {
        if (objData->unk3B8 == 0) {
            gDLL_6_AMSFX->vtbl->play(self, 0x289, MAX_VOLUME, &objData->unk3B8, NULL, 0, NULL);
        }
        if (objData->unk3B8 != 0) {
            bss_2C = arg2 * 11.6f;
            if (bss_2C < 0.0f) {
                bss_2C = -bss_2C;
            }
            
            if (bss_2C < 40.0f) {
                bss_2C = 40.0f;
            }
            if (bss_2C > 200.0f) {
                bss_2C = 200.0f;
            }
            
            gDLL_6_AMSFX->vtbl->set_pitch(objData->unk3B8, (bss_2C / 70.0f) + 0.1f);
            if (objData->unk3F2 < 0x12) {
                var_v0 = arg2 * 30.0f;
                if (var_v0 < 0) {
                    var_v0 = -var_v0;
                }
                if (var_v0 > MAX_VOLUME) {
                    var_v0 = MAX_VOLUME;
                }
                gDLL_6_AMSFX->vtbl->set_vol(objData->unk3B8, var_v0);
            } else {
                gDLL_6_AMSFX->vtbl->set_vol(objData->unk3B8, 0);
            }
        }
    }
    
    if (arg5 & 2) {
        if (objData->unk3C0 == 0) {
            gDLL_6_AMSFX->vtbl->play(self, 0x28F, MAX_VOLUME, &objData->unk3C0, NULL, 0, NULL);
        }
        if (objData->unk3C0 != 0) {
            bss_2C = arg2 ? ((self->srt.roll * arg2) / 30000.0f) : 0;
            if (bss_2C < 0) {
                bss_2C = -bss_2C;
            } else if (bss_2C > 1.0f) {
                bss_2C = 1.0f;
            }
            
            gDLL_6_AMSFX->vtbl->set_pitch(objData->unk3C0, 0.1f + bss_2C);
            if (objData->unk3F2 < 0x12) {
                bss_2C *= MAX_VOLUME_F;
                if (bss_2C > MAX_VOLUME_F) {
                    bss_2C = MAX_VOLUME_F;
                } else if (bss_2C < 0.0f) {
                    bss_2C = 0.0f;
                }

                gDLL_6_AMSFX->vtbl->set_vol(objData->unk3C0, bss_2C);
            } else {
                gDLL_6_AMSFX->vtbl->stop(objData->unk3C0);
                objData->unk3C0 = 0;
            }
        }
    }
    
    if (arg5 & 4) {
        if (objData->unk3EA <= 0) {
            if (objData->unk3BC == 0) {
                gDLL_6_AMSFX->vtbl->play(self, 0x50D, MAX_VOLUME, &objData->unk3BC, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play(self, 0x28E, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            if (objData->unk3C4 == 0) {
                gDLL_6_AMSFX->vtbl->play(self, 0x50C, MAX_VOLUME, &objData->unk3C4, NULL, 0, NULL);
            }
        }
        
        if (objData->unk3BC != 0) {
            gDLL_6_AMSFX->vtbl->set_pitch(objData->unk3BC, (objData->unk398 * 0.00048828125f) + 0.5f);
            if (arg3 >= 6) {
                objData->unk398 += gUpdateRateF;
            } else {
                if (objData->unk398 > 55.0f) {
                    objData->unk398 -= (0.5f * gUpdateRateF);
                } else {
                    objData->unk398 += (0.2f * gUpdateRateF);
                }
            }
            if (objData->unk398 > 90.0f) {
                objData->unk398 = 90.0f;
            }
            gDLL_6_AMSFX->vtbl->set_vol(objData->unk3BC, objData->unk398);
        }

        if (objData->unk3C4 != 0) {
            gDLL_6_AMSFX->vtbl->set_pitch(objData->unk3C4, (objData->unk394 / 75.0f) + 0.2f);
            if (arg3 >= 6) {
                objData->unk394 = (arg3 * 0.6f) + 15.0f;
            } else {
                if (0.5f * gUpdateRateF < objData->unk394) {
                    objData->unk394 -= 0.5f * gUpdateRateF;
                } else {
                    objData->unk394 = 0.0f;
                }
            }
            if (objData->unk394 > MAX_VOLUME_F) {
                objData->unk394 = MAX_VOLUME_F;
            }
            gDLL_6_AMSFX->vtbl->set_vol(objData->unk3C4, objData->unk394);
        }
    }
    
    if (objData->unk2F4 == NULL) {
        objData->unk2F4 = dll_load_deferred(0x1025, 1);
    }
    
    if (objData->unk2F8 == NULL) {
        objData->unk2F8 = dll_load_deferred(0x1026, 1);
    }
    
    if (objData->unk3EA <= 0) {
        sp54 = 0.7f;
        if (arg3 > 10) {
            sp54 = arg3;
            sp54 = (0.01f * sp54) + 0.1f;
            objData->unk2F4->vtbl->func0(self, 0, 0, 0x404, -1, &sp54);
        }

        objData->unk3EA = 30;

        if ((self->srt.roll > 1000) && (arg2 < -1.0f)) {
            sp3C.scale = self->srt.roll / 8000.0f;
            sp3C.scale *= (-arg2 / 3.8f);
            objData->unk2F8->vtbl->func0(self, 0, &sp3C, 0x404, -1, 0);
        } else if ((self->srt.roll < -0x3E8) && (arg2 < -1.0f)) {
            sp3C.scale = self->srt.roll / -8000.0f;
            sp3C.scale *= (-arg2 / 3.8f);
            objData->unk2F8->vtbl->func0(self, 1, &sp3C, 0x404, -1, 0);
        }
    }
    objData->unk3EA -= gUpdateRate;
}

// offset: 0x4B30 | func: 33 | export: 20
void dll_732_func_4B30(s32 arg0, s32 arg1, s32 arg2) {

}
