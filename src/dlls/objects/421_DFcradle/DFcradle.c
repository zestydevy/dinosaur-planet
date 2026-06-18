#include "common.h"
#include "sys/curves.h"
#include "sys/objtype.h"

typedef struct {
    /* 0000 */ f32 unk0;
    /* 0004 */ f32 unk4; // curveProgress? (lerp t-value from 0 to 100?)
    /* 0008 */ f32 unk8;
    /* 000C */ f32 unkC;
    /* 0010 */ s32 unk10;
    /* 0014 */ f32 unk14[20];
    /* 0064 */ f32 unk64;
    /* 0068 */ Vec3f unk68; //lerp_result_x?
    /* 0074 */ f32 unk74; //some x component
    /* 0078 */ f32 unk78; //some y component
    /* 007C */ f32 unk7C; //some z component
    /* 0080 */ s32 unk80;
    /* 0084 */ f32* unk84;
    /* 0088 */ f32* unk88;
    /* 008C */ f32* unk8C;
    /* 0090 */ s32 numControlPoints;
    /* 0094 */ SplineFunc splineFunc;
    /* 0098 */ SplineConverterFunc splineConverterFunc;
    // CurveSetup *unk9C;
    // CurveSetup *unkA0;
    // CurveSetup *unkA4;
    // f32 unkA8[4];
    // f32 unkB8[4];
    // f32 unkC8[4];
    // f32 unkD8[4];
    // f32 unkE8[4];
    // f32 unkF8[4];
} UnkCurvesStruct_Split;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
} DLL421_Setup;

typedef struct {
    UnkCurvesStruct_Split unk0;
    f32 unk9C;
    f32* unkA0;
    f32* unkA4;
    f32* unkA8;
    f32 unkAC;
    f32 unkB0;
    s8 unkB4;
    s8 unkB5;
    u8 unkB6;
    u8 unkB7;
    s8 unkB8;
} DLL421_Data;

/*0x0*/ static s16 data_0[] = {
    0x0783, 0x0784, 0x0785
};
/*0x8*/ static u32 data_8[] = {
    0x0003254b, 0x0003254f, 0x00032562, 0x00032563
};

static u8 dll_421_func_99C(Object* self, f32 x, f32 z, u8 isPlayer);

// offset: 0x0 | ctor
void dll_421_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_421_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_421_setup(Object* self, DLL421_Setup* setup, s32 reset) {
    DLL421_Data* objData = self->data;
    /*0x18*/ s32 data_18[] = { 1 };
    Object* node;
    s32 stride;
    CurveSetup* sp2C;
    s32 temp_v0;
    
    self->srt.roll = setup->unk18 << 8;
    self->srt.pitch = setup->unk19 << 8;
    self->srt.yaw = setup->unk1A << 8;
    
    temp_v0 = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], data_18, 1, -1);
    if (temp_v0 != 0) {
        sp2C = gDLL_26_Curves->vtbl->func_39C(temp_v0);
        
        stride = (gDLL_26_Curves->vtbl->func_D8C(sp2C) - 1) << 2;
        
        objData->unkA0 = mmAlloc((stride * 3) * sizeof(f32*), 0xFF, NULL);
        objData->unkA4 = objData->unkA0 + stride;
        objData->unkA8 = objData->unkA4 + stride;
        
        objData->unk0.unk84 = objData->unkA0;
        objData->unk0.unk88 = objData->unkA4;
        objData->unk0.unk8C = objData->unkA8;
        objData->unk0.unk80 = 0;
        
        objData->unk0.splineFunc = curves_hermite;
        objData->unk0.splineConverterFunc = curves_hermite_converter;
        objData->unk0.numControlPoints = gDLL_26_Curves->vtbl->func_E40(sp2C, objData->unk0.unk84, objData->unk0.unk88, objData->unk0.unk8C, NULL);
        curves_move((UnkCurvesStruct*)&objData->unk0);
    }
    
    objData->unkB4 = main_get_bits(BIT_19);
    objData->unkB5 = 1;
    objData->unkB6 = 0;
    objData->unkB8 = 0;
    objData->unk9C = 0.0f;
    
    node = func_800211B4(0x208E);
    objData->unkAC = node->globalPosition.x + (node->globalPosition.z * 0.4f);
    
    node = func_800211B4(0x208F);
    objData->unkB0 = node->globalPosition.x + (node->globalPosition.z * 0.4f);
}

