#include "common.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "dlls/objects/237_WG_PollenCannon.h"
#include "dlls/objects/261_PollenFragment.h"

#include "dlls/objects/260_Pollen.h"

// offset: 0x0 | ctor
void Pollen_ctor(void *dll) { }

// offset: 0xC | dtor
void Pollen_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Pollen_setup(Object* self, ObjSetup* setup, s32 arg2) {
    Pollen_Data* objdata = self->data;

    objdata->unk0 = rand_next(-M_180_DEGREES, M_180_DEGREES - 1);
    objdata->unkC = rand_next(4000, 5000) * 0.01f;
    objdata->unk4 = rand_next(-M_180_DEGREES, M_180_DEGREES - 1);
    objdata->unk8 = 0.0f;
    objdata->unk6 = rand_next(230, 500);
    objdata->unk10 = 0;
    objdata->unk12 = 10;

    self->opacity = OBJECT_OPACITY_MAX;

    func_800267A4(self);

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->maxDistScale = self->shadow->scale * 2.5f;
    }
}

// offset: 0x13C | func: 1
static void Pollen_create_fragments(Object* self) {
    PollenFragment_Setup* setup;
    Object* fragment;
    s32 i;

    i = 6;
    while (i--) {
        setup = obj_alloc_setup(sizeof(PollenFragment_Setup), OBJ_PollenFragment);
        setup->base.x = self->srt.transl.x;
        setup->base.y = self->srt.transl.y;
        setup->base.z = self->srt.transl.z;
        setup->base.loadFlags = 1;
        setup->base.byte5 = 1;
        setup->base.byte6 = 0xFF;
        setup->base.fadeDistance = 0xFF;
        fragment = obj_create(
            (ObjSetup*)setup, 
            OBJINIT_STANDALONE | OBJINIT_FLAG4, 
            -1, 
            -1, 
            NULL
        );

        if (fragment != NULL) {
            fragment->srt.pitch = 0;
            fragment->srt.yaw = 0;
            fragment->velocity.x = 0.01f * rand_next(-50, 50);
            fragment->velocity.y = 0.01f * rand_next(0, 30);
            fragment->velocity.z = 0.01f * rand_next(-50, 50);
            fragment->unkC4 = self;
        }
    }
}

// offset: 0x2A4 | func: 2 | export: 1
void Pollen_control(Object* self) {
    Object* pollenCannon;
    PollenCannon_Data *pollenCannonData;
    Pollen_Data* objData;
    u32 index;
    s32 count;
    PollenCannonUnk3F4* cannonUnk;

    objData = self->data;
    
    pollenCannon = self->unkC4;
    pollenCannonData = (PollenCannon_Data*)pollenCannon->data;
    cannonUnk = pollenCannonData->unk3F4;
    
    if (objData->unk12 != 0) {
        objData->unk12--;
    }
    
    //Apply gravity and move
    self->velocity.y -= 0.045f * gUpdateRateF;
    obj_move(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);
    
    //Objhits
    func_80026128(self, 0x15, 1, 0);
    func_80026940(self, 7);
    func_8002674C(self);

    //React to hitting the player or sidekick
    if (self->objhitInfo->unk48 && 
        ((get_player() == self->objhitInfo->unk48) || (get_sidekick() == self->objhitInfo->unk48))
    ) {
        camera_enable_y_offset();
        camera_set_shake_offset(1.0f);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AFF_Gas_Disperse_Burst, MAX_VOLUME, NULL, NULL, 0, NULL);
        self->opacity = 0;
        func_800267A4(self);
    }

    //Create gas particles
    index = 3;
    while (index--) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4BA, NULL, 1, -1, NULL);
    }
    
    //Destroy self after colliding
    if (self->opacity == 0) {
        if (pollenCannon->id == OBJ_WG_PollenCannon) {
            index = cannonUnk->unk6_0;
            while (index--) {
                if (self == cannonUnk->unk8[index]) {
                    cannonUnk->unk8[index] = cannonUnk->unk8[--cannonUnk->unk6_0];
                }
            } 
        }
        
        Pollen_create_fragments(self);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_722_Impact_Wobble, 0x40, NULL, NULL, 0, NULL);
        obj_destroy_object(self);
    }
}

// offset: 0x5C0 | func: 3 | export: 2
void Pollen_update(Object* self) {
    ObjectHitInfo* objHits;

    objHits = self->objhitInfo;
    if (objHits->unk9D) {
        self->srt.transl.x = objHits->unk34;
        self->srt.transl.y = objHits->unk38;
        self->srt.transl.z = objHits->unk3C;
        self->opacity = 0;
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
        func_800267A4(self);
    }
}

// offset: 0x634 | func: 4 | export: 3
void Pollen_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x688 | func: 5 | export: 4
void Pollen_free(Object* self, s32 a1) {
    camera_disable_y_offset();
}

// offset: 0x6C8 | func: 6 | export: 5
u32 Pollen_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6D8 | func: 7 | export: 6
u32 Pollen_get_data_size(Object* self, u32 a1) {
    return sizeof(Pollen_Data);
}
