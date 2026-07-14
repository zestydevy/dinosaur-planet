#include "common.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/objects/220_Chuka.h"
#include "dlls/objects/216_SnowWorm.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "dlls/objects/222_SnowWormSmall.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/objhits.h"

typedef struct {
    s16 unk0;
} ChukaChuck_Data;

static void ChukaChuck_handle_player_or_sidekick_collision(Object* self);
static void ChukaChuck_handle_terrain_collision(Object* self);
static int ChukaChuck_does_parent_exist(Object* self);

// offset: 0x0 | ctor
void ChukaChuck_ctor(void* dll) { }

// offset: 0xC | dtor
void ChukaChuck_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void ChukaChuck_setup(Object* self, ChukaChuck_Setup* setup, s32 reset) {
    self->unkDC = 180;
    func_800267A4(self);
    self->opacity = OBJECT_OPACITY_MAX;
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->maxDistScale = self->shadow->scale * 2.5f;
    }
}

// offset: 0xA0 | func: 1 | export: 1
void ChukaChuck_control(Object* self) {
    //Count down until unloading
    self->unkDC -= gUpdateRateF;
    if (self->unkDC < 0) {
        obj_destroy_object(self);
        return;
    }
    
    if (self->opacity == 0) {
        return;
    }
    
    //Motion
    {
        //Fall
        self->velocity.y -= 0.07f * gUpdateRateF;
        self->velocity.y *= 0.97f;

        //Spin
        self->srt.yaw += M_5_DEGREES;
        self->srt.roll += M_5_DEGREES;
        self->srt.pitch += M_5_DEGREES;

        //Move
        obj_move(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);
    }

    //ObjHits
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, 0);
    func_80026940(self, 5);
    func_8002674C(self);
    
    //Handle colliding with the player/sidekick
    if (self->objhitInfo->unk48 && 
        ((get_player() == self->objhitInfo->unk48) || (get_sidekick() == self->objhitInfo->unk48))
    ) {
        ChukaChuck_handle_player_or_sidekick_collision(self);
        self->opacity = 0;
        self->unkDC = 120;
        self->objhitInfo->unk58 &= ~1;
        return;
    }
    
    //Handle colliding with terrain
    if (self->objhitInfo->unk9D != 0) {
        ChukaChuck_handle_terrain_collision(self);
        self->opacity = 0;
        self->unkDC = 120;
        self->objhitInfo->unk58 &= ~1;
    }
}

// offset: 0x2D4 | func: 2 | export: 2
void ChukaChuck_update(Object* self) { }

// offset: 0x2E0 | func: 3 | export: 3
void ChukaChuck_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x334 | func: 4 | export: 4
void ChukaChuck_free(Object* self, s32 onlySelf) {
    camera_disable_y_offset();
}

// offset: 0x374 | func: 5 | export: 5
u32 ChukaChuck_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x384 | func: 6 | export: 6
u32 ChukaChuck_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(ChukaChuck_Data);
}

// offset: 0x398 | func: 7
void ChukaChuck_handle_player_or_sidekick_collision(Object* self) {
    s32 i;

    //Shake camera
    camera_enable_y_offset();
    camera_set_shake_offset(1.0f);

    //Play impact sound
    gDLL_6_AMSFX->vtbl->play(self, SOUND_4B1, MAX_VOLUME, NULL, NULL, 0, NULL);
    
    if (self->id == OBJ_ChukaChuck) {
        //Send a message to the parent ChukaChuck object
        if (self->unkC4 && ChukaChuck_does_parent_exist(self->unkC4)) {
            ((DLL_220_Chuka*)self->unkC4->dll)->vtbl->receive_message(self->unkC4, 0x80);
        }
        
        //Create particles
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_340, NULL, 1, -1, NULL);
        }
        return;
    }
    
    if (self->id == OBJ_IceBall) {
        //Send a message to the parent SnowWorm object
        if (self->unkC4 && ChukaChuck_does_parent_exist(self->unkC4)) {
            ((DLL_216_SnowWorm*)self->unkC4->dll)->vtbl->receive_message(self->unkC4, 0x80);
        }
        
        //Create particles
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, 1, -1, NULL);
        }
        return;
    }
    
    if (self->id == OBJ_IceBallSmall) {
        //Send a message to the parent SnowWormSmall object
        if (self->unkC4 && ChukaChuck_does_parent_exist(self->unkC4)) {
            ((DLL_222_SnowWormSmall*)self->unkC4->dll)->vtbl->receive_message(self->unkC4, 0x80, 0);
        }
        
        //Create particles
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, 1, -1, NULL);
        }
        return;
    }
}

// offset: 0x620 | func: 8
void ChukaChuck_handle_terrain_collision(Object* self) {
    s32 i;

    //Create particles
    if (self->id == OBJ_ChukaChuck) {
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_342, NULL, 1, -1, NULL);
        }
    } else if ((self->id == OBJ_IceBall) || (self->id == OBJ_IceBallSmall)) {
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_344, NULL, 1, -1, NULL);
        }
    }
    
    //Play impact sound
    gDLL_6_AMSFX->vtbl->play(self, SOUND_4B1, 0x40, NULL, NULL, 0, NULL);

    //Shake camera
    camera_enable_y_offset();
    camera_set_shake_offset(1.0f);
}

// offset: 0x77C | func: 9
int ChukaChuck_does_parent_exist(Object* thrownBy) {
    s32 index;
    s32 count;
    Object** objects;

    objects = get_world_objects(&index, &count);
    while (index < count) {
        if (thrownBy == objects[index++]) {
            //@bug: this should possibly also check whether the object's StateFlags have OBJSTATE_DESTROYED set
            return TRUE;
        }
    }
    return FALSE;
}
