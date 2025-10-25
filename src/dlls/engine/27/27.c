#include "dlls/engine/27.h"
#include "PR/os.h"
#include "sys/main.h"
#include "sys/objhits.h"
#include "functions.h"

/*0x0*/ static Object *bss_0;
/*0x8*/ static Func_80057F1C_Struct bss_8[15];
/*0x170*/ static s32 bss_170;   

void dll_27_reset(Object* obj, DLL27_Data* data);

static void dll_27_func_A74(Object* obj, DLL27_Data* data);
static void dll_27_func_B68(Object* obj, DLL27_Data* data);
static void dll_27_func_1278(Object* obj, DLL27_Data* data);
static void dll_27_func_151C(Object* obj, DLL27_Data* data);
static void dll_27_func_15C0(Object* obj, DLL27_Data* data);
static void dll_27_func_1AA0(Object* obj, DLL27_Data* data);
static void dll_27_func_1BA8(Object* obj, DLL27_Data* data);
static void dll_27_func_1D60(Object* obj, DLL27_Data* data);
static void dll_27_func_214C(Object* obj, DLL27_Data* data);
static void dll_27_func_2394(Object* obj, DLL27_Data* data);

static void dll_27_get_obj_world_matrix(Object* obj, DLL27_Data* data, MtxF* mtx);
static void dll_27_get_obj_local_matrix(Object* obj, DLL27_Data* data, MtxF* mtx);

// offset: 0x0 | ctor
void dll_27_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_27_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_27_init(DLL27_Data *data, s32 flags1, s32 flags2, s32 mode) {
    bzero(data, sizeof(DLL27_Data));
    data->mode = mode;
    data->flags = flags2 | flags1;
    data->boundsYExtension = 5;
}

// offset: 0x84 | func: 1 | export: 1
// setup hits detection
void dll_27_setup_hits_collider(DLL27_Data *data, s32 testCount, Vec3f *testPoints, f32 *testRadii, s32 a4) {
    data->numTestPoints &= 0xf0;
    data->numTestPoints |= (testCount & 0xf);
    data->unk259 = a4;
    data->localHitsTestPoints = testPoints;
    data->hitsTestRadii = testRadii;
    data->flags |= DLL27FLAG_HAS_HITS_COLLIDER;
}

// offset: 0xC0 | func: 2 | export: 2
// setup terrain detection
void dll_27_setup_terrain_collider(DLL27_Data *data, s32 testCount, Vec3f *testPoints, f32 *arg3, u8 *arg4) {
    s32 i;

    data->numTestPoints &= 0xF;
    data->numTestPoints |= ((testCount & 0xF) << 4);
    data->unk4 = testPoints;

    for (i = 0; i < testCount; i++) {
        data->unk68.unk54[i] = arg4[i];
        data->unk68.unk50[i] = -1;
        data->unk68.unk40[i] = arg3[i];
    }
    
    data->flags |= DLL27FLAG_HAS_TERRAIN_COLLIDER;
}

