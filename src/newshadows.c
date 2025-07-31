#include "common.h"
#include "sys/map.h"

static const char str_8009a9d0[] = "shadows: group overflow error\n";
static const char str_8009a9f0[] = "newshadows.c: max lift planes exceeded\n";

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004D470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004D698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004D844.s")

void func_8004D880(Object *arg0) {
    ObjectStruct64* temp_v0;

    if (arg0->ptr0x64 != NULL) {
        arg0->ptr0x64->flags &= ~0x20;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004D8A4.s")

void func_8004D974(s32 arg0) {
    D_80092BE8 = (s8) arg0;
}

void func_8004D984(s32 arg0) {
    D_80092BE8 = 1;
}

void func_8004D99C(s32 arg0) {
    D_80092BF8 = arg0;
}

void func_8004D9AC(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004D9B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004DABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004DBAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004E540.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004E64C.s")
#else
void func_8005BCE0(Object *object, u32 param_2, Gfx **gdl, Mtx **rspMtxs, u32 param_5, u32 param_6, u32 param_7, f32 y, u32 idx);
void _func_8004E64C(Object *object, Gfx **gdl, Mtx **rspMtxs, u32 param_4, u32 param_5)
{
    ObjectStruct64 *unk;
    Vec3f v0;
    Vec3f v1;

    unk = object->ptr0x64;
    if (unk->gdl != NULL)
    {
        if (unk->flags & 0x20)
        {
            bcopy(&object->srt.transl, &v0, sizeof(Vec3f));
            bcopy(&object->positionMirror, &v1, sizeof(Vec3f));
            bcopy(&unk->tr, &object->srt.transl, sizeof(Vec3f));

            if (object->parent != NULL) {
                transform_point_by_object(unk->tr.x, unk->tr.y, unk->tr.z, &unk->tr.x, &unk->tr.y, &unk->tr.z, object->parent);
            } else {
                bcopy(&unk->tr, &object->positionMirror, sizeof(Vec3f));
            }
        }

        if (unk->flags & 0x8) {
            // This function seems to be responsible for drawing shadows.
            func_8005BCE0(object, *(s16*)0x80092c18, gdl, rspMtxs, param_4, param_5, *(s8*)0x800bb170, *(f32*)0x800bb18c, unk->flags & 0x3);
        }

        if (unk->flags & 0x20) {
            bcopy(&v0, &object->srt.transl, sizeof(Vec3f));
            bcopy(&v1, &object->positionMirror, sizeof(Vec3f));
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004E7A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004EEC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004F378.s")

s32 func_8004FA3C(s32 arg0) {
    return 0;
}

s32 func_8004FA4C(void) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004FA58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_800502AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80050B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_800511E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_800516BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80051944.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80051C54.s")
#else
void func_80051C54(Vec3f* A, Vec3f* B, Vec3f* C, Vec3f* D) {
    f32 sp8;
    f32 sp4;
    f32 sp0;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_ft0;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    
    temp_fv0 = C->x - A->x;
    temp_fa0 = C->y - A->y;
    temp_ft4 = C->z - A->z;
    sp0 = B->x - A->x;
    sp4 = B->y - A->y;
    sp8 = B->z - A->z;
    
    D->x = (sp4 * temp_ft4) - (sp8 * temp_fa0);
    D->y = -((sp0 * temp_ft4) - (sp8 * temp_fv0));
    D->z = (sp0 * temp_fa0) - (sp4 * temp_fv0);

    // D->x = (((B->y - A->y) * (C->z - A->z)) - ((B->z - A->z) * (C->y - A->y)));
    // D->y = -(((B->x - A->x) * (C->z - A->z)) - ((B->z - A->z) * (C->x - A->x)));
    // D->z = (((B->x - A->x) * (C->y - A->y)) - ((B->y - A->y) * (C->x - A->x)));
}
#endif

s32 func_80051CFC(Vec3f* arg0, Vec3f* arg1) {
    s32 var_v1;
    float product;

    var_v1 = 1;
    if (D_80092BFC != 0) {
        return 1;
    }
    
    //Check for negative dot product?
    product = (arg0->x * arg1->x) + (arg0->y * arg1->y) + (arg0->z * arg1->z);
    if (product < 0.0f) {
        var_v1 = -1;
    }
    return var_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80051D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80051F64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80052148.s")

/** 
  * Seems to have something to do with getting the dot product of two vectors?
  */
void func_80052230(Vec3f *A, Vec3f *B, f32 *arg2)
{
    f32 AdotB;
    f32 AdotA;
    f32 BdotB;
    f32 product;
    Vec3f *Acopy;
    
    Acopy = A;
    AdotB = ((A->x * B->x) + (A->y * B->y)) + (A->z * B->z);
    product = (A->x * Acopy->x) + (A->y * A->y) + (A->z * A->z);
    AdotA = product;
    BdotB = ((B->x * B->x) + (B->y * B->y)) + (B->z * B->z);
    product = AdotA * BdotB;
    
    if (product){
        BdotB = sqrtf(product);
    }
    if (BdotB != 0){
        *arg2 = AdotB / BdotB;
        return;
    }
    *arg2 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80052300.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80052644.s")

#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_800528AC.s")