// offset: 0x240 | func: 1 | export: 1
void dll_421_control(Object* self) {
    DLL421_Data* objData;
    Object** objects; //78
    Object* player;
    s32 count; //70
    s32 idx; //6C
    u8 cradleVal;
    u8 var_a0;
    u8 playerVal; //69
    f32 dx; //64
    Object* curveNode;
    f32 dy;
    f32 dz;
    s32 pad;
    u8 i;
    s8 sp52; //52

    objData = self->data;
    
    if (objData->unkB4 ) {
        if (objData->unkB8-- < 0) {
            gDLL_6_AMSFX->vtbl->play(self, data_0[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->unkB8 = rand_next(40, 60);
        }
        
        if (objData->unkB6 != 0) {
            objData->unkB6--;
        }
        
        player = get_player();
        if (player != NULL) {
            playerVal = dll_421_func_99C(self, player->globalPosition.f[0], player->globalPosition.f[2], 1);
            cradleVal = dll_421_func_99C(self, self->globalPosition.f[0],   self->globalPosition.f[2],   0);
            var_a0 = (cradleVal > playerVal && objData->unkB5 > 0) || (cradleVal < playerVal && objData->unkB5 < 0);
            dx = self->globalPosition.f[0] - player->globalPosition.f[0];
            dy = self->globalPosition.f[1] - player->globalPosition.f[1];
            dz = self->globalPosition.f[2] - player->globalPosition.f[2];
            dx = SQ(dx) + SQ(dy) + SQ(dz);
            
            if (!objData->unkB6 && cradleVal != objData->unkB7) {
                if (dx < 40000.0f) {
                    objData->unkB6 = 40;
                } else {
                    objData->unkB6 = 10;
                }
            }
            
            objData->unkB7 = cradleVal;
            if ((objData->unkB6 == 10) && (self != player->parent) && var_a0) {
                objData->unkB5 = -objData->unkB5;
                if (objData->unkB5 > 0) {
                    main_set_bits(0x1C, 0);
                } else {
                    main_set_bits(0x1C, 1);
                }
            }
            
            if (objData->unkB6 <= 10) {
                objects = obj_get_all_of_type(OBJTYPE_Pulley, &count);
                for (idx = 0; idx < count; idx++) {
                    objects[idx]->srt.yaw -= objData->unk9C * 500.0f * objData->unkB5 * gUpdateRateF * (((objects[idx]->stateFlags & 1) * 2) - 1);
                }                

                cradleVal = main_get_bits(BIT_1C);
                if (cradleVal == 0 && (objData->unkB5 == 0)) {
                    cradleVal = 0;
                }
                if (cradleVal != 0 && (objData->unkB5 == -1)) {
                    cradleVal = 0;
                }
                
                if (cradleVal) {
                    if (objData->unk9C < 0.02f) {
                        objData->unkB5 = -objData->unkB5;
                        objData->unk9C = 0.0f;
                    }
                } else {
                    s32 minDistance = 10000;
                    s32 distance; 
                    
                    for (idx = 0; idx < count; idx++) {
                        distance = vec3_distance(&self->globalPosition, &objects[idx]->globalPosition);
                        if (distance < minDistance) {
                            minDistance = distance;
                        }
                    }
                    
                    if (minDistance <= 20) {
                        minDistance = 20;
                    }
                    if (minDistance >= 120) {
                        minDistance = 120;
                    }

                    if (1){}
                    
                    objData->unk9C = (((minDistance - 20.0f) * 1.8f) / 100.0f) + 0.2f;
                }
                
                sp52 = (objData->unk9C * -53.0f) * (f32)objData->unkB5;

                if (curves_func_800053B0((UnkCurvesStruct*)objData, -sp52 / 53.0f)) {
                    objData->unkB5 = -objData->unkB5;
                    objData->unk9C = 0/*0.0f*/;
                    
                    if (dx < 40000.0f) {
                        objData->unkB6 = 60;
                    } else {
                        objData->unkB6 = 20;
                    }
                    
                    if (objData->unkB5 > 0) {
                        main_set_bits(BIT_1C, 0);
                    } else {
                        main_set_bits(BIT_1C, 1);
                    }
                } else {
                    if ((objData->unk0.unk74 != 0/*.0f*/) || (objData->unk0.unk7C != 0/*.0f*/)) {
                        self->srt.yaw = arctan2_f(objData->unk0.unk74, objData->unk0.unk7C) + M_180_DEGREES;
                    }
                }
                self->srt.transl.x = objData->unk0.unk68.x;
                self->srt.transl.y = objData->unk0.unk68.y;
                self->srt.transl.z = objData->unk0.unk68.z;
            } else {
                sp52 = 0;
            }

            for (i = 0; i < 4; i++) {
                curveNode = func_800211B4(data_8[i]);
                if (curveNode != NULL) {
                    ((DLL_Unknown*)curveNode->dll)->vtbl->func[7].withTwoArgs(curveNode, sp52);
                }
            }
        }
        return;
    }

    objData->unkB4 = main_get_bits(BIT_19);
    
    if (objData->unk9C > 0.0f) {
        objData->unk9C -= 0.02f;
    } else {
        objData->unk9C = 0.0f;
    }
}

// offset: 0x908 | func: 2 | export: 2
void dll_421_update(Object* self) { }

// offset: 0x914 | func: 3 | export: 3
void dll_421_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x968 | func: 4 | export: 4
void dll_421_free(Object* self, s32 onlySelf) { }

// offset: 0x978 | func: 5 | export: 5
u32 dll_421_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x988 | func: 6 | export: 6
u32 dll_421_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL421_Data);
}

// offset: 0x99C | func: 7
u8 dll_421_func_99C(Object* self, f32 x, f32 z, u8 isPlayer) {
    DLL421_Data* objData;
    f32 temp_fv0;
    s32 temp_fv1;

    objData = self->data;
    
    temp_fv0 = isPlayer * 80;
    temp_fv1 = (z * 0.4f) + x;
    
    if ((objData->unkAC + temp_fv0) < temp_fv1) {
        return 0;
    }
    
    if ((objData->unkB0 + temp_fv0) < temp_fv1) {
        return 1;
    }
    
    return 2;
}