// offset: 0x1E8 | func: 3 | export: 3
void dll_27_func_1E8(Object *obj, DLL27_Data *data, f32 delayFloat) {
    MtxF worldMtx;
    Vec3f spE0[4];
    f32 spD0[4];
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 minX;
    f32 maxY;
    f32 minY;
    f32 maxZ;
    f32 minZ;
    f32 maxX;
    s32 var_s4;
    s32 i;

    if (data->mode == DLL27MODE_DISABLED) {
        return;
    }

    transform_point_by_object(
        obj->srt.transl.f[0], obj->srt.transl.f[1], obj->srt.transl.f[2], 
        &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z, 
        obj->parent);
    
    dll_27_get_obj_world_matrix(obj, data, &worldMtx);

    for (var_s4 = 0, i = 0; var_s4 < ((s32) data->numTestPoints >> 4); ) {
        vec3_transform(&worldMtx, 
                       data->unk4[var_s4].x, data->unk4[var_s4].y, data->unk4[var_s4].z, 
                       &spE0[var_s4].x, &spE0[var_s4].y, &spE0[var_s4].z);
        spD0[i] = data->unk68.unk40[i];
        spD0[i] = sqrtf((2 * spD0[i]) * spD0[i]);
        i++;
        var_s4++;
    }
    
    maxX = -100000.0f;
    minX = 100000.0f;
    maxY = -100000.0f;
    minY = 100000.0f;
    maxZ = -100000.0f;
    minZ = 100000.0f;
    var_s4 = 0;
    i = 0;
    while (i < ((s32) data->numTestPoints >> 4)) {
        temp_fv0 = spE0[var_s4].x + spD0[i];
        if (maxX < temp_fv0) {
            maxX = temp_fv0;
        }
        temp_fv0 = spE0[var_s4].x - spD0[i];
        if (temp_fv0 < minX) {
            minX = temp_fv0;
        }
        temp_fv0 = spE0[var_s4].y + spD0[i];
        if (maxY < temp_fv0) {
            maxY = temp_fv0;
        }
        temp_fv0 = spE0[var_s4].y - spD0[i];
        if (temp_fv0 < minY) {
            minY = temp_fv0;
        }
        temp_fv0 = spE0[var_s4].z + spD0[i];
        if (maxZ < temp_fv0) {
            maxZ = temp_fv0;
        }
        temp_fv0 = spE0[var_s4].z - spD0[i];
        if (temp_fv0 < minZ) {
            minZ = temp_fv0;
        }

        if (((!data->unk38[var_s4].x) && (!data->unk38[var_s4].x)) && (!data->unk38[var_s4].x)){} // @fake

        temp_fa1 = data->unk38[var_s4].y;
        temp_ft4 = data->unk38[var_s4].z;

        temp_fv0 = data->unk38[var_s4].x + spD0[i];
        if (maxX < temp_fv0) {
            maxX = temp_fv0;
        }
        temp_fv0 = data->unk38[var_s4].x - spD0[i];
        if (temp_fv0 < minX) {
            minX = temp_fv0;
        }
        temp_fv0 = temp_fa1 + spD0[i];
        if (maxY < temp_fv0) {
            maxY = temp_fv0;
        }
        temp_fv0 = temp_fa1 - spD0[i];
        if (temp_fv0 < minY) {
            minY = temp_fv0;
        }
        temp_fv0 = temp_ft4 + spD0[i];
        if (maxZ < temp_fv0) {
            maxZ = temp_fv0;
        }
        temp_fv0 = temp_ft4 - spD0[i];
        if (temp_fv0 < minZ) {
            minZ = temp_fv0;
        }
        var_s4++;
        i++;
        
    }
    data->aabb.min.x = minX;
    data->aabb.max.x = maxX;
    data->aabb.min.y = (minY - data->boundsYExtension);
    data->aabb.max.y = (data->boundsYExtension + maxY);
    data->aabb.min.z = minZ;
    data->aabb.max.z = maxZ;
}

// offset: 0x5A8 | func: 4 | export: 4
// run objhit code for map (terrain and hits)
void dll_27_func_5A8(Object* arg0, DLL27_Data* arg1) {
    s32 var_a2;

    var_a2 = 0;
    if ((arg1->mode == DLL27MODE_1) || (arg1->mode == DLL27MODE_2)) {
        if (arg1->flags & DLL27FLAG_4) {
            var_a2 = 1;
        }
        if (arg1->flags & DLL27FLAG_1000000) {
            var_a2 = (var_a2 | 0x20) & 0xFF;
        }
        func_80053750(arg0, &arg1->aabb, var_a2);
    }
}

