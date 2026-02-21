#include "sys/objhits.h"
#include "sys/asset_thread.h"
#include "sys/dll.h"
#include "sys/fs.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objanim.h"
#include "sys/gfx/modgfx.h"
#include "dll.h"

// -------- .data start 800916e0 -------- //
Object *D_800916E0[5] = {NULL};
// function statics start here
// -------- .data start 80091710 -------- //

// -------- .bss start 800b1990 -------- //
f32 D_800B1990;
Object **D_800B1994; // object list
s32 D_800B1998; // count for D_800B1994
Unk800B199C *D_800B199C; // 30 elements
Unk800B19A0 D_800B19A0[64];
MtxF* D_800B20A0;
s32 D_800B20A4; // unused
void* D_800B20A8;
void* D_800B20AC;
void* D_800B20B0;
void* D_800B20B4;
Unk800B20B8 D_800B20B8[64];
Unk800B20B8* D_800B23B8;
Unk800B20B8* D_800B23BC;
u8 _bss_800B23C0[0xF8];
Unk80030A24 D_800B24B8[21]; // TODO: unknown length
// -------- .bss end 800b28b0 -------- //

void alloc_some_object_arrays(void) {
    D_800B1994 = mmAlloc(0xA0, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:hitmods"));
    D_800B199C = mmAlloc(0x708, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:lasthits"));
    D_800B20A0 = mmAlloc(0x1900, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:hitvols"));
    D_800B20A8 = mmAlloc(0x400, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:hitspheres"));
    D_800B20AC = mmAlloc(0x400, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:hitspheres2"));
    D_800B20B0 = mmAlloc(0x400, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:hitspheres3"));
    D_800B20B4 = mmAlloc(0x400, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("ohits:hitspheres4"));
    D_800B1990 = 2.0f;
    func_80028D90();
}

Object **func_80025DD4(s32 *arg0) {
    *arg0 = D_800B1998;
    return D_800B1994;
}

void func_80025DF0(void) {
    s32 i;

    for (i = 0; i < 30; i++) {
        D_800B199C[i].unk0 = 0;
    }

    D_800B1998 = 0;
}

void func_80025E58(void) {
    s32 i;

    i = 0;
    for (i = 0; i < D_800B1998; i++) {
        if (!(D_800B1994[i]->def->flags & 0x40) && (D_800B1994[i]->updatePriority != 0x64)) {
            update_object(D_800B1994[i]);
        }
    }
    for (i = 0; i < D_800B1998; i++) {
        func_8002B410(D_800B1994[i], 1);
    }
}

/** Checks for collisions (Projectile/Ice Blast Spell, etc.) 
  * Lower collisionType values have higher priority.
  * arg2 seems to store model hitSphere index
  */
s32 func_80025F40(Object* obj, Object **hitBy, s32 *arg2, s32 *damage) {
    ObjectHitInfo* objHitInfo;
    s8 collisionType;
    s8 collisionIndex;
    s32 i;

    objHitInfo = obj->objhitInfo;
    if (objHitInfo == NULL) {
        return 0;
    }
    if (objHitInfo->unk62 != 0) {
        collisionType = 0x7F;
        collisionIndex = -1;
        for (i = 0; i < objHitInfo->unk62; i++) {
            if (objHitInfo->hitTypeList[i] < collisionType) {
                collisionType = objHitInfo->hitTypeList[i];
                collisionIndex = i;
            }
        }
        if (collisionIndex != -1) {
            if (hitBy != 0) {
                *hitBy = objHitInfo->hitByList[collisionIndex];
            }
            if (arg2 != 0) {
                *arg2 = objHitInfo->unk63[collisionIndex];
            }
            if (damage != 0) {
                *damage = objHitInfo->hitDamageList[collisionIndex];
            }
            return collisionType;
        }
    }
    return 0;
}

/** Checks for collisions, see func_80025F40. Returns a collision type. */
s8 func_8002601C(Object* obj, Object** hitBy, s32* arg2, s32* damage, f32* hitX, f32* hitY, f32* hitZ) {
    ObjectHitInfo* objHitInfo;
    s8 collisionType;
    s8 collisionIndex;
    s32 i;

    objHitInfo = obj->objhitInfo;
    if (objHitInfo == NULL) {
        return 0;
    }

    if (objHitInfo->unk62 != 0) {
        collisionType = 0x7F;
        collisionIndex = -1;
        for (i = 0; i < objHitInfo->unk62; i++) {
            if (objHitInfo->hitTypeList[i] < collisionType) {
                collisionType = objHitInfo->hitTypeList[i];
                collisionIndex = i;
            }
        }
        if (collisionIndex != -1) {
            if (hitBy != 0) {
                *hitBy = objHitInfo->hitByList[collisionIndex];
            }
            if (arg2 != 0) {
                *arg2 = objHitInfo->unk63[collisionIndex];
            }
            if (damage != 0) {
                *damage = objHitInfo->hitDamageList[collisionIndex];
            }
            if (hitX != NULL) {
                *hitX = objHitInfo->hitXList[collisionIndex];
                *hitY = objHitInfo->hitYList[collisionIndex];
                *hitZ = objHitInfo->hitZList[collisionIndex];
            }
            return collisionType;
        }
    }
    return 0;
}

void func_80026128(Object* arg0, s32 arg1, s32 arg2, s32 arg3) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = arg0->objhitInfo;
    if (objHitInfo == NULL) {
        return;
    }

    objHitInfo->unk5F = arg1;
    objHitInfo->unk60 = arg2;
    if (arg3 == -1) {
        return;
    }

    objHitInfo->unk40 = 1 << (arg3 + 4);
    objHitInfo->unk44 = 1 << (arg3 + 4);
}

void func_80026160(Object* obj) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = obj->objhitInfo;
    if (objHitInfo != NULL) {
        objHitInfo->unk5F = 0;
        objHitInfo->unk60 = 0;
        objHitInfo->unk40 = 0;
        objHitInfo->unk44 = 0;
    }
}

void func_80026184(Object* obj, Object* arg1) {
    ObjectPolyhits* polyhits;
    s32 i;

    polyhits = obj->polyhits;
    if (polyhits->unk10F < 3) {
        for (i = 0; i < polyhits->unk10F; i++) {
            if (arg1 == polyhits->unk100[i]) {
                return;
            }
        }
        
        polyhits->unk100[polyhits->unk10F] = arg1;
        polyhits = obj->polyhits;
        polyhits->unk10F += 1;
    }
}

s32 func_800261E8(Object* obj, Object* hitBy, s8 hitType, s8 damage, s8 arg4, f32 hitX, f32 hitY, f32 hitZ) {
    ObjectHitInfo* objHitInfo;
    ObjectHitInfo* objHitInfo2;
    s32 i;

    if (hitType == 0) {
        return FALSE;
    }

    objHitInfo = obj->objhitInfo;
    if (!(objHitInfo->unk58 & 1)) {
        return FALSE;
    }

    if (hitBy != NULL) {
        objHitInfo2 = hitBy->objhitInfo;
        if (objHitInfo2 != NULL) {
            objHitInfo2->unk48 = obj;
        }
    }

    for (i = 0; i < objHitInfo->unk62; i++) {
        if (hitBy == objHitInfo->hitByList[i]) {
            if (hitType < objHitInfo->hitTypeList[i]) {
                objHitInfo->unk63[i] = arg4;
                objHitInfo->hitTypeList[i] = hitType;
                objHitInfo->hitDamageList[i] = damage;
                objHitInfo->hitXList[i] = hitX;
                objHitInfo->hitYList[i] = hitY;
                objHitInfo->hitZList[i] = hitZ;
            }
            i = objHitInfo->unk62 + 1;
        }
    }

    if (i == objHitInfo->unk62 && objHitInfo->unk62 < 3) {
        objHitInfo->unk63[objHitInfo->unk62] = arg4;
        objHitInfo->hitTypeList[objHitInfo->unk62] = hitType;
        objHitInfo->hitDamageList[objHitInfo->unk62] = damage;
        objHitInfo->hitByList[objHitInfo->unk62] = hitBy;
        objHitInfo->hitXList[objHitInfo->unk62] = hitX;
        objHitInfo->hitYList[objHitInfo->unk62] = hitY;
        objHitInfo->hitZList[objHitInfo->unk62] = hitZ;
        objHitInfo->unk62 += 1;
    }

    return TRUE;
}

s32 func_8002635C(Object* obj, Object* hitBy, s8 hitType, s8 damage, s8 arg4) {
    ObjectHitInfo* objHitInfo;
    ObjectHitInfo* objHitInfo2;
    s32 i;

    if (hitType == 0) {
        return FALSE;
    }

    objHitInfo = obj->objhitInfo;
    if (!(objHitInfo->unk58 & 1)) {
        return FALSE;
    }

    if (hitBy != NULL) {
        objHitInfo2 = hitBy->objhitInfo;
        if (objHitInfo2 != NULL) {
            objHitInfo2->unk48 = obj;
        }
    }

    for (i = 0; i < objHitInfo->unk62; i++) {
        if (hitBy == objHitInfo->hitByList[i]) {
            if (hitType < objHitInfo->hitTypeList[i]) {
                objHitInfo->unk63[i] = arg4;
                objHitInfo->hitTypeList[i] = hitType;
                objHitInfo->hitDamageList[i] = damage;
                objHitInfo->hitXList[i] = obj->srt.transl.x;
                objHitInfo->hitYList[i] = obj->srt.transl.y;
                objHitInfo->hitZList[i] = obj->srt.transl.z;
            }
            i = objHitInfo->unk62 + 1;
        }
    }

    if ((i == objHitInfo->unk62) && (objHitInfo->unk62 < 3)) {
        objHitInfo->unk63[objHitInfo->unk62] = arg4;
        objHitInfo->hitTypeList[objHitInfo->unk62] = hitType;
        objHitInfo->hitDamageList[objHitInfo->unk62] = damage;
        objHitInfo->hitByList[objHitInfo->unk62] = hitBy;
        objHitInfo->hitXList[objHitInfo->unk62] = obj->srt.transl.x;
        objHitInfo->hitYList[objHitInfo->unk62] = obj->srt.transl.y;
        objHitInfo->hitZList[objHitInfo->unk62] = obj->srt.transl.z;
        objHitInfo->unk62 += 1;
    }

    return TRUE;
}

void func_800264D0(Object* arg0) {
    ObjectHitInfo* objHitInfo;
    f32 temp_fv0;
    s16 var_a3;
    s16 var_t0;

    objHitInfo = arg0->objhitInfo;
    if (objHitInfo == NULL) {
        return;
    }

    objHitInfo->unk58 = arg0->def->helpText;
    objHitInfo->unk5A = arg0->def->unk93;
    objHitInfo->unk5B = arg0->def->_unk91[0];
    objHitInfo->unk5C = arg0->def->_unk91[1];
    objHitInfo->unk52 = arg0->def->hitbox_flagsB6;
    objHitInfo->unk54 = arg0->def->unk94;
    objHitInfo->unk56 = arg0->def->unk96;
    objHitInfo->unkA0 = arg0->def->colorIdx;
    objHitInfo->unk50 = 0x400;
    temp_fv0 = (f32) objHitInfo->unk52;
    objHitInfo->unkC = temp_fv0 * temp_fv0;
    objHitInfo->unk2C = arg0->unkA8;
    if (objHitInfo->unk5A & 2) {
        if (objHitInfo->unk54 < 0) {
            var_a3 = objHitInfo->unk54 * -1;
        } else {
            var_a3 = objHitInfo->unk54;
        }
        if (objHitInfo->unk56 < 0) {
            var_t0 = objHitInfo->unk56 * -1;
        } else {
            var_t0 = objHitInfo->unk56;
        }
        if (var_t0 < var_a3) {
            var_t0 = var_a3;
        }
        if (objHitInfo->unk2C < var_t0) {
            objHitInfo->unk2C = var_t0;
        }
    } else if (objHitInfo->unk5A & 1) {
        if (objHitInfo->unk2C < objHitInfo->unk52) {
            objHitInfo->unk2C = objHitInfo->unk52;
        }
    }

    objHitInfo->unk30 = arg0->unkA8;
    if ((objHitInfo->unk5A & 2) || (objHitInfo->unk5A & 1)) {
        if (objHitInfo->unk30 < objHitInfo->unk52) {
            objHitInfo->unk30 = objHitInfo->unk52;
        }
    }
}

u32 func_8002667C(Object* obj, u32 addr) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = (ObjectHitInfo *)mmAlign4(addr);
    obj->objhitInfo = objHitInfo;
    addr = (u32)(objHitInfo + 1);
    func_800264D0(obj);
    objHitInfo->unk9E = 1;
    if (objHitInfo->unk5A & 0x30) {
        objHitInfo->unk9F = 2;
    }
    return addr;
}

void func_800266E0(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        objhitInfo->unk10.x = obj->srt.transl.x;
        objhitInfo->unk10.y = obj->srt.transl.y;
        objhitInfo->unk10.z = obj->srt.transl.z;
        objhitInfo->unk20.x = obj->positionMirror.x;
        objhitInfo->unk20.y = obj->positionMirror.y;
        objhitInfo->unk20.z = obj->positionMirror.z;
    }
}

s32 func_80026724(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        return objhitInfo->unk58 & 1;
    }

    return 0;
}

void func_8002674C(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo == NULL) {
        return;
    }

    if (!(objhitInfo->unk58 & 1)) {
        objhitInfo->unk58 |= 1;
        objhitInfo->unk10.x = obj->srt.transl.x;
        objhitInfo->unk10.y = obj->srt.transl.y;
        objhitInfo->unk10.z = obj->srt.transl.z;
        objhitInfo->unk20.x = obj->positionMirror.x;
        objhitInfo->unk20.y = obj->positionMirror.y;
        objhitInfo->unk20.z = obj->positionMirror.z;
    }
}

void func_800267A4(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        objhitInfo->unk58 &= ~1;
    }
}

void func_800267C4(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo == NULL) {
        // "Failed assertion object->hits" (default.dol)
        return;
    }

    if (objhitInfo->unk58 & 0x40) {
        objhitInfo->unk58 &= ~0x40;
        objhitInfo->unk10.x = obj->srt.transl.x;
        objhitInfo->unk10.y = obj->srt.transl.y;
        objhitInfo->unk10.z = obj->srt.transl.z;
        objhitInfo->unk20.x = obj->positionMirror.x;
        objhitInfo->unk20.y = obj->positionMirror.y;
        objhitInfo->unk20.z = obj->positionMirror.z;
    }
}

void func_8002681C(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        objhitInfo->unk58 |= 0x40;
    }
}

void func_8002683C(Object* arg0, s16 arg1, s16 arg2, s16 arg3) {
    ObjectHitInfo* objhitInfo;
    f32 var_fv0;

    objhitInfo = arg0->objhitInfo;
    if (objhitInfo == NULL) {
        return;
    }
    
    if (!(objhitInfo->unk5A & 2)) {
        return;
    }

    objhitInfo->unk52 = arg1;
    objhitInfo->unk54 = arg2;
    objhitInfo->unk56 = arg3;
    var_fv0 = objhitInfo->unk52;
    objhitInfo->unk50 = 0x400;
    objhitInfo->unkC = var_fv0 * var_fv0;
    objhitInfo->unk2C = arg0->unkA8;
    if (arg2 < 0) {
        arg2 = -arg2;
    }
    if (arg3 < 0) {
        arg3 = -arg3;
    }
    if (arg3 < arg2) {
        arg3 = arg2;
    }
    if (objhitInfo->unk2C < arg3) {
        objhitInfo->unk2C = arg3;
    }
    objhitInfo->unk30 = arg0->unkA8;
    var_fv0 = objhitInfo->unk52;
    if (objhitInfo->unk30 < var_fv0) {
        objhitInfo->unk30 = var_fv0;
    }
}

void func_80026940(Object* obj, s16 arg1) {
    ObjectHitInfo* objhitInfo;
    f32 temp_fv0;

    objhitInfo = obj->objhitInfo;
    if ((objhitInfo != NULL) && (objhitInfo->unk5A & 1)) {
        objhitInfo->unk52 = arg1;
        temp_fv0 = (f32) objhitInfo->unk52;
        objhitInfo->unkC = temp_fv0 * temp_fv0;
        objhitInfo->unk2C = obj->unkA8;
        if (objhitInfo->unk2C < temp_fv0) {
            objhitInfo->unk2C = temp_fv0;
        }
        objhitInfo->unk30 = obj->unkA8;
        if (objhitInfo->unk30 < temp_fv0) {
            objhitInfo->unk30 = temp_fv0;
        }
    }
}

void func_800269CC(Object* obj, ObjectHitInfo* objHitInfo, s32 arg2) {
    if (arg2 >= obj->def->numModels) {
        arg2 = obj->def->numModels - 1;
    } else if (arg2 < 0) {
        arg2 = 0;
    }

    func_8002B5C0(obj);
    objHitInfo->unkA0 = arg2;
}

u32 func_80026A20(s32 objId, ModelInstance* modelInstance, ObjectHitInfo* objHitInfo, u32 arg3, Object* obj) {
    if (modelInstance == NULL) {
        return arg3;
    }

    objHitInfo->unk6 = 0x12C;
    objHitInfo->unk8 = (u32 *)mmAlign8(arg3);
    arg3 = (s32)objHitInfo->unk8 + objHitInfo->unk6;
    objHitInfo->unk9E = 1;
    if (objHitInfo->unk5A & 0x30) {
        objHitInfo->unk9F = 2;
    }
    func_80026AB8(obj, modelInstance, objId, objHitInfo, 0, 1);
    return arg3;
}

void func_80026AB8(Object* obj, ModelInstance* modelInstance, s32 arg2, ObjectHitInfo* objHitInfo, s32 arg4, s32 arg5) {
    ObjDefHit* temp_a2;
    s32 temp_a2_2;
    s32 i;

    temp_a2 = obj->def->pHits;
    objHitInfo->unk4 = 0;
    if (temp_a2 == NULL) {
        return;
    }

    for (i = 0; temp_a2[i].id != -1; i++) {
        if (arg4 != temp_a2[i].id) {
            continue;
        }

        temp_a2_2 = temp_a2[i].fileOffsetInBytes;
        objHitInfo->unk4 = temp_a2[i].fileSizeInBytes;
        if (objHitInfo->unk6 < objHitInfo->unk4) {
            STUBBED_PRINTF("objhits.c: keysize overflow error\n");
            objHitInfo->unk4 = objHitInfo->unk6;
        }

        if (arg5 == 0) {
            queue_load_file_region_to_ptr((void **)objHitInfo->unk8, OBJHITS_BIN, temp_a2_2, objHitInfo->unk4);
        } else {
            read_file_region(OBJHITS_BIN, (void *)objHitInfo->unk8, temp_a2_2, objHitInfo->unk4);
        }
        return;
    }
}

void func_80026B84(Object* obj) {
    ObjectPolyhits* obj58;

    obj58 = obj->polyhits;
    if (obj58 != NULL) {
        obj58->unk10C = 0;
        obj->polyhits->unk10D = 0xA;
        obj->polyhits->unk10F = 0;
        func_8002B410(obj, 1);
        func_8002B410(obj, 1);
    }
}

u32 func_80026BD8(Object* obj, u32 addr) {
    obj->polyhits = (ObjectPolyhits *)mmAlign4(addr);
    addr = (u32)(obj->polyhits + 1);
    func_80026B84(obj);
    return addr;
}

void update_obj_hitboxes(s32 arg0) {
    Object* curObj;
    Object** objects;
    ObjectHitInfo* objhitInfo;
    s32 j;
    s32 numObjs;
    s32 i;

    objects = get_world_objects(&i, &numObjs);
    D_800B1998 = 0;

    for (j = 0; j < arg0; j++) {
        curObj = objects[j];
        objhitInfo = curObj->objhitInfo;
        if (objhitInfo != NULL && (objhitInfo->unk58 & 1) && (objhitInfo->unk5A & 8)) {
            if (curObj->polyhits == NULL) {
                STUBBED_PRINTF(" Warning HitModel %x [%d] has no Polyhits\n", curObj, curObj->id);
            }
            if (D_800B1998 < 40) {
                D_800B1994[D_800B1998++] = curObj;
            } else {
                STUBBED_PRINTF("OBJHITS: hitmodel overflow\n");
            }
            objhitInfo->unk0 = 0;
            objhitInfo->unk58 &= ~8;
            objhitInfo->unk50 = 0x400;
        }
    }
}

