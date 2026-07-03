#include "dlls/engine/2_camcontrol.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/segment_1460.h"

// Active camera during intro Galleon fight while riding the CloudRunner

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
} CamShipBattle; //len: 0x2C

static CamShipBattle* sState;

// offset: 0x0 | ctor
void camshipbattle_ctor(void *dll) { }

// offset: 0xC | dtor
void camshipbattle_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void camshipbattle_setup(Cam* cam, s32 arg1, void* data) {   
    sState = mmAlloc(sizeof(CamShipBattle), ALLOC_TAG_CAM_COL, ALLOC_NAME("camshipbattle"));
    
    sState->unk0 = 0.0f;
    sState->unk4 = 0.0f;
    sState->unk8 = 100.0f;
    sState->unk10 = 120.0f;
    sState->unkC = sState->unk10;
    sState->unk14 = 1.0f;
    sState->unk28 = 0;
    sState->unk1C = 75.0f;

    func_80000860(cam->player, cam->player, 0x84, 0);
}

// offset: 0xF4 | func: 1 | export: 1
void camshipbattle_control(Cam* cam) {
    Object* obj;
    f32 var_fa0;
    f32 var_fv0;
    f32 var_fv1;
    f32 speed;
    u32 returnVal;

    obj = cam->player;
    
    returnVal = 0;    
    if (obj != NULL) {
        // TODO: this is likely SB_Galleon export 11, confirm
        returnVal = ((DLL_Unknown*)obj->dll)->vtbl->func[11].withOneArgS32((s32)obj);
    }
    
    if (returnVal != sState->unk28) {
        if (returnVal == 2) {
            var_fa0 = 220.0f;
        } else {
            var_fa0 = 120.0f;
        }
        
        if ((returnVal != 2) && (returnVal != 5)) {
            var_fv0 = 75.0f;
            var_fv1 = 0.0f;
        } else {
            var_fv0 = 100.0f;
            var_fv1 = sState->unk4;
        }
        
        sState->unk28 = returnVal;
        sState->unk18 = var_fa0 - sState->unkC;
        sState->unk10 = sState->unkC;
        sState->unk24 = var_fv0 - (sState->unk1C + var_fv1);
        sState->unk20 = sState->unk1C;
        sState->unk14 = 0.0f;
    }
    
    if (sState->unk14 < 1.0f) {
        sState->unk14 += 0.005f * gUpdateRateF;
        if (sState->unk14 > 1.0f) {
            sState->unk14 = 1.0f;
        }
        
        sState->unkC = sState->unk10 + (sState->unk14 * sState->unk18);
        sState->unk1C = sState->unk20 + (sState->unk14 * sState->unk24);
    }
    
    if ((returnVal != 2) && (returnVal != 5)) {
        sState->unk0 -= obj->srt.roll / 3367.0f;
        sState->unk4 -= obj->srt.pitch / 1365.0f;

        sState->unk0 -= sState->unk0 * 0.02f * gUpdateRateF;
        sState->unk4 -= sState->unk4 * 0.02f * gUpdateRateF;
        
        cam->srt.transl.y = obj->srt.transl.y + sState->unk1C + sState->unk4;
    } else {
        sState->unk4 -= obj->srt.pitch / 2275.0f;
        sState->unk0 -= sState->unk0 * 0.02f * gUpdateRateF;
        sState->unk4 -= sState->unk4 * 0.013f * gUpdateRateF;
        
        cam->srt.transl.y = obj->srt.transl.y + sState->unk1C;
    }
    
    cam->srt.transl.x = obj->srt.transl.x + 98.0f + sState->unk8;
    cam->srt.transl.z = obj->srt.transl.z + sState->unk0;
    cam->srt.pitch = 0x708;
    cam->srt.yaw = 0x4000;
    cam->srt.roll = -obj->srt.roll >> 3;
    cam->letterboxGoal = 30;
    cam->fov = 40.0f;
    
    speed = (sState->unkC - sState->unk8) / 100.0f;
    if (speed > 3.0f) {
        speed = 3.0f;
    } else if (speed < -3.0f) {
        speed = -3.0f;
    }
    speed *= gUpdateRateF;
    sState->unk8 += speed;
}

// offset: 0x4CC | func: 2 | export: 2
void camshipbattle_free(Cam* cam) {
    mmFree(sState);
}

// offset: 0x50C | func: 3 | export: 3
// @bug: Missing 2nd parameter
void camshipbattle_func_50C(void* arg0) {

}