// offset: 0x624 | func: 5 | export: 5
void dll_27_func_624(Object* arg0, DLL27_Data* arg1, f32 delayFloat) {
    f32 sp3C;

    sp3C = 1.0f / delayFloat;
    arg1->unkD4 = NULL;
    if (arg1->mode == DLL27MODE_1) {
        bss_0 = NULL;
        bss_170 = 0;
        arg1->floorNormalY = 1.0f;
        arg1->floorNormalX = 0.0f;
        arg1->floorNormalZ = 0.0f;
        if (arg1->flags & DLL27FLAG_HAS_HITS_COLLIDER) {
            // Hit line detection and resolution
            dll_27_func_A74(arg0, arg1);
            dll_27_func_1278(arg0, arg1);
            transform_point_by_object(
                arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, 
                &arg0->positionMirror.x, &arg0->positionMirror.y, &arg0->positionMirror.z, 
                arg0->parent);
        }
        if (arg1->flags & DLL27FLAG_HAS_TERRAIN_COLLIDER) {
            dll_27_func_B68(arg0, arg1);
            dll_27_func_151C(arg0, arg1);
            switch (arg1->flags & (DLL27FLAG_2000000 | DLL27FLAG_4000000)) {
            case DLL27FLAG_2000000:
                dll_27_func_214C(arg0, arg1);
                break;
            case (DLL27FLAG_2000000 | DLL27FLAG_4000000):
                dll_27_func_2394(arg0, arg1);
                break;
            default:
                dll_27_func_1D60(arg0, arg1);
                break;
            }
            if (arg1->flags & DLL27FLAG_100) {
                dll_27_func_1AA0(arg0, arg1);
            }
            if (arg1->flags & DLL27FLAG_80) {
                dll_27_func_1BA8(arg0, arg1);
            }
            if (arg1->flags & DLL27FLAG_1) {
                dll_27_func_15C0(arg0, arg1);
            }
        }
        if (arg1->flags & DLL27FLAG_800) {
            if (arg0->srt.pitch >= 0x3401) {
                arg0->srt.pitch = 0x3400;
            }
            if (arg0->srt.pitch < -0x3400) {
                arg0->srt.pitch = -0x3400;
            }
        }
        if (arg1->flags & DLL27FLAG_1000) {
            if (arg0->srt.roll >= 0x3401) {
                arg0->srt.roll = 0x3400;
            }
            if (arg0->srt.roll < -0x3400) {
                arg0->srt.roll = -0x3400;
            }
        }
        if (!(arg1->flags & DLL27FLAG_40000)) {
            if ((arg0->objhitInfo != NULL) && (arg0->objhitInfo->unk58 & 1)) {
                arg0->speed.y = (arg0->positionMirror.y - arg0->objhitInfo->unk20.y) * sp3C;
                if (arg0->objhitInfo->unk20.y < arg0->positionMirror.y) {
                    arg0->speed.y = 0.0f;
                }
            } else {
                arg0->speed.y = (arg0->positionMirror.y - arg0->positionMirror3.y) * sp3C;
            }
        }
    } else if (arg1->mode == DLL27MODE_2) {
        dll_27_reset(arg0, arg1);
        if (arg1->flags & DLL27FLAG_HAS_TERRAIN_COLLIDER) {
            dll_27_func_B68(arg0, arg1);
            bcopy(arg1->unk8, arg1->unk38, (arg1->numTestPoints >> 4) * sizeof(Vec3f));
            if (arg1->flags & DLL27FLAG_1) {
                dll_27_func_15C0(arg0, arg1);
            }
        }
    } else {
        dll_27_reset(arg0, arg1);
    }
    inverse_transform_point_by_object(
        arg0->positionMirror.x, arg0->positionMirror.y, arg0->positionMirror.z, 
        &arg0->srt.transl.x, &arg0->srt.transl.y, &arg0->srt.transl.z, 
        arg0->parent);
}

// offset: 0xA74 | func: 6
static void dll_27_func_A74(Object* obj, DLL27_Data* data) {
    s32 i;
    s32 pad;
    MtxF localMtx;

    dll_27_get_obj_local_matrix(obj, data, &localMtx);

    i = 0;
    while (i < (data->numTestPoints & 0xF)) {
        vec3_transform(&localMtx, 
                       data->localHitsTestPoints[i].x, data->localHitsTestPoints[i].y, data->localHitsTestPoints[i].z, 
                       &data->unkE0[i].x, &data->unkE0[i].y, &data->unkE0[i].z);
        i++;
    }
}

// offset: 0xB68 | func: 7
static void dll_27_func_B68(Object *obj, DLL27_Data *data) {
    s32 i;
    s32 i2;
    MtxF worldMtx;

    dll_27_get_obj_world_matrix(obj, data, &worldMtx);

    for (i = 0, i2 = 0; i < (data->numTestPoints >> 4); i++, i2 += 3) {
        vec3_transform(&worldMtx, 
                       data->unk4[0].f[i2], data->unk4[0].f[i2+1], data->unk4[0].f[i2+2], 
                       &data->unk8[0].f[i2], &data->unk8[0].f[i2+1], &data->unk8[0].f[i2+2]);
        data->unk68.unk50[i] = -1;
    }
}

// offset: 0xC7C | func: 8 | export: 6
Func_80057F1C_Struct* dll_27_func_C7C(Object* arg0, f32 arg1, f32 arg2, s32* arg3, s32 arg4) {
    Func_80057F1C_Struct** sp4C;
    s32 var_v0;
    s32 i;

    if (arg0 != bss_0) {
        bss_0 = arg0;
        
        if (arg4 != 0) {
            var_v0 = 0;
        } else {
            var_v0 = -1;
        }
        
        bss_170 = func_80057F1C(arg0, arg1, arg0->positionMirror.y, arg2, &sp4C, var_v0, 0);
        
        for (i = 0; i < bss_170; i++) {
            bss_8[i].unk0[0] = sp4C[i]->unk0[0];
            bss_8[i].unk0[1] = sp4C[i]->unk0[1];
            bss_8[i].unk0[2] = sp4C[i]->unk0[2];
            bss_8[i].unk0[3] = sp4C[i]->unk0[3];
            bss_8[i].unk10 = sp4C[i]->unk10;
            bss_8[i].unk14 = sp4C[i]->unk14;
        }
    }
    *arg3 = bss_170;
    return bss_8;
}

