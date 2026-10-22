#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/objsetups.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "macros.h"

typedef struct {
    s32 blockShapesUpdated;
    s8 unk4;
    u8 hitCount;
} BlastedObject_Data;

/*0x0*/ static s32 data_0 = 0;

/*0x0*/ static Object* sHitByList[4]; // unknown length

static s32 BlastedObject_hideBlockShapes(Object *self, s32 animatorID);

// offset: 0x0 | ctor
void BlastedObject_ctor(void* dll) { }

// offset: 0xC | dtor
void BlastedObject_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void BlastedObject_obj_Setup(Object* self, Ctrl51_Setup* setup, s32 reset) {
    BlastedObject_Data* objdata = self->data;
    
    objdata->blockShapesUpdated = FALSE;
    objSetPriority(self, OBJPRIORITY_BLASTEDOBJ);
    self->objhitInfo->unk58 |= 1;
    objdata->unk4 = (s8) setup->blastedObj.extraDamageStages;
    if (setup->blastedObj.hitCountGamebit != -1) {
        if ((objdata->hitCount = mainGetBits(setup->blastedObj.hitCountGamebit))) {
            objSetModel(self, objdata->hitCount);
        }
    }
    // @bug: Always re-enables hit count 0 bit
    mainSetBits(BIT_2DE, 1);
    self->srt.yaw = setup->blastedObj.yaw << 8;
    if (mainGetBits(setup->blastedObj.blastedGamebit) != 0) {
        objdata->blockShapesUpdated = BlastedObject_hideBlockShapes(self, setup->blastedObj.shapeAnimatorID);
    }
}

// offset: 0x138 | func: 1 | export: 1
void BlastedObject_obj_Control(Object* self) {
    Ctrl51_Setup* setup;
    s32 extraDamageStages;
    Object* hitBy;
    s32 alreadyHitBy;
    s32 i;
    s32 damageType;
    s32 hitIdx;
    BlastedObject_Data* objdata;

    objdata = self->data;
    setup = (Ctrl51_Setup*)self->setup;
    extraDamageStages = setup->blastedObj.extraDamageStages;
    if (objdata->blockShapesUpdated) {
        return;
    }
    if (mainGetBits(setup->blastedObj.blastedGamebit) != 0) {
        objdata->blockShapesUpdated = BlastedObject_hideBlockShapes(self, setup->blastedObj.shapeAnimatorID);
        return;
    }
    for (i = 0; i < self->objhitInfo->unk62; i++) {
        damageType = self->objhitInfo->hitTypeList[i];
        hitBy = self->objhitInfo->hitByList[i];
        alreadyHitBy = FALSE;
        if (damageType != Damage_Type_Explosion) {
            continue;
        }
        STUBBED_PRINTF(" Hit by barrels ");
        if (extraDamageStages == 0) {
            mainSetBits(setup->blastedObj.blastedGamebit, 1);
            return;
        }
        if (damageType == Damage_Type_Explosion) {
            // @bug: this loop is really problematic as it relies on a static list that is never cleared.
            //       it's possible for this list to fill up and eventually overrun.
            hitIdx = 0;
            while (hitIdx != objdata->hitCount) {
                if (hitBy == sHitByList[hitIdx++]) {
                    hitIdx = objdata->hitCount; // break;
                    alreadyHitBy = TRUE;
                }
            }
        }
        if (!alreadyHitBy) {
            sHitByList[objdata->hitCount] = hitBy;
            mainSetBits(objdata->hitCount + BIT_2DE, 0);
            mainSetBits(objdata->hitCount + BIT_2DE + 1, 1);
            if (setup->blastedObj.hitCountGamebit != -1) {
                mainSetBits(setup->blastedObj.hitCountGamebit, objdata->hitCount + 1);
            }
            data_0 = 300;
            if (objdata->hitCount >= extraDamageStages) {
                // Freed Kyte in CRF throne room
                mainSetBits(objdata->hitCount + BIT_2DE + 1, 0);
                STUBBED_PRINTF(" Hello My Friend ");
                mainSetBits(setup->blastedObj.blastedGamebit, 1);
                BlastedObject_hideBlockShapes(self, setup->blastedObj.shapeAnimatorID);
                objSetModel(self, 2);
                objdata->blockShapesUpdated = TRUE;
            } else {
                objSetModel(self, ++objdata->hitCount);
            }
        }
    }
}

/*0x28*/ static const char str_28[] = " Yee Had ";

// offset: 0x3D8 | func: 2 | export: 2
void BlastedObject_obj_Update(Object* self) { }

// offset: 0x3E4 | func: 3 | export: 3
void BlastedObject_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Ctrl51_Setup* setup = (Ctrl51_Setup*)self->setup;
    BlastedObject_Data* objdata = self->data;
    
    if ((visibility != 0) && (setup->blastedObj.extraDamageStages != 0) && !objdata->blockShapesUpdated) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x45C | func: 4 | export: 4
void BlastedObject_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x46C | func: 5 | export: 5
u32 BlastedObject_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x47C | func: 6 | export: 6
u32 BlastedObject_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(BlastedObject_Data);
}

// offset: 0x490 | func: 7
static s32 BlastedObject_hideBlockShapes(Object *self, s32 animatorID) {
    Block *block;
    BlockShape *shapes;
    s32 i;
    s32 ret;
    u8 shapeAnimatorID;

    ret = 0;
    block = mapGetBlockByIndex(
        mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if ((block == NULL) || (!(block->vtxFlags & 8))) {
        return 0;
    }
    shapes = block->shapes;
    for (i = 0; i < block->shapeCount; i++) {
        shapeAnimatorID = shapes[i].animatorID;
        if (animatorID == shapeAnimatorID) {
            shapes[i].flags |= RENDER_UNK800;
            shapes[i].flags |= RENDER_SHAPE_HIDE;
            ret = 1;
            dummy_label_963912: ;
        }
    }
    
    return ret;
}

// probably for BlastedObject_hideBlockShapes
/*0x34*/ static const char str_34[] = " AnimGroup %i \n";
/*0x44*/ static const char str_44[] = " Found Group  G1 %i ";
/*0x5C*/ static const char str_5C[] = " Current Flag %i ";