u8 func_80026DF4(Object* obj, Unk80026DF4* arg1, u8 arg2, u8 arg3, f32* arg4) {
    SRT sp70;
    s32 i;
    Unk80026DF4* sp68;
    DLL_IModgfx* loadedDLL;
    ModelInstance *modelInst;
    s32 sp5C;
    f32 sp58;
    s32 sp48[4] = {0x08, 0xB4, 0xF0, 0xFF};

    if (arg3 != 0) {
        // STUBBED_PRINTF("hitstate frame=%f\n", obj->animProgress); // (default.dol)
        if (func_80024108(obj, *arg4, gUpdateRateF, NULL)) {
            arg3 = 0;
        }
    }
    i = func_8002601C(obj, NULL, &sp5C, NULL, &sp70.transl.x, &sp70.transl.y, &sp70.transl.z);
    if (i != 0) {
        modelInst = obj->modelInsts[obj->modelInstIdx];
        sp70.transl.x += gWorldX;
        sp70.yaw = 0;
        sp70.pitch = 0;
        sp70.transl.z += gWorldZ;
        sp70.roll = 0;
        sp70.scale = 1.0f;
        sp5C = modelInst->model->hitSpheres[sp5C].unkC;
        if (sp5C >= arg2) {
            STUBBED_PRINTF("objHitReact.c: sphere overflow!\n");
            // STUBBED_PRINTF("objHitReact.c: sphere overflow! %d\n", sp5C); // (default.dol)
            sp5C = 0;
        }
        sp68 = &arg1[sp5C];
        if (i != 15) {
            if (sp68->unk0 >= 0) {
                gDLL_6_AMSFX->vtbl->play_sound(obj, sp68->unk0, MAX_VOLUME, 0, 0, 0, 0);
            }
            if (sp68->unk2 >= 0) {
                gDLL_6_AMSFX->vtbl->play_sound(obj, sp68->unk2, MAX_VOLUME, 0, 0, 0, 0);
            }
            // @fake
            if (!i) {}
            if (sp68->unk8 == 1) {
                loadedDLL = dll_load(DLL_ID_106, 1U, FALSE);
                // @fake
                if (1) {}
                loadedDLL->vtbl->func0(0, 1, &sp70, 0x401, -1, sp48);
                if (loadedDLL != NULL) {
                    dll_unload(loadedDLL);
                }
            } else {
                sp58 = 0.014f;
                gDLL_17_partfx->vtbl->spawn(obj, PARTICLE_325, &sp70, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, &sp58);
                sp70.scale = 92.0f;
                gDLL_17_partfx->vtbl->spawn(obj, PARTICLE_323, &sp70, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                sp58 = 0.015f;
                sp70.scale = 231.0f;
                gDLL_17_partfx->vtbl->spawn(obj, PARTICLE_323, &sp70, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, &sp58);
                sp70.transl.x -= obj->positionMirror.x;
                sp70.transl.y -= obj->positionMirror.y;
                sp70.transl.z -= obj->positionMirror.z;
                sp70.scale = 123.0f;
                for (i = 0; i < 15; i++) {
                    gDLL_17_partfx->vtbl->spawn(obj, PARTICLE_324, &sp70, PARTFXFLAG_2, -1, NULL);
                }
            }
        }
        if (!arg3) {
            if (sp68->unk4 >= 0) {
                func_80023D30(obj, sp68->unk4, 0.0f, 0);
                *arg4 = sp68->unkC;
                arg3 = 1;
            }
        }
    }
    return arg3;
}

static const char str_800998f0[] = " WARNING : OBJHITS hitlist has overflowed \n\n";
#ifndef NON_EQUIVALENT
void obj_do_hit_detection(s32 arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/obj_do_hit_detection.s")
#else
// https://decomp.me/scratch/ftxul
void obj_do_hit_detection(s32 numObjs) {
    u8 pad[0x9A0 - 0x984];
    Object* sp980;
    u8 pad2[0x980 - 0x560];
    s32 sp55C;
    u8 pad3[0x55C - 0x150];
    s32 sp14C;
    u8 pad4[0x14C - 0xEC];
    s32 spE8;
    ModelInstance* temp_a1;
    Object* linkedObj;
    Object* parentObj;
    Object* currentObj;
    Object* linkedParentObj;
    Object** objects;
    Object** objectsCopy;
    ObjectHitInfo* parentObjInfo; // s1
    ObjectHitInfo* currentObjHitInfo; // s3
    Unk800B20B8** temp_v1;
    Unk800B20B8** var_s5;
    f32 var_fv0;
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp74;
    s32 s4;
    s32 var_s6;
    s32 var_s7;
    Unk800B20B8** sp68;

    objects = get_world_objects(&sp7C, &sp80);
    D_800B20B8->unk4 = -36288576.0f;
    D_800B20B8->unk0 = D_800B20B8->unk4;
    D_800B23B8 = D_800B20B8;
    for (var_s6 = 0, var_s7 = 1; var_s6 < numObjs; var_s6++) {
        currentObj = objects[var_s6];
        currentObjHitInfo = currentObj->objhitInfo;
        if (currentObjHitInfo != NULL) {
            if ((currentObjHitInfo->unk58 & 3) && (currentObjHitInfo->unk5A != 8)) {
                temp_v1 = &(&D_800B23B8)[var_s7];
                *temp_v1 = &D_800B20B8[var_s7];
                (*temp_v1)->unk8 = currentObj;
                temp_a1 = currentObj->modelInsts[currentObj->modelInstIdx];
                if (temp_a1 != NULL) {
                    temp_a1->unk34 &= ~0x20;
                }
                (*temp_v1)->unk4 = currentObj->positionMirror.x - currentObjHitInfo->unk30;
                (*temp_v1)->unk0 = currentObj->positionMirror.x + currentObjHitInfo->unk30;
                var_s7++;
                if (var_s7 > 0x100) {
                    // " WARNING : OBJHITS hitlist has overflowed \n\n"
                }
            }
            currentObjHitInfo->unk9D = 0;
            currentObjHitInfo->unk9C = -1;
            currentObjHitInfo->unk58 &= ~8;
            if (currentObj->linkedObject != NULL) {
                currentObjHitInfo = currentObj->linkedObject->objhitInfo;
                currentObjHitInfo->unk9D = 0;
                currentObjHitInfo->unk9C = -1;
            }
        }
    }
    func_8002B7CC(&D_800B23B8, var_s7);
    sp74 = 1;
    var_s6 = 1;
    if (var_s7 >= 2) {
        sp68 = D_800B23BC;
        do {
            currentObj = sp68[0]->unk8;
            currentObjHitInfo = currentObj->objhitInfo;
            sp980 = currentObj->linkedObject;
            if (sp980 != NULL && (sp980->objhitInfo == NULL || !(sp980->objhitInfo->unk58 & 1))) {
                sp980 = NULL;
            }
            if (currentObjHitInfo->unk58 & 4) {
                s4 = sp74;
                if (((&D_800B23B8)[s4]->unk0 < sp68[0]->unk4) && (s4 < var_s7)) {
                    var_s5 = &(&D_800B23B8)[s4];
                    do {
                        s4++;
                    } while ((*&(&D_800B23B8)[s4])->unk0 < sp68[0]->unk4 && s4 < var_s7);
                }
                sp74 = s4;
                if (sp74 < var_s7) {
                    var_s5 = &(&D_800B23B8)[sp74];
                    if ((*var_s5)->unk4 < sp68[0]->unk0) {
                        do {
                            parentObj = (*var_s5)->unk8;
                            parentObjInfo = parentObj->objhitInfo;
                            if ((var_s6 != sp74) && (parentObj != currentObj->parent)) {
                                var_fv0 = currentObj->positionMirror.z - parentObj->positionMirror.z;
                                if (!(var_fv0 > 0.0f)) {
                                    var_fv0 = -var_fv0;
                                }
                                if (var_fv0 < (currentObjHitInfo->unk30 + parentObjInfo->unk30)) {
                                    var_fv0 = currentObj->positionMirror.y - parentObj->positionMirror.y;
                                    if (!(var_fv0 > 0.0f)) {
                                        var_fv0 = -var_fv0;
                                    }
                                    if (var_fv0 < (currentObjHitInfo->unk2C + parentObjInfo->unk2C) && parentObjInfo->unk58 & 1) {
                                        if (!(currentObjHitInfo->unk58 & 0x40) && !(parentObjInfo->unk58 & 0x40) && (!(parentObjInfo->unk58 & 4) || (var_s6 >= sp74))) {
                                            if ((parentObj->def->_unk98[0] & currentObj->def->_unk98[2]) && (currentObj->def->_unk98[0] & parentObj->def->_unk98[2])) {
                                                if (parentObjInfo->unk5A & 0x20) {
                                                    func_80027DAC(parentObj, currentObj, D_800B24B8, &sp55C, &sp14C, &spE8, &sp84, 0);
                                                } else if (currentObjHitInfo->unk5A & 0x20) {
                                                        func_80027DAC(currentObj, parentObj, D_800B24B8, &sp55C, &sp14C, &spE8, &sp84, 0);
                                                } else if ((currentObjHitInfo->unk5A == 0x10) || parentObjInfo->unk5A == 0x10) {
                                                    if (currentObjHitInfo->unk5B || parentObjInfo->unk5B) {
                                                        func_80029C04(currentObj, parentObj, currentObj, 0, 1, -1U, 0U);
                                                    }
                                                } else if (currentObjHitInfo->unk5B || parentObjInfo->unk5B) {
                                                    func_80028238(currentObj, parentObj);
                                                }
                                            }
                                        }
                                        if (!(currentObjHitInfo->unk58 & 0x100) && !(parentObjInfo->unk58 & 0x100)) {
                                            if (parentObj->def->_unk98[0] & currentObj->def->_unk98[1]) {
                                                if (parentObj->def->_unk98[1] & 0x80 || currentObj->def->_unk98[0] & parentObj->def->_unk98[1]) {
                                                    linkedParentObj = parentObj->linkedObject;
                                                    if (linkedParentObj != NULL && (linkedParentObj->objhitInfo == NULL || !(linkedParentObj->objhitInfo->unk58 & 1))) {
                                                        linkedParentObj = NULL;
                                                    }
                                                    func_80028DCC(currentObj, parentObj, sp980, linkedParentObj, gUpdateRateF);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            sp74++;
                            var_s5++;
                        } while (sp74 < var_s7 && (*var_s5)->unk4 < sp68[0]->unk0);
                    }
                }
            }
            var_s6++;
            sp68++;
        } while (var_s6 != var_s7);
    }

    var_s6 = 1;
    if (var_s7 >= 2) {
        sp68 = D_800B23BC;
        do {
            currentObj = sp68[0]->unk8;
            if (currentObj->objhitInfo->unk58 & 0x200) {
                func_80027934(currentObj, currentObj);
                linkedObj = currentObj->linkedObject;
                if (linkedObj != NULL) {
                    func_80027934(currentObj, linkedObj);
                }
            }
            var_s6++;
            sp68++;
        } while (var_s6 != var_s7);
    }

    var_s6 = 1;
    if (var_s7 >= 2) {
        sp68 = D_800B23BC;
        do {
            currentObj = sp68[0]->unk8;
            currentObjHitInfo = currentObj->objhitInfo;
            currentObjHitInfo->unk10.x = currentObj->srt.transl.x;
            currentObjHitInfo->unk10.y = currentObj->srt.transl.y;
            currentObjHitInfo->unk10.z = currentObj->srt.transl.z;
            if (currentObj->parent != NULL) {
                transform_point_by_object(currentObjHitInfo->unk10.x, currentObjHitInfo->unk10.y, currentObjHitInfo->unk10.z, &currentObjHitInfo->unk20.x, &currentObjHitInfo->unk20.y, &currentObjHitInfo->unk20.z, currentObj->parent);
            } else {
                currentObjHitInfo->unk20.x = currentObj->srt.transl.x;
                currentObjHitInfo->unk20.y = currentObj->srt.transl.y;
                currentObjHitInfo->unk20.z = currentObj->srt.transl.z;
            }
            currentObjHitInfo->unk1C = currentObj->animProgress;
            currentObjHitInfo->unk9E = 0;
            currentObjHitInfo->unk58 &= ~0x2000;
            if (((currentObjHitInfo->unk62 != 0) || (currentObjHitInfo->unk58 & 8)) && !(currentObjHitInfo->unk58 & 0x40)) {
                currentObj->speed.x = (currentObj->srt.transl.x - currentObj->positionMirror2.x) * gUpdateRateInverseF;
                currentObj->speed.z = (currentObj->srt.transl.z - currentObj->positionMirror2.z) * gUpdateRateInverseF;
            }
            var_s6++;
            sp68++;
        } while (var_s6 != var_s7);
    }
    func_80028D90();
}
#endif

static const char str_80099920[] = "ACTIVE POLY HITS overflow\n";
static const char str_8009993c[] = "ACTIVE POLY HITS overflow\n";
#ifndef NON_EQUIVALENT
void func_80027934(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_80027934.s")
#else
// https://decomp.me/scratch/R1u2I
void func_80027934(Object* obj, Object* otherObj) {
    u32 sp17C;
    Model* temp_fp;
    ModelInstance* temp_v0_2;
    s32 new_var;
    ObjectHitInfo* objectHitInfo; // sp16C
    f32 *temp_s2;
    f32 *temp_s3;
    u16 temp_t7_2;
    s32 var_t2;
    u8 temp_v1_3;
    Unk80027934 spF0;
    AABBs32 spD8;
    f32 spA8[4 * 3];
    f32 sp78[4 * 3];
    s32 temp_t8;
    s32 var_s0;
    s32 i;
    HitSphere* temp_v0_3;

    objectHitInfo = obj->objhitInfo;
    if (otherObj == obj) {
        sp17C = objectHitInfo->unk40 >> 4;
    } else {
        sp17C = objectHitInfo->unk40 & 0xF;
    }
    if (sp17C == 0) {
        return;
    }
    if (objectHitInfo->unk61 != 0) {
        return;
    }

    objectHitInfo = otherObj->objhitInfo;
    if (objectHitInfo->unk5A & 0x10) {
        var_s0 = 0;
        temp_v0_2 = otherObj->modelInsts[otherObj->modelInstIdx];
        temp_fp = temp_v0_2->model;
        temp_t8 = (temp_v0_2->unk34 >> 2) & 1;
        temp_s2 = temp_v0_2->unk1C[temp_t8];
        temp_s3 = temp_v0_2->unk1C[temp_t8 ^ 1];
        for (i = 0; i < temp_fp->hitSphereCount; i++) {
            temp_v0_3 = &temp_fp->hitSpheres[i];
            if ((i == temp_v0_3->unkC) && ((1 << temp_v0_3->unkD) & sp17C)) {
                if (temp_v0_3->unkA != 0) {
                    for (var_t2 = temp_v0_3->unkA; (u16)var_t2 != 0; var_t2 <<= 4) {
                        if (var_s0 < 4) {
                            new_var = var_s0 * 3;
                            temp_t7_2 = (((var_t2 & 0xF000) >> 0xC) + i);
                            spA8[new_var + 0] = temp_s2[temp_t7_2 * 4 + 1] + gWorldX;
                            spA8[new_var + 1] = temp_s2[temp_t7_2 * 4 + 2];
                            spA8[new_var + 2] = temp_s2[temp_t7_2 * 4 + 3] + gWorldZ;
                            sp78[new_var + 0] = temp_s3[temp_t7_2 * 4 + 1] + gWorldX;
                            sp78[new_var + 1] = temp_s3[temp_t7_2 * 4 + 2];
                            sp78[new_var + 2] = temp_s3[temp_t7_2 * 4 + 3] + gWorldZ;
                            spF0.unk40[var_s0] = temp_s2[temp_t7_2 * 4 + 0];
                            spF0.unk50[var_s0] = -1;
                            spF0.unk54[var_s0] = 6;
                            var_s0++;
                        } else {
                            // "ACTIVE POLY HITS overflow\n"
                        }
                    }
                } else {
                    if (var_s0 < 4) {
                        new_var = var_s0 * 3;
                        spA8[new_var + 0] = temp_s2[i * 4 + 1] + gWorldX;
                        spA8[new_var + 1] = temp_s2[i * 4 + 2];
                        spA8[new_var + 2] = temp_s2[i * 4 + 3] + gWorldZ;
                        sp78[new_var + 0] = temp_s3[i * 4 + 1] + gWorldX;
                        sp78[new_var + 1] = temp_s3[i * 4 + 2];
                        sp78[new_var + 2] = temp_s3[i * 4 + 3] + gWorldZ;
                        spF0.unk40[var_s0] = temp_s2[i * 4 + 0];
                        spF0.unk50[var_s0] = -1;
                        spF0.unk54[var_s0] = 6;
                        var_s0++;
                    } else {
                        // "ACTIVE POLY HITS overflow\n"
                    }
                }
            }
            
        }
    } else {
        var_s0 = 1;
        spA8[0] = obj->srt.transl.x;
        spA8[1] = obj->srt.transl.y;
        spA8[2] = obj->srt.transl.z;
        sp78[0] = objectHitInfo->unk10.x;
        sp78[1] = objectHitInfo->unk10.y;
        sp78[2] = objectHitInfo->unk10.z;
        spF0.unk40[0] = (f32) objectHitInfo->unk52;
        spF0.unk50[0] = -1;
        spF0.unk54[0] = 6;
    }

    if (var_s0 == 0) {
        return;
    }

    fit_aabb_around_cubes(&spD8, (Vec3f *) sp78, (Vec3f *) spA8, spF0.unk40, var_s0);
    func_80053750(otherObj, &spD8, objectHitInfo->unkA1);
    temp_v1_3 = func_8005509C(otherObj, (Vec3f *) sp78, (Vec3f *) spA8, var_s0, &spF0, 0);
    if (!temp_v1_3) {
        return;
    }

    if (temp_v1_3 & 1) {
        var_s0 = 0;
    } else if (temp_v1_3 & 2) {
        var_s0 = 1;
    } else if (temp_v1_3 & 4) {
        var_s0 = 2;
    } else {
        var_s0 = 3;
    }
    objectHitInfo->unk9C = spF0.unk50[var_s0];
    objectHitInfo->unk34 = spA8[var_s0 * 3 + 0];
    objectHitInfo->unk38 = spA8[var_s0 * 3 + 1];
    objectHitInfo->unk3C = spA8[var_s0 * 3 + 2];
    if (spF0.unk58[var_s0] != 0) {
        objectHitInfo->unk9D |= 2;
    } else {
        objectHitInfo->unk9D |= 1;
    }
}
#endif

void func_80027DAC(Object* obj, Object* obj2, Unk80030A24* arg2, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4, s32* arg5, s32* arg6, s32 arg7) {
    f32 var_fv0;
    f32 spE8;
    Vec3f spDC;
    Vec3f spD0;
    ModelInstance_0x14_0x14 sp9C;
    ModelInstance_0x14_0x14 sp68;
    Unk80030A24* sp64;
    ObjectHitInfo* temp_v0;
    ObjectHitInfo* sp5C;
    ModelInstance* sp58;
    ModelInstance* sp54;

    temp_v0 = obj->objhitInfo;
    sp5C = obj2->objhitInfo;
    if ((obj->objhitInfo->unk9F == 0) && (sp5C->unk9F == 0) && (sp5C->unk9E == 0) && (obj->objhitInfo->unk9E == 0)) {
        sp58 = obj->modelInsts[obj->modelInstIdx];
        sp54 = obj2->modelInsts[obj2->modelInstIdx];
        if (sp5C->unk5A & 1) {
            spDC.x = obj2->positionMirror.x - gWorldX;
            spDC.y = obj2->positionMirror.y;
            spDC.z = obj2->positionMirror.z - gWorldZ;
            if (func_8002DFB8(spDC, sp5C->unk52, sp58->unk14, sp58->model, arg2, &sp64, &spE8) != 0) {
                var_fv0 = obj2->unkA8 / obj->unkA8;
                if (var_fv0 > 1.0f) {
                    var_fv0 = 1.0f;
                }
                func_8002D0DC(&spDC, sp5C->unk52, obj2, arg2, sp58->unk14, sp58->model, sp64, var_fv0, spE8, &spD0);
                func_800287E4(obj, obj2, spD0.x, spD0.y, spD0.z, 0);
            }
        } else if (sp5C->unk5A & 2) {
            spDC.x = obj2->positionMirror.x - gWorldX;
            spDC.y = obj2->positionMirror.y;
            spDC.z = obj2->positionMirror.z - gWorldZ;
            if (func_8002E3D0(spDC, sp5C->unk52, sp58->unk14, sp58->model, arg2, &sp64, obj2->positionMirror.y + sp5C->unk56, obj2->positionMirror.y + (f32) sp5C->unk54, &spE8) != 0) {
                var_fv0 = obj2->unkA8 / obj->unkA8;
                if (var_fv0 > 1.0f) {
                    var_fv0 = 1.0f;
                }
                func_8002D69C(&spDC, sp5C->unk52, obj2, arg2, sp58->unk14, sp58->model, sp64, var_fv0, spE8, &spD0);
                func_800287E4(obj, obj2, spD0.x, spD0.y, spD0.z, 0);
            }
        } else if (sp5C->unk5A & 0x20) {
            if (arg7 <= 0) {
                arg7++;
                func_80027DAC(obj2, obj, arg2, arg3, arg4, arg5, arg6, arg7);
                return;
            }
            func_8002F498(sp58->unk14->unk0, sp58->unk14, sp58->model, arg3, &sp9C);
            func_8002F498(sp54->unk14->unk0, sp54->unk14, sp54->model, arg4, &sp68);
            if (func_8002F998(
                sp58->unk14, sp58->model, sp58->unk14->unk0,
                arg3, &sp9C, arg5,
                arg6, sp54->unk14, sp54->model,
                sp54->unk14->unk0, arg4, &sp68,
                arg2, &spE8
            ) != 0) {
                func_80030A24(obj, obj2, arg2, spE8, 0, &spD0);
                func_800287E4(obj2, obj, spD0.x, spD0.y, spD0.z, 1);
            }
        }
    }
}

#ifndef NON_EQUIVALENT
void func_80028238(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_80028238.s")
#else
// https://decomp.me/scratch/Zv5Yv
void func_80028238(Object* obj, Object* otherObj) {
    f32 spA4;
    f32 temp_fa1;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 temp_ft4_2;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 var_fa1;
    f32 var_ft4_2;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    s32 var_v0;
    f32 sp60;
    f32 sp5C;
    ObjectHitInfo* sp58;
    ObjectHitInfo* sp54;
    s8 sp53;
    f32 var_fv0;

    sp58 = obj->objhitInfo;
    sp54 = otherObj->objhitInfo;
    if (sp58->unk9E != 0) {
        return;
    }

    if (sp54->unk9E != 0) {
        return;
    }

    sp70 = otherObj->positionMirror.x - obj->positionMirror.x;
    sp6C = otherObj->positionMirror.y - obj->positionMirror.y;
    sp68 = otherObj->positionMirror.z - obj->positionMirror.z;
    sp60 = sp58->unk52;
    sp5C = sp54->unk52;
    sp53 = 0;
    if (sp54->unk5A & 2 || sp58->unk5A & 2) {
        if (sp6C > 0.0f) {
            if (sp58->unk5A & 2) {
                var_fa1 = obj->positionMirror.y + sp58->unk56;
            } else {
                var_fa1 = obj->positionMirror.y + sp60;
            }
            if (sp54->unk5A & 2) {
                var_fv0 = otherObj->positionMirror.y + sp54->unk54;
            } else {
                var_fv0 = otherObj->positionMirror.y - sp5C;
            }
            if (var_fa1 < var_fv0) {
                return;
            }
        } else {
            if (sp54->unk5A & 2) {
                var_fa1 = otherObj->positionMirror.y + sp54->unk56;
            } else {
                var_fa1 = otherObj->positionMirror.y + sp5C;
            }
            if (sp58->unk5A & 2) {
                var_fv0 = obj->positionMirror.y + sp58->unk54;
            } else {
                var_fv0 = obj->positionMirror.y - sp60;
            }

            if (var_fa1 < var_fv0) {
                return;
            }
        }
        sp53 = 1;
        sp6C = 0.0f;
    }

    spA4 = sqrtf((sp70 * sp70) + (sp6C * sp6C) + (sp68 * sp68));
    var_v0 = (s32) (f32) (s32) spA4;
    if (var_v0 > 0x400) {
        var_v0 = 0x400;
    }
    if (sp58->unk50 >= var_v0) {
        sp58->unk0 = otherObj;
        sp58->unk50 = var_v0;
    }
    var_v0 = (s32) (f32) (s32) spA4;
    if (var_v0 > 0x400) {
        var_v0 = 0x400;
    }
    if (sp54->unk50 >= var_v0) {
        sp54->unk0 = obj;
        sp54->unk50 = var_v0;
    }
    if (!(sp54->unk58 & 1)) {
        return;
    }

    sp8C = obj->positionMirror.x - sp58->unk20.x;
    sp88 = obj->positionMirror.y - sp58->unk20.y;
    sp84 = obj->positionMirror.z - sp58->unk20.z;
    if (sp53 != 0) {
        sp88 = 0.0f;
    }
    temp_fa1 = (sp8C * sp8C) + (sp88 * sp88) + (sp84 * sp84);
    if (temp_fa1 > 1.0f) {
        temp_fv1 = (((otherObj->positionMirror.x - sp58->unk20.x) * sp8C) + ((otherObj->positionMirror.y - sp58->unk20.y) * sp88) + ((otherObj->positionMirror.z - sp58->unk20.z) * sp84)) / temp_fa1;
        if (temp_fv1 >= 0.0f && temp_fv1 <= 1.0f) {
            sp9C = sp58->unk20.x + (temp_fv1 * sp8C);
            sp98 = sp58->unk20.y + (temp_fv1 * sp88);
            sp94 = sp58->unk20.z + (temp_fv1 * sp84);
            temp_fv0_2 = sp9C - otherObj->positionMirror.x;
            temp_fa1 = sp98 - otherObj->positionMirror.y;
            temp_ft4_2 = sp94 - otherObj->positionMirror.z;
            spA4 = sqrtf((temp_fv0_2 * temp_fv0_2) + (temp_fa1 * temp_fa1) + (temp_ft4_2 * temp_ft4_2));
        }
    }
    if ((spA4 < (sp5C + sp60)) && (spA4 > 0.0f)) {
        func_8002635C(otherObj, obj, sp58->unk5D, sp58->unk5E, 0);
        func_8002635C(obj, otherObj, sp54->unk5D, sp54->unk5E, 0);
        if (!(sp54->unk58 & 2) && !(sp58->unk58 & 2)) {
            sp9C = sp54->unk20.x - sp58->unk20.x;
            sp98 = sp54->unk20.y - sp58->unk20.y;
            sp94 = sp54->unk20.z - sp58->unk20.z;
            if (sp53 != 0) {
                sp98 = 0.0f;
            }
            temp_fv0_2 = sqrtf((sp9C * sp9C) + (sp98 * sp98) + (sp94 * sp94));
            if (temp_fv0_2 > 0.0f) {
                sp70 = sp9C / temp_fv0_2;
                sp6C = sp98 / temp_fv0_2;
                sp68 = sp94 / temp_fv0_2;
            } else {
                sp70 /= spA4;
                sp6C /= spA4;
                sp68 /= spA4;
            }
            spA4 = temp_fv0_2 - spA4;
            if (spA4 < 0.0f) {
                spA4 = -spA4;
            }
            sp70 *= spA4;
            sp6C *= spA4;
            sp68 *= spA4;
            func_800287E4(obj, otherObj, sp70, sp6C, sp68, 0);
        }
    }
}
#endif

void func_800287E4(Object* obj, Object* otherObj, f32 x, f32 y, f32 z, s32 arg5) {
    f32 sp74;
    f32 cosSquared;
    f32 temp_fa1;
    s32 v0;
    ObjectHitInfo* objhitInfo;
    ObjectHitInfo* otherObjhitInfo;
    f32 temp_fv1;
    s32 thetaB;
    f32 cos;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 thetaA;

    func_80032804(obj, otherObj);
    objhitInfo = obj->objhitInfo;
    otherObjhitInfo = otherObj->objhitInfo;
    objhitInfo->unk58 |= 8;
    otherObjhitInfo->unk58 |= 8;
    objhitInfo->unk0 = otherObj;
    otherObjhitInfo->unk0 = obj;
    if (obj->parent != NULL) {
        inverse_rotate_point_by_object(x, y, z, &ox, &oy, &oz, obj->parent);
    } else {
        ox = x;
        oy = y;
        oz = z;
    }

    if (otherObj->parent != NULL) {
        inverse_rotate_point_by_object(x, y, z, &dx, &dy, &dz, otherObj->parent);
    } else {
        dx = x;
        dy = y;
        dz = z;
    }

    if ((obj->group == GROUP_UNK1) && !(otherObjhitInfo->unk58 & 0x400)) {
        obj->srt.transl.x -= ox;
        obj->srt.transl.y -= oy;
        obj->srt.transl.z -= oz;
        if (arg5 == 0) {
            return;
        }

        obj->positionMirror.x -= x;
        obj->positionMirror.y -= y;
        obj->positionMirror.z -= z;
        return;
    }
    
    if ((otherObj->group == GROUP_UNK1) && !((objhitInfo->unk58) & 0x400)) {
        otherObj->srt.transl.x += dx;
        otherObj->srt.transl.y += dy;
        otherObj->srt.transl.z += dz;
        if (arg5 == 0) {
            return;
        }

        obj->positionMirror.x += x;
        obj->positionMirror.y += y;
        obj->positionMirror.z += z;
        return;
    }
    
    if (otherObjhitInfo->unk5B == 0) {
        if (objhitInfo->unk5B == 0) {
            return;
        }

        obj->srt.transl.x -= ox;
        obj->srt.transl.y -= oy;
        obj->srt.transl.z -= oz;
        if (arg5 == 0) {
            return;
        }

        obj->positionMirror.x -= x;
        obj->positionMirror.y -= y;
        obj->positionMirror.z -= z;
        return;
    }

    if (objhitInfo->unk5B == 0) {
        if (objhitInfo->unk58 & 0x400) {
            STUBBED_PRINTF(" Guard 1 ");
        }
        if (objhitInfo->unk58 & 0x400) {
            STUBBED_PRINTF(" player 1 ");
        }
        
        if (otherObjhitInfo->unk5B == 0) {
            return;
        }

        otherObj->srt.transl.x += dx;
        otherObj->srt.transl.y += dy;
        otherObj->srt.transl.z += dz;
        if (arg5 == 0) {
            return;
        }

        otherObj->positionMirror.x += x;
        otherObj->positionMirror.y += y;
        otherObj->positionMirror.z += z;
        return;
    }

    v0 = arctan2_f(-x, -z);
    thetaA = obj->srt.yaw - (v0 & 0xFFFF);
    CIRCLE_WRAP(thetaA);
    thetaB = otherObj->srt.yaw - ((v0 + 0x8000) & 0xFFFF);
    CIRCLE_WRAP(thetaB);
    
    cos = fcos16_precise(thetaA);
    cosSquared = cos * cos;
    sp74 = (objhitInfo->unk5B * cosSquared) + (objhitInfo->unk5C * (1.0f - cosSquared));
    
    cos = fcos16_precise(thetaB);
    cosSquared = cos * cos;
    temp_fa1 = (otherObjhitInfo->unk5B * cosSquared) + (otherObjhitInfo->unk5C * (1.0f - cosSquared));
    
    temp_fv1 = temp_fa1 / (sp74 + temp_fa1);
    
    obj->srt.transl.x -= ox * temp_fv1;
    obj->srt.transl.y -= oy * temp_fv1;
    obj->srt.transl.z -= oz * temp_fv1;
    otherObj->srt.transl.x += dx * (1.0f - temp_fv1);
    otherObj->srt.transl.y += dy * (1.0f - temp_fv1);
    otherObj->srt.transl.z += dz * (1.0f - temp_fv1);
}

void func_80028D2C(Object* obj) {
    s32 i = 0;

    while (i < 5 && D_800916E0[i] != NULL) {
        i++;
    }

    if (i == 5) {
        D_800916E0[0] = obj;
        return;
    }

    D_800916E0[i] = obj;
}

void func_80028D90(void) {
    s32 i = 0;

    while (i < 5) {
        D_800916E0[i++] = NULL;
    }
}

void func_80028DC4(void) {

}

void func_80028DCC(Object* obj, Object* obj2, Object* obj3, Object* obj4, f32 updateRate) {
    ObjectHitInfo* sp44;
    ObjectHitInfo* sp40;
    ObjectHitInfo* sp3C;
    ObjectHitInfo* sp38;
    u32 var_v1;
    s32 sp30;
    u8 sp2F;
    ModelInstance* sp28;
    u8 temp_v0_4;

    sp3C = obj->objhitInfo;
    sp38 = obj2->objhitInfo;
    if (obj3 != NULL) {
        sp44 = obj3->objhitInfo;
    } else {
        sp44 = NULL;
    }
    if (obj4 != NULL) {
        sp40 = obj4->objhitInfo;
    } else {
        sp40 = NULL;
    }
    sp2F = 0;
    var_v1 = sp3C->unk40;
    if ((var_v1 != 0) && (sp3C->unk61 == 0)) {
        if (obj->group == GROUP_UNK1) {
            sp28 = obj->modelInsts[obj->modelInstIdx];
            sp30 = ((s32) sp28->unk34 >> 2) & 1;
            if (sp3C->unk58 & 0x2000) {
                bcopy(D_800B20A8, (void* ) sp28->unk1C[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20AC, (void* ) sp28->unk1C[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
            } else {
                bcopy((void* ) sp28->unk1C[sp30], D_800B20A8, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk1C[sp30 ^ 1], D_800B20AC, sp28->model->hitSphereCount * 0x10);
            }
            sp28 = obj3->modelInsts[obj3->modelInstIdx];
            sp30 = ((s32) sp28->unk34 >> 2) & 1;
            if (sp3C->unk58 & 0x2000) {
                bcopy(D_800B20B0, (void* ) sp28->unk1C[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20B4, (void* ) sp28->unk1C[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
                var_v1 = sp3C->unk40;
            } else {
                bcopy((void* ) sp28->unk1C[sp30], D_800B20B0, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk1C[sp30 ^ 1], D_800B20B4, sp28->model->hitSphereCount * 0x10);
                var_v1 = sp3C->unk40;
                sp3C->unk58 |= 0x2000;
            }
        }
        if (var_v1 >> 4) {
            sp2F = func_80029C04(obj, obj2, obj, 1, 0, var_v1 >> 4, sp3C->unk44 >> 4);
        }
        if ((obj3 != NULL) && (sp2F == 0)) {
            if (sp3C->unk40 & 0xF) {
                sp2F = func_80029C04(obj3, obj2, obj, 1, 0, sp3C->unk40 & 0xF, sp3C->unk44 & 0xF);
            }
        }
        temp_v0_4 = sp2F;
        sp2F = 0;
        if ((((u32)sp2F < temp_v0_4) == 0) && (obj->group == GROUP_UNK1)) {
            func_8002949C(obj, obj2, obj3, sp3C, sp44, updateRate);
        }
    }
    if (obj2->def->_unk98[1] & 0x80) {
        return;
    }

    var_v1 = sp38->unk40;
    if ((var_v1 != 0) && (sp38->unk61 == 0)) {
        if (obj2->group == GROUP_UNK1) {
            sp28 = obj2->modelInsts[obj2->modelInstIdx];
            sp30 = (sp28->unk34 >> 2) & 1;
            if (sp38->unk58 & 0x2000) {
                bcopy(D_800B20A8, (void* ) sp28->unk1C[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20AC, (void* ) sp28->unk1C[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
            } else {
                bcopy((void* ) sp28->unk1C[sp30], D_800B20A8, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk1C[sp30 ^ 1], D_800B20AC, sp28->model->hitSphereCount * 0x10);
            }
            sp28 = obj4->modelInsts[obj4->modelInstIdx];
            sp30 = (sp28->unk34 >> 2) & 1;
            if (sp38->unk58 & 0x2000) {
                bcopy(D_800B20B0, (void* ) sp28->unk1C[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20B4, (void* ) sp28->unk1C[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
                var_v1 = sp38->unk40;
            } else {
                bcopy((void* ) sp28->unk1C[sp30], D_800B20B0, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk1C[sp30 ^ 1], D_800B20B4, sp28->model->hitSphereCount * 0x10);
                var_v1 = sp38->unk40;
                sp38->unk58 |= 0x2000;
            }
        }
        if (var_v1 >> 4) {
            sp2F = func_80029C04(obj2, obj, obj2, 1, 0, var_v1 >> 4, sp38->unk44 >> 4);
        }
        if ((obj4 != NULL) && (sp2F == 0)) {
            if (sp38->unk40 & 0xF) {
                sp2F = func_80029C04(obj4, obj, obj2, 1, 0, sp38->unk40 & 0xF, sp38->unk44 & 0xF);
            }
        }
        if ((sp2F == 0) && (obj2->group == GROUP_UNK1)) {
            func_8002949C(obj2, obj, obj4, sp38, sp40, updateRate);
        }
    }
}

void func_8002949C(Object* obj, Object* obj2, Object* obj3, ObjectHitInfo* objHitInfo, ObjectHitInfo* objHitInfo2, f32 updateRate) {
    Model* model;
    f32 sp288;
    f32 sp284;
    f32 sp280;
    f32 temp_fv0;
    f32 var_fs0;
    f32 var_fs1;
    s32 attachPointIdx;
    s32 sp26C;
    s32 matrixIdx;
    s32 sp264;
    u32 *sp260;
    u32 pad;
    s32 sp258;
    s32 i;
    s32 var_s4;
    s32 var_v1;
    ModelInstance* sp248;
    ModelInstance* sp244;
    AnimState animState; // sp1DC
    s32 stopLoop;
    Model* model3; // sp1D4
    MtxF sp194;
    MtxF* sp190;
    MtxF sp150;
    MtxF sp110;
    MtxF* sp10C;
    s32 tempJointCount;
    s16 sp106;
    SRT spEC;
    s8 sp80[0xEC - 0x80];
    s32 pad2;

    sp26C = updateRate;
    sp264 = objHitInfo->unk4 >> 2;
    if (sp264 > 0) {
        sp260 = objHitInfo->unk8;
    } else {
        sp260 = 0;
    }

    if (sp260 == 0) {
        return;
    }

    sp288 = obj->srt.transl.x - objHitInfo->unk10.x;
    sp284 = obj->srt.transl.y - objHitInfo->unk10.y;
    sp280 = obj->srt.transl.z - objHitInfo->unk10.z;
    temp_fv0 = objHitInfo->unk1C;\
    var_fs1 = obj->animProgress - temp_fv0;
    if (var_fs1 < 0.0f) {
        var_fs1 = 0.0f;
    }
    var_fs1 /= updateRate;
    sp248 = obj->modelInsts[obj->modelInstIdx];
    model = sp248->model;
    matrixIdx = sp248->unk34 & 1;
    animState.unk58[0] = 0;
    animState.unk58[1] = 0;
    animState.unk5C[0] = 0;
    var_fs0 = temp_fv0 + var_fs1;
    tempJointCount = model->jointCount;
    i = 0;
    if (tempJointCount > 0) {
        do {
            sp80[i] = model->joints[i].jointLayer_plusID;
            i++;
        } while (i < model->jointCount);
    }
    sp10C = sp248->matrices[matrixIdx];
    sp248->matrices[matrixIdx] = D_800B20A0;
    func_8001943C(obj, &sp194, 1.0f, 0.0f);
    sp106 = 0x1000;
    if (obj3 != NULL) {
        sp244 = obj3->modelInsts[obj3->modelInstIdx];
        model3 = sp244->model;
        attachPointIdx = obj->unkB0 & 0xFFFF & 3;
        sp258 = obj->def->pAttachPoints[attachPointIdx].bones[obj->modelInstIdx];
        spEC.transl.x = obj->def->pAttachPoints[attachPointIdx].pos.x;
        spEC.transl.y = obj->def->pAttachPoints[attachPointIdx].pos.y;
        spEC.transl.z = obj->def->pAttachPoints[attachPointIdx].pos.z;
        spEC.yaw = obj->def->pAttachPoints[attachPointIdx].rot.x;
        spEC.pitch = obj->def->pAttachPoints[attachPointIdx].rot.y;
        spEC.roll = obj->def->pAttachPoints[attachPointIdx].rot.z;
        spEC.scale = 1.0f;
        sp190 = (MtxF*)(((f32*)sp248->matrices[matrixIdx]) + (sp258 << 4));
        matrix_from_srt(&sp150, &spEC);
    } else {
        sp258 = -1;
    }
    stopLoop = FALSE;
    for (var_s4 = 1; var_s4 < sp26C; var_s4++) {
        var_v1 = (s32) (sp264 * var_fs0);
        if (var_v1 >= sp264) {
            var_v1 = sp264 - 1;
        }
        objHitInfo->unk44 = objHitInfo->unk40;
        tempJointCount = sp260[var_v1];
        objHitInfo->unk40 = tempJointCount;
        tempJointCount = objHitInfo->unk40;
        if (tempJointCount) {
            pad =  (u32)tempJointCount >> 4;
            pad2 = tempJointCount & 0xF;
            func_80029A14(model, sp248->animState0, &animState, var_fs0, 1);
            tempJointCount = -1;
            func_80029AB4(model->joints, model->jointCount, model->hitSpheres, model->hitSphereCount, pad, pad2 ? sp258 : -1);
            temp_fv0 = var_s4 / updateRate;
            sp194.m[3][0] = (objHitInfo->unk10.x - gWorldX) + (temp_fv0 * sp288);
            sp194.m[3][1] = objHitInfo->unk10.y + (temp_fv0 * sp284);
            sp194.m[3][2] = (objHitInfo->unk10.z - gWorldZ) + (temp_fv0 * sp280);
            func_8001B4F0(&sp248->matrices[matrixIdx], &sp194, &animState, model->joints, (s32) model->jointCount, &sp106, -1, 0);
            if (pad) {
                func_8001AC44(sp248, model, obj, NULL, sp248->matrices[matrixIdx], pad, obj->srt.scale);
                stopLoop = func_80029C04(obj, obj2, obj, 1, 0, pad, objHitInfo->unk44 >> 4);
            }
            if (stopLoop == 0 && objHitInfo2 != NULL && (pad2)) {
                matrix_concat_4x3(&sp150, sp190, &sp110);
                func_8001AC44(sp244, model3, obj3, &sp110, sp248->matrices[matrixIdx], pad2, obj->srt.scale);
                stopLoop = func_80029C04(obj3, obj2, obj, 1, 0, pad2, objHitInfo->unk44 & 0xF);
            }
        }
        if (stopLoop) {
            // break
            var_s4 = sp26C;
        }
        var_fs0 += var_fs1;
    }
    if ((!obj->def) && (!obj->def)) {}
    sp248->matrices[matrixIdx] = sp10C;
    for (i = 0; i < model->jointCount; i++) {
        model->joints[i].jointLayer_plusID = sp80[i];
    }
}

void func_80029A14(Model* model, AnimState* animState, AnimState* arg2, f32 arg3, u8 arg4) {
    arg2->anims[0] = animState->anims[0];
    arg2->anims[1] = animState->anims[1];
    arg2->anims2[0] = animState->anims2[0];
    arg2->anims2[1] = animState->anims2[1];
    arg2->animIndexes[0] = animState->animIndexes[0];
    arg2->unk60[0] = animState->unk60[0];
    arg2->totalAnimationFrames[0] = animState->totalAnimationFrames[0];
    if (arg4 != 0) {
        arg2->curAnimationFrame[0] = animState->totalAnimationFrames[0] * arg3;
    } else {
        arg2->curAnimationFrame[0] = animState->curAnimationFrame[0];
    }
    arg2->unk34[0] = animState->unk34[0];
    func_8001A1D4(model, arg2, 1);
}

void func_80029AB4(ModelJoint* joints, s32 jointsCount, HitSphere* hitSpheres, s32 hitSpheresCount, s32 arg4, s32 arg5) {
    s16 jointIndex;
    s32 i;

    for (i = 0; i < jointsCount; i++) {
        joints[i].jointLayer_plusID |= 0x80;
    }

    if (arg4 != 0) {
        for (i = 0; i < hitSpheresCount; i++) {
            if ((1 << hitSpheres[i].unkD) & arg4) {
                jointIndex = hitSpheres[i].jointIndex;
                if ((joints[jointIndex].jointLayer_plusID & 0x80)) {
                    while (jointIndex != -1) {
                        joints[jointIndex].jointLayer_plusID &= ~0x80;
                        jointIndex = joints[jointIndex].parentJointID;
                    }
                }
            }
        }
    }

    if (arg5 != -1 && (joints[arg5].jointLayer_plusID & 0x80)) {
        while (arg5 != -1) {
            joints[arg5].jointLayer_plusID &= ~0x80;
            arg5 = joints[arg5].parentJointID;
        }
    }
}

static const char str_80099970[] = "OBJHITS: Temporary array overflow in 'hitVolumes'\n";

static const char str_800999a4[] = "HIT VOLUMES: an object has too many hit spheres\n";
#ifndef NON_EQUIVALENT
u8 func_80029C04(Object *obj, Object *obj2, Object *obj3, s8 arg3, s8 arg4, u32 arg5, u32 arg6);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_80029C04.s")
#else
// https://decomp.me/scratch/WeRFa
u8 func_80029C04(Object* obj, Object* obj2, Object* obj3, s8 arg3, s8 arg4, u32 arg5, u32 arg6) {
    f32 sp1CC;
    f32 sp1C8;
    f32 sp1C4;
    f32 sp1C0;
    f32 sp1BC;
    f32 sp1B8;
    f32 sp1B4;
    f32 sp1B0;
    f32 sp1AC;
    f32 sp1A8;
    f32 sp1A0;
    f32 sp18C;
    f32 sp188;
    f32 sp184;
    f32 sp180;
    f32 sp17C;
    f32 sp174;
    f32* sp170;                                     /* compiler-managed */
    f32* sp16C;
    f32* sp168;                                     /* compiler-managed */
    MtxF sp128;
    HitSphere sp11A;
    HitSphere sp10C;
    HitSphere* spheres2; // sp108
    HitSphere* spheres; // sp104
    s32 sphereCount; // sp100
    s32 spFC;
    s32 spF8; // s7
    s32 spF4; // s3
    s8 spE6;
    ObjectHitInfo* spE0;
    ObjectHitInfo* spDC;
    ObjectHitInfo* spD8;
    u64 spB0[3];
    Model* model;
    ModelInstance* temp_v1;
    f32 temp_fv0;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_fs1;
    f32 var_fs2;
    f32 var_fs3;
    f32 var_fs4;
    f32 var_fs5;
    f32 var_ft2;
    f32 var_ft4;
    f32 var_ft5;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v0_4;
    s32 var_fp;
    s32 var_s4;
    s32 var_s5;
    s32 i;
    s32 j;
    u16 var_v1_4;
    f32* temp_s1;

    spE0 = obj->objhitInfo;
    spDC = obj2->objhitInfo;
    spD8 = obj3->objhitInfo;
    if (spDC->unk9E != 0 || spD8->unk9E != 0) {
        return 0U;
    }
    if (spDC->unk9F != 0 || spD8->unk9F != 0) {
        return 0U;
    }
    var_fp = 0;
    spE6 = 0;
    if ((arg3 != 0 && (spE0->unk5A & 0x10)) || (arg4 != 0 && spE0->unk5A == 0x10)) {
        temp_v1 = obj->modelInsts[obj->modelInstIdx];
        model = temp_v1->model;
        spFC = (s32) model->hitSphereCount;
        sp170 = temp_v1->unk24;
        sp168 = temp_v1->unk1C[(((s32) temp_v1->unk34 >> 2) & 1) ^ 1];
        spheres2 = model->hitSpheres;
        if (obj3 != obj) {
            sp1AC = model->maxAnimatedVertDistance * obj3->srt.scale;
        } else {
            sp1AC = model->maxAnimatedVertDistance * obj->srt.scale;
        }
        if (obj->srt.flags & 0x4000) {
            return 0U;
        }
    } else {
        spFC = 1;
        sp170 = sp128.m[0];
        sp168 = sp128.m[2];
        spheres2 = &sp10C;
        if (spE0->unk5A & 2) {
            var_fp = 1;
        }
        sp128.m[0][0] = spE0->unk52;
        sp128.m[0][1] = obj->positionMirror.x - gWorldX;
        sp128.m[0][2] = obj->positionMirror.y;
        sp128.m[0][3] = obj->positionMirror.z - gWorldZ;
        sp128.m[2][0] = sp128.m[0][0];
        sp128.m[2][1] = spE0->unk20.x - gWorldX;
        sp128.m[2][2] = spE0->unk20.y;
        sp128.m[2][3] = spE0->unk20.z - gWorldZ;
        sp10C.unkC = 0;
        sp10C.unkD = 0;
        sp10C.unkA = 0;
        sp1AC = sp128.m[0][0];
    }

    if ((arg3 != 0 && (spDC->unk5A & 0x10)) || (arg4 != 0 && spDC->unk5A == 0x10)) {
        temp_v1 = obj2->modelInsts[obj2->modelInstIdx];
        model = temp_v1->model;
        sphereCount = model->hitSphereCount;
        sp16C = temp_v1->unk24;
        spheres = model->hitSpheres;
        sp1B0 = model->maxAnimatedVertDistance * obj2->srt.scale;
        if (obj2->srt.flags & 0x4000) {
            return 0U;
        }
    } else {
        sp16C = sp128.m[1];
        sphereCount = 1;
        spheres = &sp11A;
        if (spDC->unk5A & 2) {
            spE6 = 1;
        }
        sp128.m[1][0] = spDC->unk52;
        sp128.m[1][1] = obj2->positionMirror.x - gWorldX;
        sp128.m[1][2] = obj2->positionMirror.y;
        sp128.m[1][3] = obj2->positionMirror.z - gWorldZ;
        sp128.m[3][0] = sp128.m[0][0];
        sp128.m[3][1] = spE0->unk20.x - gWorldX;
        sp128.m[3][2] = spE0->unk20.y;
        sp128.m[3][3] = spE0->unk20.z - gWorldZ;
        sp11A.unkC = 0;
        sp11A.unkD = 0;
        sp11A.unkA = 0;
        sp1B0 = sp128.m[1][0];
    }
    if (spFC > 0x40 || sphereCount > 0x40) {
        // "HIT VOLUMES: an object has too many hit spheres\n"
    }
    var_fs1 = obj->positionMirror.x - obj2->positionMirror.x;
    var_fs4 = obj->positionMirror.y - obj2->positionMirror.y;
    var_fs2 = obj->positionMirror.z - obj2->positionMirror.z;
    temp_fv0 = sqrtf(SQ(var_fs1) + SQ(var_fs4) + SQ(var_fs2));
    var_fs5 = temp_fv0;
    i = 0;
    if ((sp1B0 + sp1AC + 100.0f) < var_fs5) {
        return 0U;
    }
    spB0[2] = 0;
    spB0[1] = 0;
    spB0[0] = 0;
    
    for (i = 0; i < spFC; i++) {
        if (i == spheres2[i].unkC) {
            temp_v0_4 = 1 << spheres2[i].unkD;
            if (temp_v0_4 & arg5) {
                spB0[2] |= (1 << i);
            }
            if (temp_v0_4 & arg6) {
                spB0[0] |= (1 << i);
            }
        }
    }
    for (i = 0; i < sphereCount; i++) {
        if (i == spheres[i].unkC) {
            spB0[1] |= 1 << i;
        }
    }

    sp188 = -1.0f;
    do {
        var_s4 = 0;
        for (i = 0; i < spFC; i++) {
            if ((1 << i) & spB0[2]) {
                var_s5 = 1;
                sp1CC = sp170[i * 4 + 0];
                sp1C8 = sp170[i * 4 + 1];
                sp1C4 = sp170[i * 4 + 2];
                sp1C0 = sp170[i * 4 + 3];
                if ((1 << i) & spB0[0]) {
                    var_s5 = 0;
                }
                if (var_s5 == 0) {
                    sp1BC = sp168[i * 4 + 1];
                    sp1B8 = sp168[i * 4 + 2];
                    sp1B4 = sp168[i * 4 + 3];
                    sp184 = sp1C8 - sp1BC;
                    sp180 = sp1C4 - sp1B8;
                    sp17C = sp1C0 - sp1B4;
                    sp174 = SQ(sp184) + SQ(sp180) + SQ(sp17C);
                    if (sp174 > 0.0f) {
                        sp174 = 1.0f / sp174;
                    } else {
                        var_s5 = 1;
                    }
                }
                for (j = 0; j < sphereCount; j++) {
                    if ((1 << j) & spB0[1]) {
                        spF4 = 0;
                        temp_s1 = &sp16C[j * 4];
                        if (((i == 0) && (var_fp != 0)) || ((j == 0) && (spE6 != 0))) {
                            if (var_fp != 0) {
                                var_fa0 = spE0->unk54 + sp1C4;
                                var_fa1 = spE0->unk56 + sp1C4;
                                var_ft4 = temp_s1[2] - temp_s1[0];
                                var_ft5 = temp_s1[0] + temp_s1[2];
                            } else {
                                var_fa0 = sp1C4 - sp1CC;
                                var_fa1 = sp1C4 + sp1CC;
                                var_ft4 = temp_s1[2] + spDC->unk54;
                                var_ft5 = temp_s1[2] + spDC->unk56;
                            }
                            if ((!(var_ft4 < var_fa0) || !(var_ft5 < var_fa0)) && (!(var_fa1 < var_ft4) || !(var_fa1 < var_ft5))) {
                                sp18C = temp_s1[0] + sp1CC;
                                sp18C = SQ(sp18C);
                                var_fs1 = sp1C8 - temp_s1[1];
                                var_fs5 = SQ(var_fs1);
                                if (var_fs5 < sp18C) {
                                    var_fs2 = sp1C0 - temp_s1[3];
                                    var_fs5 += SQ(var_fs2);
                                    if (var_fs5 < sp18C) {
                                        var_fs4 = 0.0f;
                                        spF4 = 1;
                                    }
                                }
                            }
                        } else {
                            sp18C = temp_s1[0] + sp1CC;
                            sp18C = SQ(sp18C);
                            if (var_s5 != 0) {
                                var_fs1 = sp1C8 - temp_s1[1];
                                var_fs5 = SQ(var_fs1);
                                if (var_fs5 < sp18C) {
                                    var_fs4 = sp1C4 - temp_s1[2];
                                    var_fs5 += SQ(var_fs4);
                                    if (var_fs5 < sp18C) {
                                        var_fs2 = sp1C0 - temp_s1[3];
                                        var_fs5 += SQ(var_fs2);
                                        if (var_fs5 < sp18C) {
                                            spF4 = 1;
                                        }
                                    }
                                }
                            } else {
                                temp_fv0 = -((sp1BC * sp184) + (sp1B8 * sp180) + (sp1B4 * sp17C) + -((temp_s1[3] * sp17C) + ((temp_s1[1] * sp184) + (temp_s1[2] * sp180)))) * sp174;
                                if (temp_fv0 <= 0.0f) {
                                    var_fs1 = sp1BC - temp_s1[1];
                                    var_fs4 = sp1B8 - temp_s1[2];
                                    var_fs2 = sp1B4 - temp_s1[3];
                                } else {
                                    var_ft2 = sp1C0;
                                    if (temp_fv0 >= 1.0f) {
                                        var_fs1 = sp1C8 - temp_s1[1];
                                        var_fs4 = sp1C4 - temp_s1[2];
                                    } else {
                                        var_fs1 = (sp1BC + (temp_fv0 * sp184)) - temp_s1[1];
                                        var_fs4 = (sp1B8 + (temp_fv0 * sp180)) - temp_s1[2];
                                        var_ft2 = sp1B4 + (temp_fv0 * sp17C);
                                    }
                                    var_fs2 = var_ft2 - temp_s1[3];
                                }
                                var_fs5 = SQ(var_fs1) + SQ(var_fs4) + SQ(var_fs2);
                                if (var_fs5 < sp18C) {
                                    spF4 = 1;
                                }
                            }
                        }
                        if (spF4 != 0) {
                            if (var_s4 < 64) {
                                if (arg4 != 0) {
                                    if (var_fs5 > 0.0f) {
                                        sp18C = (sp18C - var_fs5) / sp18C;
                                        D_800B19A0[var_s4].unk14 = sp18C;
                                        D_800B19A0[var_s4].unk0 = var_fs1 * sp18C;
                                        D_800B19A0[var_s4].unk4 = var_fs2 * sp18C;
                                    }
                                } else {
                                    sp18C = sqrtf(SQ(var_fs1) + SQ(var_fs4) + SQ(var_fs2));
                                    if (sp18C > 0.0f) {
                                        var_fs1 /= sp18C;
                                        var_fs4 /= sp18C;
                                        var_fs2 /= sp18C;
                                    }
                                    D_800B19A0[var_s4].unk8 = temp_s1[0] * var_fs1;
                                    D_800B19A0[var_s4].unkC = temp_s1[0] * var_fs4;
                                    D_800B19A0[var_s4].unk10 = temp_s1[0] * var_fs2;
                                }
                                D_800B19A0[var_s4].unk18 = i;
                                D_800B19A0[var_s4].unk19 = j;
                                var_s4 += 1;
                            } /*else {
                                // "objhits.c: MAX_HITSTORE overflow\n" (default.dol)
                            }*/
                        }
                    } 
                }
            }
        }
        spB0[2] = 0;
        spB0[1] = 0;
        for (i = 0; i < var_s4; i++) {
            spF8 = D_800B19A0[i].unk18;
            spF4 = D_800B19A0[i].unk19;
            temp_a0 = spheres2[spF8].unkA;
            temp_a1 = spheres[spF4].unkA;
            var_v1_4 = temp_a0;
            if (temp_a0 != 0){
                do {
                    spB0[2] |= (1 << (spF8 + ((var_v1_4 & 0xF000) >> 0xC)));
                    var_v1_4 <<= 4;
                } while (var_v1_4);
            }
            var_v1_4 = temp_a1;
            if (temp_a1 != 0){
                do {
                    spB0[1] |= (1 << (spF4 + ((var_v1_4 & 0xF000) >> 0xC)));
                    var_v1_4 <<= 4;
                } while (var_v1_4);
            }
            if ((temp_a0 == 0) && (temp_a1 == 0)) {
                if (arg3 != 0) {
                    var_fs3 = spE6 != 0 ? sp170[spF8 * 4 + 2] : D_800B19A0[i].unkC + sp16C[spF4 * 4 + 2];
                    func_800261E8(obj2, obj, spD8->unk5F, spD8->unk60, spF4, D_800B19A0[i].unk8 + sp16C[spF4 * 4 + 1], var_fs3, D_800B19A0[i].unk10 + sp16C[spF4 * 4 + 3]);
                } else if (arg4 != 0) {
                    if (sp188 < D_800B19A0[i].unk14) {
                        sp188 = D_800B19A0[i].unk14;
                        sp1A8 = D_800B19A0[i].unk0;
                        sp1A0 = D_800B19A0[i].unk4;
                    }
                }
            } else if (temp_a0 == 0) {
                spB0[2] |= 1 << spF8;
            } else if (temp_a1 == 0) {
                spB0[1] |= 1 << spF4;
            }
        }
    } while (var_s4 != 0);
    if ((arg3 != 0) && (spDC->unk62 != 0)) {
        if (spE0->unk58 & 0x80) {
            func_800267A4(obj);
        }
        if (spDC->unk58 & 0x80) {
            func_800267A4(obj2);
        }
        return 1;
    }
    if (arg4 != 0 && (sp188 > 0.0f) && (obj == obj3)) {
        func_8002635C(obj2, obj, spD8->unk5D, spD8->unk5E, (s8) spF4);
        func_8002635C(obj, obj2, spDC->unk5D, spDC->unk5E, (s8) spF8);
        func_800287E4(obj, obj2, -sp1A8, 0.0f, -sp1A0, 0);
        return 1;
    }
    return 0;
}
#endif

#ifndef NON_EQUIVALENT
s32 func_8002AD3C(Object *obj, Vec3f *arg1, Vec3f *arg2, Vec3f *arg3, f32 *arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_8002AD3C.s")
#else
// https://decomp.me/scratch/z2C6y
s32 func_8002AD3C(Object* obj, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, f32* arg4) {
    HitSphere* temp_s4;
    Model* model;
    ModelInstance* modelInstance;
    ObjectHitInfo* objHitInfo;
    f32 temp_fa0;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fv0_2;
    f32 temp_fv0_3;
    f32 var_ft5;
    f32 var_fv1;
    s32 temp_a0;
    s32 i;
    s32 var_s1;
    u16 var_v1;
    s32 temp_s3;
    f32* temp_s5;
    s32 spEC;
    u64 spE8;
    f32 spE4;
    f32 spE0;
    f32 spDC;
    f32 spD8;
    u8 pad_spC4[0xD8 - 0xC4];
    f32 spC0;
    f32 spBC;
    u8 pad_spA8[0xBC - 0xA8];
    f32 spA4;
    f32 spA0;
    f32 sp9C;

    objHitInfo = obj->objhitInfo;
    modelInstance = obj->modelInsts[obj->modelInstIdx];
    if ((objHitInfo->unk9E != 0) || (objHitInfo->unk9F != 0)) {
        return FALSE;
    }
    if (objHitInfo->unk5A & 0x10) {
        model = modelInstance->model;
        spE4 = 10.0f;
        spE0 = arg2->x - arg1->x;
        spDC = arg2->y - arg1->y;
        spD8 = arg2->z - arg1->z;
        spA4 = arg1->x - gWorldX;
        spA0 = arg1->y;
        sp9C = arg1->z - gWorldZ;
        temp_s3 = model->hitSphereCount;
        temp_s5 = modelInstance->unk24;
        temp_s4 = model->hitSpheres;
        spE8 = 0;
        for (i = 0; i < temp_s3; i++) {
            if (i == temp_s4[i].unkC) {
                spE8 |= 1 << i;
            }
        }
        do {
            var_s1 = 0;
            for (i = 0; i < temp_s3; i++) {
                if (((1 << i) & spE8)) {
                    temp_fv0_2 = SQ(temp_s5[i * 4 + 0]);
                    temp_fv0_3 = spA4 - temp_s5[i * 4 + 1];
                    var_fv1 = spA0 - temp_s5[i * 4 + 2];
                    temp_fa0 = sp9C - temp_s5[i * 4 + 3];
                    spC0 = 2.0f * ((spE0 * temp_fv0_3) + (spDC * var_fv1) + (spD8 * temp_fa0));
                    var_ft5 = SQ(spC0);
                    temp_fs1 = SQ(spE0) + SQ(spDC) + SQ(spD8);
                    temp_fs0 = (4.0f * temp_fs1) * ((SQ(temp_fv0_3) + SQ(var_fv1) + SQ(temp_fa0)) - (temp_fv0_2));
                    spBC = var_ft5 - temp_fs0;
                    if (temp_fs0 < var_ft5) {
                        temp_fv0_3 = sqrtf(spBC);
                        temp_fs0 = 2.0f * temp_fs1;
                        if (temp_fs0 != 0.0f) {
                            spC0 = -spC0;
                            var_ft5 = (spC0 + temp_fv0_3) / temp_fs0;
                            if (var_ft5 > 0.0f) {
                                var_fv1 = (spC0 - temp_fv0_3) / temp_fs0;
                                if ((var_fv1 > 0.0f) && !(var_ft5 < var_fv1)) {
                                    var_ft5 = var_fv1;
                                }
                            } else {
                                var_fv1 = (spC0 - temp_fv0_3) / temp_fs0;
                                if (var_fv1 > 0.0f) {
                                    var_ft5 = var_fv1;
                                }
                            }
                            if ((var_ft5 >= 0.0f) && (var_ft5 <= 1.0f)) {
                                if (var_ft5 < spE4) {
                                    spE4 = var_ft5;
                                }
                                D_800B19A0[var_s1].unk18 = i;
                                var_s1 += 1;
                            }
                        }
                    }
                }
            }
            spE8 = 0;
            for (i = 0; i < var_s1; i++) {
                temp_a0 = D_800B19A0[i].unk18;
                var_v1 = temp_s4[temp_a0].unkA;
                while (var_v1) {
                    spE8 |= (1 << (temp_a0 + (u16)((var_v1 & 0xF000) >> 0xC)));
                    var_v1 <<= 4;
                }
            }
        } while (var_s1 != 0);
    }
    if (spE4 != 10.0f) {
        arg3->x = arg1->x + (spE4 * spE0);
        arg3->y = arg1->y + (spE4 * spDC);
        arg3->z = arg1->z + (spE4 * spD8);
        *arg4 = spE4;
        return TRUE;
    }
    *arg4 = 10.0f;
    return FALSE;
}
#endif

void func_8002B2D0(s32* arg0, s32 arg1, s32 arg2, HitSphere *hitSpheres) {
    s32 sp20[48];
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < arg1; i++) {
        if (hitSpheres[i].unkA == 0) {
            sp20[count] = i;
            count += 1;
        }
    }

    for (i = 0; i < count; arg0++, i++) {
        arg0[0] = arg2;
    }
}

void func_8002B410(Object* obj, s32 arg1) {
    SRT srt;
    ObjectPolyhits* sp24;
    MtxF *a0;

    sp24 = obj->polyhits;
    if (sp24 != NULL) {
        if (arg1 != 0) {
            sp24->unk10C = (sp24->unk10C + 1) & 1;
        }
        a0 = (MtxF *)((f32 *)sp24 + (sp24->unk10C << 4));
        srt.yaw = -obj->srt.yaw;
        if (obj->objhitInfo->unk58 & 0x800) {
            srt.pitch = 0;
        } else {
            srt.pitch = -obj->srt.pitch;
        }
        if (obj->objhitInfo->unk58 & 0x1000) {
            srt.roll = 0;
        } else {
            srt.roll = -obj->srt.roll;
        }
        srt.scale = 1.0f;
        srt.transl.x = -obj->srt.transl.x;
        srt.transl.y = -obj->srt.transl.y;
        srt.transl.z = -obj->srt.transl.z;
        matrix_from_srt_reversed(a0, &srt);
        srt.yaw = obj->srt.yaw;
        if (obj->objhitInfo->unk58 & 0x800) {
            srt.pitch = 0;
        } else {
            srt.pitch = obj->srt.pitch;
        }
        if (obj->objhitInfo->unk58 & 0x1000) {
            srt.roll = 0;
        } else {
            srt.roll = obj->srt.roll;
        }
        srt.scale = 1.0f;
        srt.transl.x = obj->srt.transl.x;
        srt.transl.y = obj->srt.transl.y;
        srt.transl.z = obj->srt.transl.z;
        matrix_from_srt((MtxF *)((f32 *)sp24 + ((sp24->unk10C + 2) << 4)), &srt);
        if (sp24->unk10D > 0) {
            sp24->unk10D--;
        }
    }
}

s32 func_8002B5A4(s32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
    return 0;
}

void func_8002B5C0(Object* obj) {
    s16 i;
    Unk800B199C* temp_v1;

    i = 0;
    while (i < 30) {
        temp_v1 = &D_800B199C[i];
        if ((temp_v1->unk0 != 0) && (obj == temp_v1->unk8)) {
            temp_v1->unk0 = 0;
        }
        i += 1;
    }
}

Unk800B199C* func_8002B61C(Object* obj, Object* otherObj) {
    Unk800B199C* temp_v1;
    s16 i;

    i = 0;
    while(i < 30) {
        temp_v1 = &D_800B199C[i];
        if ((temp_v1->unk0 != 0) && (obj == temp_v1->unk4) && (otherObj == temp_v1->unk8)) {
            temp_v1->unk0 = 0;
            return temp_v1;
        }
        i++;
    }
    return NULL;
}

Unk800B199C* func_8002B68C(Object* obj, Object* otherObj) {
    Unk800B199C* temp_v1;
    s16 i;

    i = 0;
    while (i < 30) {
        temp_v1 = &D_800B199C[i];
        if (temp_v1->unk0 == 0) {
            temp_v1->unk4 = obj;
            temp_v1->unk8 = otherObj;
            temp_v1->unk0 = 2;
            return temp_v1;
        }
        i++;
    }

    return NULL;
}

void func_8002B6EC(void) {
    s16 i;
    Unk800B199C* temp_v1;

    i = 0;
    while (i < 30) {
        temp_v1 = &D_800B199C[i];
        if (temp_v1->unk0 != 0) {
            temp_v1->unk0--;
        }
        i++;
    }

    D_800B1990 = gUpdateRateF;
}

void func_8002B74C(f32** arg0, s32 arg1) {
    f32 temp_fv0;
    f32 *temp_a3;
    s32 var_t0;
    f32 *temp;
    s32 i;

    for (i = 1; i < arg1; i++) {
        temp = arg0[i];
        temp_a3 = arg0[i];
        var_t0 = i;
        temp_fv0 = temp[1];
        while (temp_fv0 < arg0[var_t0-1][1]) {
            arg0[var_t0] = arg0[var_t0-1];
            var_t0--;
        }
        arg0[var_t0] = temp_a3;
    }
}

void func_8002B7CC(Unk800B20B8** arg0, s32 arg1) {
    Unk800B20B8* temp_a0_2;
    f32* temp_a0_3;
    s32 var_t0;
    s32 i;
    s32 var_v1;

    for (i = 1; (s32)((arg1 - 1) / 9) >= i; i = (i * 3) + 1) {}

    while (i > 0) {
        var_v1 = i + 1;
        while (var_v1 < arg1) {
            temp_a0_2 = arg0[var_v1];
            var_t0 = var_v1;
            while (i < var_t0 && temp_a0_2->unk4 < (arg0[var_t0 - i])->unk4) {
                arg0[var_t0] = arg0[var_t0 - i];
                var_t0 -= i;
            }
            var_v1++;
            arg0[var_t0] = temp_a0_2;
        }
        i /= 3;
    }
}


s32 func_8002B910(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32* argA, f32* argB, s32* argC, f32* argD, Vec3f* argE) {
    s32 sp64;
    Vec3f sp58;
    Vec3f sp4C;
    Vec3f sp40;
    f32 sp3C;
    f32 temp_fv0_3;
    f32 temp_fv1_3;
    f32 sp30;
    f32 sp2C;

    *argC = 0;
    sp2C = arg5 < arg4 ? arg4 : arg5;
    sp30 = arg7 < arg6 ? arg6 : arg7;
    VECTOR_SUBTRACT(*arg0, *arg2, sp58);
    vec3_cross_product_2(arg1, arg3, argE);
    sp3C = sqrtf((argE->z * argE->z) + ((argE->x * argE->x) + (argE->y * argE->y)));
    if (sp3C == 0.0f) {
        *argD = (arg3->z * sp58.z) + ((sp58.x * arg3->x) + (sp58.y * arg3->y));
        sp40.x = (*argD * arg3->x) + sp58.x;
        sp40.y = (*argD * arg3->y) + sp58.y;
        sp40.z = (*argD * arg3->z) + sp58.z;
        *argD = vec3_length(&sp40);
        *argA = 1.0737418e9f;
        *argB = 1.0737418e9f;
        return *argD <= (sp30 + sp2C);
    }
    VECTOR_SCALE((*argE), 1.0f / sp3C);
    *argD = (argE->z * sp58.z) + ((sp58.x * argE->x) + (sp58.y * argE->y));
    if (*argD > 0.0f) {
        *argD = *argD;
    } else {
        *argD = -*argD;
    }
    sp64 = 0;
    // @fake
    if (!arg5) {}
    if (*argD <= (sp30 + sp2C)) {
        sp64 = 1;
    }
    if (sp64 != 0) {
        vec3_cross_product(&sp58, arg3, &sp4C);
        *argA = -((argE->z * sp4C.z) + ((sp4C.x * argE->x) + (sp4C.y * argE->y))) / sp3C;
        vec3_cross_product(&sp58, arg1, &sp4C);
        *argB = -((argE->z * sp4C.z) + ((sp4C.x * argE->x) + (sp4C.y * argE->y))) / sp3C;
        temp_fv0_3 = *argA;
        if ((temp_fv0_3 >= 0.0f) && (temp_fv0_3 <= arg8)) {
            temp_fv1_3 = *argB;
            if ((temp_fv1_3 >= 0.0f) && (temp_fv1_3 <= arg9)) {
                *argC = *argD <= (((temp_fv1_3 / arg9) * (arg7 - arg6)) + (arg4 + ((arg5 - arg4) * (temp_fv0_3 / arg8)) + arg6));
            }
        }
    }
    return sp64;
}

s32 func_8002BD04(Vec3f* arg0, f32 arg1, Vec3f* arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6, f32 arg7, f32* arg8, f32* arg9, f32* argA) {
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fs0;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv1;
    f32 sp14;

    temp_ft4 = arg0->x - arg2->x;
    temp_ft5 = arg0->y - arg2->y;
    temp_fs0 = arg0->z - arg2->z;
    *arg8 = (arg3->z * temp_fs0) + ((temp_ft4 * arg3->x) + (temp_ft5 * arg3->y));
    if (arg7 < *arg8) {
        temp_fv1 = arg4->x - arg0->x;
        temp_fa0 = arg4->y - arg0->y;
        temp_fa1 = arg4->z - arg0->z;
        *arg9 = (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1);
        *argA = arg1 + arg6;
        return *arg9 <= (*argA * *argA);
    }
    if (*arg8 < 0.0f) {
        *arg9 = (temp_ft4 * temp_ft4) + (temp_ft5 * temp_ft5) + (temp_fs0 * temp_fs0);
        *argA = arg1 + arg5;
        return *arg9 <= (*argA * *argA);
    }
    temp_fv1 = (-*arg8 * arg3->x) + temp_ft4;
    temp_fa0 = (-*arg8 * arg3->y) + temp_ft5;
    temp_fa1 = (-*arg8 * arg3->z) + temp_fs0;
    *arg9 = (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1);
    sp14 = ((*arg8 / arg7) * (arg6 - arg5)) + (arg1 + arg5);
    *argA = sp14;
    return *arg9 <= (sp14 * sp14);
}

s32 func_8002BF0C(Vec3f* arg0, f32 arg1, Vec3f* arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6, f32 arg7, f32* arg8, f32* arg9, f32* argA) {
    f32 sp24;
    f32 pad_sp20;
    f32 sp1C;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 sp4;

    temp_fv0 = arg0->x - arg2->x;
    temp_fv1 = arg0->z - arg2->z;
    *arg8 = (arg3->x * temp_fv0) + (temp_fv1 * arg3->z);
    if (arg7 < *arg8) {
        *arg9 = ((arg4->x - arg0->x) * (arg4->x - arg0->x)) + ((arg4->z - arg0->z) * (arg4->z - arg0->z));
        *argA = arg1 + arg6;
        return *arg9 <= (*argA * *argA);
    }

    if (*arg8 < 0.0f) {
        // Not sure why this is required but.. oh well
        temp_ft5 = temp_fv0;
        *arg9 = (temp_ft5 * temp_ft5) + (temp_fv1 * temp_fv1);
        *argA = arg1 + arg5;
        return *arg9 <= (*argA * *argA);
    }

    sp1C = (-*arg8 * arg3->x) + temp_fv0;
    sp24 = (-*arg8 * arg3->z) + temp_fv1;
    *arg9 = (sp1C * sp1C) + (sp24 * sp24);
    temp_ft5 = ((*arg8 / arg7) * (arg6 - arg5)) + (arg1 + arg5);
    *argA = temp_ft5;
    return *arg9 <= (temp_ft5 * temp_ft5);
}

s32 func_8002C0C4(Vec3f* arg0, u32 arg1, u32 arg2, f32 arg3, Vec3f* arg4, f32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, f32 argC, f32 argD, f32 arg12, f32* arg13) {
    Vec3f sp2C;
    Vec3f sp20;
    f32 temp_fv0;

    vec3_sub((Vec3f* ) &arg4, (Vec3f* ) &arg0, &sp20);
    *arg13 = vec3_dot_product(&sp20, (Vec3f* ) &arg9);
    if (*arg13 > 0.0f) {
        temp_fv0 = *arg13;
        if (temp_fv0 < arg12) {
            vec3_add_with_scale(&sp20, (Vec3f* ) &arg9, temp_fv0, &sp2C);
            return ((sp2C.x * sp2C.x) + (sp2C.z * sp2C.z)) <= ((*arg13 * argD) + (arg3 + argC));
        }
    }
    temp_fv0 = *arg13;
    if (-(arg12 * 0.2f) < temp_fv0) {
        *arg13 = 0.0f;
        return vec3_length(&sp20) <= argC;
    }
    if (temp_fv0 < (arg12 * 1.2f)) {
        *arg13 = arg12;
        sp20.x += arg9;
        sp20.y += argA;
        sp20.z += argB;
        return vec3_length(&sp20) <= (argC + argD);
    }

    return 0;
}

s32 func_8002C278(Vec3f arg0, Vec3f arg1, f32 arg2, f32 arg3, Vec3f arg4, Vec3f arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32* argA, Vec3f* argB) {
    Vec3f sp6C;
    Vec3f sp60;
    f32 sp5C;
    f32 sp58;
    f32 temp_fv0;
    s32 sp50;
    f32 temp_fv1;

    VECTOR_SUBTRACT((arg1), (arg0), sp60);
    VECTOR_SUBTRACT((arg5), (arg4), sp6C);
    if (arg8 != 0.0f) {
        VECTOR_SCALE(sp60, 1.0f / arg8);
        VECTOR_SCALE(sp6C, 1.0f / arg9);
    }
    if (func_8002B910(&arg0, &sp60, &arg4, &sp6C, arg2, arg3, arg6, arg7, arg8, arg9, &sp5C, &sp58, &sp50, argA, argB)) {
        if (sp50 != 0) {
            return 1;
        }
        return 0;
    }

    return 0;
}

#ifndef NON_MATCHING
Vec3f* func_8002C3EC(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4, Vec3f* arg5, Vec3f* arg6, Vec3f* arg7, f32 arg8, f32 arg9, f32 argA, f32* argB, f32* argC, Vec3f* argD) ;
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_8002C3EC.s")
#else
// https://decomp.me/scratch/5vB1O
Vec3f* func_8002C3EC(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4, Vec3f* arg5, Vec3f* arg6, Vec3f* arg7, f32 arg8, f32 arg9, f32 argA, f32* argB, f32* argC, Vec3f* argD) {
    f32 pad_sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 var_fv0;
    f32 var_fv1;

    func_8002BD04(arg0, arg2, arg5, arg7, arg6, arg8, arg9, argA, &sp64, &sp68, &sp60);
    arg3 /= argA;
    sp64 /= argA;
    arg4 = sqrtf(arg4);
    sp68 = sqrtf(sp68) - arg2;
    arg4 -= arg2;
    if (arg3 != sp64) {
        var_fv0 = arg9 - arg8;
        if (var_fv0 == ((1.0f / (arg3 - sp64)) * (arg4 - sp68))) {
            if (sp64 > 0.0f) {
                var_fv1 = 1.0f;
            } else {
                var_fv1 = 0.0f;
            }
        } else {
            var_fv1 = (arg8 - (((arg4 - sp68) * (-arg3 / (arg3 - sp64))) + arg4)) / (((1.0f / (arg3 - sp64)) * (arg4 - sp68)) - var_fv0);
        }
    } else {
        var_fv1 = arg3;
        var_fv0 = arg9 - arg8;
    }
    *argB = var_fv1;
    *argC = (var_fv0 * var_fv1) + arg8 + arg2;
    if (sp64 != arg3) {
        var_fv0 = (var_fv1 - arg3) / (sp64 - arg3);
    } else if (sp68 != arg4) {
        var_fv0 = (*argC - arg4) / (sp68 - arg4);
    } else {
        var_fv0 = 0.0f;
    }
    VECTOR_SUBTRACT((*arg0), (*arg1), (*argD));
    VECTOR_SCALE((*argD), var_fv0);
    argD->x += arg1->x;
    argD->y += arg1->y;
    argD->z += arg1->z;
    return argD;
}
#endif

Vec3f* func_8002C658(Vec3f* arg0, f32 arg1, Vec3f* arg2, Vec3f* arg3, f32 arg4, f32 arg5, f32 arg6, Vec3f* arg7) {
    f32 temp_fv0;
    Vec3f sp60;
    Vec3f sp54;
    Vec3f sp48;
    Vec3f sp3C;
    Vec3f sp30;

    if (arg1 <= 0.0f) {
        VECTOR_SUBTRACT(*arg0, *arg3, *arg7);
        vec3_normalize(arg7);
        return arg7;
    }
    if (arg6 <= arg1) {
        VECTOR_SUBTRACT(*arg0, *arg3, *arg7);
        vec3_normalize(arg7);
        return arg7;
    }
    VECTOR_SUBTRACT(*arg3, *arg2, sp60);
    vec3_normalize(&sp60);
    vec3_add_with_scale(arg2, &sp60, arg1, &sp30);
    VECTOR_SUBTRACT(*arg0, sp30, sp54);
    vec3_normalize( &sp54);
    if (arg5 == arg4) {
        arg7->x = sp54.x;
        arg7->y = sp54.y;
        arg7->z = sp54.z;
        return arg7;
    }
    VECTOR_SCALE(sp60, arg1);
    vec3_add_with_scale(&sp60, &sp54, (arg5 - arg4) * (arg1 / arg6), &sp48);
    vec3_normalize(&sp48);
    VECTOR_SCALE(sp60, 1.0f / arg1);
    vec3_cross_product(&sp54, &sp60, &sp3C);
    vec3_normalize(&sp3C);
    vec3_cross_product(&sp3C, &sp48, arg7);
    return arg7;
}

Vec3f* func_8002C8C0(Vec3f* arg0, f32 arg1, f32 arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6, f32 arg7, Vec3f* arg8) {
    f32 temp_fv0;
    Vec3f sp38;
    Vec3f sp2C;

    if (arg2 < 0.0f) {
        VECTOR_SUBTRACT(*arg0, *arg3, *arg8);
        vec3_normalize(arg8);
        arg1 += arg5;
        VECTOR_SCALE(*arg8, arg1);
        arg8->x += arg3->x;
        arg8->y += arg3->y;
        arg8->z += arg3->z;
        return arg8;
    }

    if (arg7 < arg2) {
        VECTOR_SUBTRACT(*arg0, *arg4, *arg8);
        vec3_normalize(arg8);
        arg1 += arg6;
        VECTOR_SCALE(*arg8, arg1);
        arg8->x += arg4->x;
        arg8->y += arg4->y;
        arg8->z += arg4->z;
        return arg8;
    }

    VECTOR_SUBTRACT(*arg4, *arg3, sp38);
    temp_fv0 = 1.0f / arg7;
    VECTOR_SCALE(sp38, temp_fv0);
    vec3_add_with_scale(arg3, &sp38, arg2, &sp2C);
    VECTOR_SUBTRACT(*arg0, sp2C, *arg8);
    vec3_normalize(arg8);
    arg1 = arg5 + arg1 + ((arg6 - arg5) * (arg2 / arg7));
    VECTOR_SCALE(*arg8, arg1);
    arg8->x += sp2C.x;
    arg8->y += sp2C.y;
    arg8->z += sp2C.z;
    return arg8;
}

Vec3f* func_8002CBD4(Vec3f* arg0, f32 arg1, f32 arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6, f32 arg7, Vec3f* arg8) {
    f32 pad;
    Vec3f sp38;
    Vec3f sp2C;

    if (arg2 < 0.0f) {
        arg8->x = arg0->x - arg3->x;
        arg8->y = 0.0f;
        arg8->z = arg0->z - arg3->z;
        vec3_normalize(arg8);
        arg1 += arg5;
        VECTOR_SCALE(*arg8, arg1);
        arg8->x += arg3->x;
        arg8->y += arg3->y;
        arg8->z += arg3->z;
        return arg8;
    }
    if (arg7 < arg2) {
        arg8->x = arg0->x - arg4->x;
        arg8->y = 0.0f;
        arg8->z = arg0->z - arg4->z;
        vec3_normalize(arg8);
        arg1 += arg6;
        VECTOR_SCALE(*arg8, arg1);
        arg8->x += arg4->x;
        arg8->y += arg4->y;
        arg8->z += arg4->z;
        return arg8;
    }
    VECTOR_SUBTRACT(*arg4, *arg3, sp38);
    VECTOR_SCALE(sp38, 1.0f / arg7);
    vec3_add_with_scale(arg3, (Vec3f* ) &sp38, arg2, &sp2C);
    arg8->x = arg0->x - sp2C.x;
    arg8->y = 0.0f;
    arg8->z = arg0->z - sp2C.z;
    vec3_normalize(arg8);
    arg1 = arg5 + arg1 + ((arg6 - arg5) * (arg2 / arg7));
    VECTOR_SCALE(*arg8, arg1);
    arg8->x += sp2C.x;
    arg8->y += sp2C.y;
    arg8->z += sp2C.z;
    return arg8;
}

#ifndef NON_MATCHING
void func_8002CEC8(Vec3f* arg0, f32 arg1, f32 arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, Vec3f* arg10);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_8002CEC8.s")
#else
// https://decomp.me/scratch/7dHqX
void func_8002CEC8(Vec3f* arg0, f32 arg1, f32 arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, Vec3f* arg10) {
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;

    arg1 = arg8 / (arg9 - arg1);
    if (arg2 < 0) {
        arg10->x = arg0->x - arg3->x;
        arg10->y = 0;
        arg10->z = arg0->z - arg3->z;
        VECTOR_SCALE(*arg10, arg1);
        arg10->x += arg3->x;
        arg10->y += arg3->y;
        arg10->z += arg3->z;
        return;
    }

    if (arg7 < arg2) {
        arg10->x = arg0->x - arg4->x;
        arg10->y = 0;
        arg10->z = arg0->z - arg4->z;
        VECTOR_SCALE(*arg10, arg1);
        arg10->x += arg4->x;
        arg10->y += arg4->y;
        arg10->z += arg4->z;
        return;
    }

    sp1C = arg4->x - arg3->x;
    sp20 = arg4->y - arg3->y;
    sp24 = arg4->z - arg3->z;
    arg2 /= arg7;
    sp10 = arg3->x + (sp1C * arg2);
    sp14 = arg3->y + (sp20 * arg2);
    sp18 = arg3->z + (sp24 * arg2);
    arg10->x = arg0->x - sp10;
    arg10->y = 0;
    arg10->z = arg0->z - sp18;
    VECTOR_SCALE(*arg10, arg1);
    arg10->x += sp10;
    arg10->y += sp14;
    arg10->z += sp18;
}
#endif


s32 func_8002D0DC(Vec3f* arg0, f32 arg1, Object* obj, Unk80030A24* arg3, ModelInstance_0x14* arg4, Model *arg5, Unk80030A24* arg6, f32 arg7, f32 arg8, Vec3f* arg9) {
    Vec3f sp104;
    Vec3f spF8;
    s32 var_s3;
    Vec3f spE8;
    Vec3f spDC;
    s32 pad_spD8;
    Vec3f* temp_v0;
    f32 temp_fs0;
    s32 pad_spCC;
    s32 pad_spC8;
    s32 pad_spC4;
    Vec3f spB8;
    Vec3f spAC;
    Vec3f spA0;
    s32 pad_sp9C;
    Vec3f sp90;
    s32 pad_sp8C;
    s32 pad_sp88;
    f32 sp84;
    Vec3f *temp;
    Unk80030A24* sp7C;

    sp7C = arg3;
    obj->speed.x = (obj->srt.transl.x - obj->positionMirror2.x) / gUpdateRateF;
    obj->speed.y = (obj->srt.transl.y - obj->positionMirror2.y) / gUpdateRateF;
    obj->speed.z = (obj->srt.transl.z - obj->positionMirror2.z) / gUpdateRateF;
    VECTOR_SUBTRACT(obj->srt.transl, obj->positionMirror2, spE8);
    sp84 = vec3_length(&spE8);

    sp90.x = arg0->x;
    sp90.y = arg0->y;
    sp90.z = arg0->z;

    sp90.x -= spE8.x;\
    sp90.y -= spE8.y;\
    sp90.z -= spE8.z;

    spDC.x = 0.0f;\
    spDC.y = 0.0f;\
    spDC.z = 0.0f;

    sp104.x = 0.0f;\
    sp104.y = 0.0f;\
    sp104.z = 0.0f;

    temp = func_8002C658(&sp90, arg6->unk14.x, arg6->unk0, arg6->unk4, arg4->unk4[arg6->unk28], arg4->unk4[arg6->unk2C], arg4->unkC[arg6->unk28], &spA0);
    vec3_normalize(temp);
    while (arg3->unk28 != -1) {
        temp_v0 = func_8002C8C0(&sp90, arg1, arg3->unk14.x, arg3->unk0, arg3->unk4, arg4->unk4[arg3->unk28], arg4->unk4[arg3->unk2C], arg4->unkC[arg3->unk28], &spB8);
        arg3->unk24 /= arg8;
        VECTOR_SCALE(*temp_v0, arg3->unk24);
        spDC.x += temp_v0->x;
        spDC.y += temp_v0->y;
        spDC.z += temp_v0->z;
        temp_v0 = func_8002C658(arg0, arg3->unk14.x, arg3->unk0, arg3->unk4, arg4->unk4[arg3->unk28], arg4->unk4[arg3->unk2C], arg4->unkC[arg3->unk28], &spA0);
        vec3_normalize(temp_v0);
        sp104.x += temp_v0->x;
        sp104.y += temp_v0->y;
        sp104.z += temp_v0->z;
        arg3++;
    }
    vec3_normalize(&sp104);

    VECTOR_SUBTRACT(spDC, sp90, spF8);
    temp_fs0 = vec3_length(&spF8);

    VECTOR_SUBTRACT(spDC, *arg0, spF8);
    vec3_normalize(&spE8);

    if (temp_fs0 < sp84) {
        VECTOR_SCALE(spE8, sp84 - temp_fs0);
        vec3_reflect(&sp104, &spE8, &spAC);
    } else {
        spAC.x = 0.0f;
        spAC.y = 0.0f;
        spAC.z = 0.0f;
    }

    spDC.x += spAC.x;
    spDC.y += spAC.y;
    spDC.z += spAC.z;

    spAC.x = 0.0f;\
    spAC.y = 0.0f;\
    spAC.z = 0.0f;

    arg3 = sp7C;
    while (arg3->unk28 != -1) {
        temp_v0 = func_8002C8C0(&spDC, arg1, arg3->unk14.x, arg3->unk0, arg3->unk4, arg4->unk4[arg3->unk28], arg4->unk4[arg3->unk2C], arg4->unkC[arg3->unk28], &spB8);
        VECTOR_SCALE(*temp_v0, arg3->unk24);
        spAC.x += temp_v0->x;
        spAC.y += temp_v0->y;
        spAC.z += temp_v0->z;
        arg3++;
    }
    arg9->x = spAC.x - arg0->x;
    arg9->y = spAC.y - arg0->y;
    arg9->z = spAC.z - arg0->z;
    if (arg9->y > 0.0f) {
        arg9->y += 1.8;
    }
    return 1;
}

s32 func_8002D69C(Vec3f* arg0, f32 arg1, Object* obj, Unk80030A24* arg3, ModelInstance_0x14* arg4, Model *arg5, Unk80030A24* arg6, f32 arg7, f32 arg8, Vec3f* arg9) {
    Vec3f sp104;
    Vec3f spF8;
    s32 pad_spF0;
    Vec3f spE8;
    Vec3f spDC;
    s32 pad_spD8;
    s32 pad_spD4;
    s32 pad_spD0;
    s32 pad_spCC;
    s32 pad_spC8;
    s32 pad_spC4;
    Vec3f spB8;
    Vec3f spAC;
    Vec3f spA0;
    s32 pad_sp9C;
    Vec3f sp90;
    Vec3f *temp;
    Vec3f* temp_v0;
    f32 sp84;
    f32 temp_fv0;
    Unk80030A24* sp7C;

    sp7C = arg3;
    obj->speed.x = (obj->positionMirror.x - obj->positionMirror3.x) / gUpdateRateF;
    obj->speed.z = (obj->positionMirror.z - obj->positionMirror3.z) / gUpdateRateF;
    spE8.x = obj->positionMirror.x - obj->positionMirror3.x;
    spE8.y = obj->srt.transl.y - obj->positionMirror3.y;
    spE8.z = obj->positionMirror.z - obj->positionMirror3.z;

    sp84 = vec3_length(&spE8);

    sp90.x = arg0->x;
    sp90.y = arg0->y;
    sp90.z = arg0->z;

    VECTOR_SCALE(spE8, arg7);

    sp90.x -= spE8.x;\
    sp90.y -= spE8.y;\
    sp90.z -= spE8.z;

    spDC.x = 0.0f;\
    spDC.y = 0.0f;\
    spDC.z = 0.0f;

    sp104.x = 0.0f;\
    sp104.y = 0.0f;\
    sp104.z = 0.0f;

    temp = func_8002C658(&sp90, arg6->unk14.x, arg6->unk0, arg6->unk4, arg4->unk4[arg6->unk28], arg4->unk4[arg6->unk2C], arg4->unkC[arg6->unk28], &spA0);
    vec3_normalize(temp);
    while (arg3->unk28 != -1) {
        temp_v0 = func_8002CBD4(&sp90, arg1, arg3->unk14.x, arg3->unk0, arg3->unk4, arg4->unk4[arg3->unk28], arg4->unk4[arg3->unk2C], arg4->unkC[arg3->unk28], &spB8);
        arg3->unk24 /= arg8;
        VECTOR_SCALE(*temp_v0, arg3->unk24);
        spDC.x += temp_v0->x;
        spDC.y += temp_v0->y;
        spDC.z += temp_v0->z;
        temp_v0 = func_8002C658(arg0, arg3->unk14.x, arg3->unk0, arg3->unk4, arg4->unk4[arg3->unk28], arg4->unk4[arg3->unk2C], arg4->unkC[arg3->unk28], &spA0);
        vec3_normalize(temp_v0);
        sp104.x += temp_v0->x;
        sp104.y += temp_v0->y;
        sp104.z += temp_v0->z;
        arg3 += 1;
    }
    vec3_normalize(&sp104);
    spF8.x = spDC.x - sp90.x;
    spF8.y = 0.0f;
    spF8.z = spDC.z - sp90.z;
    temp_fv0 = vec3_length(&spF8);

    spF8.x = spDC.x - arg0->x;
    spF8.y = 0.0f;
    spF8.z = spDC.z - arg0->z;
    vec3_normalize(&spE8);
    if (temp_fv0 < sp84) {
        arg7 = ((1.0f - arg7) * 0.25f) + 0.75;
        VECTOR_SCALE(spE8, (sp84 - temp_fv0) * arg7);
        vec3_reflect(&sp104, &spE8, &spAC);
    } else {
        spAC.x = 0.0f;
        spAC.y = 0.0f;
        spAC.z = 0.0f;
    }
    spDC.x += spAC.x;
    spDC.y += spAC.y;
    spDC.z += spAC.z;

    spAC.x = 0.0f;\
    spAC.y = 0.0f;\
    spAC.z = 0.0f;

    arg3 = sp7C;
    while (arg3->unk28 != -1) {
        temp_v0 = func_8002CBD4(&spDC, arg1, arg3->unk14.x, arg3->unk0, arg3->unk4, arg4->unk4[arg3->unk28], arg4->unk4[arg3->unk2C], arg4->unkC[arg3->unk28], &spB8);
        VECTOR_SCALE(*temp_v0, arg3->unk24);
        spAC.x += temp_v0->x;
        spAC.y += temp_v0->y;
        spAC.z += temp_v0->z;
        arg3 += 1;
    }
    arg9->x = spAC.x - arg0->x;
    arg9->y = 0.0f;
    arg9->z = spAC.z - arg0->z;
    return 1;
}

s32 func_8002DC58(Unk8002F498* arg0, Unk8002F498* arg1, Unk8002F498* arg2, Unk8002F498* arg3) {
    Vec3f* sp104;
    Vec3f* sp100;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    s32 pad_spE8;
    Vec3f spE4;
    f32* spE0;
    f32* spDC;
    Vec3f *pad_spD8;
    Vec3f *pad_spD4;
    Vec3f *pad_spD0;
    Vec3f *pad_spCC;
    f32 spC8;
    s32 pad_spC4;
    s32 i; // s4
    s32 j; // s3
    s8 temp_s1;
    s8 temp_s7;
    f32 f0;
    f32 f2;

    sp104 = arg0->unk0;
    sp100 = arg1->unk0;
    VECTOR_SUBTRACT(*sp104, *sp100, spE4);
    spC8 = sqrtf(SQ(spE4.x) + SQ(spE4.y) + SQ(spE4.z));
    spE0 = arg0->unk4;
    spDC = arg1->unk4;
    for (i = 1; i < arg2->unk6F; i++) {
        temp_fs0 = spC8 - arg0->unk10[i];
        temp_s7 = arg2->unk20[i].unk0;
        temp_fs1 = spE0[i];
        temp_fs2 = spE0[temp_s7];
        for (j = 1, pad_spC4 = 1; j < arg3->unk6F; j++) {
            if (temp_fs0 < arg1->unk10[j]) {
                temp_s1 = arg3->unk20[j].unk0;
                f0 = spDC[j];
                f2 = spDC[temp_s1];
                // temp assignment below required for match
                if (func_8002C278(
                    (pad_spD8 = &sp104[i])[0], (pad_spD8 = &sp104[temp_s7])[0],
                    temp_fs1, temp_fs2,
                    (pad_spD8 = &sp100[j])[0], (pad_spD8 = &sp100[temp_s1])[0],
                    f0, f2,
                    arg0->unkC[i], arg1->unkC[j],
                    &spC8, &spE4
                ) != 0) {
                    arg0->unk18[i] = 1;
                    arg1->unk18[j] = 1;
                    arg0->unk18[temp_s7] = 1;
                    arg1->unk18[temp_s1] = 1;
                }
            }
        }
    }

    return 1;
}

s32 func_8002DFB8(Vec3f arg0, f32 arg1, ModelInstance_0x14* arg2, Model* model, Unk80030A24* arg4, Unk80030A24** arg5, f32* arg6) {
    Vec3f* sp11C;
    Unk80030A24* var_s0;
    Vec3f* temp_s2;
    Vec3f sp108;
    s32 pad[3];
    f32 spF8;
    Vec3f* temp_s1;
    f32 spF0;
    f32 temp_fa0;
    f32 temp_fs1;
    s32 var_s3;
    f32* spE0;
    f32 var_fa1;
    s32 var_fp;
    f32 spD4;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 spC4;
    f32 spC0;
    f32 spBC;
    s32 temp_s5;
    f32 zero_f;
    s32 pad2[2];

    var_fp = 0;
    if (arg2 == NULL) {
        return 0;
    }
    var_s0 = arg4;
    sp11C = arg2->unk0;
    spE0 = arg2->unk4;
    *arg5 = var_s0;
    zero_f = 0.0f;
    *arg6 = zero_f;
    spBC = sqrtf(SQ(sp11C->x - arg0.x) + SQ(sp11C->y - arg0.y) + SQ(sp11C->z - arg0.z)) - arg1;
    spF0 = 2.0f * arg0.x;
    spF8 = 2.0f * arg0.z;
    var_s3 = model->jointCount - 1;
    while (var_s3) {
        if (spBC < arg2->unk10[var_s3]) {
            temp_s5 = model->joints[var_s3].parentJointID;
            temp_fv1 = spE0[var_s3];
            temp_fa0 = spE0[temp_s5];
            arg2->unk18[var_s3] = 1;
            arg2->unk18[temp_s5] = 1;
            temp_s1 = &sp11C[var_s3];
            temp_s2 = &sp11C[temp_s5];
            temp_ft4 = (temp_s2->x + temp_s1->x) - spF0;
            temp_fs1 = (temp_s2->z + temp_s1->z) - spF8;
            if (temp_fa0 < temp_fv1) {
                var_fa1 = 2.0f * temp_fv1;
            } else {
                var_fa1 = 2.0f * temp_fa0;
            }
            temp_fv0 = arg2->unkC[var_s3] + var_fa1 + (2.0f * arg1);
            temp_fv0 *= temp_fv0;
            if ((SQ(temp_ft4) + SQ(zero_f) + SQ(temp_fs1)) < temp_fv0) {
                VECTOR_SUBTRACT(*temp_s2, *temp_s1, sp108);
                temp_fv0 = 1.0f / arg2->unkC[var_s3];
                VECTOR_SCALE(sp108, temp_fv0);
                if (func_8002BD04(&arg0, arg1, temp_s1, &sp108, temp_s2, temp_fv1, temp_fa0, arg2->unkC[var_s3], &spD4, &spC4, &spC0) != 0) {
                    arg2->unk18[var_s3] = 1;
                    arg2->unk18[temp_s5] = 1;
                    var_s0->unk14.y = (sqrtf(spC4) - spC0) + arg1;
                    if (var_s0->unk14.y > 0.0f) {
                        var_fa1 = var_s0->unk14.y;
                    } else {
                        var_fa1 = -var_s0->unk14.y;
                    }
                    var_s0->unk24 = 1 / var_fa1;
                    *arg6 += var_s0->unk24;
                    if (var_s0->unk14.y < (*arg5)->unk14.y) {
                        *arg5 = var_s0;
                    }
                    var_s0->unk0 = temp_s1;
                    var_s0->unk4 = temp_s2;
                    var_s0->unk14.x = spD4;
                    var_s0->unk20 = spC0;
                    var_s0->unk14.z = sqrtf(spC4);
                    var_s0->unk8.x = sp108.x;
                    var_s0->unk8.y = sp108.y;
                    var_s0->unk8.z = sp108.z;
                    var_s0->unk2C = temp_s5;
                    var_s0->unk28 = var_s3;
                    if (var_fp < 20) {
                        var_fp += 1;
                        var_s0 += 1;
                    }
                }
            }
        }
        var_s3 -= 1;
    }
    var_s0->unk28 = -1;
    return (s32) var_s0 != (s32) arg4;
}

#ifndef NON_MATCHING
s32 func_8002E3D0(Vec3f arg0, f32 arg1, ModelInstance_0x14* arg2, Model* model, Unk80030A24* arg4, Unk80030A24** arg5, f32 arg6, f32 arg7, f32* arg8);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_8002E3D0.s")
#else
// https://decomp.me/scratch/vFxGY
s32 func_8002E3D0(Vec3f arg0, f32 arg1, ModelInstance_0x14* arg2, Model* model, Unk80030A24* arg4, Unk80030A24** arg5, f32 arg6, f32 arg7, f32* arg8) {
    s32 pad[3];
    Vec3f sp110;
    f32 pad2;
    Vec3f* temp_s1;
    Vec3f* temp_s2;
    f32 sp100;
    Vec3f* temp_s7;
    f32 spF8;
    Unk80030A24* var_s0;
    f32 temp_fa0;
    f32 temp_fa1;
    f32* spE8;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 spDC;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 spCC;
    f32 spC8;
    f32 spC4;
    f32 var_fv1;
    s32 var_fp;
    s32 var_s3;
    s32 temp_s5;
    f32 zero_f;

    zero_f = 0.0f;
    var_fp = 0;
    if (arg2 == NULL) {
        return 0;
    }
    temp_s7 = arg2->unk0;
    if (temp_s7 == NULL) {
        return 0;
    }
    var_s0 = arg4;
    spE8 = arg2->unk4;
    *arg5 = var_s0;
    *arg8 = zero_f;
    spC4 = sqrtf(SQ(temp_s7->x - arg0.x) + SQ(zero_f) + SQ(temp_s7->z - arg0.z)) - arg1;
    spF8 = 2.0f * arg0.x;
    sp100 = 2.0f * arg0.z;
    var_s3 = model->jointCount - 1;
    while (var_s3) {
        if (spC4 < arg2->unk10[var_s3]) {
            temp_s5 = model->joints[var_s3].parentJointID;
            temp_s1 = &temp_s7[var_s3];
            temp_s2 = &temp_s7[temp_s5];
            temp_fa0 = spE8[var_s3];
            temp_fa1 = spE8[temp_s5];
            if ((!(arg6 < (temp_s1->y - temp_fa0))) || !(arg6 < (temp_s2->y - temp_fa1))) {
                if (!((temp_s1->y + temp_fa0) < arg7) || !((temp_s2->y + temp_fa1) < arg7)) {
                    temp_ft4 = (temp_s2->x + temp_s1->x) - spF8;
                    temp_ft5 = (temp_s2->z + temp_s1->z) - sp100;
                    if (temp_fa1 < temp_fa0) {
                        var_fv1 = 2.0f * temp_fa0;
                    } else {
                        var_fv1 = 2.0f * temp_fa1;
                    }
                    temp_fv0 = arg2->unkC[var_s3] + var_fv1 + (2.0f * arg1);
                    temp_fv0 *= temp_fv0;
                    if ((SQ(temp_ft4) + SQ(zero_f) + SQ(temp_ft5)) < (temp_fv0)) {
                        VECTOR_SUBTRACT(*temp_s2, *temp_s1, sp110);
                        var_fv1 = arg2->unkC[var_s3];
                        if (var_fv1 != 0.0f) {
                            temp_fv0 = 1.0f / var_fv1;
                            VECTOR_SCALE(sp110, temp_fv0);
                            var_fv1 = arg2->unkC[var_s3];
                        }
                        if (func_8002BF0C(&arg0, arg1, temp_s1, &sp110, temp_s2, temp_fa0, temp_fa1, var_fv1, &spDC, &spCC, &spC8) != 0) {
                            arg2->unk18[var_s3] = 1;
                            arg2->unk18[temp_s5] = 1;
                            var_s0->unk14.y = (sqrtf(spCC) - spC8) + arg1;
                            temp_fa0 = var_s0->unk14.y;
                            if (temp_fa0 > 0.0f) {
                                var_fv1 = temp_fa0;
                            } else {
                                var_fv1 = -temp_fa0;
                            }
                            var_s0->unk24 = 1.0f / var_fv1;
                            *arg8 += var_s0->unk24;
                            if (var_s0->unk14.y < (*arg5)->unk14.y) {
                                *arg5 = var_s0;
                            }
                            var_s0->unk0 = temp_s1;
                            var_s0->unk4 = temp_s2;
                            var_s0->unk14.x = spDC;
                            var_s0->unk20 = spC8;
                            var_s0->unk14.z = sqrtf(spCC);
                            var_s0->unk8.x = sp110.x;
                            var_s0->unk8.y = sp110.y;
                            var_s0->unk8.z = sp110.z;
                            var_s0->unk2C = temp_s5;
                            var_s0->unk28 = var_s3;
                            if (var_fp < 20) {
                                var_s0 += 1;
                                var_fp += 1;
                            }
                        }
                    }
                }
            }
        }
        var_s3 -= 1;
    }
    var_s0->unk28 = -1;
    return (s32) var_s0 != (s32) arg4;
}
#endif

void func_8002E84C(ModelInstance_0x14_0x14* arg0, ModelInstance_0x14_0x14* arg1) {
    arg0->unk18.x = arg1->unk18.x;
    arg0->unk18.y = arg1->unk18.y;
    arg0->unk18.z = arg1->unk18.z;
    arg0->unk24.x = arg1->unk24.x;
    arg0->unk24.y = arg1->unk24.y;
    arg0->unk24.z = arg1->unk24.z;
    arg0->unk0[0] = 1;
}

int func_8002E88C(Vec3f* arg0, Vec3f* arg1) {
    return arg0->x < arg1->x || arg0->y < arg1->y || arg0->z < arg1->z;
}

s32 func_8002E904(ModelInstance_0x14_0x14* arg0, ModelInstance_0x14_0x14* arg1) {
    if (func_8002E88C(&arg0->unk18, &arg1->unk24) || func_8002E88C(&arg1->unk18, &arg0->unk24)) {
        return 0;
    }

    return 1;
}

int func_8002E964(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    return (arg4 < (arg0 - arg1) && arg4 < (arg2 - arg3)) || ((arg0 + arg1) < arg5 && (arg2 + arg3) < arg5);
}

int func_8002EA0C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    return ((arg4 + arg5) < (arg0 - arg1) && (arg6 + arg7) < (arg0 - arg1)) || ((arg2 + arg3) < (arg4 - arg5) && (arg2 + arg3) < (arg6 - arg7));
}

// Return type has to be int
int func_8002EAD0(ModelInstance_0x14_0x14* arg0, Vec3f* arg1, Vec3f* arg2, f32 arg3, f32 arg4) {
    return !func_8002E964(arg1->x, arg3, arg2->x, arg4, arg0->unk18.x, arg0->unk24.x) &&
        !func_8002E964(arg1->y, arg3, arg2->y, arg4, arg0->unk18.y, arg0->unk24.y) &&
        !func_8002E964(arg1->z, arg3, arg2->z, arg4, arg0->unk18.z, arg0->unk24.z);
}

s32 func_8002EBA8(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3, Vec3f* arg4, Vec3f* arg5, f32 arg6, f32 arg7) {
    if ((arg0->x - arg2) < (arg1->x - arg3)) {
        if (func_8002EA0C(arg0->x, arg2, arg1->x, arg3, arg4->x, arg6, arg5->x, arg7)) {
            return 0;
        }
    } else if (func_8002EA0C(arg1->x, arg3, arg0->x, arg2, arg4->x, arg6, arg5->x, arg7)) {
        return 0;
    }

    if ((arg0->y - arg2) < (arg1->y - arg3)) {
        if (func_8002EA0C(arg0->y, arg2, arg1->y, arg3, arg4->y, arg6, arg5->y, arg7)) {
            return 0;
        }
    } else if (func_8002EA0C(arg1->y, arg3, arg0->y, arg2, arg4->y, arg6, arg5->y, arg7)) {
        return 0;
    }

    if ((arg0->z - arg2) < (arg1->z - arg3)) {
        if (func_8002EA0C(arg0->z, arg2, arg1->z, arg3, arg4->z, arg6, arg5->z, arg7)) {
            return 0;
        }
    } else if (func_8002EA0C(arg1->z, arg3, arg0->z, arg2, arg4->z, arg6, arg5->z, arg7)) {
        return 0;
    }

    return 1;
}

s32 func_8002EE38(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3, Vec3f* arg4, Vec3f* arg5, f32 arg6, f32 arg7) {
    if ((arg0->x - arg2) < (arg1->x - arg3)) {
        if (func_8002E964(arg4->x, arg6, arg5->x, arg7, arg0->x + arg2, arg1->x + arg3) != 0) {
            return 0;
        }
    } else if (func_8002E964(arg4->x, arg6, arg5->x, arg7, arg1->x + arg3, arg0->x - arg2) != 0) {
        return 0;
    }

    if ((arg0->y - arg2) < (arg1->y - arg3)) {
        if (func_8002E964(arg4->y, arg6, arg5->y, arg7, arg0->y + arg2, arg1->y + arg3) != 0) {
            return 0;
        }
    } else if (func_8002E964(arg4->y, arg6, arg5->y, arg7, arg1->y + arg3, arg0->y - arg2) != 0) {
        return 0;
    }

    if ((arg0->z - arg2) < (arg1->z - arg3)) {
        if (func_8002E964(arg4->z, arg6, arg5->z, arg7, arg0->z + arg2, arg1->z + arg3) != 0) {
            return 0;
        }
    } else if (func_8002E964(arg4->z, arg6, arg5->z, arg7, arg1->z + arg3, arg0->z - arg2) != 0) {
        return 0;
    }

    return 1;
}

void func_8002F04C(ModelInstance_0x14_0x14* arg0, Vec3f* arg1, Vec3f* arg2, f32 arg3, f32 arg4) {
    if ((arg2->x + arg4) < (arg1->x + arg3)) {
        arg0->unk18.x = arg1->x + arg3;
    } else {
        arg0->unk18.x = arg2->x + arg4;
    }

    if ((arg2->y + arg4) < (arg1->y + arg3)) {
        arg0->unk18.y = arg1->y + arg3;
    } else {
        arg0->unk18.y = arg2->y + arg4;
    }

    if ((arg2->z + arg4) < (arg1->z + arg3)) {
        arg0->unk18.z = arg1->z + arg3;
    } else {
        arg0->unk18.z = arg2->z + arg4;
    }

    if ((arg1->x - arg3) < (arg2->x - arg4)) {
        arg0->unk24.x = arg1->x - arg3;
    } else {
        arg0->unk24.x = arg2->x - arg4;
    }

    if ((arg1->y - arg3) < (arg2->y - arg4)) {
        arg0->unk24.y = arg1->y - arg3;
    } else {
        arg0->unk24.y = arg2->y - arg4;
    }

    if ((arg1->z - arg3) < (arg2->z - arg4)) {
        arg0->unk24.z = arg1->z - arg3;
    } else {
        arg0->unk24.z = arg2->z - arg4;
    }
}

void func_8002F164(ModelInstance_0x14_0x14* arg0, ModelInstance_0x14_0x14* arg1) {
    if (arg1->unk18.x < arg0->unk18.x) {
        arg1->unk18.x = arg0->unk18.x;
    }

    if (arg1->unk18.y < arg0->unk18.y) {
        arg1->unk18.y = arg0->unk18.y;
    }

    if (arg1->unk18.z < arg0->unk18.z) {
        arg1->unk18.z = arg0->unk18.z;
    }

    if (arg0->unk24.x < arg1->unk24.x) {
        arg1->unk24.x = arg0->unk24.x;
    }

    if (arg0->unk24.y < arg1->unk24.y) {
        arg1->unk24.y = arg0->unk24.y;
    }

    if (arg0->unk24.z < arg1->unk24.z) {
        arg1->unk24.z = arg0->unk24.z;
    }
}

f32 func_8002F214(ModelInstance_0x14_0x14* arg0) {
    return (arg0->unk18.x - arg0->unk24.x) * (arg0->unk18.y - arg0->unk24.y) * (arg0->unk18.z - arg0->unk24.z);
}

void func_8002F24C(ModelInstance_0x14_0x14* arg0, ModelInstance_0x14_0x14* arg1, s32* arg2) {
    s32 i;

    i = 0;
    while (arg0->unk0[0] != -1) {
        if (func_8002E904(arg0, arg1) != 0) {
            arg2[0] = i;
            arg2++;
        }
        arg0++;
        i++;
    }
    arg2[0] = i;
}

s32 func_8002F2D8(Vec3f* arg0, f32 arg1, ModelInstance_0x14_0x14* arg2) {
    f32 var_fv0;

    var_fv0 = 0.0f;
    if (arg0->x < arg2->unk24.x) {
        var_fv0 += SQ(arg0->x - arg2->unk24.x);
    } else if (arg2->unk18.x < arg0->x) {
        var_fv0 += SQ(arg0->x - arg2->unk18.x);
    }

    if (arg0->y < arg2->unk24.y) {
        var_fv0 += SQ(arg0->y - arg2->unk24.y);
    } else if (arg2->unk18.y < arg0->y) {
        var_fv0 += SQ(arg0->y - arg2->unk18.y);
    }

    if (arg0->z < arg2->unk24.z) {
        var_fv0 += SQ(arg0->z - arg2->unk24.z);
    } else if (arg2->unk18.z < arg0->z) {
        var_fv0 += SQ(arg0->z - arg2->unk18.z);
    }

    return var_fv0 <= arg1;
}

s32 func_8002F3DC(Vec3f* arg0, f32 arg1, ModelInstance_0x14_0x14* arg2) {
    f32 var_fv0;

    var_fv0 = 0.0f;
    if (arg0->x < arg2->unk24.x) {
        var_fv0 += SQ(arg0->x - arg2->unk24.x);
    } else if (arg2->unk18.x < arg0->x) {
        var_fv0 += SQ(arg0->x - arg2->unk18.x);
    }

    if (arg0->z < arg2->unk24.z) {
        var_fv0 += SQ(arg0->z - arg2->unk24.z);
    } else if (arg2->unk18.z < arg0->z) {
        var_fv0 += SQ(arg0->z - arg2->unk18.z);
    }

    return var_fv0 <= arg1;
}

void func_8002F498(Vec3f* arg0, ModelInstance_0x14* arg1, Model* model, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4) {
    s32 sp1EC;
    ModelInstance_0x14_0x14* var_s2;
    s32 var_a2;
    s32 sp1E0;
    ModelInstance_0x14_0x14 *sp50[100];
    s32 var_a3;

    bzero(sp50, model->jointCount * 4);
    arg4->unk18.x = arg0->x;
    arg4->unk18.y = arg0->y;
    arg4->unk18.z = arg0->z;
    arg4->unk24.x = arg0->x;
    arg4->unk24.y = arg0->y;
    arg4->unk24.z = arg0->z;
    sp1E0 = -1;
    sp1EC = 0;
    while (sp1EC < model->jointCount) {
        if (model->collisionB[sp1EC] != 0.0f) {
            arg3->unk0[0] = sp1EC;
            var_a2 = sp1EC;
            arg3->unk18.x = arg0[var_a2].x;
            arg3->unk18.y = arg0[var_a2].y;
            arg3->unk18.z = arg0[var_a2].z;
            arg3->unk24.x = arg0[var_a2].x;
            arg3->unk24.y = arg0[var_a2].y;
            arg3->unk24.z = arg0[var_a2].z;
            arg3->unk18.x += arg1->unk4[var_a2];
            arg3->unk18.y += arg1->unk4[var_a2];
            arg3->unk18.z += arg1->unk4[var_a2];
            arg3->unk24.x -= arg1->unk4[var_a2];
            arg3->unk24.y -= arg1->unk4[var_a2];
            arg3->unk24.z -= arg1->unk4[var_a2];
            sp50[var_a2] = arg3;
            var_a3 = 0;
            var_s2 = NULL;
            while(sp1E0 < model->joints[var_a2].parentJointID && var_s2 == NULL) {
                var_a2 = model->joints[var_a2].parentJointID;
                var_s2 = sp50[var_a2];
                sp50[var_a2] = arg3;
                var_a3++;
                if (arg3->unk18.x < (arg0[var_a2].x + arg1->unk4[var_a2])) {
                    arg3->unk18.x = arg0[var_a2].x + arg1->unk4[var_a2];
                }
                if (arg3->unk18.y < (arg0[var_a2].y + arg1->unk4[var_a2])) {
                    arg3->unk18.y = arg0[var_a2].y + arg1->unk4[var_a2];
                }
                if (arg3->unk18.z < (arg0[var_a2].z + arg1->unk4[var_a2])) {
                    arg3->unk18.z = arg0[var_a2].z + arg1->unk4[var_a2];
                }
                if (arg0[var_a2].x < (arg1->unk4[var_a2] + arg3->unk24.x)) {
                    arg3->unk24.x = arg0[var_a2].x - arg1->unk4[var_a2];
                }
                if (arg0[var_a2].y < (arg1->unk4[var_a2] + arg3->unk24.y)) {
                    arg3->unk24.y = arg0[var_a2].y - arg1->unk4[var_a2];
                }
                if (arg0[var_a2].z < (arg1->unk4[var_a2] + arg3->unk24.z)) {
                    arg3->unk24.z = arg0[var_a2].z - arg1->unk4[var_a2];
                }
            }
            if (arg4->unk18.x < arg3->unk18.x) {
                arg4->unk18.x = arg3->unk18.x;
            }
            if (arg4->unk18.y < arg3->unk18.y) {
                arg4->unk18.y = arg3->unk18.y;
            }
            if (arg4->unk18.z < arg3->unk18.z) {
                arg4->unk18.z = arg3->unk18.z;
            }
            if (arg3->unk24.x < arg4->unk24.x) {
                arg4->unk24.x = arg3->unk24.x;
            }
            if (arg3->unk24.y < arg4->unk24.y) {
                arg4->unk24.y = arg3->unk24.y;
            }
            if (arg3->unk24.z < arg4->unk24.z) {
                arg4->unk24.z = arg3->unk24.z;
            }
            if (sp1E0 == 0 && var_s2 == NULL) {
                var_s2 = sp50[0];
            }
            if (var_a3 < 3 && var_s2 != NULL && var_s2->unk30 < 3) {
                func_8002F164(arg3, var_s2);
                var_s2->unkC[var_s2->unk30] = model->joints[var_a2].parentJointID;
                var_s2->unk0[var_s2->unk30] = sp1EC;
                var_s2->unk30++;
            } else {
                if ((var_s2 != NULL) && (var_s2->unk30 >= 3)) {
                    sp50[var_a2] = arg3;
                }
                arg3->unkC[0] = model->joints[var_a2].parentJointID;
                arg3->unk30 = 1;
                arg3++;
                if (var_a2 == 0) {
                    sp1E0 = 0;
                }
            }
        }
        sp1EC++;
    }
    arg3->unk0[0] = -1;
}

static const char str_800999d8[] = " No Joints On Given Skeleton Object Error ";

static const char str_80099a04[] = "\nWARNING : OBJHITS skeleton vs skeleton hits overflow for cnt2 \n";
static const char str_80099a48[] = "\nWARNING : OBJHITS skeleton vs skeleton hits overflow for cnt \n";
#ifndef NON_EQUIVALENT
s32 func_8002F998(ModelInstance_0x14 *arg0, Model* arg1, Vec3f *arg2, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4, s32* arg5, s32* arg6, ModelInstance_0x14 *arg7, Model* arg8, Vec3f* arg9, ModelInstance_0x14_0x14* argA, ModelInstance_0x14_0x14* argB, Unk80030A24* argC, f32* argD);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_8002F998.s")
#else
// https://decomp.me/scratch/q2iY8
s32 func_8002F998(ModelInstance_0x14* arg0, Model* arg1, Vec3f* arg2, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4, s32* arg5, s32* arg6, ModelInstance_0x14* arg7, Model* arg8, Vec3f* arg9, ModelInstance_0x14_0x14* argA, ModelInstance_0x14_0x14* argB, Unk80030A24* argC, f32* argD) {
    Vec3f* sp14C;
    Vec3f* sp148;
    s32 sp13C;
    s32 sp138; // s2
    s32 sp130;
    s32 sp128;
    s32 sp124;
    s32 sp118;
    ModelInstance_0x14_0x14* sp114;
    ModelInstance_0x14_0x14* sp110;
    f32 spF4;
    Vec3f spE4;
    Unk80030A24* var_s3;
    Vec3f* temp_s0;
    Vec3f* temp_s6;
    Vec3f* var_s1;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fv0;
    f32 var_fa0;
    f32 var_fs0;
    f32 var_fs3;
    f32 var_fv1;
    s32 temp_a0_2;
    s32 temp_s5;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s4;
    s32 var_s2;
    s32 var_s5;

    sp118 = 0;
    temp_fs0 = func_8002F214(arg4);
    temp_fv0 = func_8002F214(argB);
    if ((temp_fs0 / temp_fv0) > 4.0f) {
        func_8002F24C(arg3, argB, arg5);
        for (sp13C = 0; sp13C != 25; sp13C++) {
            arg6[sp13C] = sp13C;
        }
    } else {
        for (sp13C = 0; sp13C < 25; sp13C++) {
            arg5[sp13C] = sp13C;
        }
        if ((temp_fv0 / temp_fs0) > 4.0f) {
            func_8002F24C(argA, arg4, arg6);
        } else {
            for (sp13C = 0; sp13C < 25; sp13C++) { arg6[sp13C] = sp13C; }
        }
    }
    sp114 = argA;
    sp110 = arg3;
    var_s3 = argC;
    sp128 = 0;
    arg3 = &sp110[arg5[sp128]];
    *argD = 0.0f;
    while (arg3->unk0[0] != -1){
        sp124 = 0;
        argA = &sp114[arg6[sp124]];
        while (argA->unk0[0] != -1) {
            if (!(arg3->unk18.x < argA->unk24.x) && !(arg3->unk18.y < argA->unk24.y) && !(arg3->unk18.z < argA->unk24.z) && !(argA->unk18.x < arg3->unk24.x) && !(argA->unk18.y < arg3->unk24.y) && !(argA->unk18.z < arg3->unk24.z)) {
                temp_v0 = arg3->unk30 - 1;
                do {
                    temp_v0_2 = arg3->unk0[temp_v0];
                    sp130 = temp_v0_2;
                    var_fs3 = arg0->unk4[temp_v0_2];
                    sp138 = arg1->joints[temp_v0_2].parentJointID;
                    sp14C = &arg2[temp_v0_2];
                    while (sp138 != arg3->unkC[temp_v0]) {
                        sp148 = &arg2[sp138];
                        temp_fs2 = arg0->unk4[sp138];
                        if (
                            (!(argA->unk18.x < (sp14C->x - var_fs3)) || !(argA->unk18.x < (sp148->x - temp_fs2))) &&
                            (!((sp14C->x + var_fs3) < argA->unk24.x) || !((sp148->x + temp_fs2) < argA->unk24.x)) &&
                            (!(argA->unk18.y < (sp14C->y - var_fs3)) || !(argA->unk18.y < (sp148->y - temp_fs2))) &&
                            (!((sp14C->y + var_fs3) < argA->unk24.y) || !((sp148->y + temp_fs2) < argA->unk24.y)) &&
                            (!(argA->unk18.z < (sp14C->z - var_fs3)) || !(argA->unk18.z < (sp148->z - temp_fs2)))
                        ) {
                            if (!((sp14C->z + var_fs3) < argA->unk24.z) || !((sp148->z + temp_fs2) < argA->unk24.z)) {
                                temp_a0_2 = argA->unk30 - 1;
                                do {
                                    var_s5 = argA->unk0[temp_a0_2];
                                    sp138 = arg8->joints[var_s5].parentJointID;
                                    var_s4 = var_s5;
                                    var_s1 = &arg9[var_s5];
                                    var_fs0 = arg7->unk4[var_s5];
                                    var_s5 = sp138;
                                    while (var_s5 != argA->unkC[temp_a0_2]) {
                                        temp_s0 = &arg9[var_s5];
                                        temp_s6 = &arg2[var_s5];
                                        temp_fs1 = arg7->unk4[var_s5];
                                        if (temp_fs1 <= var_fs0) {
                                            var_fv1 = 0;
                                        } else {
                                            var_fv1 = temp_fs1;
                                        }
                                        if (func_8002EBA8(sp14C, temp_s6, var_fs3, temp_fs2, var_s1, temp_s0, var_fs0, var_fv1) != 0) {
                                            if (func_8002C278(*sp14C, *temp_s6, var_fs3, temp_fs2, *var_s1, *temp_s0, var_fs0, temp_fs1, arg0->unkC[sp130], arg7->unkC[var_s4], &spF4, &spE4) != 0) {
                                                arg0->unk18[sp138] = 1;
                                                arg0->unk18[sp130] = 1;
                                                arg7->unk18[sp138] = 1;
                                                arg7->unk18[var_s4] = 1;
                                                var_s3->unk8.x = spE4.x;
                                                var_s3->unk8.y = spE4.y;
                                                var_s3->unk8.z = spE4.z;
                                                if (spF4 == 0.0f) {
                                                    spF4 = 0.1f;
                                                }
                                                var_s3->unk14.y = spF4;
                                                var_s3->unk24 = 1.0f;
                                                *argD += 1.0f;
                                                if (temp_fs2 < var_fs3) {
                                                    var_fa0 = var_fs3;
                                                } else {
                                                    var_fa0 = temp_fs2;
                                                }
                                                if (temp_fs1 < var_fs0) {
                                                    var_fv1 = var_fs0;
                                                } else {
                                                    var_fv1 = temp_fs1;
                                                }
                                                temp_fv0 = (var_fv1 + var_fa0) - var_s3->unk14.y;
                                                var_s3->unk14.z = temp_fv0;
                                                VECTOR_SCALE(var_s3->unk8, temp_fv0);
                                                var_s3->unk28 = sp13C;
                                                if (sp118 < 20) {
                                                    var_s3 += 1;
                                                    sp118 += 1;
                                                }
                                            }
                                        }
                                        var_s4 = var_s5;
                                        sp138 = arg8->joints[var_s5].parentJointID;
                                        var_s1 = temp_s0;
                                        var_fs0 = temp_fs1;
                                        var_s5 = sp138;
                                    }
                                    sp138 = temp_a0_2 < 1;
                                    temp_a0_2 -= 1;
                                } while (sp138 == 0);
                            }
                        }
                        sp14C = sp148;
                        var_fs3 = temp_fs2;
                        sp130 = sp138;
                        sp138 = arg1->joints[sp138].parentJointID;
                    }
                    sp138 = temp_v0 < 1;
                    temp_v0 -= 1;
                } while (sp138 == 0);
            }
            if (sp124 + 1 > 25) {
                // "\nWARNING : OBJHITS skeleton vs skeleton hits overflow for cnt2 \n"
            }
            sp124 += 1;
            argA = &sp114[arg6[sp124]];
        }
        if (sp128 + 1 > 25) {
            // "\nWARNING : OBJHITS skeleton vs skeleton hits overflow for cnt \n"
        }
        sp128 += 1;
        arg3 = &sp110[arg5[sp128]];
    }
    
    var_s3->unk28 = -1;
    return !((u32) var_s3 == (u32) argC);
}
#endif

static const char str_80099a88[] = " Result1 ";
static const char str_80099a94[] = " x %f y %f z %f \n";
static const char str_80099aa8[] = " Result2 ";
static const char str_80099ab4[] = " x %f y %f z %f \n";

void func_800302B4(u8* arg0, u8 arg1, ModelInstance_0x14_0x14* arg2, Vec3f* arg3, Vec3f* arg4, f32 arg5, f32 arg6) {
    if (arg0[0] & arg1) {
        return;
    }

    func_8002F04C(arg2, arg3, arg4, arg5, arg6);
    arg2->unk0[0] = -3;
    arg0[0] |= arg1;
}

void func_80030338(ModelInstance_0x14* arg0, Model* arg1, Vec3f *arg2, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4, Vec3f* arg6, Vec3f *arg7, f32 arg8) {
    Vec3f* var_s2;
    f32 temp_fs0;
    Vec3f sp1BC;
    s32 pad_sp1B8;
    s32 pad_sp1B4;
    Vec3f* var_s1;
    f32 var_fs1;
    f32 var_fv0;
    s32 temp_a0;
    ModelInstance_0x14_0x14* sp1A0;
    s32 spD8[50];
    s32 temp_var_s0;
    s32 var_s6;
    s32 var_s0;
    s32 i;
    f32 spC4;
    s32 var_s5;
    f32 spBC;
    f32 spB8;
    s32 spA4;

    spC4 = 0.0f;
    sp1A0 = arg0->unk14;
    for (i = 0; i < 50; i++) {
        spD8[i] = 23;
    }

    if (func_8002F2D8(arg7, SQ(arg8), arg4) == 0) {
        return;
    }

    while (arg3->unk0[0] != -1) {
        var_fv0 = 0.0f;
        if (arg7->x < arg3->unk24.x) { var_fv0 += SQ(arg7->x - arg3->unk24.x); } else if (arg3->unk18.x < arg7->x) { var_fv0 += SQ(arg7->x - arg3->unk18.x); }\
        if (arg7->y < arg3->unk24.y) { var_fv0 += SQ(arg7->y - arg3->unk24.y); } else if (arg3->unk18.y < arg7->y) { var_fv0 += SQ(arg7->y - arg3->unk18.y); }\
        if (arg7->z < arg3->unk24.z) { var_fv0 += SQ(arg7->z - arg3->unk24.z); } else if (arg3->unk18.z < arg7->z) { var_fv0 += SQ(arg7->z - arg3->unk18.z); }

        if (var_fv0 <= SQ(arg8)) {
            if (spD8[0] & 1) {
                func_8002E84C(sp1A0, arg3);
                sp1A0 += 1;
                spD8[0] &= ~1;
            }
            temp_a0 = arg3->unk30 - 1;
            do {
                var_s5 = arg3->unk0[temp_a0];
                var_s6 = var_s5;
                var_s2 = &arg2[var_s5];
                var_fs1 = arg0->unk4[var_s5];
                var_s0 = arg1->joints[var_s5].parentJointID;
                var_s5 = var_s0;
                while (var_s0 != arg3->unkC[temp_a0]) {
                    temp_var_s0 = var_s0;
                    var_s1 = &arg2[temp_var_s0];
                    temp_fs0 = arg0->unk4[temp_var_s0];
                    sp1BC.x = var_s1->x - var_s2->x;
                    sp1BC.y = var_s1->y - var_s2->y;
                    sp1BC.z = var_s1->z - var_s2->z;
                    var_fv0 = 1.0f / arg0->unkC[var_s6];
                    VECTOR_SCALE(sp1BC, var_fv0);
                    if (func_8002BD04(arg7, arg8, var_s2, &sp1BC, var_s1, var_fs1, temp_fs0, arg0->unkC[var_s6], &spC4, &spBC, &spB8) != 0) {
                        if (spD8[temp_var_s0] & 4) {
                            func_8002F04C(sp1A0, var_s2, &arg2[temp_var_s0], sqrtf(var_fs1), sqrtf(temp_fs0));
                            sp1A0->unk0[0] = -3;
                            sp1A0 += 1;
                            spD8[temp_var_s0] &= ~4;
                        }
                        arg0->unk18[temp_var_s0] = 1;
                        arg0->unk18[var_s6] = 1;
                    }
                    var_s6 = var_s5;
                    var_s0 = arg1->joints[var_s5].parentJointID;
                    var_s2 = var_s1;
                    var_fs1 = temp_fs0;
                    var_s5 = var_s0;
                }
                var_s0 = temp_a0 < 1;
                temp_a0 -= 1;
            } while (var_s0 == 0);
        }
        arg3++;
    }
    sp1A0->unk0[0] = -1;
}

void func_800307C4(f32 arg0, f32 arg1) {
    ModelInstance* modelInstance;
    Object** objects;
    s32 i;
    s32 sp38;
    s32 sp34;

    objects = get_world_objects(&sp38, &sp34);
    i = sp38;
    while (i < sp34) {
        modelInstance = objects[i]->modelInsts[objects[i]->modelInstIdx];
        if ((modelInstance != NULL) && (modelInstance->unk34 & 0x20)) {
            func_80030994(modelInstance, arg0, arg1);
        }
        i++;
    }
}

void func_80030880(void) {
    ModelInstance* temp_v1;
    Object** objects;
    s32 i;
    s32 sp28;
    s32 sp24;

    objects = get_world_objects(&sp28, &sp24);
    i = sp28;
    while (i < sp24) {
        temp_v1 = objects[i]->modelInsts[objects[i]->modelInstIdx];
        if ((temp_v1->unk34 & 0x20) != 0) {
            temp_v1->unk34 &= ~0x20;
            temp_v1->unk34 ^= 1;
        }
        i += 1;
    }
}

void func_80030994(ModelInstance* modelInstance, f32 arg1, f32 arg2) {
    f32* current;
    f32* target;

    current = modelInstance->matrices[(modelInstance->unk34 & 1) ^ 1]->m[3];
    target = (modelInstance->model->jointCount << 4) + current;
    while ((u32) current < (u32) target) {
        current[0] += arg1;
        current[2] += arg2;
        current += 0x10;
    }
}

void func_80030A24(Object *arg0, Object *arg1, Unk80030A24* arg2, f32 arg3, s32 arg4, Vec3f* arg5) {
    arg5->x = 0.0f;
    arg5->y = 0.0f;
    arg5->z = 0.0f;
    arg3 = (2.0f * (1.0f / arg3));
    while (arg2->unk28 != -1) {
        VECTOR_SCALE(arg2->unk8, arg2->unk24 * arg3);
        arg5->x += arg2->unk8.x;
        arg5->y += arg2->unk8.y;
        arg5->z += arg2->unk8.z;
        arg2++;
    }

    if (arg4 == 0) {
        arg5->y = 0.0f;
    }
}

#ifndef NON_MATCHING
void func_80030AEC(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/objhits/func_80030AEC.s")
#else
// https://decomp.me/scratch/5K9Wp
void func_80030AEC(Object* obj, Object* otherObj) {
    f32 spC4;
    f32 spC0;
    f32 spBC;
    f32 spB8;
    f32 pad_spB4;
    f32 pad_spB0;
    f32 spAC;
    f32 spA8;
    f32 pad_spA4;
    Vec3f sp98;
    Vec3f sp8C;
    Vec3f sp80;
    Vec3f sp74;
    f32 pad_sp70;
    f32 pad_sp6C;
    f32 pad_sp68;
    f32 temp_fa0;
    f32 sp34;
    f32 sp30;
    Vec3f sp50;
    Vec3f sp44;
    Vec3f sp38;

    func_80030E2C(obj, &spC4, &sp98, &sp80, &sp44);
    func_80030E2C(otherObj, &spC0, &sp8C, &sp74, &sp38);
    sp50.x = sp74.x - sp80.x;
    sp50.y = 0.0f;
    sp50.z = sp74.z - sp80.z;
    temp_fa0 = vec3_normalize(&sp50) / (obj->unkA8 + otherObj->unkA8);
    temp_fa0 = 1.0f - temp_fa0;
    spBC = (sp98.x * sp50.x) + (sp98.y * sp50.y) + (sp98.z * sp50.z);
    spB8 = (sp8C.x * sp50.x) + (sp8C.y * sp50.y) + (sp8C.z * sp50.z);
    if (spBC == 0.0f) {
        spBC = 0.2f;
    }
    if (spB8 == 0.0f) {
        spB8 = -0.2f;
    }
    spAC = (spC0 / spC4) * temp_fa0;
    spA8 = (spC4 / spC0) * temp_fa0;
    if (spAC > 10.0f) {
        spAC = sqrtf(spAC);
    }
    if (spA8 > 10.0f) {
        spA8 = sqrtf(spA8);
    }
    sp34 = spAC * spB8;
    sp30 = spA8 * spBC;
    if (((sp34 >= 0.0f) && (spBC <= 0.0f)) || ((sp34 <= 0.0f) && (spBC >= 0.0f))) {
        sp34 = -spBC;
    }
    if (((sp30 >= 0.0f) && (spB8 <= 0.0f)) || ((sp30 <= 0.0f) && (spB8 >= 0.0f))) {
        sp30 = -spB8;
    }
    pad_sp70 = sp50.x * sp34 + sp98.x;
    pad_sp6C = sp50.y * sp34 + sp98.y;
    pad_sp68 = sp50.z * sp34 + sp98.z;
    sp44.x += pad_sp70;
    sp44.y += pad_sp6C;
    sp44.z += pad_sp68;
    pad_sp70 = sp8C.x + sp50.x * sp30;
    pad_sp6C = sp8C.y + sp50.y * sp30;
    pad_sp68 = sp8C.z + sp50.z * sp30;
    sp38.x += pad_sp70;
    sp38.y += pad_sp6C;
    sp38.z += pad_sp68;
    obj->srt.transl.x = sp44.x;
    obj->srt.transl.z = sp44.z;
    obj->positionMirror.x = obj->srt.transl.x;
    obj->positionMirror.z = obj->srt.transl.z;
    otherObj->srt.transl.x = sp38.x;
    otherObj->srt.transl.z = sp38.z;
    otherObj->positionMirror.x = otherObj->srt.transl.x;
    otherObj->positionMirror.z = otherObj->srt.transl.z;
}
#endif

void func_80030E2C(Object* obj, f32* arg1, Vec3f* arg2, Vec3f* arg3, Vec3f* arg4) {
    *arg1 = SQ(obj->unkA8);
    *arg1 = SQ(*arg1);
    *arg1 *= 3.926991f;
    arg3->x = obj->srt.transl.x;
    arg3->y = obj->srt.transl.y;
    arg3->z = obj->srt.transl.z;
    arg4->x = obj->positionMirror2.x;
    arg4->y = obj->positionMirror2.y;
    arg4->z = obj->positionMirror2.z;
    arg2->x = obj->srt.transl.x - obj->positionMirror2.x;
    arg2->y = 0.0f;
    arg2->z = obj->srt.transl.z - obj->positionMirror2.z;
}