// offset: 0xDF4 | func: 9 | export: 8
f32 dll_27_func_DF4(Object* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    Func_80057F1C_Struct* temp_v0;
    s32 sp40;
    s32 i;

    temp_v0 = dll_27_func_C7C(arg0, arg1, arg3, &sp40, 1);
    
    for (i = 0; i < sp40; i++) {
        if ((temp_v0[i].unk0[0] < (arg2 + arg4)) && (temp_v0[i].unk0[2] > 0.0f)) {
            return temp_v0[i].unk0[0];
        }
    }
    
    return arg2;
}

// offset: 0xFB8 | func: 10 | export: 7
// reset detection state?
void dll_27_reset(Object *obj, DLL27_Data *data) {
    s32 i;

    transform_point_by_object(
        obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, 
        &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z, 
        obj->parent);

    for (i = 0; i < ((s32) data->numTestPoints >> 4); i++) {
        data->unk38[i].x = obj->positionMirror.x;
        data->unk38[i].y = obj->positionMirror.y + 5.0f;
        data->unk38[i].z = obj->positionMirror.z;
    }

    for (i = 0; i < (data->numTestPoints & 0xF); i++) {
        data->unk110[i].x = obj->positionMirror.x;
        data->unk110[i].y = obj->positionMirror.y + 5.0f;
        data->unk110[i].z = obj->positionMirror.z;
    }

    data->unk25C = 0;
    data->unk25B = 0;
    if (data->mode != DLL27MODE_2) {
        data->unkD4 = 0;
        data->waterY = -100000.0f;
        data->unk1AC = 100000.0f;
        data->floorY = -100000.0f;
        data->underwaterDist = 0;
        data->floorDist = 0;
        
        for (i = 0; i < ((s32) data->numTestPoints >> 4); i++) {
            data->waterYList[i] = -100000.0f;
            data->floorYList[i] = -100000.0f;
            data->unk1CC[i] = 100000.0f;
        }
    }
}

// offset: 0x1148 | func: 11
static void dll_27_get_obj_world_matrix(Object *obj, DLL27_Data *data, MtxF *mtx) {
    SRT srt;

    srt.yaw = obj->srt.yaw;
    if (data->flags & DLL27FLAG_20) {
        srt.pitch = 0;
        srt.roll = 0;
    } else {
        srt.pitch = obj->srt.pitch;
        srt.roll = obj->srt.roll;
    }
    srt.scale = 1.0f;
    srt.transl.x = obj->positionMirror.x;
    srt.transl.y = obj->positionMirror.y;
    srt.transl.z = obj->positionMirror.z;
    matrix_from_srt(mtx, &srt);
}

// offset: 0x11E0 | func: 12
static void dll_27_get_obj_local_matrix(Object* obj, DLL27_Data* data, MtxF* mtx) {
    SRT srt;

    srt.yaw = obj->srt.yaw;
    if (data->flags & DLL27FLAG_20) {
        srt.pitch = 0;
        srt.roll = 0;
    } else {
        srt.pitch = obj->srt.pitch;
        srt.roll = obj->srt.roll;
    }
    srt.scale = 1.0f;
    srt.transl.x = obj->srt.transl.x;
    srt.transl.y = obj->srt.transl.y;
    srt.transl.z = obj->srt.transl.z;
    matrix_from_srt(mtx, &srt);
}

// offset: 0x1278 | func: 13
// does hit line detection and resolves collisions (moves object if needed)
static void dll_27_func_1278(Object* obj, DLL27_Data* data) {
    MtxF sp90;
    f32 sp8C;
    f32 sp88;
    f32 temp_fv0;
    s32 temp_t5;
    s32 i;
    u8 numHitsTestPoints;
    s32 i2;

    numHitsTestPoints = data->numTestPoints & 0xF;
    data->hitsTouchBits = 0;

    for (i = 0, i2 = 0; i < numHitsTestPoints; i++, i2 += 3) {
        temp_t5 = func_80059C40(
            &data->unk110[0].f[i2], 
            &data->unkE0[0].f[i2], 
            data->hitsTestRadii[i], 
            (data->flags & 0x200000) ? 2 : 0, 
            &data->unk140, 
            obj, 
            data->unk259, -1, 0, 0);
        data->hitsTouchBits |= (temp_t5 << i);
    }
    
    if (numHitsTestPoints >= 2) {
        obj->srt.transl.x = 0.0f;
        obj->srt.transl.z = 0.0f;

        for (i = 0; i < (numHitsTestPoints*3); i+=3) {
            obj->srt.transl.x += data->unkE0[0].f[i];
            obj->srt.transl.z += data->unkE0[0].f[i+2];
        }
        
        temp_fv0 = 1.0f / (f32) numHitsTestPoints;
        obj->srt.transl.x *= temp_fv0;
        obj->srt.transl.z *= temp_fv0;
    } else {
        obj->srt.transl.x = data->unkE0[0].x;
        obj->srt.transl.z = data->unkE0[0].z;
    }
    dll_27_get_obj_local_matrix(obj, data, &sp90);

    for (i = 0; i < (numHitsTestPoints*3); i+=3) {
        data->unk110[0].f[i] = data->unkE0[0].f[i];
        data->unk110[0].f[i+2] = data->unkE0[0].f[i+2];
        vec3_transform(&sp90, 
                       data->localHitsTestPoints[0].f[i], data->localHitsTestPoints[0].f[i+1], data->localHitsTestPoints[0].f[i+2], 
                       &sp8C, &data->unk110[0].f[i+1], &sp88);
    }
}

