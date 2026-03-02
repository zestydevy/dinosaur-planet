#include "common.h"
#include "sys/memory.h"

#include "dlls/engine/2_camcontrol.h"

/*0x0*/ static UNK_PTR* _bss_0;

// offset: 0x0 | ctor
void camcannon_ctor(void *dll) { }

// offset: 0xC | dtor
void camcannon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void camcannon_setup(CamControl_Data* camData, s32 arg1, UNK_PTR* arg2) {
    _bss_0 = mmAlloc(8, ALLOC_TAG_CAM_COL, NULL);
}

// offset: 0x68 | func: 1 | export: 1
void camcannon_control(CamControl_Data* data) {
    Camera* camera;
    s32 dYaw;
    Object* obj;

    obj = data->player;
    if (obj != NULL) {
        data->srt.transl.x = (fsin16_precise(obj->srt.yaw) * 13.0f) + obj->srt.transl.x;
        data->srt.transl.z = (fcos16_precise(obj->srt.yaw) * 13.0f) + obj->srt.transl.z;
        data->srt.transl.y = obj->srt.transl.y + 20.0f;
        camera = get_main_camera();

        dYaw = (-arctan2_f(camera->srt.transl.x - obj->srt.transl.x, camera->srt.transl.z - obj->srt.transl.z) - (data->srt.yaw & 0xFFFF)) + 0x8000;
        CIRCLE_WRAP(dYaw)
        
        data->srt.yaw += (dYaw * gUpdateRate) >> 3;
    }
}

// offset: 0x1A8 | func: 2 | export: 2
void camcannon_free(s32 arg0) {
    mmFree(_bss_0);
}

// offset: 0x1E8 | func: 3 | export: 3
void camcannon_func_1E8(CameraAction* camAction, s32 arg1) {

}
