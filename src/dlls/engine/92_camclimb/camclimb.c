#include "common.h"
#include "unktypes.h"

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
} CamClimbUnk; //len: 0x2C

static CamClimbUnk* _bss_0;

// offset: 0x0 | ctor
void camclimb_ctor(void *dll) { }

// offset: 0xC | dtor
void camclimb_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void camclimb_setup(CamControl_Data* arg0, s32 arg1, s32 arg2) {   
    _bss_0 = mmAlloc(sizeof(CamClimbUnk), ALLOC_TAG_CAM_COL, NULL);
    
    _bss_0->unk0 = 0.0f;
    _bss_0->unk4 = 0.0f;
    _bss_0->unk8 = 100.0f;
    _bss_0->unk10 = 120.0f;
    _bss_0->unkC = _bss_0->unk10;
    _bss_0->unk14 = 1.0f;
    _bss_0->unk28 = 0;
    _bss_0->unk1C = 75.0f;

    func_80000860(arg0->player, arg0->player, 0x84, 0);
}

// offset: 0xF4 | func: 1 | export: 1
void camclimb_control(CamControl_Data* camData) {
    Object* obj;
    f32 var_fa0;
    f32 var_fv0;
    f32 var_fv1;
    f32 speed;
    u32 returnVal;

    obj = camData->player;
    
    returnVal = 0;    
    if (obj != NULL) {
        /* TO-DO: the number of args doesn't match the player DLL's export 11. Maybe this isn't the player Object?
           The "camclimb" DLL doesn't seem to load while climbing, so maybe the args here are outdated?
           Check whether this DLL is used elsewhere!
        */
        returnVal = ((DLL_Unknown*)obj->dll)->vtbl->func[11].withOneArgS32((s32)obj);
    }
    
    if (returnVal != _bss_0->unk28) {
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
            var_fv1 = _bss_0->unk4;
        }
        
        _bss_0->unk28 = returnVal;
        _bss_0->unk18 = var_fa0 - _bss_0->unkC;
        _bss_0->unk10 = _bss_0->unkC;
        _bss_0->unk24 = var_fv0 - (_bss_0->unk1C + var_fv1);
        _bss_0->unk20 = _bss_0->unk1C;
        _bss_0->unk14 = 0.0f;
    }
    
    if (_bss_0->unk14 < 1.0f) {
        _bss_0->unk14 += 0.005f * gUpdateRateF;
        if (_bss_0->unk14 > 1.0f) {
            _bss_0->unk14 = 1.0f;
        }
        
        _bss_0->unkC = _bss_0->unk10 + (_bss_0->unk14 * _bss_0->unk18);
        _bss_0->unk1C = _bss_0->unk20 + (_bss_0->unk14 * _bss_0->unk24);
    }
    
    if ((returnVal != 2) && (returnVal != 5)) {
        _bss_0->unk0 -= obj->srt.roll / 3367.0f;
        _bss_0->unk4 -= obj->srt.pitch / 1365.0f;

        _bss_0->unk0 -= _bss_0->unk0 * 0.02f * gUpdateRateF;
        _bss_0->unk4 -= _bss_0->unk4 * 0.02f * gUpdateRateF;
        
        camData->srt.transl.y = obj->srt.transl.y + _bss_0->unk1C + _bss_0->unk4;
    } else {
        _bss_0->unk4 -= obj->srt.pitch / 2275.0f;
        _bss_0->unk0 -= _bss_0->unk0 * 0.02f * gUpdateRateF;
        _bss_0->unk4 -= _bss_0->unk4 * 0.013f * gUpdateRateF;
        
        camData->srt.transl.y = obj->srt.transl.y + _bss_0->unk1C;
    }
    
    camData->srt.transl.x = obj->srt.transl.x + 98.0f + _bss_0->unk8;
    camData->srt.transl.z = obj->srt.transl.z + _bss_0->unk0;
    camData->srt.pitch = 0x708;
    camData->srt.yaw = 0x4000;
    camData->srt.roll = -obj->srt.roll >> 3;
    camData->letterboxGoal = 30;
    camData->fov = 40.0f;
    
    speed = (_bss_0->unkC - _bss_0->unk8) / 100.0f;
    if (speed > 3.0f) {
        speed = 3.0f;
    } else if (speed < -3.0f) {
        speed = -3.0f;
    }
    speed *= gUpdateRateF;
    _bss_0->unk8 += speed;
}

// offset: 0x4CC | func: 2 | export: 2
void camclimb_free(CamControl_Data* camData) {
    mmFree(_bss_0);
}

// offset: 0x50C | func: 3 | export: 3
void camclimb_func_50C(s32 arg0) {

}