// offset: 0x151C | func: 14
static void dll_27_func_151C(Object* arg0, DLL27_Data* arg1) {
    if (arg1->flags & DLL27FLAG_2) {
        arg1->unk25C = func_8005509C(arg0, 
                                     arg1->unk38[0].f, arg1->unk8[0].f, arg1->numTestPoints >> 4, 
                                     &arg1->unk68, 0);
        arg1->unk25B = 0;
        arg1->unk25D = arg1->unk68.unk68;
    }
    bcopy(arg1->unk8, arg1->unk38, (arg1->numTestPoints >> 4) * sizeof(Vec3f));
}

// offset: 0x15C0 | func: 15
// does terrain checks
static void dll_27_func_15C0(Object* arg0, DLL27_Data* arg1) {
    Func_80057F1C_Struct* temp_v0;
    s32 sp60;
    s32 var_t1;
    s32 var_t4;
    s8 var_t2;
    u8 var_v0;

    var_v0 = arg1->numTestPoints >> 4;
    if (!(arg1->flags & DLL27FLAG_8000000)) {
        var_v0 = 1;
    }
    for (var_t4 = 0; var_t4 < var_v0; var_t4++) { 
        if (var_v0 >= 2) {
            bss_0 = NULL;
        }

        var_t2 = 0;

        temp_v0 = dll_27_func_C7C(arg0, arg1->unk8[var_t4].x, arg1->unk8[var_t4].z, &sp60, 0);
        
        arg1->waterYList[var_t4] = -100000.0f;
        arg1->floorYList[var_t4] = -100000.0f;
        arg1->unk1CC[var_t4] = 100000.0f;
        arg1->underwaterDistList[var_t4] = 0;
        arg1->floorDistList[var_t4] = 0;
        arg1->waterNormalXList[var_t4] = 0.0f;
        arg1->waterNormalZList[var_t4] = 0.0f;
        arg1->waterNormalYList[var_t4] = 1.0f;

        for (var_t1 = 0; var_t1 < sp60; var_t1++) {
            if (temp_v0[var_t1].unk14 != 0xE) {
                if ((var_t2 == 0) && (temp_v0[var_t1].unk0[0] < (arg0->positionMirror.y + 5.0f)) && (temp_v0[var_t1].unk0[2] > 0.707f)) {
                    arg1->floorYList[var_t4] = temp_v0[var_t1].unk0[0];
                    var_t2 = 1;
                    arg1->floorDistList[var_t4] = (f32) (arg0->positionMirror.y - temp_v0[var_t1].unk0[0]);
                } else {
                    if (((arg0->positionMirror.y + 5.0f) <= temp_v0[var_t1].unk0[0]) && (temp_v0[var_t1].unk0[2] < 0.0f)) {
                        arg1->unk1CC[var_t4] = temp_v0[var_t1].unk0[0];
                    }
                }
            }
        }

        if (var_t2 == 0) {
            // No floor found, assume high above one
            arg1->floorDistList[var_t4] = 100.0f;
        }

        if (arg1->unk25C & (0x10 << var_t4)) {
            arg1->floorDistList[var_t4] = 0.0f;
        }

        for (var_t1 = 0; var_t1 < sp60; var_t1++) {
            if (temp_v0[var_t1].unk14 == 0xE) {
                if ((temp_v0[var_t1].unk0[0] < arg1->unk1CC[var_t4]) && (arg1->floorYList[var_t4] < temp_v0[var_t1].unk0[0])) {
                    arg1->waterYList[var_t4] = temp_v0[var_t1].unk0[0];
                    arg1->waterNormalXList[var_t4] = (f32) temp_v0[var_t1].unk0[1];
                    arg1->waterNormalYList[var_t4] = (f32) temp_v0[var_t1].unk0[2];
                    arg1->waterNormalZList[var_t4] = (f32) temp_v0[var_t1].unk0[3];
                }
            }
        }

        if (arg1->waterYList[var_t4] != -100000.0f) {
            arg1->underwaterDistList[var_t4] = arg1->waterYList[var_t4] - arg0->positionMirror.y;
        }
    }
    arg1->waterY = arg1->waterYList[0];
    arg1->floorY = arg1->floorYList[0];
    arg1->unk1AC = arg1->unk1CC[0];
    arg1->underwaterDist = arg1->underwaterDistList[0];
    arg1->floorDist = arg1->floorDistList[0];
}

