#include "dlls/engine/2_camcontrol.h"
#include "sys/main.h"
#include "sys/memory.h"

typedef struct {
    u8 _unk0[0x8 - 0x0];
} CamCrawl;

/*0x0*/ static CamCrawl* sState;

// offset: 0x0 | ctor
void camcrawl_ctor(void *dll) { }

// offset: 0xC | dtor
void camcrawl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void camcrawl_setup(Cam* cam, s32 arg1, void* data) {
    sState = mmAlloc(sizeof(CamCrawl), ALLOC_TAG_CAM_COL, NULL);
}

// offset: 0x68 | func: 1 | export: 1
void camcrawl_control(Cam* cam) {
    Camera* camera;
    s32 dYaw;
    Object* obj;

    obj = cam->player;
    if (obj != NULL) {
        cam->srt.transl.x = (fsin16_precise(obj->srt.yaw) * 13.0f) + obj->srt.transl.x;
        cam->srt.transl.z = (fcos16_precise(obj->srt.yaw) * 13.0f) + obj->srt.transl.z;
        cam->srt.transl.y = obj->srt.transl.y + 20.0f;
        camera = camGetMain();

        dYaw = (-arctan2_f(camera->srt.transl.x - obj->srt.transl.x, camera->srt.transl.z - obj->srt.transl.z) - (cam->srt.yaw & 0xFFFF)) + 0x8000;
        CIRCLE_WRAP(dYaw)
        
        cam->srt.yaw += (dYaw * gUpdateRate) >> 3;
    }
}

// offset: 0x1A8 | func: 2 | export: 2
void camcrawl_free(Cam* cam) {
    mmFree(sState);
}

// offset: 0x1E8 | func: 3 | export: 3
void camcrawl_func_1E8(void* arg0, s32 arg1) {

}