// offset: 0x1AA0 | func: 16
// detect and resolve floor collision
static void dll_27_func_1AA0(Object* arg0, DLL27_Data* arg1) {
    Func_80057F1C_Struct* temp_v0;
    s32 sp48;
    f32 temp_fv1;
    f32 var_fa0;
    s32 temp_v1;

    temp_v0 = dll_27_func_C7C(arg0, arg1->unk8[0].x, arg1->unk8[0].z, &sp48, 0);
    temp_fv1 = arg0->positionMirror.y;
    var_fa0 = 10000.0f;

    for (temp_v1 = sp48 - 1; temp_v1 >= 0; temp_v1--) {
        if (temp_v0[temp_v1].unk14 != 0xE) {
            if ((temp_fv1 <= temp_v0[temp_v1].unk0[0]) && 
                ((temp_v0[temp_v1].unk0[0] - var_fa0) <= temp_fv1)) 
            {
                arg0->positionMirror.y = temp_v0[temp_v1].unk0[0];
                arg1->floorNormalX = temp_v0[temp_v1].unk0[1];
                arg1->floorNormalY = temp_v0[temp_v1].unk0[2];
                arg1->floorNormalZ = temp_v0[temp_v1].unk0[3];
                arg1->unk25C |= 0x11;
                arg1->unk25D += 1;
            }
            var_fa0 = 20.0f;
        }
    }
}

// offset: 0x1BA8 | func: 17
// calculate relative floor pitch/roll
static void dll_27_func_1BA8(Object* arg0, DLL27_Data* arg1) {
    s16 temp_v1;
    s16 temp_v1_2;
    SRT sp7C;
    MtxF sp3C;
    f32 x;
    f32 y;
    f32 z;

    if (arg1->unk25C & 0x10) {
        sp7C.yaw = -arg0->srt.yaw;
        sp7C.pitch = 0;
        sp7C.roll = 0;
        sp7C.transl.x = 0.0f;
        sp7C.transl.y = 0.0f;
        sp7C.transl.z = 0.0f;
        sp7C.scale = 1.0f;
        matrix_from_srt_reversed(&sp3C, &sp7C);
        vec3_transform(&sp3C, arg1->floorNormalX, arg1->floorNormalY, arg1->floorNormalZ, &x, &y, &z);
        // 0x4000 = 90deg
        temp_v1 = 0x4000 - arctan2_f(y, z);
        arg1->relativeFloorPitch = temp_v1;
        arg1->relativeFloorPitchSmooth += ((s32) ((temp_v1 - arg1->relativeFloorPitchSmooth) * delayByte) >> 4);
        temp_v1_2 = -0x4000 - -arctan2_f(y, x);
        arg1->relativeFloorRoll = temp_v1_2;
        arg1->relativeFloorRollSmooth += ((s32) ((temp_v1_2 - arg1->relativeFloorRollSmooth) * delayByte) >> 4);
    } else {
        arg1->relativeFloorPitchSmooth -= ((s32) (arg1->relativeFloorPitchSmooth * delayByte) >> 4);
        arg1->relativeFloorRollSmooth -= ((s32) (arg1->relativeFloorRollSmooth * delayByte) >> 4);
        arg1->floorNormalX = 0.0f;
        arg1->floorNormalZ = 0.0f;
        arg1->floorNormalY = 1.0f;
    }
}

// offset: 0x1D60 | func: 18
static void dll_27_func_1D60(Object* arg0, DLL27_Data* arg1) {
    f32 temp;
    f32 f2;
    f32 f0;
    s32 var_s1_2;
    s32 var_s0_2;
    f32 f14;
    u8 temp_t7;
    s8 var_a1;
    f32 f12;
    s32 i;
    SRT spDC;
    f32 temp2;
    f32 spC8[4];
    f32 spB8[4];
    f32 spA8[4];
    MtxF sp68;

    temp_t7 = (arg1->numTestPoints >> 4);
    if (temp_t7 == 2 || temp_t7 == 4) {
        arg0->positionMirror.x = 0.0f;
        arg0->positionMirror.y = 0.0f;
        arg0->positionMirror.z = 0.0f;
        for (i = 0; i < (temp_t7*3); i+=3) {
            arg0->positionMirror.x += arg1->unk38[0].f[i];
            arg0->positionMirror.y += arg1->unk38[0].f[i+1];
            arg0->positionMirror.z += arg1->unk38[0].f[i+2];
        }
        arg0->positionMirror.x *= 1.0f / temp_t7;
        arg0->positionMirror.y *= 1.0f / temp_t7;
        arg0->positionMirror.z *= 1.0f / temp_t7;
        if (!(arg1->flags & (DLL27FLAG_400 | DLL27FLAG_HAS_TERRAIN_COLLIDER | DLL27FLAG_8000))) {
            return;
        }

        spDC.yaw = -arg0->srt.yaw;
        spDC.pitch = -arg0->srt.pitch;
        spDC.roll = -arg0->srt.roll;
        spDC.scale = 1;
        spDC.transl.x = -arg0->positionMirror.x;
        spDC.transl.y = -arg0->positionMirror.y;
        spDC.transl.z = -arg0->positionMirror.z;
        matrix_from_srt_reversed(&sp68, &spDC);
        for (var_s0_2 = 0, i = 0; i < temp_t7; i++) {
            vec3_transform(&sp68, 
                           arg1->unk38[var_s0_2].x, arg1->unk38[var_s0_2].y, arg1->unk38[var_s0_2].z, 
                           &spC8[i], &spB8[i], &spA8[i]);
            var_s0_2++;
        }

        var_s1_2 = 1;
        var_s0_2 = 2;
        var_a1 = 3;
        if (temp_t7 == 2) {
            var_s1_2 = 0;
            var_s0_2 = 1;
            var_a1 = 1;
        }
        if (arg1->flags & DLL27FLAG_8000) {
            f0 = spC8[0] + spC8[var_s1_2];
            f2 = spA8[0] + spA8[var_s1_2];
            temp = spC8[var_s0_2] + spC8[var_a1];
            f12 = f0 - temp;
            temp2 = spA8[var_s0_2] + spA8[var_a1];
            f14 = f2 - temp2;
            arg0->srt.yaw += (s16) ((arctan2_f(f12, f14) & 0xFFFF) + 0x8000) >> 2;
        }
        if (arg1->flags & DLL27FLAG_200) {
            f12 = spA8[var_s0_2] - spA8[0];
            f14 = spB8[var_s0_2] - spB8[0];
            arg0->srt.pitch += (arctan2_f(f12, f14) - 0x4000) & 0xFFFF;
        }
        if ((temp_t7 == 4) && (arg1->flags & DLL27FLAG_400)) {
            f12 = spC8[var_s1_2] - spC8[0];
            f14 = spB8[var_s1_2] - spB8[0];
            arg0->srt.roll += (0x4000 - arctan2_f(f12, f14)) & 0xFFFF;
        }
    } else {
        arg0->positionMirror.x = arg1->unk38[0].x;
        arg0->positionMirror.y = arg1->unk38[0].y;
        arg0->positionMirror.z = arg1->unk38[0].z;
    }
}

// offset: 0x214C | func: 19
static void dll_27_func_214C(Object* arg0, DLL27_Data* arg1) {
    s32 spCC;
    Func_80057F1C_Struct* temp_v0;
    f32 var_ft4;
    f32 var_ft5;
    s32 var_a0;
    f32 spB0[3];
    u8 count;
    Unk80027934 sp40;

    count = arg1->numTestPoints >> 4;

    if (count >= 3) {
        if (arg1->unk25C & 4) {
            var_ft4 = arg1->unk38[2].x;
            var_ft5 = arg1->unk38[2].z;
            arg1->unk38[1].x = var_ft4;
            arg1->unk38[1].z = var_ft5;
        } else {
            var_ft4 = arg1->unk38[1].x;
            var_ft5 = arg1->unk38[1].z;
            arg1->unk38[2].x = var_ft4;
            arg1->unk38[2].z = var_ft5;
        }
    } else {
        var_ft4 = arg1->unk38[1].x;
        var_ft5 = arg1->unk38[1].z;
    }
    arg0->positionMirror.x = var_ft4;
    arg0->positionMirror.z = var_ft5;

    temp_v0 = dll_27_func_C7C(arg0, var_ft4, var_ft5, &spCC, 0);

    for (var_a0 = 0; var_a0 < spCC; var_a0++) {
        if ((temp_v0[var_a0].unk14 != 0xE) && 
            (temp_v0[var_a0].unk0[2] > 0.707f) && 
            ((arg0->positionMirror.y <= temp_v0[var_a0].unk0[0])) && 
            ((temp_v0[var_a0].unk0[0] - 50.0f) <= arg0->positionMirror.y)) 
        {
            arg0->positionMirror.y = temp_v0[var_a0].unk0[0];
            arg1->floorNormalX = temp_v0[var_a0].unk0[1];
            arg1->floorNormalY = temp_v0[var_a0].unk0[2];
            arg1->floorNormalZ = temp_v0[var_a0].unk0[3];
            arg1->unk25C |= 0x11;
            arg1->unkD4 = temp_v0[var_a0].unk10;
            arg1->unk68.unk50[0] = temp_v0[var_a0].unk14;
            arg1->unk25D += 1;
            break;
        }
    }
    
    if (arg1->unkD4 != 0) {
        func_80026184(arg1->unkD4, arg0);
    }
    if (arg1->unk25C & 0x20) {
        spB0[0] = var_ft4;
        spB0[1] = arg1->unk38[1].y;
        spB0[2] = var_ft5;
        sp40.unk40[0] = arg1->unk68.unk40[1];
        sp40.unk54[0] = 2;
        func_8005509C(arg0, spB0, spB0, 1, &sp40, 3U);
        arg1->unk38[1].x = spB0[0];
        arg1->unk38[1].z = spB0[2];
        arg0->positionMirror.x = spB0[0];
        arg0->positionMirror.z = spB0[2];
    }
    arg1->unk38[0].x = arg0->positionMirror.x;
    arg1->unk38[0].z = arg0->positionMirror.z;
}

// offset: 0x2394 | func: 20
static void dll_27_func_2394(Object* arg0, DLL27_Data* arg1) {
    f32 spB0[4];
    f32 var_ft4;
    f32 spA0[3];
    f32 sp94[3];
    f32 var_fv1;
    f32 var_fa0;
    f32 var_fa1;
    s32 sp84;
    f32* var_s2;
    s32 temp_v0;
    s32 var_a0;
    s32 var_t0;
    s32 var_s0;
    Func_80057F1C_Struct** sp6C;
    s32 v1;

    if ((arg1->numTestPoints >> 4) != 4) {
        return;
    }

    var_ft4 = 0.0f;
    var_t0 = 0;
    sp84 = 0;
    var_fv1 = 0.0f;
    var_fa0 = 0.0f;
    var_fa1 = 0.0f;
    for (sp84 = 0; sp84 < ((s32) arg1->numTestPoints >> 4); sp84++) {
        spB0[sp84] = arg1->unk38[sp84].y;
        temp_v0 = func_80057F1C(arg0, arg1->unk38[sp84].x, arg0->positionMirror.y, arg1->unk38[sp84].z, &sp6C, -1, 0);
        var_s0 = 0;
        if (temp_v0 != 0) {
            for (var_a0 = 0; var_a0 < temp_v0; var_a0++) {
                if (var_s0 == 0) {
                    if ((sp6C[var_a0]->unk0[0] < (arg0->positionMirror.y + 50.0f)) && (sp6C[var_a0]->unk14 != 0xE)) {
                        spB0[sp84] = sp6C[var_a0]->unk0[0];
                        var_t0 += 1;
                        var_s0 = 1;
                        var_fv1 += sp6C[var_a0]->unk0[1];
                        var_fa0 += sp6C[var_a0]->unk0[2];
                        var_fa1 += sp6C[var_a0]->unk0[3];
                        var_ft4 += sp6C[var_a0]->unk0[0];
                    }
                }
            }
        }
        
        arg1->unk38[sp84].y = spB0[sp84];
    }
    if (var_t0 != 0) {
        arg0->positionMirror.y = var_ft4 / var_t0;
        arg1->unk25D = 1;
        arg1->floorNormalX = var_fv1 / var_t0;
        arg1->floorNormalY = var_fa0 / var_t0;
        arg1->floorNormalZ = var_fa1 / var_t0;
    } else {
        arg1->unk25D = 0;
    }
    sp94[2] = spB0[3];
    sp94[0] = arg1->unk4[3].z;
    spA0[0] = spB0[0];
    sp94[1] = arg1->unk4[0].z;
    sp94[0] = sp94[0] - sp94[1];
    sp94[2] = spB0[3] - spB0[0];
    v1 = (arg0->srt.pitch - (arctan2_f(sp94[2], sp94[0]) & 0xFFFF));
    if (v1 >= 0x8001) {
        v1 += 0xFFFF0001;
    }
    if (v1 < -0x8000) {
        v1 += 0xFFFF;
    }
    arg0->srt.pitch = -arctan2_f(sp94[2], sp94[0]);
    if (arg1->flags & DLL27FLAG_400) {
        sp94[2] = spB0[1];
        spA0[1] = arg1->unk4[1].x;
        spA0[0] = spB0[0];
        spA0[2] = arg1->unk4[0].x;
        spA0[1] = spA0[1] - spA0[2];
        sp94[2] = spB0[1] - spB0[0];
        arg0->srt.roll = arctan2_f(sp94[2], spA0[1]);
    }
}

/*0x0*/ static const char str_0[] = "Error findBounds obj romdefno %d  lastpolyworld %f %f %f\n";

